
// See License.md for license

#ifndef AUDIOMANAGER_H_
#define AUDIOMANAGER_H_

#define FILE_BUFFER_SIZE (1024 * 1024)

class AudioManager
{
public:
    static int  Create( );
    static int  Destroy( );
    
    static void PlayAudio( WAVE_FILE *pWaveFile );
    static void PauseAudio( int pause );
    static int  StopAudio( );
    
    static int  IsPlaying( );
    static int  IsPaused( );
    
    static void Update( );
    
private:

    static int        mPaused;
    static WAVE_FILE *mpWaveFile;
    static char       mFileBuffer[ FILE_BUFFER_SIZE ];
    static RingBuffer mFileRingBuffer;
    
};

#endif
