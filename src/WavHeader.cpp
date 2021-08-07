#include <cstring>
#include <iostream>
#include <stdio.h>

#include "WavHeader.h"

using namespace std;

void WavHeader::open(const char *file_path) {
    file = fopen(file_path, "r");
    if (file == nullptr)    {
        fprintf(stderr, "Unable to open wave file: %s\n", file);
        return;
    }
}

int WavHeader::read(void *buffer, int samples) {
    return fread(buffer, BlockAlign, samples, file);
}

void WavHeader::parse() {
    char header[5];
    unsigned int size;

    // at the start of the RIFF subchuink
    fread(header, 1, 4, file);
    header[4] = '\0';
    if (strcmp(header, "RIFF")) {
        fclose(file);
        printf("InvalidFileFormat: Not a wav file.");
        return;
    }

    fread(&ChunkSize, 4, 1, file);

    // at the start of the WAVE subchunk
    fread(header, 1, 4, file);
    header[4] = '\0';
    if (strcmp(header, "WAVE")) {
        fclose(file);
        printf("InvalidFileFormat: corrupted wav file.");
        return;
    }

    fseek(file, 4, SEEK_CUR);
    fread(&Subchunk1Size, 4, 1, file);
    fread(&AudioFormat, 2, 1, file);
    fread(&NumOfChan, 2, 1, file);
    fread(&SamplesPerSec, 4, 1, file);
    fread(&BytesPerSec, 4, 1, file);
    fread(&BlockAlign, 2, 1, file);
    fread(&BitsPerSample, 2, 1, file);

    fseek(file, Subchunk1Size - 16, SEEK_CUR);

    // at the start of the data subchunk
    fread(header, 1, 4, file);
    while (strcmp(header, "data")) {
        if (fread(&size, 4, 1, file) != 1) {
            printf("InvalidFileFormat: corrupted wav file.");
            return;
        }
        fseek(file, size, SEEK_CUR);
        fread(header, 1, 4, file);
    }
    fread(&Subchunk2Size, 4, 1, file);

    print();
}

FILE* WavHeader::get_file() {
    return file;
}

uint16_t WavHeader::get_channels() {
    return NumOfChan;
}

uint32_t WavHeader::get_samples_per_sec() {
    return SamplesPerSec;
}

uint16_t WavHeader::get_bits_per_sample() {
    return BitsPerSample;
}

void WavHeader::print() {
    cout << "ChunkSize     " << ChunkSize      << endl;

    cout << "Subchunk1Size " << Subchunk1Size  << endl;
    cout << "AudioFormat   " << AudioFormat    << endl;
    cout << "NumOfChan     " << NumOfChan      << endl;
    cout << "SamplesPerSec " << SamplesPerSec  << endl;
    cout << "BytesPerSec   " << BytesPerSec    << endl;
    cout << "BlockAlign    " << BlockAlign     << endl;
    cout << "BitsPerSample " << BitsPerSample  << endl;

    cout << "Subchunk2Size " << Subchunk2Size  << endl;
}
