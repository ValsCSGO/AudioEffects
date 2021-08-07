#pragma once

#include <vector>
#include <string>

#include "WavHeader.h"
#include "Effect.h"
#include "Delay.h"

using namespace std;

class DelayEffect : public Effect {
    public:
        DelayEffect(WavHeader);
        void process(double **in, double **out, int samples);
        void set_param(int index, double value);
        double get_param(int index);
        vector<string> list_params();
        vector<range_t> list_ranges();
        string get_name();

        enum class Parameter : int { Delay };
    private:
        double m_delay;

        Delay l_buffer;
        Delay r_buffer;
};
