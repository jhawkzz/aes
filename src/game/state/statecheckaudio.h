
// See License.md for license

#ifndef STATECHECKAUDIO_H_
#define STATECHECKAUDIO_H_

class StateCheckAudio : public BaseGameState
{
public:
    virtual void Create( );
    virtual void Destroy( );
    
    virtual void EnterState( GameState oldState );
    virtual void ExitState( GameState newState );
    
    virtual GameState Update( );
    
private:

    void TryPlayAudio( );
    void TryStopAudio( );

    WAVE_FILE mWaveFile;
    char      mResultStr[ MAX_STR_SIZE ];
};

#endif
