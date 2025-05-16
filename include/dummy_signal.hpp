#pragma once
#include <vector>
#include <cmath>
#include <random>
#include "softposit.h"

typedef posit16_t posit;

#define Frequeny                5.0
#define Sampling_frequency      100.0

std::vector<posit> generate_sine_wave_posit(int N) {
    std::vector<posit> signal(N);

    for (int n = 0; n < N; ++n) {
        double val = std::sin(2.0f * M_PI * Frequeny * n / Sampling_frequency);
        
        signal[n] = convertDoubleToP16(val);
    }
    return signal;
}

std::vector<float> generate_sine_wave_float(int N) {
    std::vector<float> signal(N);
    
    for (int n = 0; n < N; ++n) {
        signal[n] = std::sin(2 * M_PI * Frequeny * n / Sampling_frequency);
    }
    return signal;
}


void add_noise_posit(std::vector<posit>& signal, posit noise_level) {
    std::default_random_engine gen;
    std::normal_distribution<float> dist(0.0, convertP16ToDouble(noise_level) );
    for (auto& s : signal)
        s = p16_add(s, convertDoubleToP16(dist(gen)) ) ;
}

void add_noise_float(std::vector<float>& signal, double noise_level) {
    std::default_random_engine gen;
    std::normal_distribution<float> dist(0.0, noise_level);
    for (auto& s : signal)
        s += dist(gen);
}

