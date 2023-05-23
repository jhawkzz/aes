
// See License.md for license

#ifndef DEFINES_H_
#define DEFINES_H_

//#define DEVELOP_MODE  // Enable this if you need to launch this FROM AES to further develop.
#define SHOW_FPS      // Enable this to show the FPS and Frame Count

#define VERSION               (1)

#define ASSET_PATH          "/mnt/aes/assets"
#define PROCESS_MOUNT_PATH  "/mnt/aes" //This is the mount we will expect the exec to exist in.

#define ROOT_PARTITION_INDEX  (1)

#define FRAMES_PER_SECOND     (60)

#define LCD_WIDTH	          (1280)
#ifdef MVSX
#define LCD_HEIGHT		      (1024)
#elif ASP
    #define LCD_HEIGHT	      (720)
#endif

#define SCREEN_BPP		      (16)
#define SCREEN_PITCH 	      (LCD_WIDTH * (SCREEN_BPP / 8))

#define MAX_STR_SIZE          (268)

#define SEC_TO_MILLISECONDS   (1000)
#define NANO_TO_MILLISECONDS  (1000000)
#define MILLI_TO_MICROSECONDS (1000)

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })
     
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

enum GameState
{
    GameState_MainMenu,
    GameState_Mounting,
    GameState_CheckInputs,
    GameState_CheckLEDs,
    GameState_CheckAudio,
    GameState_Exec,
    GameState_Credits,
    GameState_Quit,
    GameState_Count
};

#define UI_Y_POS_HEADER    (208) //The start y position of whatever 'notice' the state wants (Press P1 To Exit, etc.)
#define UI_Y_POS_MENU      (282) //The start y position for menu items
#define UI_X_POS_MENU      (90)  //The start x position for menu items
#ifdef ASP
    #define UI_ROW_HEIGHT  (28)  //The amount to increment Y by when rendering another line
#elif MVSX
    #define UI_ROW_HEIGHT  (56)  //The amount to increment Y by when rendering another line
#endif
#define UI_CURSOR_X_OFFSET (52)  //The amount of pixels to decrement by when rendering the selection cursor

#define LARGE_FONT_SIZE    (60)
#define SMALL_FONT_SIZE    (30)
#define MICRO_FONT_SIZE    (10)

#endif
