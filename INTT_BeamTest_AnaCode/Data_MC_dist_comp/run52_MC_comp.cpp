#include "data_process.cpp"
#include "draw_style.h"

void run52_MC_comp()
{
    
    // TCanvas * c1 = new TCanvas("c1","c1",850 ,800);
    // TPad * pad_c1 = new TPad(Form("pad_c1"), "", 0.0, 0.0, 1.0, 1.0);
    // Characterize_Pad(pad_c1, 0.15,  0.1,  0.1,  0.12, 1, 0);
    // pad_c1 -> Draw();
    // pad_c1 -> cd();
    
    // // note : //////////// //////////// //////////// //////////// //////////// for  MC //////////// //////////// //////////// //////////// ////////////
    // TString folder_direction_MC1 = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency/run52_MC_AlFoil/folder_run52_X0_Y-5_1M_WithSciAlFoil_BeamSmear_RoughTrigger";
    // TString cluster_file_name_MC1 = "cluster_information_offset-0.0000_adcinfo_SingleTrigger";
    // int study_chip_MC1 = 11;
    // double amount_of_alignment_MC1 = 0;
    // double slope_correction_MC1 = 0.;

    // data_process * MC1_class = new data_process("MC1_class", folder_direction_MC1, cluster_file_name_MC1, study_chip_MC1, amount_of_alignment_MC1, slope_correction_MC1);
    // cout<<"class name : "<<MC1_class->GetClassName()<<endl;
    // MC1_class->para_check();

    // vector<cluster_str> cluster_str_vec_MC1; cluster_str_vec_MC1.clear();
    // cluster_str_vec_MC1 = MC1_class->cluster_read_and_build();   

    // // note : Now the amount of the alignment correction can be given here, don't need to reproduce a new file. 
    // vector<cluster_reformat_str> cluster_reformat_str_vec_MC1; cluster_reformat_str_vec_MC1.clear();
    // cluster_reformat_str_vec_MC1 = MC1_class->cluster_reformat(cluster_str_vec_MC1);

    // TH1F * scattering_hist_MC1 = MC1_class->scattering_dist(cluster_reformat_str_vec_MC1);
    // Characterize_Hist1F(scattering_hist_MC1,1,"#A08144");
    // scattering_hist_MC1 -> SetTitle("");
    // scattering_hist_MC1 -> GetXaxis() -> SetTitle("((Cl2-Cl1)/dis)-((Cl1-Cl0)/dis)");
    // scattering_hist_MC1 -> GetYaxis() -> SetTitle("Events");


    // // note : //////////// //////////// //////////// //////////// //////////// for  MC //////////// //////////// //////////// //////////// ////////////
    // TString folder_direction_MC2 = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency/run52_MC_AlFoil/folder_run52_X0_Y-5_1M_WithSciAlFoil_10PercentHDIMetalMaterial_BeamSmear_RoughTrigger";
    // TString cluster_file_name_MC2 = "cluster_information_offset-0.0000_adcinfo_SingleTrigger";
    // int study_chip_MC2 = 11;
    // double amount_of_alignment_MC2 = 0;
    // double slope_correction_MC2 = 0.;

    // data_process * MC2_class = new data_process("MC2_class", folder_direction_MC2, cluster_file_name_MC2, study_chip_MC2, amount_of_alignment_MC2, slope_correction_MC2);
    // cout<<"class name : "<<MC2_class->GetClassName()<<endl;
    // MC2_class->para_check();

    // vector<cluster_str> cluster_str_vec_MC2; cluster_str_vec_MC2.clear();
    // cluster_str_vec_MC2 = MC2_class->cluster_read_and_build();   

    // // note : Now the amount of the alignment correction can be given here, don't need to reproduce a new file. 
    // vector<cluster_reformat_str> cluster_reformat_str_vec_MC2; cluster_reformat_str_vec_MC2.clear();
    // cluster_reformat_str_vec_MC2 = MC2_class->cluster_reformat(cluster_str_vec_MC2);

    // TH1F * scattering_hist_MC2 = MC2_class->scattering_dist(cluster_reformat_str_vec_MC2);
    // Characterize_Hist1F(scattering_hist_MC2,1,"#A08144");
    // scattering_hist_MC2 -> SetTitle("");
    // scattering_hist_MC2 -> GetXaxis() -> SetTitle("((Cl2-Cl1)/dis)-((Cl1-Cl0)/dis)");
    // scattering_hist_MC2 -> GetYaxis() -> SetTitle("Events");


    // // note : //////////// //////////// //////////// //////////// //////////// for  MC //////////// //////////// //////////// //////////// ////////////
    // TString folder_direction_MC3 = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency/run52_MC_AlFoil/folder_run52_X0_Y-5_1M_WithSciAlFoil_10PercentHDIMetalMaterial_BeamSmear_EMZ_40umCut_RoughTrigger";
    // TString cluster_file_name_MC3 = "cluster_information_offset-0.0000_adcinfo_SingleTrigger";
    // int study_chip_MC3 = 11;
    // double amount_of_alignment_MC3 = 0;
    // double slope_correction_MC3 = 0.;

    // data_process * MC3_class = new data_process("MC3_class", folder_direction_MC3, cluster_file_name_MC3, study_chip_MC3, amount_of_alignment_MC3, slope_correction_MC3);
    // cout<<"class name : "<<MC3_class->GetClassName()<<endl;
    // MC3_class->para_check();

    // vector<cluster_str> cluster_str_vec_MC3; cluster_str_vec_MC3.clear();
    // cluster_str_vec_MC3 = MC3_class->cluster_read_and_build();   

    // // note : Now the amount of the alignment correction can be given here, don't need to reproduce a new file. 
    // vector<cluster_reformat_str> cluster_reformat_str_vec_MC3; cluster_reformat_str_vec_MC3.clear();
    // cluster_reformat_str_vec_MC3 = MC3_class->cluster_reformat(cluster_str_vec_MC3);

    // TH1F * scattering_hist_MC3 = MC3_class->scattering_dist(cluster_reformat_str_vec_MC3);
    // Characterize_Hist1F(scattering_hist_MC3,1,"#A08144");
    // scattering_hist_MC3 -> SetTitle("");
    // scattering_hist_MC3 -> GetXaxis() -> SetTitle("((Cl2-Cl1)/dis)-((Cl1-Cl0)/dis)");
    // scattering_hist_MC3 -> GetYaxis() -> SetTitle("Events");

    // // note : //////////// //////////// //////////// //////////// //////////// for comparison //////////// //////////// //////////// //////////// ////////////
    // TString plot_name = "run52_MC_comp_temp_RoughTrigger";

    // vector<TString> titles_vec = {"Scattering slope","A.U."};

    // TString MC_comp_temp_direction = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency/run52_MC_AlFoil";

    // vector<TH1F*> MC_hist_array = {scattering_hist_MC1, scattering_hist_MC2, scattering_hist_MC3};     
    // vector<TString>hist_color_array = {"#1A3947", "#A08144", "#F5321D"}; 
    // vector<TString>Legend_array = {"MC1", "MC2", "MC3"}; 

    // multi_plot_comp (MC_hist_array, hist_color_array, Legend_array, MC_comp_temp_direction, plot_name, titles_vec ,false, false);

    // dataMC_comp (scattering_hist_data, scattering_hist_MC, folder_direction_MC, plot_name, titles_vec ,false, false);

    // scattering_hist_MC -> Draw("hist");
    // scattering_hist_data -> Draw("ep same");
    // c1 -> Print("test.pdf");




















































    // note : for scattering, new bin setting, with the consideration of the INTT angle resolution
    int Hist_nbins = 39;
    double Hist_ledge = -0.051468710;
    double Hist_redge = 0.051468710;

    // note : for scattering, original setting
    // int Hist_nbins = 100;
    // double Hist_ledge = -0.15;
    // double Hist_redge = 0.15;

    bool set_logY = true;


    TCanvas * c1 = new TCanvas("c1","c1",850 ,800);
    TPad * pad_c1 = new TPad(Form("pad_c1"), "", 0.0, 0.0, 1.0, 1.0);
    Characterize_Pad(pad_c1, 0.15,  0.1,  0.1,  0.12, 1, 0);
    pad_c1 -> Draw();
    pad_c1 -> cd();
    
    // note : //////////// //////////// //////////// //////////// //////////// for  MC //////////// //////////// //////////// //////////// ////////////
    // TString folder_direction_MC1 = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency/run52_MC_AlFoil/folder_run52_X0_Y-5_1M_WithSciAlFoil_BeamSmear_RoughTrigger";
    // TString cluster_file_name_MC1 = "cluster_information_offset-0.0000_adcinfo_SingleTrigger";
    // int study_chip_MC1 = 11;
    // double amount_of_alignment_MC1 = 0;
    // double slope_correction_MC1 = 0.;

    // data_process * MC1_class = new data_process(1, "MC1_class", folder_direction_MC1, cluster_file_name_MC1, study_chip_MC1, amount_of_alignment_MC1, slope_correction_MC1);
    // cout<<"class name : "<<MC1_class->GetClassName()<<endl;
    // MC1_class->para_check();

    // TH1F * scattering_hist_MC1 = MC1_class->Get_scattering_dist(Hist_nbins,Hist_ledge,Hist_redge,MC1_class->get_cluster_reformat());
    // // Characterize_Hist1F(scattering_hist_MC1,1,"#A08144","","((Cl2-Cl1)/dis)-((Cl1-Cl0)/dis)","Events");

    // // scattering_hist_MC1->Draw("hist");

    // note : //////////// //////////// //////////// //////////// //////////// for  MC //////////// //////////// //////////// //////////// ////////////
    // TString folder_direction_MC2 = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency/run52_MC_AlFoil/folder_run52_X0_Y-5_1M_WithSciAlFoil_10PercentHDIMetalMaterial_BeamSmear_RoughTrigger";
    // TString cluster_file_name_MC2 = "cluster_information_offset-0.0000_adcinfo_SingleTrigger";
    // int study_chip_MC2 = 11;
    // double amount_of_alignment_MC2 = 0;
    // double slope_correction_MC2 = 0.;

    // data_process * MC2_class = new data_process(1,"MC2_class", folder_direction_MC2, cluster_file_name_MC2, study_chip_MC2, amount_of_alignment_MC2, slope_correction_MC2);
    // cout<<"class name : "<<MC2_class->GetClassName()<<endl;
    // MC2_class->para_check();

    // TH1F * scattering_hist_MC2 = MC2_class->Get_scattering_dist(Hist_nbins,Hist_ledge,Hist_redge,MC2_class->get_cluster_reformat());
    // // Characterize_Hist1F(scattering_hist_MC2,1,"#A08144","","((Cl2-Cl1)/dis)-((Cl1-Cl0)/dis)","Events");



    // note : //////////// //////////// //////////// //////////// //////////// for  MC //////////// //////////// //////////// //////////// ////////////
    TString folder_direction_MC3 = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency/run52_MC_AlFoil/folder_run52_X0_Y-5_1M_WithSciAlFoil_10PercentHDIMetalMaterial_BeamSmear_EMZ_40umCut_RoughTrigger";
    TString cluster_file_name_MC3 = "cluster_information_offset-0.0000_adcinfo_SingleTrigger";
    int study_chip_MC3 = 11;
    double amount_of_alignment_MC3 = 0;
    double slope_correction_MC3 = 0.;

    data_process * MC3_class = new data_process(1, "MC3_class", folder_direction_MC3, cluster_file_name_MC3, study_chip_MC3, amount_of_alignment_MC3, slope_correction_MC3);
    cout<<"class name : "<<MC3_class->GetClassName()<<endl;
    MC3_class->para_check();

    TH1F * scattering_hist_MC3 = MC3_class->Get_scattering_dist(Hist_nbins,Hist_ledge,Hist_redge,MC3_class->get_cluster_reformat());
    // Characterize_Hist1F(scattering_hist_MC3,0,"#A08144","","((Cl2-Cl1)/dis)-((Cl1-Cl0)/dis)","Events");

    // note : //////////// //////////// //////////// //////////// //////////// for  MC //////////// //////////// //////////// //////////// ////////////
    TString folder_direction_MC4 = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency/run52_MC_AlFoil/folder_run52_X0_Y-5_1M_WithSciAlFoil_10PercentHDIMetalMaterial_BeamSmear_EMZ_40umCut_Run52DataU8Offset_L1_2.91425e-01mm_RoughTrigger";
    TString cluster_file_name_MC4 = "cluster_information_offset-0.0000_adcinfo_SingleTrigger";
    int study_chip_MC4 = 11;
    double amount_of_alignment_MC4 = -2.89928e-01;
    double slope_correction_MC4 = 0.;

    data_process * MC4_class = new data_process(1, "MC4_class", folder_direction_MC4, cluster_file_name_MC4, study_chip_MC4, amount_of_alignment_MC4, slope_correction_MC4);
    cout<<"class name : "<<MC4_class->GetClassName()<<endl;
    MC4_class->para_check();

    TH1F * scattering_hist_MC4 = MC4_class->Get_scattering_dist(Hist_nbins,Hist_ledge,Hist_redge,MC4_class->get_cluster_reformat());
    // Characterize_Hist1F(scattering_hist_MC4,0,"#A08144","","((Cl2-Cl1)/dis)-((Cl1-Cl0)/dis)","Events");


    // // note : //////////// //////////// //////////// //////////// //////////// for  MC //////////// //////////// //////////// //////////// ////////////
    TString folder_direction_MC5 = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency/run52_MC_AlFoil/folder_run52_X0_Y-5_1M_WithSciAlFoil_10PercentHDIMetalMaterial_BeamSmear_EMZ_40umCut_Run52DataU8Offset_L1_2.91425e-01mm_Run52DataU8TrackCandidateSlope_9.809e-02deg_RoughTrigger";
    TString cluster_file_name_MC5 = "cluster_information_offset-0.0000_adcinfo_SingleTrigger";
    int study_chip_MC5 = 11;
    double amount_of_alignment_MC5 = -2.89928e-01;
    double slope_correction_MC5 = -1.56313e-03;

    data_process * MC5_class = new data_process(1, "MC5_class", folder_direction_MC5, cluster_file_name_MC5, study_chip_MC5, amount_of_alignment_MC5, slope_correction_MC5);
    cout<<"class name : "<<MC5_class->GetClassName()<<endl;
    MC5_class->para_check();

    TH1F * scattering_hist_MC5 = MC5_class->Get_scattering_dist(Hist_nbins,Hist_ledge,Hist_redge,MC5_class->get_cluster_reformat());
    // Characterize_Hist1F(scattering_hist_MC5,0,"#A08144","","((Cl2-Cl1)/dis)-((Cl1-Cl0)/dis)","Events");


    // note : //////////// //////////// //////////// //////////// //////////// for comparison //////////// //////////// //////////// //////////// ////////////
    TString plot_name = "run52_MC345_comp_temp_RoughTrigger_test";

    vector<TString> titles_vec = {"Scattering slope [tan(#theta)]","A.U."};

    TString MC_comp_temp_direction = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency/run52_MC_AlFoil";

    vector<TH1F*> MC_hist_array = {scattering_hist_MC3, scattering_hist_MC4, scattering_hist_MC5};     
    vector<TString>hist_color_array = {"#1A3947", "#A08144", "#F5321D"}; 
    vector<TString>Legend_array = {"MC3", "MC4", "MC5"}; 

    multi_plot_comp (MC_hist_array, hist_color_array, Legend_array, MC_comp_temp_direction, plot_name, titles_vec , set_logY, false);
    
}