#include "../TRunResidual.h"
#include "../draw_style.h"

void run52_U8_MC_comp_trial()
{
    TString data_class_name = "data_run52_U8";
    int data_Initialize_mode = 0;
    TString data_folder_direction = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/BeamTest2021_data_MC_file_plot_final/data_file/run52/new_class_attempt";
    TString data_cluster_file_name = "cluster_information_offset-0.0000_adcinfo_NoCamac";
    int data_study_chip = 8;
    double data_cluster_adc_value_requirement = 0; // note : it's a cut
    // double data_l1_alignment_correction
    // double data_l0l2_slope_correction
    // double data_l0l1_slope_correction

    TRunResidual * data_run52_U8 = new TRunResidual(data_class_name, data_Initialize_mode, data_folder_direction, data_cluster_file_name, data_study_chip, data_cluster_adc_value_requirement);
    TH1F * data_scattering_hist = data_run52_U8 -> Gen_l0l1_scattering();
    TH1F * data_residual_hist   = data_run52_U8 -> Gen_l0l1_residual(50,-1,1);



    TString MC_class_name = "MC_run52_U11";
    int MC_Initialize_mode = 0;
    TString MC_folder_direction = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/BeamTest2021_data_MC_file_plot_final/MC_file/MC_run52/folder_run52_MC_run_20230716_2/new_class_attempt";
    TString MC_cluster_file_name = "cluster_information_offset-0.0000_adcinfo_SingleTrigger";
    int MC_study_chip = 11;
    double MC_cluster_adc_value_requirement = 0; // note : it's a cut
    // double MC_l1_alignment_correction
    // double MC_l0l2_slope_correction
    // double MC_l0l1_slope_correction

    TRunResidual * MC_run52_U8 = new TRunResidual(MC_class_name, MC_Initialize_mode, MC_folder_direction, MC_cluster_file_name, MC_study_chip, MC_cluster_adc_value_requirement);
    TH1F * MC_scattering_hist = MC_run52_U8 -> Gen_l0l1_scattering();
    TH1F * MC_residual_hist   = MC_run52_U8 -> Gen_l0l1_residual(50,-1,1);



    pair<double,double> ratio_Y_range_pair = {-3. + 1, 3 + 1};
    TString file_txt = "U11";

    vector<TString> titles_scattering_vec = {"Scattering [slope_{l2l1} - slope_{l1l0}]","A.U."};
    TString plot_scattering_name = Form("DataRun52U8_MC%s_scattering_",file_txt.Data());
    dataMC_comp (data_scattering_hist, MC_scattering_hist, MC_folder_direction , plot_scattering_name, titles_scattering_vec, ratio_Y_range_pair,true, false);
    
    vector<TString> titles_residual_vec = {"Residual [mm]","A.U."};
    TString plot_residual_name = Form("DataRun52U8_MC%s_residual_",file_txt.Data());
    dataMC_comp (data_residual_hist, MC_residual_hist, MC_folder_direction , plot_residual_name, titles_residual_vec, ratio_Y_range_pair,true, false);


}






// #include "../TAnaResidual.h"

// void run52_U8_effi()
// {
//     TString data_class_name = "data_run52_U8";
//     int data_Initialize_mode = 0;
//     TString data_folder_direction = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/BeamTest2021_data_MC_file_plot_final/data_file/run52/new_class_attempt";
//     TString data_cluster_file_name = "cluster_information_offset-0.0000_adcinfo_NoCamac";
//     int data_study_chip = 8;
//     // double data_l1_alignment_correction
//     // double data_l0l2_slope_correction
//     // double data_l0l1_slope_correction
    
//     TAnaResidual * data_run52_U8 = new TAnaResidual(data_class_name, data_Initialize_mode, data_folder_direction, data_cluster_file_name, data_study_chip);
//     cout<<"Before, correction check, l0l2_slope "<< data_run52_U8 -> Par_Get_l0l2_slope()<<", l1_alignment "<< data_run52_U8 -> Par_Get_l1_alignment()<<", l0l1_slope :  "<< data_run52_U8 -> Par_Get_l0l1_slope()<<endl;

//     data_run52_U8 -> Alignment_DUT(); // note : to get raw data for the alignment correction and l0l2 slope correction
//     data_run52_U8 -> Par_Set_l1_alignment( data_run52_U8 -> Get_l1_alignment(true) );  // note : print_plots = true
//     data_run52_U8 -> Par_Set_l0l2_slope( data_run52_U8 -> Get_l0l2_slope(true) ); // note : print_plots = true
//     cout<<"After, correction check, l0l2_slope "<< data_run52_U8 -> Par_Get_l0l2_slope()<<", l1_alignment "<< data_run52_U8 -> Par_Get_l1_alignment()<<", l0l1_slope :  "<< data_run52_U8 -> Par_Get_l0l1_slope()<<endl;

//     data_run52_U8 -> Alignment_DUT(); // note : this is for the "AFTER" the slope correction, the l0l2 slope plot
//     data_run52_U8 -> Get_l0l2_slope(true);
//     // note : re-generate the vector again, APPLY the l1 alignment
//     // note : vec_cluster_reformat_after replaces vec_cluster_reformat
//     data_run52_U8 -> cluster_reformat_after(); 
//     cout<<"After - 2, correction check, l0l2_slope "<< data_run52_U8 -> Par_Get_l0l2_slope()<<", l1_alignment "<< data_run52_U8 -> Par_Get_l1_alignment()<<", l0l1_slope :  "<< data_run52_U8 -> Par_Get_l0l1_slope()<<endl;

//     data_run52_U8 -> Get_l1_efficiency();

//     data_run52_U8 -> Pre_Get_l0l1_slope();
//     data_run52_U8 -> Par_Set_l0l1_slope( data_run52_U8 -> Get_l0l1_slope(true) );
//     cout<<"After - 3, correction check, l0l2_slope "<< data_run52_U8 -> Par_Get_l0l2_slope()<<", l1_alignment "<< data_run52_U8 -> Par_Get_l1_alignment()<<", l0l1_slope :  "<< data_run52_U8 -> Par_Get_l0l1_slope()<<endl;

//     data_run52_U8 -> Pre_Get_l0l1_slope();
//     data_run52_U8 -> Get_l0l1_slope(true);

//     data_run52_U8 -> Gen_l0l1_scattering();
//     data_run52_U8 -> Gen_l0l1_residual(50,-1,1);
// }