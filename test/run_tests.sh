#!/bin/sh

TESTS=( buffer_empty buffer_length buffer_small_write buffer_big_write buffer_read )

for test_case in ${TESTS[@]}; do
	echo ">>> Running test: $test_case ..."

	./test_$test_case 

	if [[ $? -ne 0 ]]; then
		echo "Failure!"
		exit -1
	fi
done
