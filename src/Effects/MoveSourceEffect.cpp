#include <stdexcept>
#include <string>
#include <cmath>

#include "WavHeader.h"
#include "Effect.h"
#include "Effects/MoveSourceEffect.h"

MoveSourceEffect::MoveSourceEffect(WavHeader header) :
    Effect(header) {
}

void MoveSourceEffect::process(double **in, double **out, int samples) {
    double *left_in   = in[0];
    double *right_in  = in[1];
    double *left_out  = out[0];
    double *right_out = out[1];

    double r_angle = m_angle * M_PI / 180;
    double r_fake_angle = m_fake_angle * M_PI / 180;

    double g_l = (cos(r_angle) * sin(r_fake_angle) + sin(r_angle) * cos(r_fake_angle)) / (2 * sin(r_angle) * cos(r_fake_angle));
    double g_r = (cos(r_angle) * sin(r_fake_angle) - sin(r_angle) * cos(r_fake_angle)) / (2 * sin(r_angle) * cos(r_fake_angle));

    for(int i = 0; i < samples; i++) {
        left_out[i]  = left_in[i]  * g_l;
        right_out[i] = right_in[i] * g_r;
    }
}

void MoveSourceEffect::set_param(int index, double value) {
    switch(static_cast<Parameter>(index)) {
        case Parameter::Angle:
            m_angle = value;
            break;
        case Parameter::FakeAngle:
            m_fake_angle = value;
            break;
        default:
            throw std::runtime_error("Error: Unknown set parameter");
            break;
    }
}

double MoveSourceEffect::get_param(int index) {
    switch(static_cast<Parameter>(index)) {
        case Parameter::Angle:
            return m_angle;
        case Parameter::FakeAngle:
            return m_fake_angle;
        default:
            throw std::runtime_error("Error: Unknown get parameter");
    }
}

vector<string> MoveSourceEffect::list_params() {
    vector<string> params = { "Angle", "FakeAngle" };
    return params;
}

vector<range_t> MoveSourceEffect::list_ranges() {
    vector<range_t> ranges = { {0, 90, 1, 45}, {0, 90, 1, -10} };
    return ranges;
}

string MoveSourceEffect::get_name() {
    return "MoveSource";
}
