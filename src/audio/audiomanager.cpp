
// See License.md for license

#include "includes.h"

int        AudioManager::mPaused;
WAVE_FILE *AudioManager::mpWaveFile;
char       AudioManager::mFileBuffer[ FILE_BUFFER_SIZE ];
RingBuffer AudioManager::mFileRingBuffer;

//FILE *gFile = 0;
   
int AudioManager::Create( )
{
    /*WAVE_FILE waveFile;
    WaveLoader::LoadWave( ASSET_PATH"/kraidstereo.wav", &waveFile );
    AudioManager::PlayAudio( &waveFile );*/
}

int AudioManager::Destroy( )
{
}

void AudioManager::PlayAudio( WAVE_FILE *pWaveFile )
{
    if( mpWaveFile == NULL )
    {
        // Debug Dump
        /*gFile = fopen( "~/Desktop/output.wav", "wb" );
        WAVE_HEADER header    = { 0 };
        header.riffTag[ 0 ]   = 'R'; header.riffTag[ 1 ] = 'I'; header.riffTag[ 2 ] = 'F'; header.riffTag[ 3 ] = 'F';
        header.wavTag[ 0 ]    = 'W'; header.wavTag[ 1 ] = 'A'; header.wavTag[ 2 ] = 'V'; header.wavTag[ 3 ] = 'E';
        header.fileSize       = 7902996;
        header.fmtTag[0]      = 'f'; header.fmtTag[1] = 'm'; header.fmtTag[2] = 't'; header.fmtTag[3] = ' ';
        header.fmtTagLen      = 16;
        header.wavFormat      = 1;
        header.numChannels    = 2;
        header.samplesPerSec  = 44100;
        header.avgBytesPerSec = 176400;
        header.blockAlign     = 4;
        header.bitsPerSample  = 16;
        header.dataTag[0]     = 'd';
        header.dataTag[1]     = 'a';
        header.dataTag[2]     = 't';
        header.dataTag[3]     = 'a';
        header.dataSectionSizeBytes = 7902960;
        fwrite( &header, 1, sizeof( header), gFile );*/
        //
        
        mpWaveFile = pWaveFile;
        mPaused    = 0;
        
        // go to the start of data
        fseek( mpWaveFile->pFile, sizeof( WAVE_HEADER ), SEEK_SET );
        
        // reset our ring buffer
        mFileRingBuffer.Reset( );

        // buffer the first chunk of audio
        int bytesRead = fread( mFileBuffer, 1, sizeof( mFileBuffer ), mpWaveFile->pFile );
        mFileRingBuffer.FillBuffer( mFileBuffer, bytesRead );   
    }
}

void AudioManager::PauseAudio( int pause )
{
    mPaused = pause;
}

int AudioManager::StopAudio( )
{
    PauseAudio( 0 );
    mpWaveFile = NULL;
}

int AudioManager::IsPlaying( )
{
    if( mpWaveFile != NULL )
    {
        return 1;
    }
    
    return 0;
}

int AudioManager::IsPaused( )
{
    return mPaused;
}

void AudioManager::Update( )
{
    if( mpWaveFile && mPaused == 0 )
    {   
        // feed the audio to OSAudio at a constant rate
        if( OSAudio::NeedBufferRefill( ) == 0 )
        {
            return;
        }
        
        char writeBuffer[ SAMPLES_PER_TICK * SAMPLE_BLOCK_ALIGN ] = { 0 };
        int bytesRead = mFileRingBuffer.ReadBuffer( writeBuffer, sizeof( writeBuffer ) );
        
        OSAudio::PlayBuffer( writeBuffer, bytesRead );
        
        //int bytesWritten = fwrite( writeBuffer, 1, bytesRead, gFile );
        //flushPrintf( "Read %d bytes from file buffer, wrote %d bytes to file\r\n", bytesRead, bytesWritten );
        
        if ( bytesRead == 0 )
        {
            //flushPrintf( "Reached end of audio track!\r\n" );
            
            //fclose( gFile );
            //exit( 0 );
            
            StopAudio( );
        }
        else
        {
            // when our ring buffer falls below 25%, refill it
            int bufferThreshold = mFileRingBuffer.GetCapacity() / 4;
            
            if( mFileRingBuffer.GetBytesFree() >= bufferThreshold )
            {
                int bytesRead = fread( mFileBuffer, 1, mFileRingBuffer.GetBytesFree(), mpWaveFile->pFile );
                if ( bytesRead > 0 )
                { 
                    int bytesFilled = mFileRingBuffer.FillBuffer( mFileBuffer, bytesRead );
                }
            }
        }
    }
}
