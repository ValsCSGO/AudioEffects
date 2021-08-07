#pragma once

#include <vector>
#include <string>

#include "WavHeader.h"
#include "Effect.h"
#include "Delay.h"

using namespace std;

class EchoEffect : public Effect {
    public:
        EchoEffect(WavHeader);
        void process(double **in, double **out, int samples);
        void set_param(int index, double value);
        double get_param(int index);
        vector<string> list_params();
        vector<range_t> list_ranges();
        string get_name();

        enum class Parameter : int { TotalEchos, Delay, Alpha };
    private:
        double m_echos;
        double m_delay;
        double m_alpha;

        double l_max_amp = -1;
        double r_max_amp = -1;

        vector<Delay> l_delays;
        vector<Delay> r_delays;
};
