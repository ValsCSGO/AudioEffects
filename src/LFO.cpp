#include <cmath>

#include "LFO.h"

LFO::LFO():
    index(0), sample_rate(0), freq(0), phase(0) { }

LFO::~LFO(void) {}

void LFO::setup(double rate, double f) {
    sample_rate = rate;
    freq = f;
}

void LFO::set_freq(double f)  {
    freq = f;
}

void LFO::set_phase(double p) {
    phase = p;
}

double LFO::get_amp() {
    const double shift = 2 * M_PI * freq / sample_rate;

    double amp = sin(phase);
    amp = (amp + 1) * 0.5;
    phase += shift;

    //wrap back to 0
    while(phase > 2 * M_PI)
        phase -= 2 * M_PI;

    return amp;
}
