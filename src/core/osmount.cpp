
// See License.md for license

#include "includes.h"

int OSMount::MountDevice( const char *pDevicePath, const char *pMountPoint )
{
    return mount( pDevicePath, pMountPoint, MOUNT_FORMAT, 0, NULL );
}

int OSMount::UnmountDevice( const char *pMountPoint )
{
    return umount( pMountPoint );
}

int OSMount::UnmountDeviceForce( const char *pMountPoint )
{
    return umount2( pMountPoint, MNT_FORCE );
}

void OSMount::BusyBoxScanForChanges( )
{
    OSProcess::LaunchProcess( BUSYBOX_EXEC, BUSYBOX_ARGS, WaitAction_Poll, 0 );
}

int OSMount::IsMountPointActive( const char *pMountPoint )
{
    return IsDeviceMountedAtMountPoint( NULL, pMountPoint );
}

int OSMount::IsDeviceMounted( const char *pDevicePath )
{
    IsDeviceMountedAtMountPoint( pDevicePath, NULL );
}

int OSMount::IsDeviceMountedAtMountPoint( const char *pDevicePath, const char *pMountPoint )
{
    FILE *pFile = fopen( "/proc/mounts", "r" );
    if( pFile == NULL )
    {
        return -1;
    }
    
    int isMounted = 0;
	char lineBuffer[ MAX_STR_SIZE ];
	while( fgets(lineBuffer, sizeof( lineBuffer ) - 1, pFile) )
	{
        const char *pFoundDevice = NULL;
        const char *pFoundMount = NULL;
        
        // if they're both valid, they both need to be found on the same line
        if( pDevicePath != NULL && pMountPoint != NULL )
        {
            pFoundDevice = strstr( lineBuffer, pDevicePath );
            pFoundMount = strstr( lineBuffer, pMountPoint );
            
            if( pFoundDevice != NULL && pFoundMount != NULL )
            {   
                isMounted = 1;        
                break;
            }
        }
        else if ( pDevicePath != NULL )
        {
            pFoundDevice = strstr( lineBuffer, pDevicePath );
            if( pFoundDevice != NULL )
            {   
                isMounted = 1;        
                break;
            }
        }
        else if ( pMountPoint != NULL )
        {
            pFoundMount = strstr( lineBuffer, pMountPoint );
            if( pFoundMount != NULL )
            {   
                isMounted = 1;        
                break;
            }
        }
	}
	
    fclose( pFile );
    pFile = NULL;
    
    if( isMounted == 1 )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int OSMount::GetHighestUSBDeviceFile( char *pDeviceFile, int deviceFilePathLen )
{
    memset( pDeviceFile, 0, deviceFilePathLen );
    
    char highestUsbLetter = 'a';
    
    DIR *pDir = opendir( "/dev" );
    if( pDir != NULL )
    {
        int currCount = 0;
        dirent *pDirEntry = NULL;
        
        while( (pDirEntry = readdir( pDir )) != NULL )
        {
            if( strlen( pDirEntry->d_name ) == 3 )
            {
                // did we find an "sdX" value?
                if( pDirEntry->d_name[ 0 ] == 's' &&
                    pDirEntry->d_name[ 1 ] == 'd' )
                {
                    if( (int) highestUsbLetter < (int) pDirEntry->d_name[ 2 ] )
                    {
                        highestUsbLetter = pDirEntry->d_name[ 2 ];
                    }
                }
            }
        }
        
        closedir( pDir );
        
        snprintf( pDeviceFile, deviceFilePathLen - 1, "/dev/sd%c", highestUsbLetter );
        return 0;
    }
    
    return -1;
}

int OSMount::MountUSBPartition( const char *pMountPoint, int partitionIndex )
{
    // assume we'll fail
    int mountResult = -1;
    
    // first, release any current mount on this point
    umount( pMountPoint );
    
    // next scan to ensure the USB's device file is assigned
    OSProcess::LaunchProcess( BUSYBOX_EXEC, BUSYBOX_ARGS, WaitAction_Poll, 0 );
    
    // now we know it's going to be /dev/sda, /dev/sdb, /dev/sdc, etc.
    // look thru available device files and see which one mounts.
    // note - this only works on the MVSX/ASP because there's a single USB port.
    DIR *pDir = opendir( "/dev" );
    if( pDir != NULL )
    {
        int currCount = 0;
        dirent *pDirEntry = NULL;
        
        while( (pDirEntry = readdir( pDir )) != NULL )
        {
            if( strlen( pDirEntry->d_name ) == 3 )
            {
                // did we find an "sdX" value?
                if( pDirEntry->d_name[ 0 ] == 's' &&
                    pDirEntry->d_name[ 1 ] == 'd' )
                {
                    // create the device path with partition (e.g. /dev/sda2)
                    char deviceFilePath[ MAX_STR_SIZE ] = { 0 };
                    snprintf( deviceFilePath, sizeof( deviceFilePath ) - 1, "/dev/%s%d", pDirEntry->d_name, partitionIndex );
                    
                    // if it mounts, that was the active one.
                    mountResult = mount( deviceFilePath, pMountPoint, MOUNT_FORMAT, 0, NULL );
                    if( mountResult == 0 )
                    {
                        break;
                    }
                }
            }
        }
        
        closedir( pDir );
    }
    
    return mountResult;
}
