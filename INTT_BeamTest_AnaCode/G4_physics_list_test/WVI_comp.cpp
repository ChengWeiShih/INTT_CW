#include "Run_ana.cpp"
#include "draw_style.h"
void WVI_comp()
{

    // TCanvas * c1 = new TCanvas("c1","c1",850 ,800);
    // TPad * pad_c1 = new TPad(Form("pad_c1"), "", 0.0, 0.0, 1.0, 1.0);
    // Characterize_Pad(pad_c1, 0.15,  0.1,  0.1,  0.12, 1, 0);
    // pad_c1 -> Draw();
    // pad_c1 -> cd();

    TString folder_direction_MC = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/Physics_list_test/FTFP_BERT_WVI";
    
    // note : draw option 2, radius
    // int Hist_nbins = 100;
    // double Hist_ledge = -0.1;
    // double Hist_redge = 1;

    // note : draw option 3, y slope
    int Hist_nbins = 39;
    double Hist_ledge = -0.051468710;
    double Hist_redge = 0.051468710;

    
    int hit_cut = 2;
    int scattering_draw_option = 3;

    const int number_of_file = 7;
    TString file_name_MC[number_of_file]; 
    data_process * MC_class[number_of_file];
    TH1F * scattering_dist_MC[number_of_file];

    for (int i = 0; i < number_of_file; i++) 
    {
        file_name_MC[i] = "ED_" +std::to_string(i)+"_FTFP_BERT_WVI_all";

        MC_class[i] = new data_process(Form("file ID : %i",i), folder_direction_MC, file_name_MC[i]);
        scattering_dist_MC[i] = MC_class[i] -> get_scattering_distribution(hit_cut,scattering_draw_option,Hist_nbins,Hist_ledge,Hist_redge,MC_class[i]->get_file_read_and_build());
    }
    
    TString plot_name = "WVIMC_scattering_"+std::to_string(number_of_file)+"Files_DrawOption"+std::to_string(scattering_draw_option);

    // vector<TString> titles_vec = {"Radius ( #sqrt{x^{2} + y^{2}} ) [mm]","A.U."};
    vector<TString> titles_vec = {"Scattering slope [tan(#theta)]","A.U."};

    TString MC_comp_temp_direction = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/Physics_list_test/FTFP_BERT_WVI";

    // TString color_code [8]={"#343434","#1A3947","#566575","#797983","#EFBD9D","#FCA26E","#F5751D","#F5321D"};
    // TString color_code_2 [8]={"#CC768D","#19768D","#DDA573","#009193","#6E9193","#941100","#A08144","#517E66"};

    vector<TH1F*> MC_hist_array = {scattering_dist_MC[0], scattering_dist_MC[1], scattering_dist_MC[2], scattering_dist_MC[3], scattering_dist_MC[4], scattering_dist_MC[5], scattering_dist_MC[6]};     
    vector<TString>hist_color_array = {"#A08144","#1A3947","#009193","#EFBD9D","#CC768D","#F5751D","#F5321D"}; 
    vector<TString>Legend_array = {"Silicon", "Pb", "Au", "Kapton", "Cu", "H2O", "Al"}; 

    multi_plot_comp (MC_hist_array, hist_color_array, Legend_array, MC_comp_temp_direction, plot_name, titles_vec ,true, false);
    multi_plot_comp (MC_hist_array, hist_color_array, Legend_array, MC_comp_temp_direction, plot_name, titles_vec ,false, false);
     
}