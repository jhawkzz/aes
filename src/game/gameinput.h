
// See License.md for license

#ifndef GAME_INPUT_H_
#define GAME_INPUT_H_

enum InputState
{
    InputState_Released = 0,
    InputState_Pressed  = 1
};

class InputControl
{
public:
    void Create( )
    {
        mPrevState       = InputState_Released;
        mState           = InputState_Released;
        mTimeHeldMS      = 0;
        mTimeHeldStartMS = 0;   
        mTimeHeldEndMS   = 0;
    }
    
    int WasPressed( )
    {
        return mPrevState == 0 && mState == 1;
    }
    
    int WasReleased( )
    {
        return mPrevState == 1 && mState == 0;
    }
    
    long GetTimeHeldMS( )
    {
        return mTimeHeldMS;
    }
    
    long GetTimePressedMS( )
    {
        return mTimeHeldStartMS;
    }
    
    long GetTimeReleasedMS( )
    {
        return mTimeHeldEndMS;
    }
    
    InputState GetState( )
    {
        return mState;
    }
    
    void Update( int newState )
    {
        mPrevState = mState;
        mState = (InputState)newState;
        
        if( WasPressed( ) )
        {
            mTimeHeldStartMS = gFrameTimeMS;
        }
        else if ( WasReleased( ) )
        {
            mTimeHeldMS    = gFrameTimeMS - mTimeHeldStartMS;
            mTimeHeldEndMS = gFrameTimeMS;
        }
    }
    
private:
    InputState mPrevState;
    InputState mState;
    long       mTimeHeldMS;
    
    // store this seperate from timeHeld. Otherwise
    // when holding the button, timeHeld will be
    // the timestamp of when the button was pressed, which
    // isn't accurate at all.
    long      mTimeHeldStartMS;
    long      mTimeHeldEndMS;
};

class GameInput
{
public:
    static void          Create( );
    static void          Destroy( );
    
    static void          Update( );
    
    static InputControl *GetInput( ButtonInput button );
    
private:
    static InputControl mInputState[ Button_Count ];
};

#endif
