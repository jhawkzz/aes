
// See License.md for license

#ifndef RENDERER_H_
#define RENDERER_H_

#define MAX_DRAWLIST_OBJECTS (256)

class DrawListObject;

class Renderer
{
public:
    static int   Create( );
    static void  Destroy( );
    
    static void  DrawBackground( short *pImageSurface );
    static void  DrawSprite( short *pSpriteBuffer, int x, int y, int spriteWidth, int spriteHeight );
    static void  DrawText( const char *pText, int x, int y, short color );
    
    static void  ToggleRenderEnabled( int enabled );
    
    static void  ResetForFrame( );
    static void  Render( );
    static void  Flip( );
    
private:
    static int 		      mDrawListCount;
    static DrawListObject mDrawList[ MAX_DRAWLIST_OBJECTS ];
    static int            mRenderEnabled;
    
#ifdef SHOW_FPS
    static void   CalcFPS( );
    static void   RenderFPS( );
    static double mFPSAvgCount;
    static double mFPSTickCount;
    static double mFPS;
#endif
};

#endif
