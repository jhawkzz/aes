
// See License.md for license

#include "includes.h"

void StateExec::Create( )
{
    BaseGameState::Create( );
    
    int xPos = UI_X_POS_MENU;
    int yPos = UI_Y_POS_MENU;
    
    mMenuItemList[ 0 ].Create( "1. Run Exec, AES Polls, P2 Start Kill", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    mMenuItemList[ 1 ].Create( "2. Run Exec, AES Sleeps", xPos, yPos, 0xFFFF );
    
    memset( mResultStr, 0, sizeof( mResultStr ) );
    
    mExecRunning = ExecState_NotRunning;
    mMenuSelection = 0;
}

void StateExec::Destroy( )
{
    BaseGameState::Destroy( );
    
    KillExecThreaded( );
}

void StateExec::EnterState( GameState oldState )
{
    BaseGameState::EnterState( oldState );
        
    // reset our output
    memset( mResultStr, 0, sizeof( mResultStr ) );
    
    mExecRunning = ExecState_NotRunning;
}

void StateExec::ExitState( GameState newState )
{
    KillExecThreaded( );
    
    BaseGameState::ExitState( newState );
}

GameState StateExec::Update( )
{
    BaseGameState::Update( );
    
    int shouldExit = 0;
    
// in develop mode, we can't launch an app, because we ARE the launched app
#ifndef DEVELOP_MODE
    switch( mExecRunning )
    {
        case ExecState_NotRunning:
        {
            // check for menu navigation
            if( GameInput::GetInput( P1_Joy_Down )->WasReleased() )
            {
                mMenuSelection = 1;
            }
            else if( GameInput::GetInput( P1_Joy_Up )->WasReleased() )
            {
                mMenuSelection = 0;
            }
            
            // check for launching the desired Exec   
            if ( GameInput::GetInput( P1_Button_1 )->WasReleased() )
            {
                if( mMenuSelection == 0 )
                {
                    LaunchExec_Threaded( );
                }
                else
                {
                    // this wont return until the exec completes
                    LaunchExec_Main();
                }
            }
            else if( GameInput::GetInput( P1_Start )->WasReleased( ) )
            {
                shouldExit = 1;
            }
            break;
        }
        
        case ExecState_Running_Threaded:
        {
            // check for killing multi-threaded Exec
            if( GameInput::GetInput( P2_Start )->WasReleased( ) )
            {
                KillExecThreaded( );
            }
            // and check for it ending on its own
            else if ( OSProcess::IsThreadedProcRunning( ) == 0 )
            {
                HandleExecFinished( );
            }
            break;
        }
    }
#else //in develop mode - only allow exiting
    if( GameInput::GetInput( P1_Start )->WasReleased( ) )
    {
        shouldExit = 1;
    }
#endif
    
    DrawMenu( );
        
    // should we exit?
    if( shouldExit == 1 )
    {
        return mLastState;
    }
    else
    {
        return GameState_Count;
    }
}

void StateExec::DrawMenu( )
{
    Renderer::DrawText( "Press P1 Start to Exit", 
                        305, // centers it on screen
                        UI_Y_POS_HEADER, 
                        0xFFFF );
    
    mMenuItemList[0].Draw( );
    mMenuItemList[1].Draw( );
    
    Renderer::DrawText( "X", 
                        mMenuItemList[ mMenuSelection ].GetXPos( ) - UI_CURSOR_X_OFFSET, 
                        mMenuItemList[ mMenuSelection ].GetYPos( ),
                        0XFFEA );

    int menuBottomYPos = mMenuItemList[ 1 ].GetYPos( );
    Renderer::DrawText( mResultStr,
                        UI_X_POS_MENU, 
                        menuBottomYPos + (UI_ROW_HEIGHT * 2), 
                        0xFFFF );
                        
#ifdef DEVELOP_MODE
    Renderer::DrawText( "In Develop Mode. Launching Execs Disabled.",
                        UI_X_POS_MENU,
                        menuBottomYPos + (UI_ROW_HEIGHT * 3),
                        0xFFFF );
#endif
    
}

void StateExec::LaunchExec_Main( )
{
    if( mExecRunning == ExecState_NotRunning )
    {
        mExecRunning = ExecState_Running;
        
        Renderer::ToggleRenderEnabled( 0 );
        
        LaunchExec( WaitAction_Block, 0 );
        
        HandleExecFinished( );
    }
}

void StateExec::LaunchExec_Threaded( )
{
    if( mExecRunning == ExecState_NotRunning )
    {
        mExecRunning = ExecState_Running_Threaded;
        
        Renderer::ToggleRenderEnabled( 0 );
        
        LaunchExec( WaitAction_Poll, 1 );
    }
}

void StateExec::LaunchExec( WaitAction waitAction, int multiThread )
{
    int result = OSMount::MountUSBPartition( PROCESS_MOUNT_PATH, ROOT_PARTITION_INDEX );
    if ( result == -1 )
    {
        snprintf( mResultStr, sizeof( mResultStr ), "Mounting %s failed. Wait a sec and try again.", PROCESS_MOUNT_PATH );
        return;
    }
    
    // reset the result string
    mResultStr[0] = 0;
    
    // refresh the config settings
    GameConfig::ReadConfigFile( );
    
    OSProcess::LaunchProcess(GameConfig::GetExec( ), "", waitAction, multiThread);
}

void StateExec::KillExecThreaded( )
{
    // Note - it's possible it won't die immediately,
    // but we're going ahead and flagging the exec as not running
    // and re-enabling our flipping. This is normally fine because
    // the exec dies immediately, but if it didn't, you'd have contention
    // in the front buffer (flickering as the exec AND this app write to it)
    if( mExecRunning == ExecState_Running_Threaded )
    {
        OSProcess::KillThreadedProc( );
        
        HandleExecFinished( );
    }
}

void StateExec::HandleExecFinished( )
{
    mExecRunning = ExecState_NotRunning;
    Renderer::ToggleRenderEnabled( 1 );
}
