
// See License.md for license

#include "includes.h"

int OSLed::mP1Led;
int OSLed::mP2Led;
int OSLed::mLedHandle;
int OSLed::mNeedsUpdate;
int OSLed::mThreadRunning;
pthread_mutex_t OSLed::mMutexLock;
int             OSLed::mMutexCreated;

int OSLed::Create( )
{
    // init to values that are guaranteed not to be set already.
    mP1Led = 0xDEADBEEF;
    mP2Led = 0xDEADBEEF;
    
    mLedHandle = open( LED_PATH, O_WRONLY );
    if( mLedHandle < 0 )
    {
        flushPrintf( "OSLed::Create() Failed to open LED device\r\n" );
        
        Destroy( );
        return -1;
    }
    
    pthread_mutex_init( &mMutexLock, NULL );
    mMutexCreated = 1;
    
    pthread_t t;
    int result = pthread_create( &t, NULL, UpdateLEDVals_ThreadProc, NULL );
    if( result != 0 )
    {
        flushPrintf( "OSLed::Create() pthread_create failed with error: %d\r\n", result );
        
        Destroy( );
        return -1;
    }
    
    pthread_detach( t );
    
    return 0;
}

void OSLed::Destroy( )
{
    if ( mLedHandle > -1 )
    {
        close( mLedHandle );
        mLedHandle = -1;
    }
    
    if( mMutexCreated == 1 )
    {
        pthread_mutex_lock( &mMutexLock );
        pthread_mutex_unlock( &mMutexLock );
        pthread_mutex_destroy( &mMutexLock );
        
        mMutexCreated = 0;
    }
    
    mThreadRunning = 0;
}

void OSLed::SetLED( int *pP1Led, int *pP2Led )
{
    // our goal here is to only update the LED vals
    // if they're different from what's already set.
    // We also want to minimize the amount of time we lock
    // the mutex to prevent any stalls.
    
    // For each LED, default the value to set to be what's already there,
    // and assume we won't update it.
    int p1NeedsUpdate = 0;
    int p1ValToSet = mP1Led;
    
    int p2NeedsUpdate = 0;
    int p2ValToSet = mP2Led;

    // Next check for a new value to be provided, and if so take it.
    if( pP1Led != NULL && mP1Led != *pP1Led )
    {
        p1NeedsUpdate = 1;
        p1ValToSet = *pP1Led;
    }
    
    if( pP2Led != NULL && mP2Led != *pP2Led )
    {
        p2NeedsUpdate = 1;
        p2ValToSet = *pP2Led;
    }
    
    // Finally if either value is new, lock, and copy BOTH values over.
    // This allows us to safely set both values, because we know it'll
    // either be the same as it already was, or a new value, but at least ONE
    // of the two IS new.
    //
    // We do this so we can do two quick copies with a locked mutex
    // vs running conditional insructions that could be more time consuming.
    if ( p1NeedsUpdate == 1 || p2NeedsUpdate == 1 )
    {
        pthread_mutex_lock( &mMutexLock );
        
        mNeedsUpdate = 1;
        
        mP1Led = p1ValToSet;
        mP2Led = p2ValToSet;
        
        pthread_mutex_unlock( &mMutexLock );
    }
}

void *OSLed::UpdateLEDVals_ThreadProc( void *pArg )
{
    mThreadRunning = 1;
    
    while( mThreadRunning )
    {
        pthread_mutex_lock( &mMutexLock );
        if( mNeedsUpdate )
        {
            char ledVals[ MAX_STR_SIZE ] = { 0 };
            snprintf( ledVals, sizeof( ledVals ) -1, "%d,%d", mP1Led, mP2Led );
            
            int bytesWritten = write( mLedHandle, ledVals, strlen( ledVals ) );
            
            mNeedsUpdate = 0;
        }
        pthread_mutex_unlock( &mMutexLock );
        
        usleep( 50 * MILLI_TO_MICROSECONDS );
    }
    
    return NULL;
}
