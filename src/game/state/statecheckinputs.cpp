
// See License.md for license

#include "includes.h"

void StateCheckInputs::Create( )
{
    BaseGameState::Create( );
}

void StateCheckInputs::Destroy( )
{
    BaseGameState::Destroy( );
}

void StateCheckInputs::EnterState( GameState oldState )
{
    BaseGameState::EnterState( oldState );
}

void StateCheckInputs::ExitState( GameState newState )
{
    BaseGameState::ExitState( newState );
}

GameState StateCheckInputs::Update( )
{
    BaseGameState::Update( );
    
    Renderer::DrawText( "Press P1 Start to Exit", 
                        305, // centers it on screen
                        UI_Y_POS_HEADER, 
                        0xFFFF );
    
    DrawInputList( );
    
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

void StateCheckInputs::DrawInputList( )
{
    int currYPos = UI_Y_POS_MENU;
    int xPos = 20;
        
    // Begin System Buttons
    DrawInput( P1_InsertCoin, "P1 Insert Coin", xPos, currYPos );
    currYPos += INPUT_ROW_HEIGHT;
    DrawInput( P2_InsertCoin, "P2_Insert Coin", xPos, currYPos );
    currYPos += INPUT_ROW_HEIGHT;
    
    // begin P1 Joystick
    currYPos += INPUT_ROW_HEIGHT;
    DrawInput( P1_Joy_Up, "P1 Joystick Up", xPos, currYPos );
    currYPos += INPUT_ROW_HEIGHT;
    DrawInput( P1_Joy_Left, "P1 Joystick Left", xPos, currYPos );
    currYPos += INPUT_ROW_HEIGHT;
    DrawInput( P1_Joy_Down, "P1 Joystick Down", xPos, currYPos );
    currYPos += INPUT_ROW_HEIGHT;    
    DrawInput( P1_Joy_Right, "P1 Joystick Right", xPos, currYPos );
    currYPos += INPUT_ROW_HEIGHT;
    
    // Begin P1 Buttons
    currYPos += INPUT_ROW_HEIGHT;
    DrawInput( P1_Start, "P1 Start", xPos, currYPos );
    currYPos += INPUT_ROW_HEIGHT;
    DrawInput( P1_Button_1, "P1 Button 1", xPos, currYPos );
    currYPos += INPUT_ROW_HEIGHT;
    DrawInput( P1_Button_2, "P1 Button 2", xPos, currYPos );
    currYPos += INPUT_ROW_HEIGHT;
    DrawInput( P1_Button_3, "P1 Button 3", xPos, currYPos );
    currYPos += INPUT_ROW_HEIGHT;
    DrawInput( P1_Button_4, "P1 Button 4", xPos, currYPos );
    currYPos += INPUT_ROW_HEIGHT;
    DrawInput( P1_Button_5, "P1 Button 5", xPos, currYPos );
    currYPos += INPUT_ROW_HEIGHT;
    DrawInput( P1_Button_6, "P1 Button 6", xPos, currYPos );
    currYPos += INPUT_ROW_HEIGHT;
    
    // Begin P2 Joystick
    currYPos += INPUT_ROW_HEIGHT;
    DrawInput( P2_Joy_Up, "P2 Joystick Up", xPos, currYPos );
    currYPos += INPUT_ROW_HEIGHT;
    DrawInput( P2_Joy_Left, "P2 Joystick Left", xPos, currYPos );
    currYPos += INPUT_ROW_HEIGHT;
    DrawInput( P2_Joy_Down, "P2 Joystick Down", xPos, currYPos );
    currYPos += INPUT_ROW_HEIGHT;
    DrawInput( P2_Joy_Right, "P2 Joystick Right", xPos, currYPos );
    currYPos += INPUT_ROW_HEIGHT;
    
    // Begin P2 Buttons
    currYPos += INPUT_ROW_HEIGHT;
    DrawInput( P2_Start, "P2 Start", xPos, currYPos );
    currYPos += INPUT_ROW_HEIGHT;
    DrawInput( P2_Button_1, "P2 Button 1", xPos, currYPos );
    currYPos += INPUT_ROW_HEIGHT;
    DrawInput( P2_Button_2, "P2 Button 2", xPos, currYPos );
    currYPos += INPUT_ROW_HEIGHT;
    DrawInput( P2_Button_3, "P2 Button 3", xPos, currYPos );
    currYPos += INPUT_ROW_HEIGHT;
    DrawInput( P2_Button_4, "P2 Button 4", xPos, currYPos );
    currYPos += INPUT_ROW_HEIGHT;
    DrawInput( P2_Button_5, "P2 Button 5", xPos, currYPos );
    currYPos += INPUT_ROW_HEIGHT;
    DrawInput( P2_Button_6, "P2 Button 6", xPos, currYPos );
}

void StateCheckInputs::DrawInput( ButtonInput button, const char *pLabel, int x, int y )
{
    InputControl *pInput = GameInput::GetInput( button );
    
    // render the label for the button
    Renderer::DrawText( pLabel, x, y, 0xFFFF );
    x += 323;
    
    // render its current state
    if( pInput->GetState() == InputState_Released )
    {
        Renderer::DrawText( "Released", x, y, 0xFFFF );
    }
    else
    {
        Renderer::DrawText( "Pressed", x, y, 0xFFFF );
    }
    x += 200;

    char text[ MAX_STR_SIZE ] = { 0 };
    snprintf( text, MAX_STR_SIZE - 1, "Time Held: %lu ms", pInput->GetTimeHeldMS( ) );
    Renderer::DrawText( text, x, y, 0xFFFF );
    
    x += 320;
    snprintf( text, MAX_STR_SIZE - 1, "Time Released: %lu ms", pInput->GetTimeReleasedMS( ) );
    Renderer::DrawText( text, x, y, 0xFFFF );   
}
