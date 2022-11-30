#include "NTrack_ClusterSize.h"

void run52_dataMC ()
{
    TString file_name = "Not_used"; // note : not used

    TString folder_direction_data = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/data/folder_run52_no_clone_filter_all_int_1000_chip_8";    
    TString cluster_file_name_data = "cluster_information_offset-0.2908_adcinfo_NoCamac";
    int study_chip_data = 8;

    TString folder_direction_MC = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/data/run52_MC_2/folder_e+_beam-para_run52_x0_y-5_500k_convert_test_filter_all_int_1000_chip_11";
    TString cluster_file_name_MC = "cluster_information_offset-0.0000_adcinfo";
    int study_chip_MC = 11;

    TString run_ID = "Run52"; // note : the title of the plot
    bool linear_or_log = false; // note : true -> linear
    bool statsbox_bool = false; // note : with the box, false -> remove the box

    vector<cluster_str> cluster_str_vec_data; cluster_str_vec_data.clear();
    cluster_str_vec_data = cluster_read_and_build(folder_direction_data, file_name, cluster_file_name_data, study_chip_data);

    vector<cluster_str> cluster_str_vec_MC; cluster_str_vec_MC.clear();
    cluster_str_vec_MC = cluster_read_and_build(folder_direction_MC, file_name, cluster_file_name_MC, study_chip_MC);



    // cluster_size_all_dist(cluster_str_vec,folder_direction);

    cluster_size_all_dist_compare(cluster_str_vec_data, cluster_str_vec_MC,folder_direction_data, run_ID, linear_or_log, statsbox_bool);


}