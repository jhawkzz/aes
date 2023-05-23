
// See License.md for license

#include "includes.h"

DrawListObject  Renderer::mDrawList[ MAX_DRAWLIST_OBJECTS ];
int             Renderer::mDrawListCount;
int             Renderer::mRenderEnabled;

#ifdef SHOW_FPS
double          Renderer::mFPSAvgCount;
double          Renderer::mFPSTickCount;
double          Renderer::mFPS;
#endif

int Renderer::Create( )
{
    if( Font::Create( ) == -1 )
    {
        flushPrintf( "Renderer::Create() Font Creation failed\r\n" );
        return -1;
    }
    
    ToggleRenderEnabled( 1 );

    return 0;
}

void Renderer::Destroy( )
{
    mDrawListCount = 0;
}

void Renderer::DrawBackground( short *pImageSurface )
{
    if( mDrawListCount + 1 < MAX_DRAWLIST_OBJECTS )
    {
        mDrawList[ mDrawListCount ].Reset( );
        
        mDrawList[ mDrawListCount ].mpImageSurface  = pImageSurface;
        
        mDrawListCount++;
    }
    else
    {
        flushPrintf( "Draw List Full. Cannot add image.\r\n" );
    }
}

void Renderer::DrawSprite( short *pSpriteBuffer, int x, int y, int spriteWidth, int spriteHeight )
{
    if( mDrawListCount + 1 < MAX_DRAWLIST_OBJECTS )
    {
        mDrawList[ mDrawListCount ].Reset( );
        
        mDrawList[ mDrawListCount ].mpSpriteBuffer  = pSpriteBuffer;
        
        mDrawList[ mDrawListCount ].mPosX = x;
        mDrawList[ mDrawListCount ].mPosY = y;
        
        mDrawList[ mDrawListCount ].mSpriteWidth = spriteWidth;
        mDrawList[ mDrawListCount ].mSpriteHeight = spriteHeight;
        
        mDrawListCount++;
    }
    else
    {
        flushPrintf( "Draw List Full. Cannot add sprite.\r\n" );
    }
}

void Renderer::DrawText( const char *pText, int x, int y, short color )
{
    if( mDrawListCount + 1 < MAX_DRAWLIST_OBJECTS )
    {
        mDrawList[ mDrawListCount ].Reset( );
        
        strncpy( mDrawList[ mDrawListCount ].mText, pText, sizeof( mDrawList[ mDrawListCount ].mText ) - 1 );
        
        mDrawList[ mDrawListCount ].mPosX = x;
        mDrawList[ mDrawListCount ].mPosY = y;
        
        mDrawList[ mDrawListCount ].mTextColor = color;
                
        mDrawListCount++;
    }
    else
    {
        flushPrintf( "Draw List Full. Cannot add text.\r\n" );
    }
}

void Renderer::Render( )
{
#ifdef SHOW_FPS
    CalcFPS( );
    RenderFPS( );
#endif
 
    if( mRenderEnabled )
    {
        short *pBackBuffer = OSFrameBuffer::GetBackBuffer( );
       
        if( pBackBuffer != NULL )
        {
            for( int i = 0; i < mDrawListCount; i++ )
            {
                mDrawList[ i ].RenderToBackBuffer( pBackBuffer );
            }
        }
    }
}

void Renderer::ResetForFrame( )
{
    mDrawListCount = 0;
}

void Renderer::Flip( )
{
    if( mRenderEnabled )
    {
        OSFrameBuffer::Flip( );
    }
}

void Renderer::ToggleRenderEnabled( int enabled )
{
    mRenderEnabled = enabled;
}

#ifdef SHOW_FPS
void Renderer::CalcFPS( )
{
    if( mFPSTickCount > 5 )
    {
        mFPS = 1000 / (mFPSAvgCount / mFPSTickCount);
        mFPSAvgCount = 0;
        mFPSTickCount = 0;
    }
    else
    {
        mFPSAvgCount += gFrameTimeMS - gOldFrameTimeMS;
        mFPSTickCount++;
    }
}

void Renderer::RenderFPS( )
{
    if( mDrawListCount + 1 < MAX_DRAWLIST_OBJECTS )
    {
        char text[ MAX_STR_SIZE ] = { 0 };
        snprintf( text, sizeof( text ) - 1, "FPS: %.2f", mFPS );
        DrawText( text, 800, 10, 0xFFFF );
        
        snprintf( text, sizeof( text ) - 1, "F: %lu", gFrameCount );
        DrawText( text, 800, 30, 0xFFFF );
    }
}
#endif
