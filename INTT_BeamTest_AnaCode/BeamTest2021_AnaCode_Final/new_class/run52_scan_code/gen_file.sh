#!/bin/bash

counting=0

while IFS= read -r line;
do
    echo $counting;
    echo "$line";
    my_directory=$(dirname "$line")
    my_filename=$(basename "$(echo "$line" | sed 's/\.root$//')")
    echo $my_directory
    echo $my_filename

    # nohup root -l -b -q /home/cwshih/sPHENIX/INTT/INTT_GitHub/INTT_CW/INTT_BeamTest_AnaCode/BeamTest2021_AnaCode_Final/MC_file_processor/MC_cluster_file_generator.C\(\"$line\"\)&>out_folder/out_${line}.out&
    nohup root -l -b -q /home/cwshih/sPHENIX/INTT/INTT_GitHub/INTT_CW/INTT_BeamTest_AnaCode/BeamTest2021_AnaCode_Final/MC_file_processor/MC_cluster_file_generator.C\(\"$my_directory\",\"$my_filename\"\)&>out.out&
    
    ((counting += 1))
    if [ $counting -eq 35 ]
        then 
	        sleep 100;
	        counting=0;
    fi

done < /data4/chengwei/Geant4/INTT_simulation/G4/for_CW/BeamTest2021_data_MC_file_plot_final/MC_file/MC_run52/folder_fix_beam_geo_scan/75K_fix_scan/full_file_directory.txt
