#!/bin/bash

# Create an array to store results
declare -A results

# Create directory for XML files if it doesn't exist
mkdir -p tests

# Collect filenames
test_files=$(find ../co24/auto-tests -type f -name '*.til' | tr ' ' '\n' | sort)

# Loop through sorted files
for file in $test_files; do
    # Extract filename without extension
    filename_with_ext=$(basename -- "$file")
    filename="${filename_with_ext%.*}"

    # Run the command and capture the output
    output0=$(cp ../co24/auto-tests/$filename.til tests/$filename.til)
    output1=$(./til --target asm tests/$filename.til)
    output2=$(yasm -felf32 -o tests/$filename.o tests/$filename.asm 2>&1)
    output3=$(ld -melf_i386 -o tests/$filename.out tests/$filename.o -lrts 2>&1)
    output4=$(sudo ./tests/$filename.out > tests/$filename.output)
    output5=$(diff ../co24/auto-tests/expected/$filename.out ./tests/$filename.output > tests/$filename.diff)
done

rm tests/*.til
rm tests/*.asm
rm tests/*.o
rm tests/*.out
rm tests/*.output
