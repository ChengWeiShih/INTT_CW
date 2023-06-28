// #include "run52_U8_par.h"
#include "run52_U9_par.h"
#include "../DUT_test_v1.h"
#include "../SU.h" // note : read the run scan result.

// note : the directory   /home/cwshih/sPHENIX/INTT/INTT_GitHub/INTT_CW/INTT_BeamTest_AnaCode/DUT_efficiency
// note : the directory   /data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency

void run52_data_NoCamac_DUT_alignment()
{
    //title : the parameters are now all moved to the "run52_par.h"
    
    vector<cluster_str> cluster_str_vec; cluster_str_vec.clear();
    cluster_str_vec = cluster_read_and_build(folder_direction, file_name, cluster_file_name, study_chip);   

    // note : Now the amount of the alignment correction can be given here, don't need to reproduce a new file. 
    vector<cluster_reformat_str> cluster_reformat_str_vec; cluster_reformat_str_vec.clear();
    cluster_reformat_str_vec = cluster_reformat(cluster_str_vec,study_chip,amount_of_alignment);  

    // note : after tracking (post-two rough cuts)
    DUT_str align_DUT_data = Alignment_DUT(cluster_reformat_str_vec,study_chip);

    plot_align_narrow_publish( align_DUT_data.middle_layer_residual, folder_direction, study_chip, false );
} 