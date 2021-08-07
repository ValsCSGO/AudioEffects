#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>

#include "Effect.h"
#include "alsa.h"
#include "WavHeader.h"
#include "Effects/ReverseEffect.h"
#include "Effects/DelayEffect.h"
#include "Effects/EchoEffect.h"
#include "Effects/VibratoEffect.h"
#include "Effects/MoveSourceEffect.h"

using namespace std;
using DParameter = DelayEffect::Parameter;
using EParameter = EchoEffect::Parameter;
using VParameter = VibratoEffect::Parameter;
using MParameter = MoveSourceEffect::Parameter;

enum class eEffect { None = 0, Reverse, Delay, Echo, Vibrato, MoveSource };

int main(int argc, char *argv[]) {
    const char *file_path;
    string input;
    if (argc <= 1) {
        cout << "Input wave file name: ";
        cin >> input;
        cin.get();
        file_path = input.c_str();
    } else {
        file_path = argv[1];
        cout << "Input wave file name: " << file_path << endl;
    }

    // open the wav file and turn it into an object
    WavHeader header;
    header.open(file_path);
    header.parse();

    // set up buffers
    static const int16_t BUFFER_SIZE = 4096;
    int16_t *buffer = new int16_t[BUFFER_SIZE * header.get_channels()];
    double **dry = new double*[header.get_channels()];
    double **wet = new double*[header.get_channels()];
    for(int i = 0; i < header.get_channels(); i++) {
        dry[i] = new double[BUFFER_SIZE];
        wet[i] = new double[BUFFER_SIZE];
    }

    // set up alsa to accept our wav file
    ALSA alsa;
    alsa.setup_device(header);
    alsa.setup_buffer(header, BUFFER_SIZE);

    // create effects
    ReverseEffect reverse_effect(header);

    DelayEffect delay_effect(header);
    delay_effect.set_param(static_cast<int>(DParameter::Delay), 1.2);

    EchoEffect echo_effect(header);
    echo_effect.set_param(static_cast<int>(EParameter::Delay), 0.25);
    echo_effect.set_param(static_cast<int>(EParameter::TotalEchos), 6);
    echo_effect.set_param(static_cast<int>(EParameter::Alpha), 0.8);

    VibratoEffect vibrato_effect(header);
    vibrato_effect.set_param(static_cast<int>(VParameter::Depth), 0.9);
    vibrato_effect.set_param(static_cast<int>(VParameter::Alpha), 1);
    vibrato_effect.set_param(static_cast<int>(VParameter::Freq), 7000);

    MoveSourceEffect move_source_effect(header);
    move_source_effect.set_param(static_cast<int>(MParameter::Angle), 45);
    move_source_effect.set_param(static_cast<int>(MParameter::FakeAngle), 25);

    //currently selected effect
    int effect = 4;

    cout << "0: None" << endl;
    cout << "1: " << reverse_effect.get_name() << endl;
    cout << "2: " << delay_effect.get_name() << endl;
    cout << "3: " << echo_effect.get_name() << endl;
    cout << "4: " << vibrato_effect.get_name() << endl;
    cout << "5: " << move_source_effect.get_name() << endl;
    printf("Choose an effect: ");
    cin >> effect;
    cout << endl;

    /*
    switch(static_cast<eEffect>(effect)) {
        case eEffect::None:
        default:
            break;
        case eEffect::Reverse:
            printf("No settings for ReverseEffect");
            break;
        case eEffect::Delay:
            handle_settings<DelayEffect>(delay_effect);
            break;
        case eEffect::Echo:
            handle_settings<EchoEffect>(echo_effect);
            break;
        case eEffect::Vibrato:
            handle_settings<VibratoEffect>(vibrato_effect);
            break;
        case eEffect::MoveSource:
            handle_settings<MoveSourceEffect>(move_source_effect);
            break;
    }
    */

    // read the wav data and
    size_t samplesRead;
    const int num_chans = header.get_channels();
    while ((samplesRead = header.read(buffer, BUFFER_SIZE)) > 0) {
       // convert samples to a array that needs to be processed before being played
        for(int channel = 0; channel < num_chans; channel++) {
            for(int sample = 0; sample < samplesRead; sample++) {
                dry[channel][sample] = (double)buffer[sample * num_chans + channel] / 32768.0;
            }
        }

        // process the audio data and apply effects to it
        switch(static_cast<eEffect>(effect)) {
            case eEffect::None:
            default:
                for(int channel = 0; channel < num_chans; channel++) {
                    memcpy(wet[channel], dry[channel], samplesRead * sizeof(double));
                }
                break;
            case eEffect::Reverse:
                reverse_effect.process(dry, wet, samplesRead);
                break;
            case eEffect::Delay:
                delay_effect.process(dry, wet, samplesRead);
                break;
            case eEffect::Echo:
                echo_effect.process(dry, wet, samplesRead);
                break;
            case eEffect::Vibrato:
                vibrato_effect.process(dry, wet, samplesRead);
                break;
            case eEffect::MoveSource:
                move_source_effect.process(dry, wet, samplesRead);
                break;
        }

        // convert the samples from the wet buffer back into the play buffer
        for(int channel = 0; channel < num_chans; channel++) {
            for(int sample = 0; sample < samplesRead; sample++) {
                buffer[sample * num_chans + channel] = wet[channel][sample] * 32768.0;
            }
        }

        // simply play the samples in the buffer
        alsa.play(buffer, samplesRead);
    }

    // clean up
    alsa.cleanup();

    delete[] buffer;

    for(int i = 0; i < header.get_channels(); i++) {
        delete[](dry[i]);
        delete[](wet[i]);
    }
    delete[](dry);
    delete[](wet);

    fclose(header.get_file());

    return 0;
}
