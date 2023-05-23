
// See License.md for license

#include "includes.h"

int             OSAudio::mDspHandle;
RingBuffer      OSAudio::mDspRingBuffer;
pthread_mutex_t OSAudio::mDspMutexLock;
int             OSAudio::mDspMutexCreated;
int             OSAudio::mDspThreadRunning;
int             OSAudio::mVolume;
int             OSAudio::mVolumeThreadRunning;

int OSAudio::Create( )
{
	mDspHandle = open( SOUND_DEVICE, O_WRONLY );
	if( mDspHandle < 0 )
	{
        flushPrintf( "OSAudio::Create() Failed to open sound device!\r\n" );
        
        Destroy( );
		return -1;
	}

	// see http://manuals.opensound.com/developer/SNDCTL_DSP_SETFRAGMENT.html
    // the high 16 bits store the number of buffer fragments. The low 16 bits
    // is a selector that maps to a buffer size. There's no guarantee
    // the driver will respect it, but we try.
    int numFragments   = 10;
    int prefBufferSize = 11; //11 Maps to 2048
    int fragBufferHint = (numFragments << 16)  | prefBufferSize;
    
	int result = ioctl(mDspHandle, SNDCTL_DSP_SETFRAGMENT, &fragBufferHint );
    if( result < 0 )
    {
        flushPrintf( "OSAudio::Create() SNDCTL_DSP_SETFRAGMENT failed. Errno: %d\r\n", errno );
        
        Destroy( );
		return -1;
    }

	int muteVal = 0;
	result = ioctl( mDspHandle, SOUND_MIXER_WRITE_MUTE, &muteVal );
    if( result < 0 )
    {
        flushPrintf( "OSAudio::Create() SOUND_MIXER_WRITE_MUTE failed. Errno: %d\r\n", errno );
        
        Destroy( );
        return -1;
    }
    
    // Configure Speaker State
    // These setup audio to go out via speaker vs HDMI
#ifdef MVSX
    int sOutMode = SNDRV_OUTMODE_SPEAKER;
#elif ASP
    int sOutMode = SNDRV_OUTMODE_HDMI;
#endif
	result = ioctl(mDspHandle, SNDRV_OUTMODE, &sOutMode);
    if( result < 0 )
    {
        flushPrintf( "OSAudio::Create() SNDRV_SOUTMODE failed. Errno: %d\r\n", errno );
        
        Destroy( );
        return -1;
    }
    
#ifdef MVSX
    int sh2DeviceState = SNDRV_SH2WDEVICE_SPEAKER;
#elif ASP
    int sh2DeviceState = SNDRV_SH2WDEVICE_HDMI;
#endif
	ioctl(mDspHandle, SNDRV_SH2WDEVICE, &sh2DeviceState);
    if( result < 0 )
    {
        flushPrintf( "OSAudio::Create() SNDRV_SH2WDEVICE failed. Errno: %d\r\n", errno );
        
        Destroy( );
        return -1;
    }
    
#ifdef MVSX
    int hwInitVal = SNDRV_PLAY_HWINIT_SPEAKER;
#elif ASP
    int hwInitVal = SNDRV_PLAY_HWINIT_HDMI;
#endif
	ioctl(mDspHandle, SNDRV_PLAY_HWINIT, &hwInitVal);
    if( result < 0 )
    {
        flushPrintf( "OSAudio::Create() SNDRV_PLAY_HWINIT failed. Errno: %d\r\n", errno );
        
        Destroy( );
        return -1;
    }
    
    // This one turns ON said speaker
#ifdef MVSX
    int speakerOn = SNDRV_SPEAKER_ON;
	ioctl(mDspHandle, SNDRV_SPEAKER, &speakerOn);
    if( result < 0 )
    {
        flushPrintf( "OSAudio::Create() SNDRV_SSPEAKER failed. Errno: %d\r\n", errno );
        
        Destroy( );
        return -1;
    }
#endif
    // Configure playback rate
    // This doesnt appear to work and doesn't seem to matter, so i
    // assume the default matches what we need
    /*int bitsPerSample = SAMPLE_BITS_PER_SAMPLE;
    result = ioctl( mDspHandle, SNDCTL_DSP_SAMPLESIZE, &bitsPerSample );
    if( result < 0 )
    {
        flushPrintf( "OSAudio::Create() SNDCTL_DSP_SAMPLESIZE failed. Errno: %d\r\n", errno );
    }*/

    int channels = SAMPLE_NUM_CHANNELS;
	result = ioctl( mDspHandle, SNDCTL_DSP_CHANNELS, &channels );
    if ( result < 0 )
    {
        flushPrintf( "OSAudio::Create() SNDCTL_DSP_CHANNELS: %d\r\n", errno );
        
        Destroy( );
        return -1;
    }
    
    int sampleRate = SAMPLE_RATE;
	result = ioctl( mDspHandle, SNDCTL_DSP_SPEED, &sampleRate );
    if( result < 0 )
    {
        flushPrintf( "OSAudio::Create() SNDCTL_DSP_SPEED: %d\r\n", errno );
        
        Destroy( );
        return -1;
    }
        
    // Setup audio and volume threads
    pthread_mutex_init( &mDspMutexLock, NULL );
    mDspMutexCreated = 1;
    
    pthread_t audioT;
    result = pthread_create( &audioT, NULL, UpdateAudio_ThreadProc, NULL );
    if( result != 0 )
    {
        flushPrintf( "OSAudio::Create() pthread_create failed with error: %d\r\n", result );
        
        Destroy( );
        return -1;
    }
    
    pthread_detach( audioT );

#ifdef MVSX
    pthread_t volumeT;
    result = pthread_create( &volumeT, NULL, UpdateVolume_ThreadProc, NULL );
    if( result != 0 )
    {
        flushPrintf( "OSAudio::Create() pthread_create failed with error: %d\r\n", result );
        
        Destroy( );
        return -1;
    }
    
    pthread_detach( volumeT );
#endif

    return 0;
}

