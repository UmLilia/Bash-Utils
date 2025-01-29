#!/bin/bash
options="-e -i -v -c -l -n -h -s -o"
patterns="grep CAT"
files="case_1 case_empty"
none="case_none"

NUMBER=0
SUCCESS=0
FAIL=0

LEAK_COUNTER=0
COUNTER=0

printf "\nTesting is starting\n\n"
rm -f fails.txt

leaks_check () {
  valgrind --tool=memcheck --leak-check=yes --log-file="vg_info.log" ./../s21_grep $(echo $@) > vg_out.log

  VG_RES="$(grep LEAK -c vg_info.log)"
  (( COUNTER++ ))
  if [ "$VG_RES" == "0" ]
    then
        printf "Test number %d SUCCESS: NO LEAKS\n" $COUNTER
    else
      (( LEAK_COUNTER++ ))
        printf "Test number %d FAIL: FUCKING LEAKS ARE HERE\n" $COUNTER
        printf "Test number %d FAIL: FUCKING LEAKS ARE HERE\n" $COUNTER >>fails.txt
  fi
  rm -f vg_info.log
  rm -f vg_out.log
}

result_check() {
    opt=$(echo $@)
    DIFF="$(diff -a <(./../s21_grep $opt) <(grep $opt))"
    (( NUMBER++ ))
    if [ "$DIFF" == "" ]
    then
        (( SUCCESS++ ))
        printf "Test number %d SUCCESS: grep %s %s %s %s %s %s %s %s\n\n" $NUMBER $opt
    else
        (( FAIL++ ))
        printf "Test number %d FAIL: grep %s %s %s %s %s %s %s %s\n\n" $NUMBER $opt
        printf "Test number %d FAIL: grep %s %s %s %s %s %s %s %s\n\n" $NUMBER $opt >>fails.txt
    fi
}

for file in $files
do
    for option in $options
    do
        for pattern in $patterns
        do
            leaks_check $option $pattern $file
            result_check $option -f patterns.reg $file
        done
    done
done

for file in $files
do
    for opt_1 in $options
    do
        for opt_2 in $options
        do
            for pattern in $patterns
            do
                result_check $opt_1 $opt_2 $pattern $file
            done
        done
    done
done

for opt_1 in $options
do
    for opt_2 in $options
    do
        for pattern in $patterns
        do
            result_check $opt_1 $opt_2 $pattern $files
        done
    done
done

result_check smth $none

printf "NUMBER OF TESTS:%d\n" $NUMBER
printf "RESULTS: | FAIL  = %d | SUCCESS = %d |\n" $FAIL $SUCCESS
printf "         | LEAKS = %d |\n" $LEAK_COUNTER

if [ $NUMBER == $SUCCESS ]
then
    printf "EVERYTHING IS ALL RIGHT! GOOD JOB!\n"
    rm -f fails.txt
else
    printf "YOUR CODE IS CRAP, REDO IT\n"
fi