#include <alsa/asoundlib.h>
#include <iostream>

#include "alsa.h"
#include "WavHeader.h"

#define PCM_DEVICE "default"

using namespace std;

int ALSA::setup_device(WavHeader header) {
    unsigned int pcm;
    int err;

    err = snd_pcm_open(&pcm_handle, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0)
        printf("ERROR: Can't open \"%s\" PCM device. %s\n", PCM_DEVICE, snd_strerror(pcm));

    err = snd_pcm_hw_params_malloc(&params);
    if (err < 0)
        printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(pcm));

    err = snd_pcm_hw_params_any(pcm_handle, params);
    if (err < 0)
        printf("ERROR: Can't set params. %s\n", snd_strerror(pcm));

    err = snd_pcm_hw_params_set_access(pcm_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (err < 0)
        printf("ERROR: Can't set access. %s\n", snd_strerror(pcm));

    err = snd_pcm_hw_params_set_format(pcm_handle, params, SND_PCM_FORMAT_S16_LE);
    if (err < 0)
        printf("ERROR: Can't set format. %s\n", snd_strerror(pcm));

    err = snd_pcm_hw_params_set_channels(pcm_handle, params, header.get_channels());
    if (err < 0)
        printf("ERROR: Can't set channels number. %s\n", snd_strerror(pcm));

    unsigned int rate = header.get_samples_per_sec();
    err = snd_pcm_hw_params_set_rate_near(pcm_handle, params, &rate, 0);
    if (err < 0)
        printf("ERROR: Can't set rate. %s\n", snd_strerror(pcm));

    err = snd_pcm_hw_params(pcm_handle, params);
    if (err < 0)
        printf("ERROR: Can't set harware parameters. %s\n", snd_strerror(pcm));

   return err;
}

void ALSA::setup_buffer(WavHeader header, int samples) {
    unsigned int channels, pcm;
    snd_pcm_hw_params_get_channels(params, &channels);
    printf("channels: %i ", channels);

    if (channels == 1)
        printf("(mono)\n");
    else if (channels == 2)
        printf("(stereo)\n");

    int16_t bytes_per_sample = header.get_bits_per_sample() / 8;

    snd_pcm_uframes_t buffer_size = samples;
    snd_pcm_hw_params_set_buffer_size_near(pcm_handle, params, &buffer_size);

    snd_pcm_uframes_t period_size = bytes_per_sample;
    snd_pcm_hw_params_set_period_size_near(pcm_handle, params, &period_size, NULL);
}

void ALSA::play(void *buffer, int frames) {
    unsigned int pcm;
    int err = snd_pcm_writei(pcm_handle, buffer, frames);
    cout << "Number of frames written: " << err << "" << endl;

    if (err == -EPIPE) {
        // an underrun occurred
        printf("XRUN.\n");
        snd_pcm_prepare(pcm_handle);
    } else if (err < 0) {
        printf("ERROR. Can't write to PCM device. %s\n", snd_strerror(pcm));
    }
}

void ALSA::cleanup() {
    snd_pcm_drain(pcm_handle);
    snd_pcm_close(pcm_handle);
}
