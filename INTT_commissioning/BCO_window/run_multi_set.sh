#!/bin/bash

file=/home/inttdev/data/IR_DAQ_server/INTT_study_run/BCO_window_06_09/data_analysis/set_list.txt

while IFS= read -r line; do
    # Process each line
    echo "Line: $line"
    
    . multiplicity_easy.sh $line

done < "$file"
