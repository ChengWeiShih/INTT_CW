folder_direction="/home/5202011/INTT_cal/INTT_cal_test/INTT_multi_run_test3"
number_of_file=20
merge_file_name="aaa_test_summary"

let number_for_final=number_of_file-1 
for seed in $(seq 0 $number_for_final)
do
    
    cp calibration_ana_code_multi.c calibration_ana_code_multi_copy.c
    sed -i "s/data_index/${seed}/g" calibration_ana_code_multi_copy.c
    root -l -b -q calibration_ana_code_multi_copy.c\(\"$folder_direction\",1,true,false,0,true,false,false,true\)
    rm calibration_ana_code_multi_copy.c
done

hadd $merge_file_name.root */*_summary.root

root -l -b -q summary_plot.c\($number_of_file,\"$folder_direction\",\"$merge_file_name\"\)


#Variable of calibration_ana_code_multi.c :
#Variable 1 : TSting,  folder direction 
#Variable 2 : int,   port_ID, 
#Variable 3 : bool,  output the adc-ampl plot for each channel (should be true) 
#Variable 4 : bool,  original unbond channel check             (should be false) 
#Variable 5 : int,   overall ampl noise level check            (0 can be good)
#Variable 6 : bool,  output offset ampl distribution plot for each channel (should be true) 
#Variable 7 : bool,  cout unbonded channel status @ without bias run. (should be false)
#Variable 8 : bool,  cout wider gaus width channel @ with bias run.   (should be false)
#Variable 9 : bool,  output multi_run_status.txt               (should be true)
