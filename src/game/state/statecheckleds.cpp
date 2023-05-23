
// See License.md for license

#include "includes.h"

void StateCheckLEDs::Create( )
{
    BaseGameState::Create( );
}

void StateCheckLEDs::Destroy( )
{
    BaseGameState::Destroy( );
}

void StateCheckLEDs::EnterState( GameState oldState )
{
    BaseGameState::EnterState( oldState );
}

void StateCheckLEDs::ExitState( GameState newState )
{
    BaseGameState::ExitState( newState );
}

GameState StateCheckLEDs::Update( )
{
    Renderer::DrawText( "Press P1 Start to Exit", 
                        305, // centers it on screen
                        UI_Y_POS_HEADER, 
                        0xFFFF );
                        
    Renderer::DrawText( "Press player buttons to test LEDs", 
                        106, 
                        UI_Y_POS_MENU, 
                        0xFFFF );
    
    // play around with the LEDs!
    UpdateLEDs( );
    
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

void StateCheckLEDs::UpdateLEDs( )
{
    if ( GameInput::GetInput( P1_Button_1 )->WasReleased() )
    {
        int led = 1;
        OSLed::SetLED( &led, NULL );
    }
    
    if ( GameInput::GetInput( P1_Button_2 )->WasReleased() )
    {
        int led = 2;
        OSLed::SetLED( &led, NULL );
    }
    
    if ( GameInput::GetInput( P1_Button_3 )->WasReleased() )
    {
        int led = 3;
        OSLed::SetLED( &led, NULL );
    }
    
    if ( GameInput::GetInput( P1_Button_4 )->WasReleased() )
    {
        int led = 4;
        OSLed::SetLED( &led, NULL );
    }
    
    if ( GameInput::GetInput( P1_Button_5 )->WasReleased() )
    {
        int led = 5;
        OSLed::SetLED( &led, NULL );
    }
    
    if ( GameInput::GetInput( P1_Button_6 )->WasReleased() )
    {
        int led = 6;
        OSLed::SetLED( &led, NULL );
    }
    
    if ( GameInput::GetInput( P2_Button_1 )->WasReleased() )
    {
        int led = 1;
        OSLed::SetLED( NULL, &led );
    }
    
    if ( GameInput::GetInput( P2_Button_2 )->WasReleased() )
    {
        int led = 2;
        OSLed::SetLED( NULL, &led );
    }
    
    if ( GameInput::GetInput( P2_Button_3 )->WasReleased() )
    {
        int led = 3;
        OSLed::SetLED( NULL, &led );
    }
    
    if ( GameInput::GetInput( P2_Button_4 )->WasReleased() )
    {
        int led = 4;
        OSLed::SetLED( NULL, &led );
    }
    
    if ( GameInput::GetInput( P2_Button_5 )->WasReleased() )
    {
        int led = 5;
        OSLed::SetLED( NULL, &led );
    }
    
    if ( GameInput::GetInput( P2_Button_6 )->WasReleased() )
    {
        int led = 6;
        OSLed::SetLED( NULL, &led );
    }
}
