
// See License.md for license

#include "includes.h"

void StateQuit::Create( )
{
    BaseGameState::Create( );
    
    mShouldQuitGame = 0;
}

void StateQuit::Destroy( )
{
    BaseGameState::Destroy( );
}

void StateQuit::EnterState( GameState oldState )
{
    BaseGameState::EnterState( oldState );
}

void StateQuit::ExitState( GameState newState )
{
    BaseGameState::ExitState( newState );
}

GameState StateQuit::Update( )
{
    Renderer::DrawText( "Press P1 Start to Exit", 
                        305, // centers it on screen
                        UI_Y_POS_HEADER, 
                        0xFFFF );
                        
    Renderer::DrawText( "Press P1 Button 1 and P1 Button 2 to quit", 
                        106, 
                        UI_Y_POS_MENU, 
                        0xFFFF );
    
    // check to see if both buttons were released in the past 100ms
    int deltaP1ReleasedTime = gFrameTimeMS - GameInput::GetInput( P1_Button_1 )->GetTimeReleasedMS( );
    int deltaP2ReleasedTime = gFrameTimeMS - GameInput::GetInput( P1_Button_2 )->GetTimeReleasedMS( );
    
    if(   (GameInput::GetInput( P1_Button_1 )->GetState() == InputState_Released && deltaP1ReleasedTime < 100)
       && (GameInput::GetInput( P1_Button_2 )->GetState() == InputState_Released && deltaP2ReleasedTime < 100) )
    {
        mShouldQuitGame = 1;
        return GameState_Count;
    }
    
    // should we exit?
    if( GameInput::GetInput( P1_Start )->WasReleased( ) )
    {
        
        return mLastState;
    }
    else
    {
        return GameState_Count;
    }
}

int StateQuit::ShouldQuitGame( )
{
    return mShouldQuitGame;
}
