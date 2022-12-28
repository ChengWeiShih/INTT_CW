#include "run89_par_align.h"
#include "DUT_test_v1.h"
#include "SU.h" // note : read the run scan result.

// note : the directory   /home/cwshih/sPHENIX/INTT/INTT_GitHub/INTT_CW/INTT_BeamTest_AnaCode/DUT_efficiency
// note : the directory   /data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency

void run89_data_NoCamac_DUT_align ()
{
    //title : the parameters are now all moved to the "run89_par.h"
    
    vector<cluster_str> cluster_str_vec; cluster_str_vec.clear();
    cluster_str_vec = cluster_read_and_build(folder_direction, file_name, cluster_file_name, study_chip);   

    // note : Now the amount of the alignment correction can be given here, don't need to reproduce a new file. 
    vector<cluster_reformat_str> cluster_reformat_str_vec; cluster_reformat_str_vec.clear();
    cluster_reformat_str_vec = cluster_reformat(cluster_str_vec,study_chip,amount_of_alignment);

    // note : it's the V1 version, more flexible, but for the residual distribution, it can be more correct.
    // DUT_str DUT_data = efficiency_DUT_method(cluster_reformat_str_vec,study_chip);



    // note : it's the v2 version, not flexible, but for the residual dsitribution, it is the correct one, right after the application of the residual cut.
    DUT_str DUT_data_v2 = efficiency_DUT_method_v2(cluster_reformat_str_vec,study_chip);
    
    
    
    // DUT_str align_DUT_data = Alignment_DUT(cluster_reformat_str_vec,study_chip);
    // // plot_residual_publish( align_DUT_data.middle_layer_residual, folder_direction, study_chip );
    // plot_align_narrow_publish( align_DUT_data.middle_layer_residual, folder_direction, study_chip );
    // plot_angle( align_DUT_data.good_combination_slope_hit3, align_DUT_data.good_combination_slope_hit2, folder_direction, study_chip );



    // note : ======================== ======================== the results ======================== ========================



    // note : make plot
    // plot_residual( DUT_data_v2.middle_layer_residual, folder_direction );
    // plot_residual_narrow_publish( DUT_data_v2.middle_layer_residual, folder_direction, study_chip );
    effi_pos_plot(DUT_data_v2.passed_middle_pos, DUT_data_v2.final_event_result,folder_direction,study_chip,beam_spot_range,effi_pos_legend_offset);
    // plot_angle( DUT_data_v2.good_combination_slope_hit3, DUT_data_v2.good_combination_slope_hit2, folder_direction );

    // note : print the used parameters
    print_used_par();

    // note : print the efficiency
    double golden_effi = print_effi(DUT_data_v2);

} 