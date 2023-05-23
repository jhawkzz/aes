
// See License.md for license

#include "includes.h"

void DrawListObject::Reset( )
{
    mpImageSurface = NULL;
    mText[ 0 ] = 0;
    mpSpriteBuffer = NULL;
}

void DrawListObject::RenderBackground( short *pBackBuffer )
{
    memcpy( (char *)pBackBuffer, (char *)mpImageSurface, LCD_HEIGHT * SCREEN_PITCH );
}

void DrawListObject::RenderSprite( short *pBackBuffer )
{
    short *pShortBackBuffer = pBackBuffer + (mPosX + (mPosY * LCD_WIDTH));
    
    // clip dimensions to the edge of the screen
    int spriteDrawWidth = min( mSpriteWidth, LCD_WIDTH - mPosX );
    int spriteDrawHeight = min( mSpriteHeight, LCD_HEIGHT - mPosY );
    
    for( int y = 0; y < spriteDrawHeight; y++ )
    {
        for( int x = 0; x < spriteDrawWidth; x++ )
        {
            pShortBackBuffer[ x ] = mpSpriteBuffer[ x ];
        }
        
        pShortBackBuffer += LCD_WIDTH;
        mpSpriteBuffer += mSpriteWidth;
    }
}

void DrawListObject::RenderText( short *pBackBuffer )
{
    Font::Print( pBackBuffer, mText, mPosX, mPosY, mTextColor );
}

void DrawListObject::RenderToBackBuffer( short *pBackBuffer )
{
    if( mpImageSurface != NULL )
    {
        RenderBackground( pBackBuffer );
    }
    else if ( mText[ 0 ] != 0 )
    {
        RenderText( pBackBuffer );
    }
    else if( mpSpriteBuffer != NULL )
    {
        RenderSprite( pBackBuffer );
    }
}
