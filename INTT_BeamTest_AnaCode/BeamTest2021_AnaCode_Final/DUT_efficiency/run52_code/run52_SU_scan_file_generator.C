// note : add the runXX_par.h in advance
// #include "run52_par_U9.h"
#include "run52_U8_par.h"

#include "../DUT_test_v1.h"

void run52_SU_scan_file_generator (int scan_id )
{
    // note : scan_id 1 = residual
    // note : scan_id 2 = slope
    // note : scan_id 3 = edge exclusion

    if (file_generated == true)
    {
        cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  scan file is generated, skip generating part  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    }
    else 
    {
        vector<cluster_str> cluster_str_vec; cluster_str_vec.clear();
        cluster_str_vec = cluster_read_and_build(folder_direction, file_name, cluster_file_name, study_chip);   

        vector<cluster_reformat_str> cluster_reformat_str_vec; cluster_reformat_str_vec.clear();
        cluster_reformat_str_vec = cluster_reformat(cluster_str_vec, study_chip, amount_of_alignment);

        vector<vector<double>> SU_vec = efficiency_DUT_method_v2_SU(cluster_reformat_str_vec,study_chip,scan_id); 
        output_SU_root_file(SU_vec,folder_direction,scan_id,study_chip);
    }


}