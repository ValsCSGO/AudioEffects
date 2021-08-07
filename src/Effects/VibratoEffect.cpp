#include <stdexcept>
#include <string>
#include <cmath>

#include "WavHeader.h"
#include "Effect.h"
#include "Effects/VibratoEffect.h"

VibratoEffect::VibratoEffect(WavHeader header) :
    Effect(header) {
}

void VibratoEffect::process(double **in, double **out, int samples) {
    double *left_in   = in[0];
    double *right_in  = in[1];
    double *left_out  = out[0];
    double *right_out = out[1];

    double lfo_amp = lfo.get_amp();

    for(int i = 0; i < samples; i++) {
        l_buffer.push(left_in[i]);
        r_buffer.push(right_in[i]);
        left_out[i]  = (l_buffer.pop() * m_alpha * lfo_amp * m_depth);
        right_out[i] = (r_buffer.pop() * m_alpha * lfo_amp * m_depth);
    }
}

void VibratoEffect::set_param(int index, double value) {
    switch(static_cast<Parameter>(index)) {
        case Parameter::Alpha:
            {
                m_alpha = value;
                double total_samples = m_header.get_samples_per_sec();
                l_buffer.set_delay(0/*total_samples*/);
                r_buffer.set_delay(0/*total_samples*/);
            }
            break;
        case Parameter::Freq:
            m_freq = value;
            lfo.setup(m_header.get_samples_per_sec(), m_freq);
            lfo.set_phase(0);
            break;
        case Parameter::Depth:
            m_depth = value;
            break;
        default:
            throw std::runtime_error("Error: Unknown set parameter");
            break;
    }
}

double VibratoEffect::get_param(int index) {
    switch(static_cast<Parameter>(index)) {
        case Parameter::Alpha:
            return m_alpha;
        case Parameter::Freq:
            return m_freq;
        case Parameter::Depth:
            return m_depth;
        default:
            throw std::runtime_error("Error: Unknown get parameter");
    }
}

vector<string> VibratoEffect::list_params() {
    vector<string> params = { "Alpha", "Freq", "Depth" };
    return params;
}

vector<range_t> VibratoEffect::list_ranges() {
    vector<range_t> ranges = { {0.0, 1.0, 0.01, 0.3}, {1000, 16000, 500, 2000}, {0, 1, 0.01, 0.5} };
    return ranges;
}

string VibratoEffect::get_name() {
    return "Vibrato";
}
