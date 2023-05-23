
// See License.md for license

#ifndef STATEMAINMENU_H_
#define STATEMAINMENU_H_

#define MAINMENU_ITEMS (7)

struct MenuItemGameState
{
    MenuItem menuItem;
    GameState gameState;
};

class StateMainMenu : public BaseGameState
{
public:
    virtual void      Create( );
    virtual void      Destroy( );
    
    virtual void      EnterState( GameState oldState );
    virtual void      ExitState( GameState newState );
    
    virtual GameState Update( );
    
private:
            void              DrawMenu( );

            int               mMenuSelection;
            MenuItemGameState mMenuItemGameState[ MAINMENU_ITEMS ];
    
};

#endif