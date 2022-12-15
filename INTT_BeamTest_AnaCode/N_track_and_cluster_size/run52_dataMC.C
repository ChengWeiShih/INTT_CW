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

    // note : data
    vector<cluster_str> cluster_str_vec_data; cluster_str_vec_data.clear();
    cluster_str_vec_data = cluster_read_and_build(folder_direction_data, file_name, cluster_file_name_data, study_chip_data);

    // note : MC
    vector<cluster_str> cluster_str_vec_MC; cluster_str_vec_MC.clear();
    cluster_str_vec_MC = cluster_read_and_build(folder_direction_MC, file_name, cluster_file_name_MC, study_chip_MC);

    // note : reformat data
    vector<cluster_reformat_str> cluster_reformat_str_vec_data; cluster_reformat_str_vec_data.clear();
    cluster_reformat_str_vec_data = cluster_reformat(cluster_str_vec_data);

    // note : reformat MC
    vector<cluster_reformat_str> cluster_reformat_str_vec_MC; cluster_reformat_str_vec_MC.clear();
    cluster_reformat_str_vec_MC = cluster_reformat(cluster_str_vec_MC);

    // note : do the tracking, data
    multi_track_str post_multi_vec_data = multiple_track_removal(cluster_reformat_str_vec_data);

    // note : do the tracking, MC
    multi_track_str post_multi_vec_MC = multiple_track_removal(cluster_reformat_str_vec_MC);


    // note : make the plot
    // cluster_size_all_dist_compare(cluster_str_vec_data, cluster_str_vec_MC,folder_direction_data, run_ID, linear_or_log, statsbox_bool);
    // post_multi_N_track(post_multi_vec_data);

    // note : make the extrapolation position distribution, data
    edge_finder_str hist_edge_data = post_multi_N_track_extrapolation(post_multi_vec_data,folder_direction_data,cluster_file_name_data);
    TH1F* Ntrack_hist_data = post_multi_N_track_check(post_multi_vec_data,run_ID,folder_direction_data,cluster_file_name_data);

    // note : make the extrapolation position distribution, MC
    edge_finder_str hist_edge_MC = post_multi_N_track_extrapolation(post_multi_vec_MC,folder_direction_MC,cluster_file_name_MC);
    TH1F * Ntrack_hist_MC = post_multi_N_track_check(post_multi_vec_MC,run_ID,folder_direction_MC,cluster_file_name_MC);
    
    // note : N track distribution comparison data, MC
    N_track_dataMC_comp(Ntrack_hist_data, Ntrack_hist_MC, folder_direction_data, run_ID);


}