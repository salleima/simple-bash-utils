#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

declare -a tests=(
"s TEST_FILES/TEST_001.txt VAR"
"for main.c grep.h Makefile VAR"
"for main.c VAR"
"-e for -e ^int functions.c grep.h Makefile VAR"
"-e for -e ^int functions.c VAR"
"-e regex -e ^print functions.c VAR -f TEST_FILES/TEST_PATTERN.txt"
"-e while -e void main.c Makefile VAR -f TEST_FILES/TEST_PATTERN.txt"
)

declare -a extra=(
"-n for TEST_FILES/TEST_001.txt TEST_FILES/TEST_002.txt"
"-n for TEST_FILES/TEST_001.txt"
"-n -e ^\} TEST_FILES/TEST_001.txt"
"-ce ^int TEST_FILES/TEST_001.txt TEST_FILES/TEST_002.txt"
"-e ^int TEST_FILES/TEST_001.txt"
"-nivh = TEST_FILES/TEST_001.txt TEST_FILES/TEST_002.txt"
"-e"
"-ie INT TEST_FILES/TEST_005.txt"
"-echar TEST_FILES/TEST_001.txt TEST_FILES/TEST_002.txt"
"-ne = -e out TEST_FILES/TEST_005.txt"
"-iv int TEST_FILES/TEST_005.txt"
"-in int TEST_FILES/TEST_005.txt"
"-c -l aboba TEST_FILES/TEST_001.txt TEST_FILES/TEST_005.txt"
"-v TEST_FILES/TEST_001.txt -e ank"
"-noe ) TEST_FILES/TEST_005.txt"
"-l for TEST_FILES/TEST_001.txt TEST_FILES/TEST_002.txt"
"-o -e int TEST_FILES/TEST_004.txt"
"-e = -e out TEST_FILES/TEST_005.txt"
"-e ing -e as -e the -e not -e is TEST_FILES/TEST_006.txt"
"-c -e . TEST_FILES/TEST_001.txt -e '.'"
"-l for TEST_FILES/TEST_NO_FILE.txt TEST_FILES/TEST_002.txt"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./grep $t > test_grep.log
    grep $t > test_sys_grep.log
    DIFF_RES="$(diff -s test_grep.log test_sys_grep.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_grep.log and test_sys_grep.log are identical" ]
    then
      (( SUCCESS++ ))
      echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32msuccess\033[0m grep $t"
    else
      (( FAIL++ ))
      echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mfail\033[0m grep $t"
    fi
    rm test_grep.log test_sys_grep.log
}

# специфические тесты
for i in "${extra[@]}"
do
    var="-"
    testing $i
done

# 1 параметр
for var1 in v c l n h o
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

# 2 параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                testing $i
            done
        fi
    done
done

# 3 параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        for var3 in v c l n h o
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    testing $i
                done
            fi
        done
    done
done

# 2 сдвоенных параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                testing $i
            done
        fi
    done
done

# 3 строенных параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        for var3 in v c l n h o
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1$var2$var3"
                    testing $i
                done
            fi
        done
    done
done

echo "\033[31mFAIL: $FAIL\033[0m"
echo "\033[32mSUCCESS: $SUCCESS\033[0m"
echo "ALL: $COUNTER"
