
// See License.md for license

#include "includes.h"

pthread_mutex_t OSProcess::mChildProc_MutexLock;
volatile int    OSProcess::mChildProcOnThread;
LaunchProcArgs  OSProcess::mLaunchProcArgs;
volatile int    OSProcess::mKillChildProc;
pthread_mutex_t OSProcess::mKillChildProc_MutexLock;
    
int OSProcess::Create( )
{
    mChildProcOnThread = 0;
    mKillChildProc = 0;
    
    pthread_mutex_init( &mKillChildProc_MutexLock, NULL );
    pthread_mutex_init( &mChildProc_MutexLock, NULL );
    
    memset( &mLaunchProcArgs, 0, sizeof( mLaunchProcArgs ) );
    
    return 0;
}

void OSProcess::Destroy( )
{
    // free our mutexes
    pthread_mutex_lock( &mKillChildProc_MutexLock );
    pthread_mutex_unlock( &mKillChildProc_MutexLock );
    pthread_mutex_destroy( &mKillChildProc_MutexLock );
    
    pthread_mutex_lock( &mChildProc_MutexLock );
    pthread_mutex_unlock( &mChildProc_MutexLock );
    pthread_mutex_destroy( &mChildProc_MutexLock );
}

void OSProcess::LaunchProcess( const char *pCommand, const char *pCommandArgs, WaitAction waitAction, int multiThread )
{
    // should the process spawning should occur on another thread?
    if( multiThread )
    {
        // make sure there's not already one running (and if there is, lock so it can exit)
        pthread_mutex_lock( &mChildProc_MutexLock );
        if ( mChildProcOnThread )
        {
            pthread_mutex_unlock( &mChildProc_MutexLock );
            return;
        }
        
        mChildProcOnThread = 1;
        pthread_mutex_unlock( &mChildProc_MutexLock );
        
        // store our launch args so that the calling thread can retrieve them
        strncpy( mLaunchProcArgs.command, pCommand, sizeof( mLaunchProcArgs.command ) -1 );
        strncpy( mLaunchProcArgs.commandArgs, pCommandArgs, sizeof( mLaunchProcArgs.commandArgs ) -1 );
        mLaunchProcArgs.waitAction = waitAction;
        
        pthread_t t;
        int result = pthread_create( &t, NULL, LaunchProcess_ThreadProc, NULL );
        if( result != 0 )
        {
            flushPrintf( "pthread_create failed with error: %d\r\n", result );
            return;
        }
    }
    else
    {
        ForkForProcess( pCommand, pCommandArgs, waitAction );
    }
}

void *OSProcess::LaunchProcess_ThreadProc( void *pArg )
{
    // This may block until the process is complete depending on what was requested
    ForkForProcess( mLaunchProcArgs.command, mLaunchProcArgs.commandArgs, mLaunchProcArgs.waitAction );
	
    // reset mutex values
    pthread_mutex_lock( &mKillChildProc_MutexLock );
    mKillChildProc = false;
    pthread_mutex_unlock( &mKillChildProc_MutexLock );
    
    pthread_mutex_lock( &mChildProc_MutexLock );
    mChildProcOnThread = 0;
    pthread_mutex_unlock( &mChildProc_MutexLock );
    
	return NULL;
}

void OSProcess::ForkForProcess( const char *pCommandStr, const char *pCommandArgs, WaitAction waitAction )
{
    // for the parent process, fork() returns with the id of the child proc.
    // for the new child proc, fork returns 0
    // no point in using "system" vs this, because system calls fork()
    pid_t pid = fork( );
	if( pid < 0 )
	{
		flushPrintf( "ForkForProcess_ThreadProc fork() failed with error: pid: %d e: %d\r\n", pid, errno );
	}
	// this is the newly created process.
	else if( pid == 0 )
	{
        // This scope is now the child process. The parent proc will never enter this branch.
        
        // call setsid() to move this child process into its own process group where 
        // its group id is the same as its process id. This way, when 'sh' spawns
        // the actual process, both can be killed by the parent process that created this.
        // Otherwise, the parent process would only be able to kill 'sh', and not the process
        // that 'sh' spawned.
        int newgpid = setsid();
        
        char commandWithArgs[ MAX_STR_SIZE * 2 ] = { 0 };
        snprintf( commandWithArgs, sizeof( commandWithArgs ) - 1, "%s %s", pCommandStr, pCommandArgs );
        
		//-c simply means "command"
		int result = execl( "/system/bin/sh", "sh", "-c", commandWithArgs, NULL );
        
        // for the pi2
        //int result = execl( "/bin/sh", "sh", "-c", commandWithArgs, NULL );
        
        // if we got here, execl failed, we're the cloned process, and we need to immediately terminate.
		flushPrintf( "ForkForProcess_ThreadProc execl() failed with error: r: %d e: %d\r\n", result, errno );
		exit(0);
	}
	else
	{
        // This scope is the parent process. The child proc will never enter this branch.
        
        // if fire and forget, we're done.
        if( waitAction == WaitAction_FireAndForget )
        {
            flushPrintf( "FireAndForget requested, so returning.\r\n" );
        }
        else
        {
            if( waitAction == WaitAction_Poll )
            {
                // monitor for the child to exit (it will return its pid)
                // or a signal from the game to kill it.
                int status = 0;
                while( waitpid( pid, &status, WNOHANG ) != pid )
                {
                    // no need to wait on the mutex, we're just reading
                    // and if we missed it, we'll get it next iteration.
                    if( mKillChildProc == true )
                    {
                        // Because we called setsid() on the child process, it was moved into its own process group
                        // where its group id matches its pid. Therefore, we have its process group id.
                        // Now we can kill all processes in the group, ensuring we kill the process that was spawned
                        // BY 'sh'. (Without that, we'd only kill 'sh' and leave the actual process it spawned running.)
                        int groupProcId = pid;
                        int result = killpg( groupProcId, SIGKILL );
                        if( result == 0 )
                        {
                            break;
                        }
                        else
                        {
                            flushPrintf( "Processes under group id %d COULD NOT be killed: r: %d errno: %d\r\n", groupProcId, result, errno );
                        }
                    }
                }
            }
            else
            {
                int status = 0;
                while( waitpid( pid, &status, 0 ) < 0 )
                {
                    if( errno != EINTR ) 
                    {
                        // The child process exited, so we can stop idling
                        break;
                    }   
                }
            }
        }
	}
}

int OSProcess::IsThreadedProcRunning( )
{
    int childProcVal = 0;
    
    pthread_mutex_lock( &mChildProc_MutexLock );
    childProcVal = mChildProcOnThread;
    pthread_mutex_unlock( &mChildProc_MutexLock );
    
    return childProcVal;
}

void OSProcess::KillThreadedProc( )
{
    pthread_mutex_lock( &mChildProc_MutexLock );
    
    if( mChildProcOnThread == 1 )
    {
        pthread_mutex_lock( &mKillChildProc_MutexLock );
        mKillChildProc = true;
        pthread_mutex_unlock( &mKillChildProc_MutexLock );
    }
    
    pthread_mutex_unlock( &mChildProc_MutexLock );
}
