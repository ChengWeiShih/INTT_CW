# include "SU.h"

// note : the directory   /home/cwshih/sPHENIX/INTT/INTT_GitHub/INTT_CW/INTT_BeamTest_AnaCode/DUT_efficiency
// note : the directory   /data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency

void run52_data_NoCamac_DUT_effi_v2_SU ()
{
    TString folder_direction = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency/run52_data_NoCamac";
    
    double golden_effi = 0.993086;

    cout<<"SU for residual : "<<endl;
    SU_out_str residual_str = cut_scan_calculate("DUT_v2_cut_scan_1",folder_direction,1,0.1632,0.305); // note : residual
    cout<<"avg - golden : "<<residual_str.average - golden_effi<<endl;
    cout<<" "<<endl;
    cout<<" "<<endl;

    cout<<"SU for slope : "<<endl;
    SU_out_str slope_str = cut_scan_calculate("DUT_v2_cut_scan_2",folder_direction,2,0.0087,0.0113); // note : slope
    cout<<"avg - golden : "<<slope_str.average - golden_effi<<endl;
    cout<<" "<<endl;
    cout<<" "<<endl;


    cout<<"SU for edge_exclusion : "<<endl;
    SU_out_str edge_str = cut_scan_calculate("DUT_v2_cut_scan_3",folder_direction,3,-1,11); // note : edge_exclusion
    cout<<"avg - golden : "<<edge_str.average - golden_effi<<endl;
    cout<<" "<<endl;
    cout<<" "<<endl;


    cout<<"SU_1 (avg) : "<<sqrt(pow(residual_str.average - golden_effi,2)+pow(slope_str.average - golden_effi,2)+pow(edge_str.average - golden_effi,2))<<endl;

    cout<<"SU_2 up : "  <<sqrt(pow(residual_str.max - golden_effi,2)+pow(slope_str.max - golden_effi,2)+pow(edge_str.max - golden_effi,2))<<endl;
    cout<<"SU_2 down : "<<sqrt(pow(residual_str.min - golden_effi,2)+pow(slope_str.min - golden_effi,2)+pow(edge_str.min - golden_effi,2))<<endl;

}