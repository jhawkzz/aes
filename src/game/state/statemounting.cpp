
// See License.md for license

#include "includes.h"

void StateMounting::Create( )
{
    BaseGameState::Create( );
    
    int xPos     = UI_X_POS_MENU;
    mMenuEndYPos = UI_Y_POS_MENU;
    
    mMenuItem[ 0 ].Create( "1. Unmount USB Partition", xPos, mMenuEndYPos, 0xFFFF );
    
    mMenuEndYPos += UI_ROW_HEIGHT;
    mMenuItem[ 1 ].Create( "2. Scan For USB Changes", xPos, mMenuEndYPos, 0xFFFF );
    
    mMenuEndYPos += UI_ROW_HEIGHT;
    mMenuItem[ 2 ].Create( "3. Mount USB Partition", xPos, mMenuEndYPos, 0xFFFF );
    
    mMenuEndYPos += UI_ROW_HEIGHT;
    mMenuItem[ 3 ].Create( "4. List USB Device Files", xPos, mMenuEndYPos, 0xFFFF );
    
    mMenuEndYPos += UI_ROW_HEIGHT;
    mMenuItem[ 4 ].Create( "5. List Mounts", xPos, mMenuEndYPos, 0xFFFF );
        
    mMenuSelection = 0;
    
    memset( mResultStr, 0, sizeof( mResultStr ) );
    
    // allocate the chunk we'll need for output
    mOutputBufferNumLines = 0;
    mpOutputBuffer = new char[ MAX_STR_SIZE * MAX_LINES_IN_BUFFER ];
    memset( mpOutputBuffer, 0, sizeof( MAX_STR_SIZE * MAX_LINES_IN_BUFFER ) );
}

void StateMounting::Destroy( )
{
    BaseGameState::Destroy( );
    
    delete [] mpOutputBuffer;
    mpOutputBuffer = NULL;
    mOutputBufferNumLines = 0;
}

void StateMounting::EnterState( GameState oldState )
{
    BaseGameState::EnterState( oldState );
        
    // reset output so it isn't confusing
    mOutputBufferNumLines = 0;
    memset( mResultStr, 0, sizeof( mResultStr ) );
}

void StateMounting::ExitState( GameState newState )
{
    BaseGameState::ExitState( newState );
}
    
