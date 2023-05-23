
// See License.md for license

#ifndef STATEXEC_H_
#define STATEXEC_H_

enum ExecState
{
    ExecState_NotRunning,
    ExecState_Running,
    ExecState_Running_Threaded
};

class StateExec : public BaseGameState
{
public:
    virtual void Create( );
    virtual void Destroy( );
    
    virtual void EnterState( GameState oldState );
    virtual void ExitState( GameState newState );
    
    virtual GameState Update( );
    
private:
    void      DrawMenu( );
    void      LaunchExec_Main( );
    void      LaunchExec_Threaded( );
    void      LaunchExec( WaitAction waitAction, int multiThread );
    void      KillExecThreaded( );
    void      HandleExecFinished( );
    
    ExecState mExecRunning;       
    MenuItem  mMenuItemList[ 2 ];
    int       mMenuSelection;
    char      mResultStr[ MAX_STR_SIZE ];
};

#endif
