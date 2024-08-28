#!/bin/zsh

TEST_TIME=$(date +"%Y%m%d%H%M")

./0-begin.sh "$TEST_TIME"

./1-clone.sh
./2-clean-ref.sh  2>/dev/null 1>/dev/null
./2-clean-work.sh 2>/dev/null 1>/dev/null
./3-prepare.sh    2>/dev/null 1>/dev/null
./4-test.sh
./5-summarize.sh

./6-end.sh "$TEST_TIME"

