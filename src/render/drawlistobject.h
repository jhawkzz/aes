
// See License.md for license

#ifndef DRAW_LIST_OBJECT_H_
#define DRAW_LIST_OBJECT_H_

class DrawListObject
{
public:
    //todo: Make this an abstract class with derived types and an derived render() func
    friend class Renderer;

private:
    void Reset( );
    
    void RenderBackground( short *pBackBuffer );
    void RenderSprite( short *pBackBuffer );
    void RenderText( short *pBackBuffer );
    
    void RenderToBackBuffer( short *pBackBuffer );
    
    short *mpImageSurface;
    
    char mText[ MAX_STR_SIZE ];
    short mTextColor;
    
    const short *mpSpriteBuffer;
    int mSpriteWidth;
    int mSpriteHeight;
    
    int mPosX;
    int mPosY;
};

#endif
