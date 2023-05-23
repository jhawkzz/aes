
// See License.md for license

#ifndef STATECREDITS_H_
#define STATECREDITS_H_

class StateCredits : public BaseGameState
{
public:
    virtual void      Create( );
    virtual void      Destroy( );
    
    virtual void      EnterState( GameState oldState );
    virtual void      ExitState( GameState newState );
    
    virtual GameState Update( );
};

#endif
