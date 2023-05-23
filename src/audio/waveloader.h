
// See License.md for license

#ifndef WAVELOADER_H_
#define WAVELOADER_H_

#define WAVE_FORMAT_PCM (1)
#define MONO_AUDIO      (1)
#define STEREO_AUDIO    (2)
#define RIFF_TAG        (0x46464952) // "RIFF"
#define WAV_TAG         (0x45564157) // "WAVE"
#define FMT_TAG         (0x20746D66) // "fmt " //note trailing space

#pragma pack(push,1)
struct WAVE_HEADER
{
    char  riffTag[ 4 ];         // should be "RIFF"
    int   fileSize;
    char  wavTag[ 4 ];          // should be "WAVE"
    char  fmtTag[ 4 ];          // should be "fmt " (note the trailing space)
    int   fmtTagLen;          
    short wavFormat;            // should be WAVE_FORMAT_PCM
    short numChannels;          // 1 Mono, 2 Stereo
    int   samplesPerSec;        // Num samples per second in hz - 44100 is typical
    int   avgBytesPerSec;       // (sampleRate * numChannels * bitsPerSample) / 8
    short blockAlign;           // size of a full sample in bytes - (numChannels * bitsPerSample) / 8
    short bitsPerSample;        // Usually 16bit
    char  dataTag[ 4 ];         // should be "data"
    int   dataSectionSizeBytes; // size of the actual audio data
};
#pragma pack(pop)

struct WAVE_FILE
{
    FILE       *pFile;
    WAVE_HEADER header;
};

class WaveLoader
{
public:
    static int  LoadWave( const char *pFileName, WAVE_FILE *pWaveFile );
    static void PrintWaveInfo( WAVE_FILE *pWaveFile );
};

#endif
