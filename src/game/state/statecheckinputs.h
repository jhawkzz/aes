
// See License.md for license

#ifndef STATECHECKINPUTS_H_
#define STATECHECKINPUTS_H_

#define INPUT_ROW_HEIGHT (20)

class StateCheckInputs : public BaseGameState
{
public:
    virtual void Create( );
    virtual void Destroy( );
    
    virtual void EnterState( GameState oldState );
    virtual void ExitState( GameState newState );
    
    virtual GameState Update( );
    
private:
    void DrawInput( ButtonInput button, const char *pLabel, int x, int y );
    void DrawInputList( );
};

#endif
