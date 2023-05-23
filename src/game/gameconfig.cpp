
// See License.md for license

#include "includes.h"

char GameConfig::mExecFile[ MAX_STR_SIZE ];
char GameConfig::mAudioFile[ MAX_STR_SIZE ];
    
void GameConfig::Create( )
{
}

void GameConfig::Destroy( )
{
}

int GameConfig::ReadConfigFile( )
{
    FILE *pFile = fopen( PROCESS_MOUNT_PATH"/config.ini", "r" );
    if( pFile != NULL )
    {
        char lineBuffer[ MAX_STR_SIZE ];
        while( fgets(lineBuffer, sizeof( lineBuffer ) - 1, pFile) )
        {
            // strip newlines
            lineBuffer[strcspn(lineBuffer, "\r\n")] = 0;
            
            ParseValueForKey( lineBuffer );
        }
        
        fclose( pFile );
        pFile = NULL;
    }
    else
    {
        flushPrintf( "Could not load " PROCESS_MOUNT_PATH"/config.ini\r\n" );
    }
}

const char *GameConfig::GetExec( )
{
    return mExecFile;
}

const char *GameConfig::GetAudioFile( )
{
    return mAudioFile;
}

void GameConfig::ParseValueForKey( const char *pLineBuffer )
{
    // look for the delimiter
    const char *pDelim = strstr( pLineBuffer, "=" );
    
    // get the key
    if( pDelim != NULL )
    {
        char keyStr[ MAX_STR_SIZE ] = { 0 };
        memcpy( keyStr, pLineBuffer, (int)(pDelim - pLineBuffer) );
        
        // get the value
        pDelim++; //skip past the '=' and we're there
        
        // copy the remaining string content
        char valueStr[ MAX_STR_SIZE ] = { 0 };
        strncpy( valueStr, pDelim, sizeof( valueStr ) - 1 );
        
        AssignValueForKey( keyStr, valueStr );
    }
}

void GameConfig::AssignValueForKey( const char *pKey, const char *pValue )
{
    // lowercase the key
    char lowerKey[ MAX_STR_SIZE ] = { 0 };
    int i = 0;
    for( i = 0; pKey[ i ] != 0; i++ )
    {
        lowerKey[ i ] = tolower( pKey[ i ] );
    }
    
    // assign to the right value
    if( strcmp( lowerKey, EXEC_KEY ) == 0 )
    {
        memset( mExecFile, 0, sizeof( mExecFile ) );
        strncpy( mExecFile, pValue, sizeof( mExecFile ) - 1 );
    }
    
    if( strcmp( lowerKey, AUDIO_KEY ) == 0 )
    {
        memset( mAudioFile, 0, sizeof( mAudioFile ) );
        strncpy( mAudioFile, pValue, sizeof( mAudioFile ) - 1 );
    }
}
