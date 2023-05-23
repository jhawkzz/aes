
// See License.md for license

#ifndef FONT_H_
#define FONT_H_

#define FONT_CLEAR_COLOR       (0)
#define FONT_WIDTH             (182)
#define FONT_HEIGHT            (64)
#define FONT_LETTER_COUNT      (44)
#define FONT_LETTER_WIDTH      (14)
#define FONT_LETTER_HEIGHT     (16)
#define FONT_SPACING           (2)
#define FONT_LETTER_FULL_WIDTH (FONT_LETTER_WIDTH + FONT_SPACING)

struct Letter
{
   unsigned char xPos;
   unsigned char yPos;
};

class Font
{
public:
    static int    Create( );
    static void   Destroy( );

    static void   Print( short *pBackBuffer, const char *pString, int xPos, int yPos, short textColor );

private:
    static void   RenderLetter( short *pBackBuffer, short *pLetterBuffer, short textColor );
    
    static short *mpFontTexture;
    static char   mCharLookup[ 128 ];
    static Letter mCharBufferPosition[ FONT_LETTER_COUNT ];
    static char   mFont[ FONT_LETTER_COUNT ];
};

#endif
