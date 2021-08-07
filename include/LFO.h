#pragma once

class LFO {
    public:
        LFO();
        ~LFO();

        void setup(double sample_rate, double freq);
        void set_freq(double freq);
        void set_phase(double phase);
        double get_amp();

    private:
        double sample_rate;
        double freq;
        double phase;
        double index;
};
