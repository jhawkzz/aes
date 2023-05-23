
// See License.md for license

#ifndef STATEQUIT_H_
#define STATEQUIT_H_

class StateQuit : public BaseGameState
{
public:
    virtual void      Create( );
    virtual void      Destroy( );
    
    virtual void      EnterState( GameState oldState );
    virtual void      ExitState( GameState newState );
    
    virtual GameState Update( );
    
            int       ShouldQuitGame( );
private:
    int mShouldQuitGame;
};

#endif
