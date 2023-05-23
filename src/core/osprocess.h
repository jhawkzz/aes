
// See License.md for license

#ifndef OS_PROCESS_H_
#define OS_PROCESS_H_

enum WaitAction
{
    WaitAction_Block, // The parent process will block (on the main or secondary thread) until the child exits
    WaitAction_Poll, //The parent process will poll the child process until it exits. Without a secondary thread this is the same as a block.
    WaitAction_FireAndForget //The parent process will not wait for the child process to exit
};

struct LaunchProcArgs
{
    WaitAction waitAction;
    char command[ MAX_STR_SIZE ];
    char commandArgs[ MAX_STR_SIZE ];
};

class OSProcess
{
public:
    static int  Create( );
    static void Destroy( );
    static void LaunchProcess( const char *pCommand, const char *pCommandArgs, WaitAction waitAction, int multiThread );
    static void KillThreadedProc( );
    static int  IsThreadedProcRunning( );
    
private:
    static void *LaunchProcess_ThreadProc( void *pArg );
    static void  ForkForProcess( const char *pCommand, const char *pCommandArgs, WaitAction waitAction );

    // these are all used for kicking off a process on another thread
    static          pthread_mutex_t mChildProc_MutexLock;
    static volatile int             mChildProcOnThread;
    static          LaunchProcArgs  mLaunchProcArgs;
    
    // used for terminating the child thread when polling
    static volatile int             mKillChildProc;
    static          pthread_mutex_t mKillChildProc_MutexLock;
};

#endif
