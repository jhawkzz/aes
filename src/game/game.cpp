
// See License.md for license

#include "includes.h"

short           *Game::mpImageBuffer;
GameState        Game::mGameState;
StateMainMenu    Game::mGameState_MainMenu;
StateMounting    Game::mGameState_Mounting;
StateCheckLEDs   Game::mGameState_CheckLEDs;
StateCheckInputs Game::mGameState_CheckInputs;
StateCheckAudio  Game::mGameState_CheckAudio;
StateExec        Game::mGameState_Exec;
StateCredits     Game::mGameState_Credits;
StateQuit        Game::mGameState_Quit;
BaseGameState   *Game::mpGameStateList[ GameState_Count ];

int Game::Create( )
{
    GameConfig::Create( );
    
    GameInput::Create( );
    
    GameConfig::ReadConfigFile( );
    
    int ledVal = 0;
    OSLed::SetLED( &ledVal, &ledVal );
    
    mGameState = GameState_Count;
    
    // Load our basic assets
    BitmapImage bmpImage = { 0 };
    int result = BitmapLoader::Load( ASSET_PATH"/images/wallpaper.bmp", &bmpImage );
    if( result == 0 )
    {
        result = BitmapLoader::Make565( &bmpImage );
        if( result == 0 )
        {
            mpImageBuffer = (short *)bmpImage.pPixelData;
        }
        else
        {
            flushPrintf( "Failed to convert background image to 565!\r\n" );
        }
    }
    else
    {
        flushPrintf( "Failed to load background image %s!\r\n", ASSET_PATH"/images/wallpaper.bmp" );
    }
    
    // setup our gamestate list
    mpGameStateList[ GameState_MainMenu ]    = &mGameState_MainMenu;
    mpGameStateList[ GameState_Mounting ]    = &mGameState_Mounting;
    mpGameStateList[ GameState_CheckInputs ] = &mGameState_CheckInputs;
    mpGameStateList[ GameState_CheckLEDs ]   = &mGameState_CheckLEDs;
    mpGameStateList[ GameState_CheckAudio ]  = &mGameState_CheckAudio;
    mpGameStateList[ GameState_Exec ]        = &mGameState_Exec;
    mpGameStateList[ GameState_Credits ]     = &mGameState_Credits;
    mpGameStateList[ GameState_Quit ]        = &mGameState_Quit;
    
    // initialize our gamestates
    for( int i = 0; i < GameState_Count; i++ )
    {
        mpGameStateList[ i ]->Create( );
    }
    
    ChangeGameState( GameState_MainMenu );
    
    return 0;
}

void Game::Destroy( )
{
    for( int i = 0; i < GameState_Count; i++ )
    {
        mpGameStateList[ i ]->Destroy( );
    }
    
    if ( mpImageBuffer != NULL )
    {
        delete [] mpImageBuffer;
        mpImageBuffer = NULL;
    }
    
    GameInput::Destroy( );
}

void Game::ChangeGameState( GameState newState )
{
    // make sure we're IN a state (and not initially booting)
	if( mGameState != GameState_Count )
    {
        mpGameStateList[ mGameState ]->ExitState( newState );
    }
    
    GameState oldState = mGameState;
    mGameState = newState;
    
	mpGameStateList[ mGameState ]->EnterState( oldState );
}

int Game::Update( )
{
    GameInput::Update( );
    
    // draw the wallpaper
    Renderer::DrawBackground( mpImageBuffer );
    
    GameState newState = mpGameStateList[ mGameState ]->Update( );
    if( newState != GameState_Count )
    {
        ChangeGameState( newState );
    }    
    
    // special quit game check
    if( mGameState_Quit.ShouldQuitGame( ) )
    {
        return 0;
    }
    
    return 1;
}
