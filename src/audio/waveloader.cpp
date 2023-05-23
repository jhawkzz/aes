
// See License.md for license

#include "includes.h"

int WaveLoader::LoadWave( const char *pFileName, WAVE_FILE *pWaveFile )
{
    pWaveFile->pFile = fopen( pFileName, "rb" );
    if( pWaveFile->pFile == NULL )
    {
        flushPrintf( "WaveLoader::LoadWave() - Failed to open file: %s\r\n", pFileName );
        return -1;
    }
    
    // validate the format
    int bytesRead = fread( &pWaveFile->header, 1, sizeof( WAVE_HEADER ), pWaveFile->pFile );
    if( bytesRead != sizeof( WAVE_HEADER ) )
    {
        flushPrintf( "WaveLoader::LoadWave() - Failed to read WAV_HEADER. Expected %d bytes. Read %d bytes.\r\n", 
                     sizeof( WAVE_HEADER ), 
                     bytesRead );
        return -1;
    }
    
    if( *(int *)pWaveFile->header.riffTag != RIFF_TAG )
    {
        flushPrintf( "WaveLoader::LoadWave() - Failed to read RIFF_TAG. Expected %s. Read %s.\r\n", RIFF_TAG, pWaveFile->header.riffTag );
        return -1;
    }
    
    if( *(int *)pWaveFile->header.wavTag != WAV_TAG )
    {
        flushPrintf( "WaveLoader::LoadWave() - Failed to read WAV_TAG. Expected %s. Read %s.\r\n", WAV_TAG, pWaveFile->header.wavTag );
        return -1;
    }
    
    if( *(int *)pWaveFile->header.fmtTag != FMT_TAG )
    {
        flushPrintf( "WaveLoader::LoadWave() - Failed to read FMT_TAG. Expected %s. Read %s.\r\n", FMT_TAG, pWaveFile->header.fmtTag );
        return -1;
    }
    
    if ( pWaveFile->header.wavFormat != WAVE_FORMAT_PCM )
    {
        flushPrintf( "WaveLoader::LoadWave() - Only PCM files are supported.\r\n" );
        return -1;
    }
    
    if( pWaveFile->header.numChannels != 2 || 
        pWaveFile->header.samplesPerSec != SAMPLE_RATE || 
        pWaveFile->header.bitsPerSample != SAMPLE_BITS_PER_SAMPLE )
    {
        flushPrintf( "WaveLoader::LoadWave() - Wav file must be: Stereo, 44.1hz Sample Rate and 16 Bits Per Sample.\r\n"
                     "   Found %d Channels, %d Sample Rate, %d Bits Per Sample",
                     pWaveFile->header.numChannels,
                     pWaveFile->header.samplesPerSec,
                     pWaveFile->header.bitsPerSample );
        
        return -1;
    }
    
    return 0;
}

void WaveLoader::PrintWaveInfo( WAVE_FILE *pWaveFile )
{
    flushPrintf( "Wav Header: \r\n"
                    "riffTag: %s\r\n"
                    "fileSize: %d\r\n"
                    "wavTag: %s\r\n"
                    "fmtTag: %s\r\n"
                    "fmtTagLen: %d\r\n"
                    "wavFormat: %d\r\n"
                    "numChannels: %d\r\n"
                    "samplesPerSec: %d\r\n"
                    "avgBytesPerSec: %d\r\n"
                    "blockAlign: %d\r\n"
                    "bitsPerSample: %d\r\n"
                    "dataTag: %s\r\n"
                    "dataSectionSizeBytes: %d\r\n",
                    
                    pWaveFile->header.riffTag,
                    pWaveFile->header.fileSize,
                    pWaveFile->header.wavTag,
                    pWaveFile->header.fmtTag,
                    pWaveFile->header.fmtTagLen,
                    pWaveFile->header.wavFormat,
                    pWaveFile->header.numChannels,
                    pWaveFile->header.samplesPerSec,
                    pWaveFile->header.avgBytesPerSec,
                    pWaveFile->header.blockAlign,
                    pWaveFile->header.bitsPerSample,
                    pWaveFile->header.dataTag,
                    pWaveFile->header.dataSectionSizeBytes );
}
