#!/bin/bash

felix_ch=(0 1 2 3 4 5 6 7 8 9 10 11 12 13)
folder="/home/inttdev/data/IR/commissioning/20230424_calib_pedestal/data_analyzer"
RC_array=()
#threshold=(15 15 15 15 15 15 15 15) # note : 0404
#threshold=(30 30 30 28 28 28 30 30) # note : 202303_1st, north
threshold=(15 15 15 15 15 15 15 15)  # note : threshold, file by file

RC_id_array_0=(0 2 4 6)
RC_id_array_1=(1 3 5 7)

l=0

#test_direc="/home/inttdev/data/IR"

for ((i=0; i<8; i+=2))
do
    # RC_array+=(RC-${i}N_$(($i+1))N)
    RC_array+=(RC-${i}S_$(($i+1))S)    
done

while read line #ROC level

      
do
    # for i in {0..13} #ladder level
    # do
	# 	echo run ${RC_array[l]} - Felix_ch ${i}
	# 	#nohup root -l -b -q  analyzer_multi.cpp\(${i},\"$(dirname "${line}")\",\"${folder}/${RC_array[l]}\",\"$(basename "${line}")\",${l},${threshold[l]}\)&>${folder}/${RC_array[i]}/run_${i}.out&

	# 	nohup root -l -b -q  analyzer_multi.cpp\(${i},\"$(dirname "${line}")\",\"${line}\",\"${folder}/${RC_array[l]}\",\"$(basename "${line//_map.txt/.root}")\",
	# 	\"$(basename "${line}")\",${l},${threshold[l]}\)&>${folder}/${RC_array[i]}/run_${i}.out&
		
	# 	echo ""
	# 	sleep 1
	
    # done


    for i in {0..6} #ladder level
    do
		echo run ${RC_array[l]} - Felix_ch ${i}

		nohup root -l -b -q  analyzer_multi.cpp\(${i},\"$(dirname "${line}")\",\"${line}\",\"${folder}/${RC_array[l]}\",\"$(basename "${line//_map.txt/.root}")\",${RC_id_array_0[l]},${threshold[l]}\)&>${folder}/${RC_array[l]}/run_${i}.out&

		echo ""
		sleep 0.1
	done

	for i in {7..13} #ladder level
    do
		echo run ${RC_array[l]} - Felix_ch ${i}

		nohup root -l -b -q  analyzer_multi.cpp\(${i},\"$(dirname "${line}")\",\"${line}\",\"${folder}/${RC_array[l]}\",\"$(basename "${line//_map.txt/.root}")\",${RC_id_array_1[l]},${threshold[l]}\)&>${folder}/${RC_array[l]}/run_${i}.out&

		echo ""
		sleep 0.1
	done

    

    l=$((l+1))
    
done
