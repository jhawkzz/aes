
// See License.md for license

#ifndef STATECHECKLEDS_H_
#define STATECHECKLEDS_H_

class StateCheckLEDs : public BaseGameState
{
public:
    virtual void      Create( );
    virtual void      Destroy( );
    
    virtual void      EnterState( GameState oldState );
    virtual void      ExitState( GameState newState );
    
    virtual GameState Update( );
    
private:
            void      UpdateLEDs( );
};

#endif
