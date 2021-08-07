#include <stdexcept>
#include <string>

#include "WavHeader.h"
#include "Effect.h"
#include "Effects/ReverseEffect.h"

ReverseEffect::ReverseEffect(WavHeader header) :
    Effect(header) {
}

void ReverseEffect::process(double **in, double **out, int samples) {
    double *left_in   = in[0];
    double *right_in  = in[1];
    double *left_out  = out[0];
    double *right_out = out[1];

    for(int i = 0; i < samples; i++) {
        left_out[samples - i - 1] = left_in[i];
        right_out[samples - i - 1] = right_in[i];
    }
}

void ReverseEffect::set_param(int index, double value) {
    switch(static_cast<Parameter>(index)) {
        default:
            throw std::runtime_error("Error: Unknown parameter");
    }
}

double ReverseEffect::get_param(int index) {
    switch(static_cast<Parameter>(index)) {
        default:
            throw std::runtime_error("Error: Unknown parameter");
    }
}

vector<string> ReverseEffect::list_params() {
    vector<string> params = {};
    return params;
}

vector<range_t> ReverseEffect::list_ranges() {
    vector<range_t> ranges = {};
    return ranges;
}

string ReverseEffect::get_name() {
    return "Reverse";
}
