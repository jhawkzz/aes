
// See License.md for license

#include "includes.h"

int BitmapLoader::Load( const char *pBmpFilePath, BitmapImage *pBmp )
{
    FILE *pBmpFile = fopen( pBmpFilePath, "rb" );
    if( pBmpFile == NULL )        
    {
        flushPrintf( "BitmapLoader::Load() Failed to open file: %s\r\n", pBmpFilePath );
        return -1;
    }
    
    int sizeRead = fread( &pBmp->fileHeader, 1, sizeof( BitmapFileHeader ), pBmpFile );
    if ( sizeRead != sizeof( BitmapFileHeader ) )
    {
        flushPrintf( "BitmapLoader::Load() Error reading BitmapFileHeader. Expected %d bytes. Read %d bytes.\r\n", 
                        sizeof( BitmapFileHeader ), 
                        sizeRead );
        return -1;
    }
    
    if( pBmp->fileHeader.tag[ 0 ] != 'B' || pBmp->fileHeader.tag[ 1 ] != 'M' )
    {
        flushPrintf( "BitmapLoader::Load() Invalid BMP Header Tag. Expected 'BM'. Read '%s'\r\n", pBmp->fileHeader.tag );
        return -1;
    }
    
    
    
    sizeRead = fread( &pBmp->infoHeader, 1, sizeof( BitmapInfoHeader ), pBmpFile );
    if ( sizeRead != sizeof( BitmapInfoHeader ) )
    {
         flushPrintf( "BitmapLoader::Load() Error reading BitmapInfoHeader. Expected %d bytes. Read %d bytes.\r\n", 
                        sizeof( BitmapInfoHeader ), 
                        sizeRead );
        return -1;
    }
    
    if( pBmp->infoHeader.compressionMethod != 0 )
    {
        flushPrintf( "BitmapLoader::Load() Compression method is not 0. Found: %d. Currently no compression is supported.\r\n", 
                      pBmp->infoHeader.compressionMethod );
                      
        return -1;
    }

    int bytesPerPixel = pBmp->infoHeader.bitsPerPixel / 8;
    int imageBytes = pBmp->infoHeader.width * pBmp->infoHeader.height * bytesPerPixel;
    
    pBmp->pPixelData = new char[ imageBytes ];
    
    // gotta read bottom up
    if( pBmp->infoHeader.height > 0 )
    {
        sizeRead = 0;
        int bmpPitch = pBmp->infoHeader.width * bytesPerPixel;
        
        // position the buffer at the start of the last row
        char *pCurrPixelData = pBmp->pPixelData + imageBytes - bmpPitch;
        
        // read each row and write it bottom-up to the buffer
        int y = 0;
        for( y = 0; y < pBmp->infoHeader.height; y++ )
        {
            sizeRead += fread( pCurrPixelData, 1, bmpPitch, pBmpFile );
            pCurrPixelData -= bmpPitch;
        }
    }
    else
    {
        sizeRead = fread( pBmp->pPixelData, 1, imageBytes, pBmpFile );
    }
    
    if( sizeRead != imageBytes )
    {
        flushPrintf( "BitmapLoader::Load() Error reading bitmap pixels! Expected: %d bytes. Read %d bytes.\r\n", imageBytes, sizeRead );
        return -1;
    }
    
    fclose( pBmpFile );
    pBmpFile = NULL;
    
    return 0;
}

int BitmapLoader::Make565( BitmapImage *pBmp )
{
    if( pBmp->infoHeader.bitsPerPixel != 16 )
    {
        flushPrintf( "BitmapLoader::Make565() Bitmap is not 16 bit. BPP is: %d\r\n", pBmp->infoHeader.bitsPerPixel );
        return -1;
    }
    
    short *pPixelBuffer = (short *)pBmp->pPixelData;
    
    int y = 0;
    for( y = 0; y < pBmp->infoHeader.height; y++ )
    {
        int x = 0;
        for( x = 0; x < pBmp->infoHeader.width; x++ )
        {
            short pixelData = pPixelBuffer[ x ];
 
            // break out the components
            int r = (pixelData & (0x1F << 10)) >> 10;
            int g = (pixelData & (0x1F << 5)) >> 5;
            int b = pixelData & 0x1F;
            
            // scale to six bits
            int scaledG = (int) (((float)g / (float)0x1F) * 0x3F);
          
            short pixelData565 = r << 11 | scaledG << 5 | b; 
            pPixelBuffer[ x ] = pixelData565;
        }
        
        pPixelBuffer += pBmp->infoHeader.width;
    }
    
    return 0;
}

void BitmapLoader::PrintBitmapInfo( BitmapImage *pBmp )
{
    // header
    flushPrintf( "BitmapLoader::Load() bmp header struct is: %d\r\n", sizeof( pBmp->fileHeader ) );
    flushPrintf( "BitmapLoader::Load() bmp tag is: %c%c\r\n", pBmp->fileHeader.tag[0], pBmp->fileHeader.tag[1] );
    flushPrintf( "BitmapLoader::Load() bmp size is: %d\r\n", pBmp->fileHeader.bmpSize );
    flushPrintf( "BitmapLoader::Load() reserved 1 & 2 are: %d %d\r\n", pBmp->fileHeader.reserved1, pBmp->fileHeader.reserved2 );
    flushPrintf( "BitmapLoader::Load() bmp offset is: %d\r\n", pBmp->fileHeader.bmpOffset );
    
    // info header
    flushPrintf( "BitmapLoader::Load() infoHeaderSize reports %d actual struct: %d\r\n", pBmp->infoHeader.infoHeaderSize, sizeof( pBmp->infoHeader ) );
    flushPrintf( "BitmapLoader::Load() width %d\r\n", pBmp->infoHeader.width );
    flushPrintf( "BitmapLoader::Load() height %d\r\n", pBmp->infoHeader.height );
    flushPrintf( "BitmapLoader::Load() numColorPlanes %d\r\n", pBmp->infoHeader.numColorPlanes );
    flushPrintf( "BitmapLoader::Load() bitsPerPixel %d\r\n", pBmp->infoHeader.bitsPerPixel );
    flushPrintf( "BitmapLoader::Load() compressionMethod %d\r\n", pBmp->infoHeader.compressionMethod );
    flushPrintf( "BitmapLoader::Load() imageSizeBytes %d\r\n", pBmp->infoHeader.imageSizeBytes );
    flushPrintf( "BitmapLoader::Load() imageHorzSize %d\r\n", pBmp->infoHeader.imageHorzSize );
    flushPrintf( "BitmapLoader::Load() imageVertSize %d\r\n", pBmp->infoHeader.imageVertSize );
    flushPrintf( "BitmapLoader::Load() paletteColors %d\r\n", pBmp->infoHeader.paletteColors );
    flushPrintf( "BitmapLoader::Load() numImportantColors %d\r\n", pBmp->infoHeader.numImportantColors );
    flushPrintf( "\r\n" );
}
