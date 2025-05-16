# Moving Average DSP Algorithm: Posit vs Float Benchmarking

## Overview

This repository contains benchmarking code and results comparing the performance and accuracy of moving average DSP algo

## Moving Average Filters

Moving average filters are fundamental DSP components used for smoothing signals and reducing random noise while retaining a sharp step response

## Benchmarking

Both formats' filters are becnhmarked using, filtering time, Memory usage, Signal-to-Noise Ratio (SNR), Mean Squared Error (MSE), Max Absoulute Error(MaxAbsError) and Energy conservation.

## From one of the results shown:

Noise level: 0.005
Posit mov avg filtering time took 51 microseconds.
Float mov filtering time took 24 microseconds.

Posit Moving average MSE: 0.552091
Float Moving average MSE: 0.552115

SNR MovAvg posit: -0.430618
SNR MovAvg float: -0.430593
SNR ratio MovAvg posit/float: 1.00006

Float implementation memory usage: 424 bytes
Posit implementation memory usage: 224 bytes
Memory difference: 200 bytes
Float/Posit memory ratio: 1.89286

MaxAbsError MovAvg posit: 1.10132
MaxAbsError MovAvg float: 1.10144

Energy Ratio MovAvg Posit Diff: 0.577568
Energy Ratio MovAvg Float Diff: 0.577585
Energy is conserved more in MovAvg posit

This shows posit implementation took more time to process data but it has HIGH SNR, low MSE, less memory usage, relatively less max absolute error and more energy is conserved as compared to float filter implementation.
These values show more and more difference when noise get more precise showcasing preciseness of posit.
