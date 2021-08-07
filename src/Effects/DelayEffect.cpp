#include <stdexcept>
#include <string>

#include "WavHeader.h"
#include "Effect.h"
#include "Effects/DelayEffect.h"

DelayEffect::DelayEffect(WavHeader header) :
    Effect(header) {
}

void DelayEffect::process(double **in, double **out, int samples) {
    double *left_in   = in[0];
    double *right_in  = in[1];
    double *left_out  = out[0];
    double *right_out = out[1];

    for(int i = 0; i < samples; i++) {
        l_buffer.push(left_in[i]);
        r_buffer.push(right_in[i]);
        left_out[i]  = l_buffer.pop();
        right_out[i] = r_buffer.pop();
    }
}

void DelayEffect::set_param(int index, double value) {
    switch(static_cast<Parameter>(index)) {
        case Parameter::Delay:
            {
                m_delay = value;
                double total_samples = m_header.get_samples_per_sec() * m_delay;
                l_buffer.set_delay(total_samples);
                r_buffer.set_delay(total_samples);
                break;
            }
        default:
            throw std::runtime_error("Error: Unknown parameter");
            break;
    }
}

double DelayEffect::get_param(int index) {
    switch(static_cast<Parameter>(index)) {
        case Parameter::Delay:
            return m_delay;
        default:
            throw std::runtime_error("Error: Unknown parameter");
    }
}

vector<string> DelayEffect::list_params() {
    vector<string> params = { "Delay" };
    return params;
}

vector<range_t> DelayEffect::list_ranges() {
    vector<range_t> ranges = { {0.0, 5.0, 0.05, 0.1} };
    return ranges;
}

string DelayEffect::get_name() {
    return "Delay";
}
