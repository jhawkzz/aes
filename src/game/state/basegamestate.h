
// See License.md for license

#ifndef BASEGAMESTATE_H_
#define BASEGAMESTATE_H_

class BaseGameState
{
public:
    virtual void Create( );
    virtual void Destroy( );
    
    virtual void EnterState( GameState oldState );
    virtual void ExitState( GameState newState );
    
    virtual GameState Update( );
    
protected:
    GameState mLastState;
    int       mDrawBGImage;
};

#endif
