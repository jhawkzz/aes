
// See License.md for license

#include "includes.h"

long gFrameTimeMS    = 0;
long gOldFrameTimeMS = 0;
long gFrameCount     = 0;

int main( )
{
    // mount ourselves immediately
    OSMount::MountUSBPartition( PROCESS_MOUNT_PATH, ROOT_PARTITION_INDEX );
    
    if ( OSProcess::Create( ) == -1 )
    {
        flushPrintf( "OSProcess::Create Error!\r\n" );
    }
    
	if( OSFrameBuffer::Create( ) == -1 )
	{
		flushPrintf( "OSFrameBuffer::Create Error!\r\n" );		
	}
	
	if( Renderer::Create( ) == -1 )
    {
        flushPrintf( "Renderer::Create Error!\r\n" );
    }

    if( OSInput::Create( ) == -1 )
    {
        flushPrintf( "OSInput::Create Error!\r\n" );
    }
    
    if( OSLed::Create( ) == -1 )
    {
        flushPrintf( "OSLED::Create Error!\r\n" );
    }
    
    if( OSAudio::Create( ) == -1 )
    {
        flushPrintf( "OSAudio::Create Error!\r\n" );
    }
    
    if( AudioManager::Create( ) == -1 )
    {
        flushPrintf( "AudioManager::Create Error!\r\n" );
    }
    
    if( Game::Create( ) == -1 )
    {
        flushPrintf( "Game::Create Error!\r\n" );
    }
        
    if( OSTimer::Create( ) == -1 )
    {
        flushPrintf( "OSTimer::Create Error!\r\n" );
    }
    
    float tickRate  = 1.0f / FRAMES_PER_SECOND;
	long tickRateMS = (long)(1000 * tickRate);
    
    gFrameTimeMS    = OSTimer::GetTotalTimeMS( );
    gFrameCount     = 0;
    gOldFrameTimeMS = 0;
    
    int gameRunning = 1;
    
	while( gameRunning == 1 )
	{
        OSTimer::Update( );
        
        gOldFrameTimeMS = gFrameTimeMS;
        
        while( gFrameTimeMS <= OSTimer::GetTotalTimeMS( ) )
        {
            // reset the renderlist each time we update
            // so we only render the final frame
            Renderer::ResetForFrame( );
            
            if ( Game::Update( ) == 0 )
            {
                gameRunning = 0;
                break;
            }
            
            AudioManager::Update( );
            
            gFrameCount++;
            
            gFrameTimeMS += tickRateMS;
        }
        
        if( gOldFrameTimeMS != gFrameTimeMS )
        {
            Renderer::Render( );
            Renderer::Flip( );
        }
        
        usleep( 0 );
	}
    
    OSTimer::Destroy( );
    Game::Destroy( );
    AudioManager::Destroy( );
    OSAudio::Destroy( );
    OSLed::Destroy( );
    OSInput::Destroy( );
    Renderer::Destroy( );
    OSFrameBuffer::Destroy( );
    OSProcess::Destroy( );
}
