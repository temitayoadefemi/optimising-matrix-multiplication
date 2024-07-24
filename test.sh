#!/bin/bash

# Compile the C program
gcc main.c -o matrix_multiply -lm

# Sizes of the matrices to multiply
sizes=("32 32 32" "64 64 64" "128 128 128" "256 256 256" "512 512 512" "1024 1024 1024")

# Loop over sizes and run each once
for size in "${sizes[@]}"; do
    read m n p <<< $size
    ./matrix_multiply $m $n $p "output_${m}x${n}x${p}.txt"
done