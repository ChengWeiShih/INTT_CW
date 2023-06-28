// note : for vertical rotation

#include "data_process.cpp"
#include "draw_style.h"

// note : the direction : /home/cwshih/sPHENIX/INTT/INTT_GitHub/INTT_CW/INTT_BeamTest_AnaCode/Data_MC_dist_comp

void run61_comp()
{
    bool set_logY = false;
    bool with_cut = false; // note : for cluster ADC and cluster size
    bool X_label_less = true;
    bool Y_label_far = true;

    TCanvas * c1 = new TCanvas("c1","c1",850 ,800);
    TPad * pad_c1 = new TPad(Form("pad_c1"), "", 0.0, 0.0, 1.0, 1.0);
    Characterize_Pad(pad_c1, 0.15,  0.1,  0.1,  0.12, set_logY, 0);
    pad_c1 -> Draw();
    pad_c1 -> cd();

    // // note : for cluster size
    int Hist_nbins = 20;
    double Hist_ledge = -3;
    double Hist_redge = 17;

    //note : for track-candidate slope
    // int Hist_nbins = 50;
    // double Hist_ledge = 0.1;
    // double Hist_redge = 0.3;
    
    // note : //////////// //////////// //////////// //////////// //////////// for data //////////// //////////// //////////// //////////// ////////////
    TString folder_direction_data = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/Beamtest_data_Different_config/data_run61_vertical_rotation/folder_run61_no_clone_filter_all_int_1000_chip_9";
    TString cluster_file_name_data = "cluster_information_offset-0.0000_adcinfo_NoCamac";
    int study_chip_data = 11;

    double amount_of_alignment_data = 0; // note : test
    double slope_correction_data = -1.98468e-01; // note : test

    // data_process * data_class = new data_process(1, "data_class", folder_direction_data, cluster_file_name_data, study_chip_data, amount_of_alignment_data, slope_correction_data);
    // cout<<"class name : "<<data_class->GetClassName()<<endl;
    // data_class->para_check();
    
    // note : for N track study, very stupid way to initialize
    // multi_track_str N_track_all_info = data_class -> Get_Ntrack(data_class->get_cluster_reformat(), true);
    
    // note : for N track study, the distribution of the track-extrapolation position
    // TH1F * Ntrack_extrapolation_pos_dist = data_class -> Get_Ntrack_extrapolation_dist(Hist_nbins,Hist_ledge,Hist_redge,N_track_all_info);
    // Characterize_Hist1F(Ntrack_extrapolation_pos_dist,1,"#A08144",set_logY,false,"","Track pos @ sci. location [mm]","Events");    
    // Ntrack_extrapolation_pos_dist -> Draw("hist");

    // note : for N track study, the distribution of the N track with cut.
    // TH1F * Ntrack_dist = data_class -> Get_Ntrack_dist(Hist_nbins,Hist_ledge,Hist_redge, extrapol_pos_cut_low, extrapol_pos_cut_high, N_track_all_info);
    // Characterize_Hist1F(Ntrack_dist,1,"#A08144",set_logY,false,"","N track in a event","Events");    
    // Ntrack_dist -> Draw("hist");

    // note : for cluster size, three layers
    // vector <TH1F *> cluster_size_dist_all_layers_data = data_class->Get_cluster_size(with_cut,Hist_nbins,Hist_ledge,Hist_redge,data_class->get_cluster_reformat());
    // Characterize_Hist1F(cluster_size_dist_all_layers_data[0],1,"#A08144","","cluster size","Events");

    // note : for cluster ADC, three layers
    // vector <TH1F *> cluster_adc_dist_all_layers_data = data_class->Get_cluster_adc(with_cut,Hist_nbins,Hist_ledge,Hist_redge,data_class->get_cluster_reformat());
    // Characterize_Hist1F(cluster_adc_dist_all_layers_data[2],1,"#A08144",set_logY,false,"","cluster adc","A.U.");
    // cluster_adc_dist_all_layers_data[2] -> Scale(1./cluster_adc_dist_all_layers_data[2]->Integral(-1,-1));
    // binbin_uniform(cluster_adc_dist_all_layers_data[2],bin_uniform_factor);
    // cluster_adc_dist_all_layers_data[2] -> Draw("hist");

    // note : for scattering
    // TH1F * scattering_hist_data = data_class->Get_scattering_dist(Hist_nbins,Hist_ledge,Hist_redge,data_class->get_cluster_reformat());
    // Characterize_Hist1F(scattering_hist_data,1,"#A08144","","((Cl2-Cl1)/dis)-((Cl1-Cl0)/dis)","Events");

    // note : for residual dist
    // TH1F * residual_hist_data = data_class->Get_residual_dist(Hist_nbins,Hist_ledge,Hist_redge,data_class->get_cluster_reformat());
    // Characterize_Hist1F(residual_hist_data,1,"#A08144",set_logY,false,"","Residual [mm]","Events");

    //note : for the track-candidates slope 
    // TH1F * track_candi_slope_data = data_class->Get_track_candidate_slope(Hist_nbins,Hist_ledge,Hist_redge,data_class->get_cluster_reformat());
    // Characterize_Hist1F(track_candi_slope_data,1,"#A08144",set_logY,false,"","Slope [tan(#theta)]","Events",X_label_less,Y_label_far);
    // track_candi_slope_data -> Draw("hist");

    // note : for fitting a gaussian
    // TF1 * gaus_fit = new TF1 ("gaus_fit","gaus",-10,10);
    // track_candi_slope_data -> Fit(gaus_fit,"N");
    // gaus_fit -> Draw("lsame");

    // note : //////////// //////////// //////////// //////////// //////////// for  MC //////////// //////////// //////////// //////////// ////////////
    // TString folder_direction_MC = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency/run52_MC_AlFoil/folder_run52_X0_Y-5_1M_WithSciAlFoil_10PercentHDIMetalMaterial_BeamSmear_EMZ_40umCut_Run52DataU9offset_L1_2.65014e-01mm_RoughTrigger";
    TString folder_direction_MC = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/Beamtest_data_Different_config/MC_run61_TotalVerticalRotation_11.23deg_Run89U10L1Offset";
    // TString folder_direction_MC = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency/run52_MC_AlFoil/folder_run52_X0_Y-5_1M_WithSciAlFoil_10PercentHDIMetalMaterial_BeamSmear_EMZ_40umCut_RoughTrigger";
    
    TString cluster_file_name_MC = "cluster_information_offset-0.0000_adcinfo_SingleTrigger";
    int study_chip_MC = 10;
    
    // note : test
    double amount_of_alignment_MC = 0; // note : It doesn't matter at this moment
    // double amount_of_alignment_MC = 0;
    double slope_correction_MC = -2.09000e-01;

    data_process * MC_class = new data_process(1, "MC_class", folder_direction_MC, cluster_file_name_MC, study_chip_MC, amount_of_alignment_MC, slope_correction_MC);
    cout<<"class name : "<<MC_class->GetClassName()<<endl;
    MC_class->para_check();

    // note : for cluster size, three layers
    vector <TH1F *> cluster_size_dist_all_layers_MC = MC_class->Get_cluster_size(with_cut,Hist_nbins,Hist_ledge,Hist_redge,MC_class->get_cluster_reformat());
    // Characterize_Hist1F(cluster_size_dist_all_layers_MC[0],1,"#A08144","","cluster size","Events");

    // note : for cluster adc, three layers
    // vector <TH1F *> cluster_adc_dist_all_layers_MC = MC_class->Get_cluster_adc(with_cut,Hist_nbins,Hist_ledge,Hist_redge,MC_class->get_cluster_reformat());
    // Characterize_Hist1F(cluster_adc_dist_all_layers_MC[2],1,"#A08144",set_logY,false,"","cluster adc","A.U.");
    // cluster_adc_dist_all_layers_MC[2] -> Scale(1./cluster_adc_dist_all_layers_MC[2]->Integral(-1,-1));
    // binbin_uniform(cluster_adc_dist_all_layers_MC[2],bin_uniform_factor);
    // cluster_adc_dist_all_layers_MC[2] -> Draw("hist");

    //note : for the track-candidates slope 
    // TH1F * track_candi_slope_MC = MC_class->Get_track_candidate_slope(Hist_nbins,Hist_ledge,Hist_redge,MC_class->get_cluster_reformat());
    // Characterize_Hist1F(track_candi_slope_MC,1,"#A08144",set_logY,false,"","Slope [tan(#theta)]","Events",X_label_less,Y_label_far);
    // track_candi_slope_MC -> Draw("hist");

    // note : for scattering
    // TH1F * scattering_hist_MC = MC_class->Get_scattering_dist(Hist_nbins,Hist_ledge,Hist_redge,MC_class->get_cluster_reformat());
    // Characterize_Hist1F(scattering_hist_MC,1,"#A08144","","((Cl2-Cl1)/dis)-((Cl1-Cl0)/dis)","Events");

    // note : for residual distribution
    // TH1F * residual_hist_MC = MC_class->Get_residual_dist(Hist_nbins,Hist_ledge,Hist_redge,MC_class->get_cluster_reformat());
    // Characterize_Hist1F(residual_hist_MC,0,"#A08144",set_logY,false,"","Residual [mm]","Events");
    
    // note : for fitting
    // TF1 * gaus_fit = new TF1 ("gaus_fit","gaus",-10,10);
    // track_candi_slope_MC -> Fit(gaus_fit,"N");
    // gaus_fit -> Draw("lsame");

    // note : //////////// //////////// //////////// //////////// //////////// for comparison //////////// //////////// //////////// //////////// ////////////
    // TString plot_name = "run52U8_comp_residual_Nbin_"+std::to_string(Hist_nbins);
    // TString plot_name = "run52U8_comp_scattering_Nbin_"+std::to_string(Hist_nbins); 

    // vector<TString> titles_vec = {"Scattering slope [tan(#theta)]","A.U."};
    // vector<TString> titles_vec = {"Residual [mm]","A.U."};
     
    // dataMC_comp (residual_hist_data, residual_hist_MC, folder_direction_MC, plot_name, titles_vec ,set_logY, false);

    

    // note : for the cluster ADC and the cluster size
    // for (int i = 0; i < 3; i++)
    // {   
    //     // TString plot_name = "run52U8_comp_clusterADC_Nbin_"+std::to_string(Hist_nbins)+"_LayerID"+std::to_string(i);
    //     TString plot_name = "run61U11_comp_clustersize_Nbin_"+std::to_string(Hist_nbins)+"_LayerID"+std::to_string(i);

    //     if (with_cut == true)
    //     {
    //         plot_name += "_WithCut";
    //     }
    //     else if (with_cut == false)
    //     {
    //         plot_name += "_WithOutCut";
    //     }
        
    //     // vector<TString> titles_vec = {"Cluster ADC","A.U."}; // note : for cluster
    //     vector<TString> titles_vec = {"Cluster size","A.U."}; // note : for cluster

    //     // note : ADC combination factor cancellation
    //     // binbin_uniform(cluster_adc_dist_all_layers_data[i],bin_uniform_factor);
    //     // binbin_uniform(cluster_adc_dist_all_layers_MC[i],bin_uniform_factor);

    //     dataMC_comp (cluster_size_dist_all_layers_data[i], cluster_size_dist_all_layers_MC[i], folder_direction_MC, plot_name, titles_vec ,set_logY, false);
    // }


    // note : =========================================== multi-hist comparison ==================================================
    TString plot_name = "run61_MC_layer_comp_NTrackDist_RoughTrigger_1";

    vector<TString> titles_vec = {"Cluster size","A.U."};

    TString MC_comp_temp_direction = folder_direction_MC; //"/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/Beamtest_data_Different_config/data_run61_vertical_rotation/folder_run61_no_clone_filter_all_int_1000_chip_9";

    vector<TH1F*> MC_hist_array = {cluster_size_dist_all_layers_MC[0],cluster_size_dist_all_layers_MC[1], cluster_size_dist_all_layers_MC[2]};     
    vector<TString>hist_color_array = {"#1A3947", "#A08144","#F5321D"}; 
    vector<TString>Legend_array = {"L0", "L1","L2"}; 

    multi_plot_comp (MC_hist_array, hist_color_array, Legend_array, MC_comp_temp_direction, plot_name, titles_vec ,set_logY, false);


    // note : for test
    // scattering_hist_MC -> Draw("hist");
    // scattering_hist_data -> Draw("ep same");
    // c1 -> Print("test.pdf");
    
}