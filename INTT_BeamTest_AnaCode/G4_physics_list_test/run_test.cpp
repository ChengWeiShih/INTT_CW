#include "Run_ana.cpp"

void run_test()
{
    TString folder_direction_MC0 = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/Physics_list_test/FTFP_BERT_WVI";
    TString file_name_MC0 = "ED_0_FTFP_BERT_WVI_all";
    
    // note : the first run
    data_process * MC0_class = new data_process("Silicon, 320 um", folder_direction_MC0, file_name_MC0);
    // vector<vector<hit_info>> test_vec = MC0_class->get_file_read_and_build();

    //  note : for scattering distribution
    int Hist_nbins = 100;
    double Hist_ledge = 0;
    double Hist_redge = 1;

    TH1F * scattering_dist_MC0 = MC0_class -> get_scattering_distribution(2,2,Hist_nbins,Hist_ledge,Hist_redge,MC0_class->get_file_read_and_build());

    scattering_dist_MC0 -> Draw("hist");
}