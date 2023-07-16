#include "../TRunResidual.h"
#include "../draw_style.h"

void run52_U8_effi()
{
    TString data_class_name = "data_run52_U8";
    int data_Initialize_mode = 0;
    TString data_folder_direction = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/BeamTest2021_data_MC_file_plot_final/data_file/run52/new_class_attempt";
    TString data_cluster_file_name = "cluster_information_offset-0.0000_adcinfo_NoCamac";
    int data_study_chip = 8;
    double double cluster_adc_value_requirement = 22.5; // note : it's a cut
    // double data_l1_alignment_correction
    // double data_l0l2_slope_correction
    // double data_l0l1_slope_correction

    TRunResidual * data_run52_U8 = new TRunResidual(data_class_name, data_Initialize_mode, data_folder_direction, data_cluster_file_name, data_study_chip);
    data_run52_U8 -> Gen_l0l1_scattering();
    data_run52_U8 -> Gen_l0l1_residual(50,-1,1);
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