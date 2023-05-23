
// See License.md for license

#ifndef STATEMOUNTING_H_
#define STATEMOUNTING_H_

#define MOUNTING_ITEMS      (5)
#define MAX_LINES_IN_BUFFER (4096)

class StateMounting : public BaseGameState
{
public:
    virtual void Create( );
    virtual void Destroy( );
    
    virtual void EnterState( GameState oldState );
    virtual void ExitState( GameState newState );
    
    virtual GameState Update( );
    
private:
    void      DrawMenu( );
    void      DoSelectedMountOption( int selectedOption );
    void      UnmountDevice( );
    void      UnmountDeviceForce( );
    void      BusyBoxScanForChanges( );
    void      MountDevice( );
    void      ListUSBDevs( int resetBuffer );
    void      ListMounts( );
    void      ListItemsInDir( const char *pStartingDir );
    void      AddLineToBuffer( const char *pLine );
    void      ResetLineBuffer( );
    void      DrawLineBuffer( );
    
    int       mMenuSelection;
    MenuItem  mMenuItem[ MOUNTING_ITEMS ];
    int       mMenuEndYPos;
    
    char      mResultStr[ MAX_STR_SIZE ];
    
    // we need a buffer to store the output from ListDevs and ListMounts
    // That'd be roughly 16 megs at 256 bytes per entry * 4096 psosible entries.
    char     *mpOutputBuffer;
    int       mOutputBufferNumLines;
};

#endif
