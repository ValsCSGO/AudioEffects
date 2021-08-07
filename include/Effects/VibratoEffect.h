#pragma once

#include <vector>
#include <string>

#include "WavHeader.h"
#include "Effect.h"
#include "Delay.h"
#include "LFO.h"

using namespace std;

class VibratoEffect : public Effect {
    public:
        VibratoEffect(WavHeader);
        void process(double **in, double **out, int samples);
        void set_param(int index, double value);
        double get_param(int index);
        vector<string> list_params();
        vector<range_t> list_ranges();
        string get_name();

        enum class Parameter : int { Alpha, Freq, Depth };
    private:
        double m_alpha;
        double m_freq;
        double m_depth;

        Delay l_buffer;
        Delay r_buffer;

        LFO lfo;
};
