#pragma once
#include <vector>
#include "softposit.h"

typedef posit16_t posit;

std::vector<posit> moving_average_posit(const std::vector<posit>& input, int window_size) {
    std::vector<posit> output(input.size());
    
    for(int i = 0; i < input.size(); ++i)
    {
        output[i] = convertDoubleToP16(0.0);

        for(int j = i; j >= 0; --j)
        {
            output[i] = p16_add(output[i], input[j] );

            if(j <= ( (i+1) - window_size) ) break;
        }

        if(i < window_size) output[i] = p16_div(output[i], convertDoubleToP16( static_cast<double>(i+1) ) );
        else output[i] = p16_div(output[i], convertDoubleToP16(static_cast<double> (window_size) ) );
    }
    return output;
}

std::vector<float> moving_average_float(const std::vector<float>& input, int window_size) {
    std::vector<float> output(input.size(), 0);
    
    for(int i = 0; i < input.size(); ++i)
    {
        for(int j = i; j >= 0; --j)
        {
            output[i] += input[j];
            if(j <= ( (i+1) - window_size) ) break;
        }

        if(i < window_size) output[i] /= (i+1);
        else output[i] /= window_size;
    }
    return output;
}
