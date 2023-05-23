
// See License.md for license

#include "includes.h"

void BaseGameState::Create( )
{
}

void BaseGameState::Destroy( )
{
}

void BaseGameState::EnterState( GameState oldState )
{
    mLastState = oldState;
}

void BaseGameState::ExitState( GameState newState )
{
}
    
GameState BaseGameState::Update( )
{
}
