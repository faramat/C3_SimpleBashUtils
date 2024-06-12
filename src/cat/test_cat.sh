#!/bin/bash

function start_pos() {
    TEST_NAME=""
    FLAG=""
    PATTERN=""
    FILE=""
    TEST_CASE=""
}

function run_utils() {
    $TEST_CASE > cat.txt
    ./s21_$TEST_CASE > s21_cat.txt
}

function compare_results() {
    DIFF="$(diff -s cat.txt s21_cat.txt)"
    
    if [ "$DIFF" == "Files cat.txt and s21_cat.txt are identical" ]
        then
          echo "Test $TEST_NAME: SUCCESS!!!"
          (( COUNTER_SUCCESS++ ))
        else
          echo "Test $TEST_NAME: FAIL!!!"
          (( COUNTER_FAIL++ ))
    fi
}

function run_test_case() {
    run_utils
    compare_results
}

echo "TEST START"
echo ""
echo "JUST CAT"
    start_pos
    TEST_NAME="1 - CAT"
    FLAG=""
    FILE="test.txt"
    TEST_CASE="cat $FILE"
    run_test_case
    
    start_pos
    TEST_NAME="2 - CAT"
    FLAG="-b"
    FILE="test.txt"
    TEST_CASE="cat $FLAG $FILE"
    run_test_case
    
    start_pos
    TEST_NAME="3 - CAT"
    FLAG="-e"
    FILE="test.txt test_ascii.txt"
    TEST_CASE="cat $FLAG $FILE"
    run_test_case

    start_pos
    TEST_NAME="4 - CAT"
    FLAG="-n"
    FILE="test.txt"
    TEST_CASE="cat $FLAG $FILE"
    run_test_case

    start_pos
    TEST_NAME="5 - CAT"
    FLAG="-s"
    FILE="test.txt"
    TEST_CASE="cat $FLAG $FILE"
    run_test_case

    start_pos
    TEST_NAME="6 - CAT"
    FLAG="-t"
    FILE="test.txt test_ascii.txt"
    TEST_CASE="cat $FLAG $FILE"
    run_test_case    

    start_pos
    TEST_NAME="7 - CAT"
    FLAG="-ne"
    FILE="test.txt test_ascii.txt"
    TEST_CASE="cat $FLAG $FILE"
    run_test_case 

    start_pos
    TEST_NAME="8 - CAT"
    FLAG="-nb"
    FILE="test.txt"
    TEST_CASE="cat $FLAG $FILE"
    run_test_case

    start_pos
    TEST_NAME="9 - CAT"
    FLAG="-ts"
    FILE="test.txt test_ascii.txt"
    TEST_CASE="cat $FLAG $FILE"
    run_test_case 

    start_pos
    TEST_NAME="10 - CAT"
    FLAG="-sb"
    FILE="test.txt"
    TEST_CASE="cat $FLAG $FILE"
    run_test_case  

echo ""
if [ $COUNTER_SUCCESS > 0 ]
    then
        echo "SUCCESS: $COUNTER_SUCCESS/10"
    else
        echo "SUCCESS: 0/10"
fi

if [ $COUNTER_FAIL > 0 ]
    then
        echo "FAIL: $COUNTER_FAIL/10"
    else
        echo "FAIL: 0/10"
fi
