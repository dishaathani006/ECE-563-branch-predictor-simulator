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

## Build

make

Produces the `sim` binary. Use `make clean` to remove build artifacts.

## Run

./sim bimodal <M2> <tracefile>
./sim gshare <M1> <N> <tracefile>
./sim hybrid <K> <M1> <N> <M2> <tracefile>


Parameters:
- M1: PC index bits for the gshare table
- M2: PC index bits for the bimodal table
- N:  global history register bits (gshare)
- K:  PC index bits for the chooser table (hybrid)

Examples:

./sim bimodal 6 gcc_trace.txt
./sim gshare 9 3 gcc_trace.txt
./sim hybrid 8 14 10 5 gcc_trace.txt

## Results
Compared misprediction rates across the three schemes to analyze how history
length and hybrid selection affect prediction accuracy.
