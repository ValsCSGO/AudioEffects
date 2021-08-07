#pragma once

#include <vector>
#include <string>

#include "WavHeader.h"
#include "Effect.h"
#include "Delay.h"

using namespace std;

class ReverseEffect : public Effect {
    public:
        ReverseEffect(WavHeader);
        void process(double **in, double **out, int samples);
        void set_param(int index, double value);
        double get_param(int index);
        vector<string> list_params();
        vector<range_t> list_ranges();
        string get_name();
    private:
        Delay l_buffer;
        Delay r_buffer;

        enum class Parameter : int {  };
};
