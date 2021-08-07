#pragma once

#include <alsa/asoundlib.h>
#include "WavHeader.h"

class ALSA {
    public:
        int setup_device(WavHeader);
        void setup_buffer(WavHeader, int);
        void play(void *, int);
        void cleanup();
    private:
        snd_pcm_t *pcm_handle;
        snd_pcm_hw_params_t *params;
};
