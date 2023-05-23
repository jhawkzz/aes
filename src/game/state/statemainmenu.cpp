
// See License.md for license

#include "includes.h"

void StateMainMenu::Create( )
{
    BaseGameState::Create( );
    
    int xPos = UI_X_POS_MENU;
    int yPos = UI_Y_POS_MENU;
    
    mMenuItemGameState[ 0 ].menuItem.Create( "1. Launch Exec", xPos, yPos, 0xFFFF );
    mMenuItemGameState[ 0 ].gameState = GameState_Exec;
    
    yPos += UI_ROW_HEIGHT;
    mMenuItemGameState[ 1 ].menuItem.Create( "2. Mounting", xPos, yPos, 0xFFFF );
    mMenuItemGameState[ 1 ].gameState = GameState_Mounting;
    
    yPos += UI_ROW_HEIGHT;
    mMenuItemGameState[ 2 ].menuItem.Create( "3. Check Inputs", xPos, yPos, 0xFFFF );
    mMenuItemGameState[ 2 ].gameState = GameState_CheckInputs;
    
    yPos += UI_ROW_HEIGHT;
    mMenuItemGameState[ 3 ].menuItem.Create( "4. Check LEDs", xPos, yPos, 0xFFFF );
    mMenuItemGameState[ 3 ].gameState = GameState_CheckLEDs;
    
    yPos += UI_ROW_HEIGHT;
    mMenuItemGameState[ 4 ].menuItem.Create( "5. Check Audio", xPos, yPos, 0xFFFF );
    mMenuItemGameState[ 4 ].gameState = GameState_CheckAudio;
    
    yPos += UI_ROW_HEIGHT;
    mMenuItemGameState[ 5 ].menuItem.Create( "6. Credits", xPos, yPos, 0xFFFF );
    mMenuItemGameState[ 5 ].gameState = GameState_Credits;
    
    yPos += UI_ROW_HEIGHT;
    mMenuItemGameState[ 6 ].menuItem.Create( "7. Quit", xPos, yPos, 0xFFFF );
    mMenuItemGameState[ 6 ].gameState = GameState_Quit;
    
    mMenuSelection = 0;
}

void StateMainMenu::Destroy( )
{
    BaseGameState::Destroy( );
}

void StateMainMenu::EnterState( GameState oldState )
{
    BaseGameState::EnterState( oldState );
}

void StateMainMenu::ExitState( GameState newState )
{
    BaseGameState::ExitState( newState );
}
    
GameState StateMainMenu::Update( )
{
    BaseGameState::Update( );
    
    // draw our menu options
    DrawMenu( );
    
    // check for menu navigation
    if( GameInput::GetInput( P1_Joy_Down )->WasReleased() )
    {
        mMenuSelection = min( MAINMENU_ITEMS - 1, mMenuSelection + 1 );
    }
    else if( GameInput::GetInput( P1_Joy_Up )->WasReleased() )
    {
        mMenuSelection = max( 0, mMenuSelection - 1 );
    }
    
    // check for entering a gamestate menu item
    if( GameInput::GetInput( P1_Button_1 )->WasReleased() )
    {   
        return mMenuItemGameState[ mMenuSelection ].gameState;
    }
    
    // check for toggling render throttle
    if( GameInput::GetInput( P1_InsertCoin )->WasReleased( ) )
    {
        OSFrameBuffer::ToggleRenderThrottle( !OSFrameBuffer::RenderThrottleEnabled( ) );
    }
    
    // check for toggling back buffer clear
    if( GameInput::GetInput( P2_InsertCoin )->WasReleased( ) )
    {
        OSFrameBuffer::ToggleClearBackBuffer( !OSFrameBuffer::ClearBackBufferEnabled( ) );
    }
    
    return GameState_Count;
}

void StateMainMenu::DrawMenu( )
{
    char text[ MAX_STR_SIZE ] = { 0 };
    snprintf( text, sizeof( text ) -1, 
              "P1_InsertCoin toggles render throttle: %d," "Select Game toggles BB Clear: %d", 
              OSFrameBuffer::RenderThrottleEnabled( ), OSFrameBuffer::ClearBackBufferEnabled( ) );
              
    Renderer::DrawText( text, 
                        100,
                        UI_Y_POS_HEADER,
                        0XFFFF );
                        
    int i = 0;
    for( i = 0; i < MAINMENU_ITEMS; i++ )
    {
        mMenuItemGameState[ i ].menuItem.Draw( );
    }
    
    Renderer::DrawText( "X", 
                        mMenuItemGameState[ mMenuSelection ].menuItem.GetXPos( ) - UI_CURSOR_X_OFFSET, 
                        mMenuItemGameState[ mMenuSelection ].menuItem.GetYPos( ),
                        0XFFEA );
}
