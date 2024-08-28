#!/bin/bash

# Set the directories
BASE_DIR="/home/unicorn_jesus/Desktop/Projetos/Comp/022/tests"
TEST_DIR="/home/unicorn_jesus/Desktop/Projetos/Comp/co24/auto-tests"
EXPECTED_DIR="$TEST_DIR/expected"
BIN_DIR="$TEST_DIR/bin"

mkdir -p "$BIN_DIR"

# Initialize counters
total_tests=0
passed_tests=0
failed_tests=0
failed_tests_list=()

# Iterate over all .til files in the test directory
for til_file in "$TEST_DIR"/*.til; do
    test_name=$(basename "$til_file" .til)
    asm_file="$TEST_DIR/$test_name.asm"
    obj_file="$TEST_DIR/$test_name.o"
    bin_file="$BIN_DIR/$test_name"
    actual_output_file="$BIN_DIR/$test_name.out"
    expected_file="$EXPECTED_DIR/$test_name.out"

    echo "Running test: $test_name"
    echo "Expected output: $expected_file"
    echo "Generated binary: $bin_file"
    echo "Generated assembly: $asm_file"
    echo "Generated object: $obj_file"

    # Compile the .til file to .asm
    ../til --target asm "$til_file"
    if [[ $? -ne 0 ]]; then
        echo "FAIL: $test_name (compilation error)"
        failed_tests_list+=("$test_name (compilation error)")
        failed_tests=$((failed_tests + 1))
        continue
    fi

    # Assemble the .asm file to .o
    yasm -felf32 "$asm_file" -o "$obj_file"
    if [[ $? -ne 0 ]]; then
        echo "FAIL: $test_name (assembly error)"
        failed_tests_list+=("$test_name (assembly error)")
        failed_tests=$((failed_tests + 1))
        continue
    fi

    # Link the .o file to create the binary
    ld -melf_i386 -o "$bin_file" "$obj_file" -L"$HOME/compiladores/root/usr/lib" -lrts
    if [[ $? -ne 0 ]]; then
        echo "FAIL: $test_name (linking error)"
        failed_tests_list+=("$test_name (linking error)")
        failed_tests=$((failed_tests + 1))
        continue
    fi

    # Run the binary and capture its output
    "$bin_file" > "$actual_output_file"
    if [[ $? -ne 0 ]]; then
        echo "FAIL: $test_name (runtime error)"
        failed_tests_list+=("$test_name (runtime error)")
        failed_tests=$((failed_tests + 1))
        continue
    fi

    # Compare the captured output with the expected output
    if cmp -s "$actual_output_file" "$expected_file"; then
        echo "PASS: $test_name"
        passed_tests=$((passed_tests + 1))
    else
        echo "FAIL: $test_name (output mismatch)"
        failed_tests_list+=("$test_name (output mismatch)")
        failed_tests=$((failed_tests + 1))
    fi

    total_tests=$((total_tests + 1))
done

rm "$TEST_DIR"/*.asm "$TEST_DIR"/*.o
# Summary
echo
echo "Test summary:"
echo "Total tests: $total_tests"
echo "Passed tests: $passed_tests"
echo "Failed tests: $failed_tests"

# Print the list of failed tests
if [ $failed_tests -ne 0 ]; then
    echo "Failed tests:"
    for failed_test in "${failed_tests_list[@]}"; do
        echo "$failed_test"
    done
fi

exit 0
