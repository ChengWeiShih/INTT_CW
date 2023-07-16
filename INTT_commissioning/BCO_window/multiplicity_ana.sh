#!/bin/bash

mother_directory="/home/inttdev/data/IR_DAQ_server/INTT_study_run/BCO_window"
set_name="8021_Time_5min_L1Delay20_Ncollision127_Opentime80"
set_logY=true # note : or false
chip_N_bin=30
chip_L_edge=1 # note : recommended to be 1
chip_R_edge=61
HL_factor=10

# note : requirement : make a file_list containing all the .root files in the folder, full directory
file=$mother_directory/data_analysis/$set_name/file_list.txt

# servername=("intt0" "intt1" "intt2" "intt3" "intt4" "intt5" "intt6" "intt7")

close_FC=(
    [0]=""        # note : intt0
    [1]=""        # note : intt1
    [2]=""        # note : intt2
    [3]=""        # note : intt3
    [4]=""        # note : intt4
    [5]=""        # note : intt5
    [6]=""        # note : intt6
    [7]=""        # note : intt7
)

# note : requirement : file name should contain the server name intt[0-7]

while IFS= read -r line; do
    
    # Process each line
    servername=$(echo "$line" | grep -o 'intt[0-7]\+')
    serverid=$(echo "$servername" | grep -o '[0-7]\+')
    
    Port_ch=(0 1 2 3 4 5 6 7 8 9 10 11 12 13)
    closeFC="${close_FC[$serverid]}"

    for CFC in $closeFC; 
    do
        Port_ch=(${Port_ch[@]/$CFC})
    done

    echo "Line: $line" 
    echo servername $servername, id $serverid
    echo Open_FC $Port_ch

    for Open_FC in "${Port_ch[@]}"; 
    do
        # nohup root -l -b -q multiplicity_ana.cpp\(\"$mother_directory\",\"$set_name\",\"$line\",\"$servername\",$Open_FC,$set_logY,$chip_N_bin,$chip_L_edge,$chip_R_edge,$HL_factor\)&>run_log/"$servername_$Open_FC.out"&
        root -l -b -q multiplicity_ana.cpp\(\"$mother_directory\",\"$set_name\",\"$line\",\"$servername\",$Open_FC,$set_logY,$chip_N_bin,$chip_L_edge,$chip_R_edge,$HL_factor\)
        sleep 0.4
    done

    echo "-----------------------"
    sleep 3

done < "$file"
