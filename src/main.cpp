#include <iostream>
#include <vector>
#include "dsp_filters.hpp"
#include "dummy_signal.hpp"
#include "softposit.h"
#include <chrono>

typedef posit16_t posit;

class Timer {
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
        std::string operation_name;
    
    public:
        Timer(const std::string& name) : operation_name(name) {
            start_time = std::chrono::high_resolution_clock::now();
        }
    
        ~Timer() {
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
            std::cout << operation_name << " took " << duration << " microseconds." << std::endl;
        }
};

//Converting posit signal to float signal values for analzying
std::vector<float> convertPositVectorTofloat(const std::vector<posit>& positVector) {
    std::vector<float> floatVector(positVector.size());
    for (size_t i = 0; i < positVector.size(); ++i) {
        floatVector[i] = static_cast<float> ( convertP16ToDouble(positVector[i]) );
    }
    return floatVector;
}

//Calculating Mean Squared Error
double calculateMSE(const std::vector<float>& signal1, const std::vector<float>& signal2) {
    if (signal1.size() != signal2.size()) {
        throw std::invalid_argument("Signals must have the same length");
    }
    
    double sum = 0.0;
    for (size_t i = 0; i < signal1.size(); ++i) {
        double diff = signal1[i] - signal2[i];
        sum += diff * diff;
    }
    
    return sum / signal1.size();
}

//Calculating SNR
double calculateSNR(const std::vector<float>& original, const std::vector<float>& processed) {
    if (original.size() != processed.size()) {
        throw std::invalid_argument("Signals must have the same length");
    }
    
    // Calculate signal power (power of original signal)
    double signalPower = 0.0;
    for (const auto& val : original) {
        signalPower += val * val;
    }
    signalPower /= original.size();
    
    // Calculate noise power (power of the difference)
    double noisePower = 0.0;
    for (size_t i = 0; i < original.size(); ++i) {
        double diff = original[i] - processed[i];
        noisePower += diff * diff;
    }
    noisePower /= original.size();
    
    if (noisePower < 1e-10) {
        return std::numeric_limits<double>::max(); // Return very high SNR
    }
    
    // SNR in decibels
    return 10.0 * std::log10(signalPower / noisePower);
}

//Estimating memory usage for float signal
size_t estimateMemoryUsage_float(const std::vector<float>& signal) {
   
    size_t capacitySize = signal.capacity() * sizeof(float);
    size_t vectorOverhead = sizeof(std::vector<float>);
    
    return vectorOverhead + capacitySize;
}

//Estimating memory usage for posit signal
size_t estimateMemoryUsage_posit(const std::vector<posit>& signal) {
    
    size_t capacitySize = signal.capacity() * sizeof(posit);
    size_t vectorOverhead = sizeof(std::vector<posit>);
    
    return vectorOverhead + capacitySize;
}

//Comparing Memory Usage
void compareMemoryUsage(const std::vector<float>& floatSignal, 
                        const std::vector<posit>& positSignal) {
    size_t floatMemory = estimateMemoryUsage_float(floatSignal);
    size_t positMemory = estimateMemoryUsage_posit(positSignal);
    
    std::cout << "Float implementation memory usage: " << floatMemory << " bytes\n";
    std::cout << "Posit implementation memory usage: " << positMemory << " bytes\n";
    std::cout << "Memory difference: " << 
        (floatMemory > positMemory ? 
         floatMemory - positMemory : 
         positMemory - floatMemory) << " bytes\n";
    
    double ratio = static_cast<double>(floatMemory) / positMemory;
    std::cout << "Float/Posit memory ratio: " << ratio << "\n";
}

//Calcuating Max Absolute Error
double calculateMaxAbsError(const std::vector<float>& signal1, const std::vector<float>& signal2) {
    if (signal1.size() != signal2.size()) {
        throw std::invalid_argument("Signals must have the same length");
    }
    
    double maxError = 0.0;
    for (size_t i = 0; i < signal1.size(); ++i) {
        double error = std::abs(signal1[i] - signal2[i]);
        maxError = std::max(maxError, error);
    }
    
    return maxError;
}

