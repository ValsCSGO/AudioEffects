#pragma once

class WavHeader {
    public:
        void open(const char *);
        int read(void *, int);
        void parse();

        FILE* get_file();
        uint16_t get_channels();
        uint32_t get_samples_per_sec();
        uint16_t get_bits_per_sample();

        void print();
    private:
        FILE *file;

        // RIFF subchunk
        uint32_t ChunkSize;     // RIFF Chunk Size

        // FMT subchunk
        uint32_t Subchunk1Size; // Size of the fmt chunk
        uint16_t AudioFormat;   // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
        uint16_t NumOfChan;     // Number of channels 1=Mono 2=Sterio
        uint32_t SamplesPerSec; // Sampling Frequency in Hz
        uint32_t BytesPerSec;   // bytes per second
        uint16_t BlockAlign;    // 2=16-bit mono, 4=16-bit stereo
        uint16_t BitsPerSample; // Number of bits per sample

        // DATA subchunk
        uint32_t Subchunk2Size; // Sampled data length
};
