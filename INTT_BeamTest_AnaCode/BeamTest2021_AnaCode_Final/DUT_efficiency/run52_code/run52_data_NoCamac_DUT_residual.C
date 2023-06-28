#include "run52_U8_par.h"
#include "../DUT_test_v1.h"
#include "../SU.h" // note : read the run scan result.

// note : the directory   /home/cwshih/sPHENIX/INTT/INTT_GitHub/INTT_CW/INTT_BeamTest_AnaCode/DUT_efficiency
// note : the directory   /data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency

void run52_data_NoCamac_DUT_residual()
{
    //title : the parameters are now all moved to the "run52_par.h"
    
    vector<cluster_str> cluster_str_vec; cluster_str_vec.clear();
    cluster_str_vec = cluster_read_and_build(folder_direction, file_name, cluster_file_name, study_chip);   

    // note : Now the amount of the alignment correction can be given here, don't need to reproduce a new file. 
    vector<cluster_reformat_str> cluster_reformat_str_vec; cluster_reformat_str_vec.clear();
    cluster_reformat_str_vec = cluster_reformat(cluster_str_vec,study_chip,amount_of_alignment);

    // note : it's the v2 version, not flexible, but for the residual dsitribution, it is the correct one, right after the application of the residual cut.
    DUT_str DUT_data_v2 = efficiency_DUT_method_residual_test(cluster_reformat_str_vec,study_chip);

    TH1F * residual_hist = plot_residual_narrow_publish( DUT_data_v2.middle_layer_residual, folder_direction, study_chip );

    TFile * output_file = new TFile(Form("%s/DUT_residual_narrow_U%i_pub.root",folder_direction.Data(),study_chip),"RECREATE");
    residual_hist -> Write(Form("run52_U%i",study_chip));
    
    output_file -> Close();
} 