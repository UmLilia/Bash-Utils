#!/bin/bash
files="case_1 case_2 case_empty"
none="case_none"
all_options="-b -e -n -s -t -E -T --number-nonblank --number --squeeze-blank"
uniqe_options="-b -e -n -s -t"

NUMBER=0
SUCCESS=0
FAIL=0

LEAK_COUNT=0
COUNT=0

printf "\nTesting has begun\n\n"
rm -f fails.txt

leaks_check () {
  valgrind --tool=memcheck --leak-check=yes --log-file="vg_info.log" ./../s21_cat $(echo $@) > vg_out.log
  VG_RES="$(grep LEAK -c vg_info.log)"
  (( COUNT++ ))
  if [ "$VG_RES" == "0" ]
    then
        printf "Test number %d SUCCESS: NO LEAKS\n" $COUNT
    else
      (( LEAK_COUNT++ ))
        printf "Test number %d FAIL: FUCKING LEAKS ARE HERE\n" $COUNT >>fails.txt
  fi
  rm -f vg_info.log
  rm -f vg_out.log
}

result_check() {
    opt=$(echo $@)
    DIFF="$(diff -a <(./../s21_cat $opt) <(cat $opt))"
    leaks_check $opt
    (( NUMBER++ ))
    if [ "$DIFF" == "" ]
    then
        (( SUCCESS++ ))
        printf "Test number %d SUCCESS: cat %s %s %s %s %s %s %s %s\n\n" $NUMBER $opt
    else
        (( FAIL++ ))
        printf "Test number %d FAIL: cat %s %s %s %s %s %s %s %s\n\n" $NUMBER $opt
        printf "Test number %d FAIL: cat %s %s %s %s %s %s %s %s\n\n" $NUMBER $opt >>fails.txt
    fi
}

for file in $files
do
    result_check $file
done

for file in $files
do
    for option in $all_options
    do
        result_check $option $file
    done
done

for option in $all_options
do
    result_check $option $files
done

result_check $none

result_check $uniqe_options $files

result_check -u $files

printf "NUMBER OF TESTS: %d\n" $NUMBER
printf "RESULTS: | FAIL  = %d | SUCCESS = %d |\n" $FAIL $SUCCESS
printf "         | LEAKS = %d |\n" $LEAK_COUNT
if [ $NUMBER == $SUCCESS ]
then
    printf "EVERYTHING IS ALL RIGHT! GOOD JOB!\n"
else
    printf "YOUR CODE IS CRAP, REDO IT\n"
fi