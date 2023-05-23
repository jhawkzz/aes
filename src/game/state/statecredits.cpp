
// See License.md for license

#include "includes.h"

void StateCredits::Create( )
{
    BaseGameState::Create( );
}

void StateCredits::Destroy( )
{
    BaseGameState::Destroy( );
}

void StateCredits::EnterState( GameState oldState )
{
    BaseGameState::EnterState( oldState );
}

void StateCredits::ExitState( GameState newState )
{
    BaseGameState::ExitState( newState );
}

GameState StateCredits::Update( )
{
    char text[ MAX_STR_SIZE ] = { 0 };
    
    Renderer::DrawText( "Press P1 Start to Exit", 
                        305, // centers it on screen
                        UI_Y_POS_HEADER, 
                        0xFFFF );
    
    int xPos = 25;
    int yPos = UI_Y_POS_MENU;
    Renderer::DrawText( "Advanced Executable Starter",
                        xPos,
                        yPos,
                        0xFFFF );
                        
    yPos += UI_ROW_HEIGHT;
    Renderer::DrawText( "Written by and * 2023 JHawkZZ", 
                        xPos, 
                        yPos, 
                        0xFFFF );
    
    snprintf( text, sizeof( text ) - 1, "Version: %d", VERSION );
    yPos += UI_ROW_HEIGHT;
    Renderer::DrawText( text, 
                        xPos, 
                        yPos, 
                        0xFFFF );
                        
    yPos += UI_ROW_HEIGHT;
    Renderer::DrawText( "License:", 
                        xPos, 
                        yPos, 
                        0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    Renderer::DrawText( "You may freely use, modify, and distribute both the Advanced Exec Starter",
                        xPos,
                        yPos,
                        0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    Renderer::DrawText( "source code and binary, however the following restrictions apply:",
                        xPos,
                        yPos,
                        0xFFFF );
                        
    
    
    yPos += UI_ROW_HEIGHT;
    Renderer::DrawText( "You may not sell, lease, rent or otherwise seek to gain monetary profit from", 
                        xPos,
                        yPos,
                        0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    Renderer::DrawText( "Advanced Exec Starter.", 
                        xPos,
                        yPos,
                        0xFFFF );
                        
    
    yPos += UI_ROW_HEIGHT;
    Renderer::DrawText( "You must make public any changes you make to the source code.",
                        xPos,
                        yPos,
                        0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    Renderer::DrawText( "You must include, verbatim, the full text of this license.",
                        xPos,
                        yPos,
                        0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    Renderer::DrawText( "You may not ask for donations to support your work on any project that uses",
                        xPos,
                        yPos,
                        0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    Renderer::DrawText( "the Advanced Exec Starter source code.",
                        xPos,
                        yPos,
                        0xFFFF );
                        
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
