
// See License.md for license

#include "includes.h"

InputControl GameInput::mInputState[ Button_Count ];

void GameInput::Create( )
{
    for( int i = 0; i < Button_Count; i++ )
    {
        mInputState[ i ].Create( );
    }
}

void GameInput::Destroy( )
{
}

void GameInput::Update( )
{
    for( int i = 0; i < Button_Count; i++ )
    {
        int inputVal = OSInput::GetValueForInput( (ButtonInput) i );
        mInputState[ i ].Update( inputVal );
    }
}

InputControl *GameInput::GetInput( ButtonInput button )
{
    return &mInputState[ button ];
}