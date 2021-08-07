#pragma once

#include <vector>
#include <string>

#include "WavHeader.h"
#include "Effect.h"
#include "Delay.h"

using namespace std;

class MoveSourceEffect : public Effect {
    public:
        MoveSourceEffect(WavHeader);
        void process(double **in, double **out, int samples);
        void set_param(int index, double value);
        double get_param(int index);
        vector<string> list_params();
        vector<range_t> list_ranges();
        string get_name();

        enum class Parameter : int { Angle, FakeAngle };
    private:
        Delay l_buffer;
        Delay r_buffer;

        double m_angle;
        double m_fake_angle;
};
