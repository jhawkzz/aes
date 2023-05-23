
// See License.md for license

#ifndef GAME_H_
#define GAME_H_

class Game
{
public:
    static int  Create( );
    static void Destroy( );
    static int  Update( );
    
private:
    static void ChangeGameState( GameState newState );
    static void UpdateGameState( );

    static short           *mpImageBuffer;
    static GameState        mGameState;
    static StateMainMenu    mGameState_MainMenu;
    static StateMounting    mGameState_Mounting;
    static StateCheckInputs mGameState_CheckInputs;
    static StateCheckLEDs   mGameState_CheckLEDs;
    static StateCheckAudio  mGameState_CheckAudio;
    static StateExec        mGameState_Exec;
    static StateCredits     mGameState_Credits;
    static StateQuit        mGameState_Quit;
    static BaseGameState   *mpGameStateList[ GameState_Count ];
};

#endif
