#!/usr/bin/bash

mkdir -p Public_tests_T0/student_outputs
mkdir -p Public_tests_T0/student_outputs/debug
mkdir -p Public_tests_T0/student_outputs/easy
# mkdir -p Public_tests_T0/student_outputs/medium
# mkdir -p Public_tests_T0/student_outputs/hard

for level in 'debug' 'easy' 'medium' 'hard'; do
    for part in 'A' 'B' 'C'; do
           for i in `seq 0 1`; do
            echo "Running test $i for $level $part"
            filename=$(printf "%s/%s%d.txt" $level $part $i)

            ./dccars "Public_tests_T0/tests/$filename" out.txt
            cp out.txt "Public_tests_T0/student_outputs/$filename"

            diff -bq "Public_tests_T0/solutions/$filename" "Public_tests_T0/student_outputs/$filename"
            if [ $? -ne 0 ]; then
                echo "Test failed\n"
            else
                echo "Test passed\n"
            fi
        done
    done
done
