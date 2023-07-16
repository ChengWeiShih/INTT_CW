#!/bin/bash

set_name="2023_06_04_pilot_run"
#servername=("intt0" "intt2" "intt3" "intt4" "intt5" "intt6") # todo : remove "intt1" & "intt7"

servername=("intt0")

FC_ALL_ON=(0 1 2 3 4 5 6 7 8 9 10 11 12 13)

Used_FC=(
    [0]="1 2 3"        # note : intt0
    [1]="${FC_ALL_ON[@]}"        # note : intt1
    [2]="${FC_ALL_ON[@]}"        # note : intt2
    [3]="${FC_ALL_ON[@]}"        # note : intt3
    [4]="${FC_ALL_ON[@]}"        # note : intt4
    [5]="${FC_ALL_ON[@]}"        # note : intt5
    [6]="${FC_ALL_ON[@]}"        # note : intt6
    [7]="${FC_ALL_ON[@]}"        # note : intt7
)

for ((i=0; i<${#servername[@]}; i++)); 
do  
    Port_ch=(${Used_FC[${servername[i]:4:1}]})

    # echo close FC $closeFC 

    # for CFC in $closeFC; 
    # do
    #     for ((j=0; j<${#Port_ch[@]}; j++)); 
    #     do
    #         if [[ ${Port_ch[$j]} != $CFC ]]; then
    #             # unset Port_ch[$j]

    #             break
    #         fi
    #     done
    # done    

    echo ${servername[i]}
    echo used ports ${Port_ch[@]}

    for Open_FC in "${Port_ch[@]}"; 
    do
        nohup root -l -b -q DAC_Scan_HL.cpp\(\"${set_name}\",\"${servername[i]}\",\{$Open_FC\}\)&>run_log/${servername[i]}_$Open_FC.out&  #"${servername[i]} $Open_FC"
        echo submitted ${servername[i]} $Open_FC
        sleep 0.3
    done

    echo "-----------------------"

done
