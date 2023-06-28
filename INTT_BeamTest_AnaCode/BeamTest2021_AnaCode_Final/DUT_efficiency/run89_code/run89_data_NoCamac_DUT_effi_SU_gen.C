#include "run89_U10_par.h"
#include "../DUT_test_v1.h"
#include "../SU.h" // note : read the run scan result.

// note : the directory   /home/cwshih/sPHENIX/INTT/INTT_GitHub/INTT_CW/INTT_BeamTest_AnaCode/DUT_efficiency
// note : the directory   /data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency

void run89_data_NoCamac_DUT_effi_SU_gen()
{
    // note : test, try to calculate the systematic uncertainty, it wiil output the root file for each scan
    if (file_generated == true)
    {
        cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  scan file is generated, skip generating part  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    }
    else 
    {
        system("nohup root -l -b -q run89_SU_scan_file_generator.C\\(1\\)&>scan_log_residual.log&"); // note : residual
        cout<<"SU file generator, residual is submitted"<<endl;
        
        system("nohup root -l -b -q run89_SU_scan_file_generator.C\\(2\\)&>scan_log_slope.log&"); // note : slope
        cout<<"SU file generator, slope is submitted"<<endl;
        
        system("nohup root -l -b -q run89_SU_scan_file_generator.C\\(3\\)&>scan_log_edge.log&"); // note : edge_exclusion
        cout<<"SU file generator, edge is submitted"<<endl;
    }
} 