#pragma once

#include <vector>
#include <string>
#include "WavHeader.h"

using namespace std;

struct range_t {
    double min;
    double max;
    double step;
    double def;
};

class Effect {
    public:
        Effect(WavHeader header) :
            m_header(header) { };
        virtual void process(double **in, double **out, int samples) = 0;
        virtual void set_param(int index, double value) = 0;
        virtual double get_param(int index) = 0;
        virtual vector<string> list_params() = 0;
        virtual vector<range_t> list_ranges() = 0;
        virtual string get_name() = 0;
    protected:
        WavHeader m_header;
};