GameState StateMounting::Update( )
{
    BaseGameState::Update( );
    
    DrawMenu( );
    
    // check for menu navigation
    if( GameInput::GetInput( P1_Joy_Down )->WasReleased() )
    {
        mMenuSelection = min( MOUNTING_ITEMS - 1, mMenuSelection + 1 );
    }
    else if( GameInput::GetInput( P1_Joy_Up )->WasReleased() )
    {
        mMenuSelection = max( 0, mMenuSelection - 1 );
    }
    
    if( GameInput::GetInput( P1_Button_1 )->WasReleased( ) )
    {
        DoSelectedMountOption( mMenuSelection );
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

void StateMounting::DrawMenu( )
{
    Renderer::DrawText( "Press P1 Start to Exit", 
                        305, // centers it on screen
                        UI_Y_POS_HEADER, 
                        0xFFFF );
    
    int i = 0;
    for( i = 0; i < MOUNTING_ITEMS; i++ )
    {
        mMenuItem[ i ].Draw( );
    }
    
    Renderer::DrawText( "X", 
                        mMenuItem[ mMenuSelection ].GetXPos( ) - UI_CURSOR_X_OFFSET, 
                        mMenuItem[ mMenuSelection ].GetYPos( ),
                        0XFFEA );
    
    int menuBottomYPos = mMenuItem[ MOUNTING_ITEMS - 1 ].GetYPos( );
    Renderer::DrawText( mResultStr, 
                        UI_X_POS_MENU, 
                        menuBottomYPos + UI_ROW_HEIGHT, 
                        0xFFFF );
    
    DrawLineBuffer( );
}

void StateMounting::DoSelectedMountOption( int selectedOption )
{
    memset( mResultStr, 0, sizeof( mResultStr ) );
    
    switch( selectedOption )
    {
        case 0: UnmountDevice( );         break;
        case 1: BusyBoxScanForChanges( ); break;
        case 2: MountDevice( );           break;
        case 3: ListUSBDevs( 1 );         break;
        case 4: ListMounts( );            break;
    }
}

void StateMounting::BusyBoxScanForChanges( )
{
    ResetLineBuffer( );
    
    AddLineToBuffer( "Devs Before Scan" );
    ListUSBDevs( 0 );
    AddLineToBuffer( "" );
    AddLineToBuffer( "Devs After Scan" );
    ListUSBDevs( 0 );
}

void StateMounting::MountDevice( )
{
    if( OSMount::IsMountPointActive( PROCESS_MOUNT_PATH ) == 0 )
    {
        int result = OSMount::MountUSBPartition( PROCESS_MOUNT_PATH, ROOT_PARTITION_INDEX );
        if ( result == -1 )
        {
            snprintf( mResultStr, 
                      sizeof( mResultStr ), 
                      "Error mounting %s. Stopping.", 
                      PROCESS_MOUNT_PATH );
        }
    }
    else
    {
        snprintf( mResultStr, 
                  sizeof( mResultStr ) - 1, 
                  "MountDevice: Mount Point Not Active: %s", 
                  PROCESS_MOUNT_PATH );
    }
}

void StateMounting::UnmountDevice( )
{
    if( OSMount::IsMountPointActive( PROCESS_MOUNT_PATH ) == 1 )
    {
        int result = OSMount::UnmountDevice( PROCESS_MOUNT_PATH );
        if( result == 0 )
        {
            snprintf( mResultStr, 
                      sizeof( mResultStr ) - 1, 
                      "UnmountDevice: Unmount SUCCESSFUL Mount Point: %s", 
                      PROCESS_MOUNT_PATH );
        }
        else
        {
            snprintf( mResultStr, 
                      sizeof( mResultStr ) - 1, 
                      "UnmountDevice: Unmount FAILED Mount Point: %s, R: %d E: %d", 
                      PROCESS_MOUNT_PATH, 
                      result, 
                      errno );
        }
    }
    else
    {
        snprintf( mResultStr, 
                  sizeof( mResultStr ) - 1, 
                  "UnmountDevice: Mount Point Not Active: %s", 
                  PROCESS_MOUNT_PATH );
    }
}

void StateMounting::UnmountDeviceForce( )
{
    if( OSMount::IsMountPointActive( PROCESS_MOUNT_PATH ) == 1 )
    {
        int result = OSMount::UnmountDeviceForce( PROCESS_MOUNT_PATH );
        if( result == 0 )
        {
            snprintf( mResultStr, 
                      sizeof( mResultStr ) - 1, 
                      "UnmountDeviceForce: Unmount (Force) SUCCESSFUL Mount Point: %s", 
                      PROCESS_MOUNT_PATH );
        }
        else
        {
            snprintf( mResultStr, 
                      sizeof( mResultStr ) - 1, 
                      "UnmountDeviceForce: Unmount (Force) FAILED: Mount Point: %s, R: %d E: %d", 
                      PROCESS_MOUNT_PATH, 
                      result, 
                      errno );
        }
    }
    else
    {
        snprintf( mResultStr, 
                  sizeof( mResultStr ) - 1, 
                  "UnmountDeviceForce: Mount Point Not Active: %s", 
                  PROCESS_MOUNT_PATH );
    }
}

void StateMounting::ListMounts( )
{
    ResetLineBuffer( );
    
    FILE *pFile = fopen( "/proc/mounts", "r" );
    if( pFile != NULL )
    {
        int isMounted = 0;
        char lineBuffer[ MAX_STR_SIZE ];
        while( fgets(lineBuffer, sizeof( lineBuffer ) - 1, pFile) )
        {
            AddLineToBuffer( lineBuffer );
        }
        
        fclose( pFile );
        pFile = NULL;
    }
    else
    {
        snprintf( mResultStr, 
                  sizeof( mResultStr ) -1 , 
                  "IsDeviceMounted: Failed to open file /proc/mounts" );
    }
}

void StateMounting::ListUSBDevs( int resetBuffer )
{
    if( resetBuffer )
    {
        ResetLineBuffer( );
    }
    
    DIR *pDir = opendir( "/dev" );
    if( pDir != NULL )
    {
        int currCount = 0;
        dirent *pDirEntry = NULL;
        
        while( (pDirEntry = readdir( pDir )) != NULL )
        {
            if( strlen( pDirEntry->d_name ) <= 4 )
            {
                if( pDirEntry->d_name[ 0 ] == 's' &&
                    pDirEntry->d_name[ 1 ] == 'd' )
                {
                    AddLineToBuffer( pDirEntry->d_name );
                }
            }
            
        }
        
        closedir( pDir );
    }
    else
    {
        snprintf( mResultStr, sizeof( mResultStr ) -1 , "ListUSBDevs: Could not open dir '/dev'" );
    }
}

void StateMounting::ListItemsInDir( const char *pStartingDir )
{
    ResetLineBuffer( );
    
    DIR *pDir = opendir( pStartingDir );
    if( pDir != NULL )
    {
        int currCount = 0;
        dirent *pDirEntry = NULL;
        
        while( (pDirEntry = readdir( pDir )) != NULL )
        {
            AddLineToBuffer( pDirEntry->d_name );
        }
        
        closedir( pDir );
    }
    else
    {
        snprintf( mResultStr, sizeof( mResultStr ) -1 , "ListItemsInDir: Could not open dir: %s", pStartingDir );
    }
}

void StateMounting::AddLineToBuffer( const char *pLine )
{
    if( mOutputBufferNumLines < MAX_LINES_IN_BUFFER )
    {
        strncpy( &mpOutputBuffer[ mOutputBufferNumLines * MAX_STR_SIZE ], pLine, MAX_STR_SIZE - 1 );
        mOutputBufferNumLines++;
    }
    else
    {
        flushPrintf( "AddLineToBuffer: Buffer full! %d items. Requested line: %s\r\n", mOutputBufferNumLines, pLine );
    }
}

void StateMounting::ResetLineBuffer( )
{
    mOutputBufferNumLines = 0;
}

void StateMounting::DrawLineBuffer( )
{
    const char *pCurrLine = mpOutputBuffer;
    
    // we need to appear just below the result string
    int resultStrStartYPos = mMenuItem[ MOUNTING_ITEMS - 1 ].GetYPos( ) + UI_ROW_HEIGHT;
    int startYPos = resultStrStartYPos + 20;
    
    int yPos = startYPos; // just below the mount menu options and function result string
    int xPos = UI_X_POS_MENU;
    
    int i = 0;
    for( i = 0; i < mOutputBufferNumLines; i++ )
    {
        Renderer::DrawText( pCurrLine, xPos, yPos, 0xFFFF );
        
        // move to the next line
        pCurrLine += MAX_STR_SIZE;
        
        yPos += 15;
        if( yPos > 1000 )
        {
            yPos = startYPos;
            xPos += 150;
        }
    }
}
