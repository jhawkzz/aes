
// See License.md for license

#include "includes.h"

short  *Font::mpFontTexture;
char   Font::mCharLookup[ 128 ];

// pixel start position for each letter within the bmp
Letter Font::mCharBufferPosition[ FONT_LETTER_COUNT ] = { { 0, 0  }, { 14, 0  }, { 28, 0  }, { 42, 0  }, { 56, 0  }, { 70, 0  }, { 84, 0  }, { 98, 0  }, { 112, 0  }, { 126, 0  }, { 140, 0  }, { 154, 0  }, { 168, 0  },
                                                          { 0, 16 }, { 14, 16 }, { 28, 16 }, { 42, 16 }, { 56, 16 }, { 70, 16 }, { 84, 16 }, { 98, 16 }, { 112, 16 }, { 126, 16 }, { 140, 16 }, { 154, 16 }, { 168, 16 },
                                                          { 0, 32 }, { 14, 32 }, { 28, 32 }, { 42, 32 }, { 56, 32 }, { 70, 32 }, { 84, 32 }, { 98, 32 }, { 112, 32 }, { 126, 32 }, { 140, 32 }, { 154, 32 }, { 168, 32 },
                                                          { 0, 48 }, { 14, 48 }, { 28, 48 }, { 42, 48 }, { 56, 48 } };

char Font::mFont[ FONT_LETTER_COUNT ] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C',
                                          'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 
                                          'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ',', '.', '!',
                                          '%', ':', '*', '-', '/' };
//* = Copyright logo

int Font::Create( )
{
    BitmapImage bmpImage = { 0 };
    
    // load our font
    int result = BitmapLoader::Load( ASSET_PATH"/fonts/font.bmp", &bmpImage );
    if( result == -1 )
    {
        flushPrintf( "Font::Create() Failed to load bitmap %s\r\n", ASSET_PATH"/fonts/font.bmp" );
        return -1;
    }
    
    result = BitmapLoader::Make565( &bmpImage );
    if( result == -1 )
    {
        flushPrintf( "Font::Create() Failed to convert bitmap to 565\r\n" );
        return -1;
    }
    
    mpFontTexture = (short *)bmpImage.pPixelData;
    
    if( bmpImage.infoHeader.bitsPerPixel != 16 )
    {
        Destroy( );
        
        flushPrintf( "Font::Create() Error expected 16 bit font texture. Got %d\r\n", bmpImage.infoHeader.bitsPerPixel );
        return -1;
    }
    
    int i;
    for ( i = 0; i < FONT_LETTER_COUNT; i++ )
    {
        mCharLookup[ mFont[ i ] ] = (char) i;

        if ( i < FONT_LETTER_COUNT - 8 )
        {
            //force lowercase to use the same indices
            mCharLookup[ mFont[ i ] + 32 ] = (char) i; 
        }
    }

    return 0;
}

void Font::Destroy( )
{
    if( mpFontTexture != NULL )
    {
        delete [] mpFontTexture;
        mpFontTexture = NULL;
    }
}

void Font::Print( short *pBackBuffer, const char *pString, int xPos, int yPos, short textColor )
{
    // clamp height
    yPos = min( yPos, LCD_HEIGHT - FONT_LETTER_HEIGHT );
    
    // clamp width (by limiting string length)
    int allowedStringLen = strlen( pString );
    
    int maxWidth = LCD_WIDTH - xPos;
    int stringWidth = (allowedStringLen * FONT_LETTER_FULL_WIDTH) - FONT_SPACING;
    
    // is to too long?
    int delta = stringWidth - maxWidth;
    if( delta > 0 )
    {
        // calculate how many letters are safe to render
        int allowedWidth = stringWidth - delta;
        allowedStringLen = allowedWidth / FONT_LETTER_FULL_WIDTH;
    }
    
    // position the screen buffer where the text should start
    short *pCurrBuffer = pBackBuffer + ( yPos * LCD_WIDTH ) + xPos;

    int strIndex = 0;
    while( strIndex < allowedStringLen )
    {
        switch( pString[ strIndex ] )
        {
            default:
            {     
                // get the index of the letter in our font
                int letter_index = mCharLookup[ pString[ strIndex ] ];
            
                // render the letter to the screen
                short *pLetter = mpFontTexture 
                                + ( mCharBufferPosition[ letter_index ].yPos * FONT_WIDTH ) 
                                +   mCharBufferPosition[ letter_index ].xPos;

                RenderLetter( pCurrBuffer, pLetter, textColor );

                break;
            }

            case 32:
            {
                break;
            }
        }

        pCurrBuffer += FONT_LETTER_WIDTH + FONT_SPACING;

        strIndex++;
    }
}

void Font::RenderLetter( short *pBackBuffer, short *pLetterBuffer, short textColor )
{
    int y;
    for ( y = 0; y < FONT_LETTER_HEIGHT; y++ )
    {
        int x;
        for ( x = 0; x < FONT_LETTER_WIDTH; x++ )
        {
            if( pLetterBuffer[ x ] != FONT_CLEAR_COLOR )
            {
                pBackBuffer[ x ] = textColor;//pLetterBuffer[ x ];
            }
        }

        pBackBuffer += LCD_WIDTH;
        pLetterBuffer += FONT_WIDTH;
    }
}
