#include <stdexcept>
#include <cmath>
#include <string>
#include <cstring>
#include <iostream>

#include "WavHeader.h"
#include "Effect.h"
#include "Effects/EchoEffect.h"

EchoEffect::EchoEffect(WavHeader header) :
    Effect(header) {
}

void EchoEffect::process(double **in, double **out, int samples) {
    double *left_in   = in[0];
    double *right_in  = in[1];
    double *left_out  = out[0];
    double *right_out = out[1];

    std::memcpy(left_out, left_in, samples * sizeof(double));
    std::memcpy(right_out, right_in, samples * sizeof(double));
    for(int echo = 0; echo < m_echos; echo++) {
        for(int sample = 0; sample < samples; sample++) {
            l_delays[echo].push(left_in[sample]);
            r_delays[echo].push(right_in[sample]);

            double l_pop = l_delays[echo].pop();
            double r_pop = r_delays[echo].pop();
            double echo_pow = pow(m_alpha, echo + 1);
            left_out[sample]  += (l_pop * echo_pow);
            right_out[sample] += (r_pop * echo_pow);

            if(left_out[sample] > l_max_amp)
                l_max_amp = left_out[sample];
            if(right_out[sample] > r_max_amp)
                r_max_amp = right_out[sample];

            left_out[sample] /= l_max_amp;
            right_out[sample] /= r_max_amp;
        }
    }
}

void EchoEffect::set_param(int index, double value) {
    switch(static_cast<Parameter>(index)) {
        case Parameter::TotalEchos:
            m_echos = value;
            for(int echo = 0; echo < m_echos; echo++) {
                Delay delay;
                delay.set_delay(m_header.get_samples_per_sec() * m_delay * echo);
                l_delays.push_back(delay);
                r_delays.push_back(delay);
            }
            break;
        case Parameter::Delay:
            m_delay = value;
            for(int echo = 0; echo < m_echos; echo++) {
                l_delays[echo].set_delay(m_header.get_samples_per_sec() * m_delay * echo);
                r_delays[echo].set_delay(m_header.get_samples_per_sec() * m_delay * echo);
            }
            break;
        case Parameter::Alpha:
            m_alpha = value;
            break;
        default:
            throw std::runtime_error("Error: Unknown parameter");
            break;
    }
}

double EchoEffect::get_param(int index) {
    switch(static_cast<Parameter>(index)) {
        case Parameter::TotalEchos:
            return m_echos;
        case Parameter::Delay:
            return m_delay;
        case Parameter::Alpha:
            return m_alpha;
        default:
            throw std::runtime_error("Error: Unknown parameter");
    }
}

vector<string> EchoEffect::list_params() {
    vector<string> params = { "TotalEchos", "Delay", "Alpha" };
    return params;
}

vector<range_t> EchoEffect::list_ranges() {
    vector<range_t> ranges = { {0.0, 20.0, 1.0, 1.0}, {0.0, 5.0, 0.05, 0.1}, {0.0, 1.0, 0.01, 0.8} };
    return ranges;
}

string EchoEffect::get_name() {
    return "Echo";
}
