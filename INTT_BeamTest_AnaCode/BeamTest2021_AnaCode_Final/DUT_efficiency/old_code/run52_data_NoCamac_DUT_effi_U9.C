//todo : change the header file
#include "run52_par_U9.h"
#include "DUT_test_v1.h"
#include "SU.h" // note : read the run scan result.

// note : the directory   /home/cwshih/sPHENIX/INTT/INTT_GitHub/INTT_CW/INTT_BeamTest_AnaCode/DUT_efficiency
// note : the directory   /data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency

void run52_data_NoCamac_DUT_effi_U9 ()
{
    //title : the parameters are now all moved to the "run52_par.h"
    
    vector<cluster_str> cluster_str_vec; cluster_str_vec.clear();
    cluster_str_vec = cluster_read_and_build(folder_direction, file_name, cluster_file_name, study_chip);   

    vector<cluster_reformat_str> cluster_reformat_str_vec; cluster_reformat_str_vec.clear();
    cluster_reformat_str_vec = cluster_reformat(cluster_str_vec);

    // note : it's the V1 version, more flexible, but for the residual distribution, it can be more correct.
    // DUT_str DUT_data = efficiency_DUT_method(cluster_reformat_str_vec,study_chip);


    // note : it's the v2 version, not flexible, but for the residual dsitribution, it is the correct one, right after the application of the residual cut.
    DUT_str DUT_data_v2 = efficiency_DUT_method_v2(cluster_reformat_str_vec,study_chip);
    
    // note : test, try to calculate the systematic uncertainty, it wiil output the root file for each scan
    if (file_generated == true)
    {
        cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  scan file is generated, skip generating part  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    }
    else 
    {
        system("nohup root -l -b -q SU_scan_file_generator.C\\(1\\)&>scan_log_residual.log&"); // note : residual
        cout<<"SU file generator, residual is submitted"<<endl;
        
        system("nohup root -l -b -q SU_scan_file_generator.C\\(2\\)&>scan_log_slope.log&"); // note : slope
        cout<<"SU file generator, slope is submitted"<<endl;
        
        system("nohup root -l -b -q SU_scan_file_generator.C\\(3\\)&>scan_log_edge.log&"); // note : edge_exclusion
        cout<<"SU file generator, edge is submitted"<<endl;
    }




    // note : ======================== ======================== the results ======================== ========================



    // note : make plot
    // plot_residual( DUT_data_v2.middle_layer_residual, folder_direction );
    // plot_residual_narrow( DUT_data_v2.middle_layer_residual, folder_direction );
    // plot_angle( DUT_data_v2.good_combination_slope_hit3, DUT_data_v2.good_combination_slope_hit2, folder_direction );

    // note : print the efficiency
    // print_effi(DUT_data);
    double golden_effi = print_effi(DUT_data_v2);
    
    // note : to make the efficiency plot as a function of position
    // effi_pos_plot(DUT_data_v2.passed_middle_pos, DUT_data_v2.final_event_result,folder_direction);

    // note : print the used parameters
    print_used_par();

    // note : print the systematic uncertainty, the function is from SU.h
    if (file_generated == true)
    {
        print_each_SU(folder_direction, golden_effi, residual_scan, slope_scan, edge_scan);
        cout<<"for SU, should times 100"<<endl;
    }



} 