void OSAudio::Destroy( )
{
    if( mDspHandle > 0 )
    {
        close( mDspHandle );
        mDspHandle = -1;
    }
    
    if( mDspMutexCreated )
    {
        pthread_mutex_lock( &mDspMutexLock );
        pthread_mutex_unlock( &mDspMutexLock );
        pthread_mutex_destroy( &mDspMutexLock );
        
        mDspMutexCreated = 0;
    }
    
    mDspThreadRunning    = 0;
    mVolumeThreadRunning = 0;
}

void OSAudio::PlayBuffer( char *pBuffer, int bytes )
{
    pthread_mutex_lock( &mDspMutexLock );
    
    mDspRingBuffer.FillBuffer( pBuffer, bytes );
    
    pthread_mutex_unlock( &mDspMutexLock );
}

int OSAudio::NeedBufferRefill( )
{
    pthread_mutex_lock( &mDspMutexLock );
    
    int bytes = mDspRingBuffer.GetBytesFilled( );
    
    pthread_mutex_unlock( &mDspMutexLock );
    
    // if the buffer is empty, we do need more data
    if( bytes == 0 )
    {
        return 1;
    }
    
    return 0;
}

int OSAudio::GetVolume( )
{
    return mVolume;
}

void *OSAudio::UpdateAudio_ThreadProc( void *pArg)
{
    mDspThreadRunning = 1;
    
    while( mDspThreadRunning )
    {
        pthread_mutex_lock( &mDspMutexLock );
        
        if( mDspRingBuffer.GetBytesFilled( ) > 0 )
        {
            char buffer[ SAMPLES_PER_TICK * SAMPLE_BLOCK_ALIGN ] = { 0 };
            int bytesRead = mDspRingBuffer.ReadBuffer( buffer, sizeof( buffer ) );
            
            int bytesWritten = write( mDspHandle, buffer, bytesRead);
            
            pthread_mutex_unlock( &mDspMutexLock );
        }
        else
        {
            // if our buffer is empty, sleep until we're refilled
            pthread_mutex_unlock( &mDspMutexLock );
            usleep( 1000 );
        }
    }
    
    return NULL;
}

void *OSAudio::UpdateVolume_ThreadProc( void *pArg )
{
    mVolumeThreadRunning = 1;
    
    while( mVolumeThreadRunning )
    {   
        // we have to open / close each loop iteration so we 
        // dont hold a lock and prevent the knob from updating.
        int volumeHandle = open( VOLUME_DEVICE, O_RDONLY );
        if( volumeHandle > -1 )
        {
            // poll the volume device 
            char buffer[ 32 ] = { 0 };
            int result = read( volumeHandle, buffer, sizeof( buffer ) );
            if( result > 0 )
            {
                int rawVolumeVal = atoi( buffer );
                
                // Volume is held in reverse, so flip, then convert to %, then scale to speaker vals
                float inverseVolume = (float) VOLUME_KNOB_MAX_VALUE - rawVolumeVal;
                float volPerc       = inverseVolume / VOLUME_KNOB_MAX_VALUE;
                int newVolume       = (int) (volPerc * SPEAKER_MAX_VALUE);
                
                if ( newVolume != mVolume )
                {
                    mVolume = min( newVolume, SPEAKER_MAX_VALUE );
                    
                    if ( ioctl( mDspHandle, SOUND_MIXER_WRITE_VOLUME, &mVolume ) == -1) 
                    {
                        flushPrintf("OSAudio::UpdateVolume_ThreadProc() SOUND_MIXER_WRITE_VOLUME failed!\r\n");
                    }
                }
            }
            
            close( volumeHandle );
        }
        else
        {
            flushPrintf( "OSAudio::UpdateVolume_ThreadProc() Failed to open volume handle!\r\n" );
        }
        
        usleep( 200 * MILLI_TO_MICROSECONDS );
    }
}
