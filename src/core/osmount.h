
// See License.md for license

#ifndef OSMOUNT_H_
#define OSMOUNT_H_

#define MOUNT_FORMAT "exfat"
#define BUSYBOX_EXEC "/bin/busybox"
#define BUSYBOX_ARGS "mdev -s"

class OSMount
{
public:
    static int  MountDevice( const char *pDevicePath, const char *pMountPoint );
    static int  UnmountDevice( const char *pMountPoint );
    static int  UnmountDeviceForce( const char *pMountPoint );
    static void BusyBoxScanForChanges( );
    static int  IsMountPointActive( const char *pMountPoint );
    static int  IsDeviceMounted( const char *pDevice );
    static int  IsDeviceMountedAtMountPoint( const char *pDevice, const char *pMountPoint );
    static int  GetHighestUSBDeviceFile( char *pDeviceFile, int deviceFilePathLen );
    static int  MountUSBPartition( const char *pMountPoint, int partitionIndex );
};

#endif
