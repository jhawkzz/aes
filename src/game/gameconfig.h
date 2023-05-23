
// See License.md for license

#ifndef GAMECONFIG_H_
#define GAMECONFIG_H_

#define EXEC_KEY  "exec"
#define AUDIO_KEY "audio"

class GameConfig
{
public:
    static       void  Create( );
    static       void  Destroy( );
    
    static       int   ReadConfigFile( );
    
    static const char *GetExec( );
    static const char *GetAudioFile( );
    
private:
    static void ParseValueForKey( const char *pLineBuffer );
    static void AssignValueForKey( const char *pKey, const char *pValue );
                 
    static char mExecFile[ MAX_STR_SIZE ];
    static char mAudioFile[ MAX_STR_SIZE ];
};

#endif
