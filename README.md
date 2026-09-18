# ECE-563-branch-predictor-simulator
C++ simulator comparing bimodal, gshare, and hybrid branch prediction on real traces.

# Branch Predictor Simulator

A C++ simulator implementing and comparing dynamic branch prediction schemes,
built for a microarchitecture course.

## Overview
Models three predictor designs and measures their misprediction rates on
instruction traces to analyze the accuracy vs. cost trade-offs between them.

## Predictors Implemented
- **Bimodal**: table of 2-bit saturating counters indexed by PC bits
- **Gshare**: counters indexed by PC XOR global history register, capturing
  branch correlation and reducing aliasing
- **Hybrid**: a chooser (meta-predictor) that selects between bimodal and
  gshare per branch

## Concepts
2-bit saturating counters, global history, aliasing, predictor correlation,
misprediction rate.

## Build & Run

make
./predictor <config> <trace_file>


## Results
Compared misprediction rates across the three schemes to analyze how history
length and hybrid selection affect prediction accuracy.
