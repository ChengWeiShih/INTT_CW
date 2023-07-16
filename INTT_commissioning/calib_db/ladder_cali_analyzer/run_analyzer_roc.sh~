#!/bin/bash

array=(0 1 2 3 4 5 6)

for i in "${array[@]}"
do
    echo $i
    nohup root -l -b -q analyzer_calib_packv5_040523_0434.cpp\($i\)&>run_$i.out&
    sleep 2
done


# declare an array called array and define 3 vales
# array=( one two three )
# for ( j=0; j<${array}; j++ );
# do
# 	echo ${array[j]}
# done