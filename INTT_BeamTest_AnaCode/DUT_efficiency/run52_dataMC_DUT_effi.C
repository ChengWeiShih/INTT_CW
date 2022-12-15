#include "DUT_test_v1.h"

void run52_dataMC_DUT_effi ()
{
    // note : the setting pars
    TString folder_direction_data = "/Users/chengweishi/Downloads/BeamTest_2021/DUT_efficiency/run52_dataMC_comp/data_folder";
    TString file_name = "not_used"; // note : not used
    TString cluster_file_name_data = "cluster_information_offset-0.2908_adcinfo_NoCamac";
    int study_chip_data = 8;

    // noise_hit_distance = 0.234; // note : the tolerance of the residual
    // double slope_cut = 0.015; // note : the parameter for the DUT
    // double slope_correction = 0.002725; // note : to correct the slope distribution. 
    // int hit_allowed_in_adjacent_layers = 1;
    // int cluster_size_requirement = 0; // note : the cluster size of single cluster
    // int cluster_size_requirement_combine = 2;
    // double cluster_adc_value_requirement = 22.5;
    // double INTT_strip_width = 0.078;
    // double lower_section_initial = -9.945;
    // double upper_section_initial = 0.039;
    // int boundary_cut = 5;

    // note : change the parameter : 
    // change_noise_hit_distance(0.1964);
    change_slope_cut(0.01);
    change_slope_correction(0.002725);
    // change_cluster_size_requirement(1); // note : " > the_value" 
    change_cluster_size_requirement_combine(1); // note : " > the_value" "to allow the combination to be 2+1"
    // change_cluster_adc_value_requirement(); // note : " > the_value"
    // change_hit_allowed_in_adjacent_layers(); // note : " == the_value "
    // change_INTT_strip_width();
    // change_lower_section_initial();
    // change_upper_section_initial();
    change_boundary_cut(5);

    vector<cluster_str> cluster_str_vec_data; cluster_str_vec_data.clear();
    cluster_str_vec_data = cluster_read_and_build(folder_direction_data, file_name, cluster_file_name_data, study_chip_data);   

    vector<cluster_reformat_str> cluster_reformat_str_vec_data; cluster_reformat_str_vec_data.clear();
    cluster_reformat_str_vec_data = cluster_reformat(cluster_str_vec_data);

    // note : it's the V1 version, more flexible, but for the residual distribution, it can be more correct.
    // DUT_str DUT_data = efficiency_DUT_method(cluster_reformat_str_vec,study_chip);


    // note : it's the v2 version, not flexible, but for the residual dsitribution, it is the correct one, right after the application of the residual cut.
    DUT_str DUT_data_v2_data = efficiency_DUT_method_v2(cluster_reformat_str_vec_data,study_chip_data);

    // note : make plot
    TH1F * residual_full_hist_data = plot_residual( DUT_data_v2_data.middle_layer_residual, folder_direction_data );
    TH1F * residual_narrow_hist_data = plot_residual_narrow( DUT_data_v2_data.middle_layer_residual, folder_direction_data );
    TH1F * angle_hist_data = plot_angle( DUT_data_v2_data.good_combination_slope_hit3, DUT_data_v2_data.good_combination_slope_hit2, folder_direction_data );

    // note : print the efficiency
    // print_effi(DUT_data);
    print_effi(DUT_data_v2_data);
    
    // note : print the used parameters
    print_used_par();







    // note ========================================== for MC =============================================
    TString folder_direction_MC = "/Users/chengweishi/Downloads/BeamTest_2021/DUT_efficiency/run52_dataMC_comp/MC_folder";
    TString cluster_file_name_MC = "cluster_information_offset-0.0000_adcinfo";
    int study_chip_MC = 11;

    // noise_hit_distance = 0.234; // note : the tolerance of the residual
    // double slope_cut = 0.015; // note : the parameter for the DUT
    // double slope_correction = 0.002725; // note : to correct the slope distribution. 
    // int hit_allowed_in_adjacent_layers = 1;
    // int cluster_size_requirement = 0; // note : the cluster size of single cluster
    // int cluster_size_requirement_combine = 2;
    // double cluster_adc_value_requirement = 22.5;
    // double INTT_strip_width = 0.078;
    // double lower_section_initial = -9.945;
    // double upper_section_initial = 0.039;
    // int boundary_cut = 5;

    // note : change the parameter : 
    // change_noise_hit_distance(0.1964);
    change_slope_cut(0.01);
    change_slope_correction(0.0);
    // change_cluster_size_requirement(1); // note : " > the_value" 
    change_cluster_size_requirement_combine(1); // note : " > the_value" "to allow the combination to be 2+1"
    // change_cluster_adc_value_requirement(); // note : " > the_value"
    // change_hit_allowed_in_adjacent_layers(); // note : " == the_value "
    // change_INTT_strip_width();
    // change_lower_section_initial();
    // change_upper_section_initial();
    change_boundary_cut(5);


    vector<cluster_str> cluster_str_vec_MC; cluster_str_vec_MC.clear();
    cluster_str_vec_MC = cluster_read_and_build(folder_direction_MC, file_name, cluster_file_name_MC, study_chip_MC);   

    vector<cluster_reformat_str> cluster_reformat_str_vec_MC; cluster_reformat_str_vec_MC.clear();
    cluster_reformat_str_vec_MC = cluster_reformat(cluster_str_vec_MC);

    // note : it's the V1 version, more flexible, but for the residual distribution, it can be more correct.
    // DUT_str DUT_data = efficiency_DUT_method(cluster_reformat_str_vec,study_chip);


    // note : it's the v2 version, not flexible, but for the residual dsitribution, it is the correct one, right after the application of the residual cut.
    DUT_str DUT_data_v2_MC = efficiency_DUT_method_v2(cluster_reformat_str_vec_MC,study_chip_MC);

    // note : make plot
    TH1F * residual_full_hist_MC = plot_residual( DUT_data_v2_MC.middle_layer_residual, folder_direction_MC );
    TH1F * residual_narrow_hist_MC = plot_residual_narrow( DUT_data_v2_MC.middle_layer_residual, folder_direction_MC );
    TH1F * angle_hist_MC = plot_angle( DUT_data_v2_MC.good_combination_slope_hit3, DUT_data_v2_MC.good_combination_slope_hit2, folder_direction_MC );

    // note : make plot by using v1 method 
    // hit3_plot_residual_slope_2D(DUT_data.middle_layer_residual,DUT_data.good_combination_slope_hit3, folder_direction);
    // hit3_plot_residual_position_2D(DUT_data.middle_layer_residual,DUT_data.good_combination_l0_pos_hit3, folder_direction);

    // note : print the efficiency
    // print_effi(DUT_data);
    print_effi(DUT_data_v2_MC);
    
    // note : print the used parameters
    print_used_par();


    // note : make the comparison results
    dataMC_comp(residual_full_hist_data,residual_full_hist_MC,folder_direction_data,"Run52, middle-layer residual comparison, full size",false,false); // note : full size residual
    dataMC_comp(residual_narrow_hist_data,residual_narrow_hist_MC,folder_direction_data,"Run52, middle-layer residual comparison, narrow",false,false); // note : narrow size residual
    dataMC_comp(angle_hist_data,angle_hist_MC,folder_direction_data,"Run52, slope comparison",true,false); // note : angle plot

} 