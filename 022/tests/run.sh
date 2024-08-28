#!/bin/bash

# Iterate over each test file in the auto-tests directory
for test_file in ../../co24/auto-tests/*.til; do
    # Extract the base filename without the extension
    base_filename=$(basename "$test_file" .til)
    # Run the compiler on the test file and generate the XML file
    ../til "$test_file" -o "../../xmls/${base_filename}.xml"
done
