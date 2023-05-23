
// See License.md for license

#include "includes.h"

void StateCheckAudio::Create( )
{
    BaseGameState::Create( );
    
    memset( mResultStr, 0, sizeof( mResultStr ) );
}

void StateCheckAudio::Destroy( )
{
    BaseGameState::Destroy( );
}

void StateCheckAudio::EnterState( GameState oldState )
{
    BaseGameState::EnterState( oldState );
    
    // reset our output
    memset( mResultStr, 0, sizeof( mResultStr ) );
}

void StateCheckAudio::ExitState( GameState newState )
{
    BaseGameState::ExitState( newState );
    
    TryStopAudio( );
}

GameState StateCheckAudio::Update( )
{
    char text[ MAX_STR_SIZE ] = { 0 }; 
    
    Renderer::DrawText( "Press P1 Start to Exit", 
                        305, // centers it on screen
                        UI_Y_POS_HEADER, 
                        0xFFFF );
                        
    Renderer::DrawText( "P1 Button 1 to Play, P1 Button 2 to Pause, P1 Button 3 to Stop", 
                        106, 
                        UI_Y_POS_MENU, 
                        0xFFFF );
                        
    int yPos = UI_Y_POS_MENU + UI_ROW_HEIGHT;
    snprintf( text, sizeof( text ) - 1, "File: %s", GameConfig::GetAudioFile( ) );
    Renderer::DrawText( text, 
                        106, 
                        yPos, 
                        0xFFFF );
                        
    // Render audio playback state
    yPos += UI_ROW_HEIGHT;                   
    if( AudioManager::IsPlaying( ) )
    {
        if( AudioManager::IsPaused( ) )
        {
            snprintf( text, sizeof( text ) - 1, "Audio Status: Paused" );
        }
        else
        {
            snprintf( text, sizeof( text ) - 1, "Audio Status: Playing" );
        }
    }
    else
    {
        snprintf( text, sizeof( text ) - 1, "Audio Status: Stopped" );
    }
    Renderer::DrawText( text, 
                        106, 
                        yPos, 
                        0xFFFF );
    
    // Render volume
    yPos += UI_ROW_HEIGHT;
    
    int volPerc = ((float) OSAudio::GetVolume( ) / SPEAKER_MAX_VALUE) * 100;
    snprintf( text, sizeof( text ) - 1, "Volume: %d%%, Real Val: %d", volPerc, OSAudio::GetVolume() );
    Renderer::DrawText( text, 
                        106, 
                        yPos, 
                        0xFFFF );
                        
    // Render result
    yPos += UI_ROW_HEIGHT;
    
    Renderer::DrawText( mResultStr, 
                        106, 
                        yPos, 
                        0xFFFF );
    
    // Toggle playback
    if( GameInput::GetInput( P1_Button_2 )->WasReleased( ) )
    {
        if( mWaveFile.pFile != NULL )
        {
            AudioManager::PauseAudio( !AudioManager::IsPaused( ) );
        }
    }
    
    if( GameInput::GetInput( P1_Button_1 )->WasReleased( ) )
    {
        TryPlayAudio( );
    }
    
    if( GameInput::GetInput( P1_Button_3 )->WasReleased( ) )
    {
        TryStopAudio( );
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

void StateCheckAudio::TryPlayAudio( )
{
    if( AudioManager::IsPlaying( ) == 0 )
    {
        int result = 0;
#ifndef DEVELOP_MODE //In develop mode, dont try mounting because it wont work.
        result = OSMount::MountUSBPartition( PROCESS_MOUNT_PATH, ROOT_PARTITION_INDEX );
        if ( result == -1 )
        {
            snprintf( mResultStr, sizeof( mResultStr ), "Mounting %s failed. Wait a sec and try again.", PROCESS_MOUNT_PATH );
            return;
        }
#endif
        
        // reset the result string
        mResultStr[0] = 0;
        
        // refresh the config settings
        GameConfig::ReadConfigFile( );
        
        result = WaveLoader::LoadWave( GameConfig::GetAudioFile( ), &mWaveFile );
        if ( result == -1 )
        {
            snprintf( mResultStr, sizeof( mResultStr ), "Failed to load audio file %s", GameConfig::GetAudioFile( ) );
            return;
        }
        
        // reset the result string
        mResultStr[0] = 0;
        
        if( mWaveFile.pFile != NULL )
        {
            AudioManager::PlayAudio( &mWaveFile );
        }
    }
}

void StateCheckAudio::TryStopAudio( )
{
    if( AudioManager::IsPlaying( ) == 1 )
    {
        AudioManager::StopAudio( );
    }
    
    if ( mWaveFile.pFile != NULL )
    {
        fclose( mWaveFile.pFile );
        memset( &mWaveFile, 0, sizeof( mWaveFile ) );
    }
}
