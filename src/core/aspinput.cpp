
// See License.md for license
#ifdef ASP
#include "includes.h"

ASPInput      OSInput::mASPInputLookup[ Button_Count ];
ASPInputState OSInput::mASPInputState[ ASPInput_Count ];
int           OSInput::mInputFileHandle;
int           OSInput::mThreadRunning;

int OSInput::Create( )
{
	mInputFileHandle = open( ASP_INTEGRATED_DEVICE, O_RDONLY );
	if( mInputFileHandle < 0 )
	{
		flushPrintf( "Failed to open ASP_INTEGRATED_DEVICE\r\n" );
        
        Destroy( );
        return -1;
	}
    
    // setup all our inputs
    for( int i = 0; i < ASPInput_Count; i++ )
    {
        mASPInputState[ i ].value = 0;
        pthread_mutex_init( &mASPInputState[ i ].mutexLock, NULL );
        mASPInputState[ i ].mutexCreated = 1;
    }
    
    CreateLookup( );
	
    // start the polling thread
	pthread_t t;
	int result = pthread_create( &t, NULL, PollInput_ThreadProc, NULL );
	if( result != 0 )
	{
        flushPrintf( "Failed to start Poll Thread\r\n" );
        
        Destroy( );
        return -1;
	}
    
    pthread_detach( t );
    
    return 0;
}

void OSInput::Destroy( )
{
    for( int i = 0; i < ASPInput_Count; i++ )
    {
        if ( mASPInputState[ i ].mutexCreated == 1 )
        {
            pthread_mutex_lock( &mASPInputState[ i ].mutexLock );
            pthread_mutex_unlock( &mASPInputState[ i ].mutexLock );
            pthread_mutex_destroy( &mASPInputState[ i ].mutexLock );
            
            mASPInputState[ i ].mutexCreated = 0;
        }
    }
    
    if( mInputFileHandle > -1 )
    {
        close( mInputFileHandle );
        mInputFileHandle = -1;
    }
    
    mThreadRunning = 0;
}

void OSInput::CreateLookup( )
{
    mASPInputLookup[ P1_Joy_Up     ] = ASPInput_P1_Joy_Up;
    mASPInputLookup[ P1_Joy_Left   ] = ASPInput_P1_Joy_Left,
    mASPInputLookup[ P1_Joy_Right  ] = ASPInput_P1_Joy_Right,
    mASPInputLookup[ P1_Joy_Down   ] = ASPInput_P1_Joy_Down,
    
    mASPInputLookup[ P1_Button_1   ] = ASPInput_RedA;
    mASPInputLookup[ P1_Button_2   ] = ASPInput_GreenC;
    mASPInputLookup[ P1_Button_3   ] = ASPInput_TopGray;
    mASPInputLookup[ P1_Button_4   ] = ASPInput_YellowB;
    mASPInputLookup[ P1_Button_5   ] = ASPInput_BlueD;
    mASPInputLookup[ P1_Button_6   ] = ASPInput_BotGray;
    
    mASPInputLookup[ P1_InsertCoin ] = ASPInput_Options;
    mASPInputLookup[ P1_Start      ] = ASPInput_Start;
    
    mASPInputLookup[ P2_Start      ] = ASPInput_TopWhite;
    mASPInputLookup[ P2_Joy_Left   ] = ASPInput_BotWhite;
    
    mASPInputLookup[ P2_InsertCoin ] = ASPInput_Select;
}

void *OSInput::PollInput_ThreadProc(void *data)
{
    mThreadRunning = 1;
	
    while( mThreadRunning )
	{
        pollfd inputState;
		memset( &inputState, 0, sizeof( pollfd ) );
        
		inputState.fd = mInputFileHandle;
		inputState.events = POLLIN;
		inputState.revents = 0;
		int ret = poll( &inputState, 1, INPUT_POLL_TIMEOUT );
		if( ret >= 0 )
		{
			ReadInputs( );
		}
        else
        {
            flushPrintf( "Failed to poll input. Skipping read inputs, will try again next iteration.\r\n" );
        }
	}
    
    return NULL;
}

void OSInput::ReadInputs( )
{
	input_event input;
	int ret = read( mInputFileHandle, &input, sizeof( input_event ) );
	if(ret < 0)
	{
		flushPrintf( "ReadInputs( ) Failed\r\n" );
        return;
	}
    
    // Use this to learn what inputs are what code
    // flushPrintf( "Input: code: %d, value: %d, type: %d\r\n", input.code, input.value, input.type );
    
    // guard against inputs out of range
    if( input.code >= ASPInput_P1_Joy_Up && input.code < ASPInput_Count )
    {
        int buttonIndex = input.code;
        
        // quickly lock, set the value, release
        pthread_mutex_lock( &mASPInputState[ buttonIndex ].mutexLock );
        
        mASPInputState[ buttonIndex ].value = input.value;
        
        pthread_mutex_unlock( &mASPInputState[ buttonIndex ].mutexLock );
    }
}

int OSInput::GetValueForInput( ButtonInput buttonInput )
{
    // quickly lock around the value and copy it out
    int inputVal = 0;
    
    ASPInput input = mASPInputLookup[ buttonInput ];
    
    pthread_mutex_lock( &mASPInputState[ input ].mutexLock );
    inputVal = mASPInputState[ input ].value;
    pthread_mutex_unlock( &mASPInputState[ input ].mutexLock );
    
    return inputVal;
}
#endif