//Energy Conservation Analysis
float analyzeEnergyConservation(const std::vector<float>& inputSignal, 
                               const std::vector<float>& outputSignal) {
    if (inputSignal.size() != outputSignal.size()) {
        std::cerr << "Error: Signals must have the same length\n";
        return 0.0;
    }
    
    // Calculate input signal energy
    float inputEnergy = 0.0f;
    for (const auto& val : inputSignal) {
        inputEnergy += val * val;
    }
    
    // Calculate output signal energy
    float outputEnergy = 0.0f;
    for (const auto& val : outputSignal) {
        outputEnergy += val * val;
    }
    
    //calculating energy ratio
    float ratio = outputEnergy / inputEnergy;
    float percentChange = (outputEnergy - inputEnergy) / inputEnergy * 100.0f;
    
    return std::abs(ratio - 1.0f);
}

int main()
{
    const int N = 100;

    double noiseLevel = 0.005;
    int window_size = 10;

    auto signal_posit = generate_sine_wave_posit(N);
    auto original_signal_posit = signal_posit;

    auto signal_float = generate_sine_wave_float(N);
    auto original_signal_float = signal_float;


    add_noise_posit(signal_posit, convertDoubleToP16(noiseLevel) );
    add_noise_float(signal_float, static_cast<float> (noiseLevel) );

    std::cout<< "Noise level: " << noiseLevel << "\n";

    std::vector<posit> mov_avg_posit;
    {
        Timer t1("Posit mov avg filtering time");
        mov_avg_posit = moving_average_posit(signal_posit, window_size);
    }

    std::vector<float> mov_avg_float;
    {
        Timer t2("Float mov filtering time");
        mov_avg_float = moving_average_float(signal_float, window_size);
    }

    //Converting posit signal values to float signal values for analysis
    std::vector<float> mov_avg_posit_float = convertPositVectorTofloat(mov_avg_posit);
    std::vector<float> original_signal_posit_float = convertPositVectorTofloat(original_signal_posit);

    //Mean Squared Error
    double mov_avg_posit_MSE = calculateMSE(original_signal_posit_float, mov_avg_posit_float);
    double mov_avg_float_MSE = calculateMSE(original_signal_float, mov_avg_float);

    //Signal to noise ratio
    double SNR_posit = calculateSNR(original_signal_posit_float, mov_avg_posit_float);
    double SNR_float = calculateSNR(original_signal_float, mov_avg_float);

    std::cout << "Posit Moving average MSE: " << mov_avg_posit_MSE << "\n";
    std::cout << "Float Moving average MSE: " << mov_avg_float_MSE << "\n";

    std::cout << "SNR MovAvg posit: " << SNR_posit << "\n";
    std::cout << "SNR MovAvg float: " << SNR_float << "\n";
    std::cout << "SNR ratio MovAvg posit/float: " << SNR_posit / SNR_float << "\n";

    compareMemoryUsage(mov_avg_float, mov_avg_posit);

    //Maximum Absolute Error for posit
    double maxAbsError_posit = calculateMaxAbsError(original_signal_posit_float, mov_avg_posit_float);
    std::cout << "MaxAbsError MovAvg posit: " << maxAbsError_posit << "\n";

    //Maximum Absolute Error for float
    double maxAbsError_float = calculateMaxAbsError(original_signal_float, mov_avg_float);
    std::cout << "MaxAbsError MovAvg float: " << maxAbsError_float << "\n";

    //Analyzing Energy conservation
    float positEnergy_Abs_diff = analyzeEnergyConservation(original_signal_posit_float, mov_avg_posit_float);
    float floatEnergy_Abs_diff = analyzeEnergyConservation(original_signal_float, mov_avg_float);

    std::cout << "Energy Ratio MovAvg Posit Diff: " << positEnergy_Abs_diff << "\n";
    std::cout << "Energy Ratio MovAvg Float Diff: " << floatEnergy_Abs_diff << "\n";

    if(positEnergy_Abs_diff < floatEnergy_Abs_diff) {
        std::cout << "Energy is conserved more in MovAvg posit" << "\n";
    }
    else {
        std::cout << "Energy is conserved more in MovAvg float" << "\n";
    }

    return 0;
}