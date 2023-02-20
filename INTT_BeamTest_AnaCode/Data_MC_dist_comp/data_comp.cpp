#include "data_process.cpp"
#include "draw_style.h"

void data_comp()
{
    TCanvas * c1 = new TCanvas("c1","c1",850 ,800);
    TPad * pad_c1 = new TPad(Form("pad_c1"), "", 0.0, 0.0, 1.0, 1.0);
    Characterize_Pad(pad_c1, 0.15,  0.1,  0.1,  0.12, 1, 0);
    pad_c1 -> Draw();
    pad_c1 -> cd();

    int Hist_nbins = 100;
    double Hist_ledge = -0.15;
    double Hist_redge = 0.15;


    // note : //////////// //////////// //////////// //////////// //////////// for  run52 U8 //////////// //////////// //////////// //////////// ////////////
    TString folder_direction_data_1 = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency/run52_new_alignment";
    TString cluster_file_name_data_1 = "cluster_information_offset-0.0000_adcinfo_NoCamac";
    int study_chip_data_1 = 8;
    double amount_of_alignment_data_1 = -0.2975;
    double slope_correction_data_1 = 0.001712;

    data_process * data_class_1 = new data_process(1, "data_class_1", folder_direction_data_1, cluster_file_name_data_1, study_chip_data_1, amount_of_alignment_data_1, slope_correction_data_1);
    data_class_1->para_check();

    TH1F * scattering_hist_data_1 = data_class_1->Get_scattering_dist(Hist_nbins,Hist_ledge,Hist_redge,data_class_1->get_cluster_reformat());
    Characterize_Hist1F(scattering_hist_data_1,1,"#A08144","","((Cl2-Cl1)/dis)-((Cl1-Cl0)/dis)","Events");

    // note : //////////// //////////// //////////// //////////// //////////// for  run52 U9 //////////// //////////// //////////// //////////// ////////////
    TString folder_direction_data_2 = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency/run52_new_alignment";
    TString cluster_file_name_data_2 = "cluster_information_offset-0.0000_adcinfo_NoCamac";
    int study_chip_data_2 = 9;
    double amount_of_alignment_data_2 = -0.2651;
    double slope_correction_data_2 = 0;

    data_process * data_class_2 = new data_process(1, "data_class_2", folder_direction_data_2, cluster_file_name_data_2, study_chip_data_2, amount_of_alignment_data_2, slope_correction_data_2);
    data_class_2->para_check();

    TH1F * scattering_hist_data_2 = data_class_2->Get_scattering_dist(Hist_nbins,Hist_ledge,Hist_redge,data_class_2->get_cluster_reformat());
    Characterize_Hist1F(scattering_hist_data_2,1,"#A08144","","((Cl2-Cl1)/dis)-((Cl1-Cl0)/dis)","Events");

    
    

    // note : //////////// //////////// //////////// //////////// //////////// for comparison //////////// //////////// //////////// //////////// ////////////
    TString plot_name = "data_scattering_dist_comp";

    vector<TString> titles_vec = {"Scattering slope [tan(#theta)]","A.U."};

    TString MC_comp_temp_direction = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency";

    vector<TH1F*> MC_hist_array = {scattering_hist_data_1, scattering_hist_data_2};     
    vector<TString>hist_color_array = {"#1A3947", "#A08144"/*, "#F5321D"*/}; 
    vector<TString>Legend_array = {"Run 52 U8", "Run 52 U9"}; 

    multi_plot_comp (MC_hist_array, hist_color_array, Legend_array, MC_comp_temp_direction, plot_name, titles_vec ,false, false);
    
}