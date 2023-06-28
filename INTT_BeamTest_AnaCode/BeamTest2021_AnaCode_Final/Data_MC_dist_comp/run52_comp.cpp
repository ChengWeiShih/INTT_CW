#include "data_process.cpp"
#include "draw_style.h"

// note : the direction : /home/cwshih/sPHENIX/INTT/INTT_GitHub/INTT_CW/INTT_BeamTest_AnaCode/Data_MC_dist_comp

void run52_comp()
{
    bool set_logY = true;
    bool with_cut = false; // note : for cluster ADC and cluster size
    bool X_label_less = true;
    bool Y_label_far = true;

    TCanvas * c1 = new TCanvas("c1","c1",850 ,800);
    TPad * pad_c1 = new TPad(Form("pad_c1"), "", 0.0, 0.0, 1.0, 1.0);
    Characterize_Pad(pad_c1, 0.15,  0.1,  0.1,  0.12, set_logY, 0);
    pad_c1 -> Draw();
    pad_c1 -> cd();

    // note : for nothing
    // int Hist_nbins = 100;
    // double Hist_ledge = -0.15;
    // double Hist_redge = 0.15;

    // note : for scattering 
    int Hist_nbins = 39;
    double Hist_ledge = -0.051468710;
    double Hist_redge = 0.051468710;

    // note : TEST TEST TEST for scattering TEST TEST TEST
    // int Hist_nbins = 21;
    // double Hist_ledge = -0.051468710;
    // double Hist_redge = 0.051468710;

    // note : for residual
    // int Hist_nbins = 31;
    // double Hist_ledge = -1.209;
    // double Hist_redge = 1.209;

    // // note : for cluster size
    // int Hist_nbins = 20;
    // double Hist_ledge = -3;
    // double Hist_redge = 17;

    // note : for cluster adc
    // int Hist_nbins = 30;
    // double Hist_ledge = 0;
    // double Hist_redge = 450;
    // note : for cluster adc, Nbin = 30, L = 0, R = 450;
    // vector<double> bin_uniform_factor = {0,0.0066,0,0.008224,0.00341,0.006604,0.005622,0.007342,0.007052,0.008782,0.008766,0.010166,0.010694,0.011796,0.01281,0.014282,0.015806,0.010648,0.015438,0.01343,0.014654,0.015004,0.015058,0.01595,0.015878,0.016812,0.016642,0.0176,0.016598,0.017946};

    // note : for N track extrapolation position distribution
    // int Hist_nbins = 100;
    // double Hist_ledge = -20.;
    // double Hist_redge = 20.;
    // note : for N track distribution
    // int Hist_nbins = 13;
    // double Hist_ledge = -3;
    // double Hist_redge = 10;
    // double extrapol_pos_cut_low  = -10;
    // double extrapol_pos_cut_high = 10;

    //note : for track-candidate slope
    // int Hist_nbins = 50;
    // double Hist_ledge = -0.05;
    // double Hist_redge = 0.05;

    // note : for alignment
    // int Hist_nbins = 31;
    // double Hist_ledge = -1.209;
    // double Hist_redge = 1.209;
    
    // note : //////////// //////////// //////////// //////////// //////////// for data //////////// //////////// //////////// //////////// ////////////
    TString folder_direction_data = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency/run52_new_alignment";
    TString cluster_file_name_data = "cluster_information_offset-0.0000_adcinfo_NoCamac";
    int study_chip_data = 8;

    double amount_of_alignment_data = -2.91425e-01;// note : run52 U8, for the scattering and the residual study
    // double amount_of_alignment_data = -0.2975;// note : run52 U8, for the L1 efficiency, and the cluster adc distribution
    double slope_correction_data = 0.001712; // note : run52 U8

    // double amount_of_alignment_data = -2.65014e-01;// note : run52 U9
    // double slope_correction_data = 0.001712; // note : run52 U9, the value here (0.001712) is for run52 U8, should be studied

    // double amount_of_alignment_data = 0; // note : test
    // double slope_correction_data = 0.; // note : test

    data_process * data_class = new data_process(1, "data_class", folder_direction_data, cluster_file_name_data, study_chip_data, amount_of_alignment_data, slope_correction_data);
    cout<<"class name : "<<data_class->GetClassName()<<endl;
    data_class->para_check();
    
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
    TH1F * scattering_hist_data = data_class -> Get_scattering_dist(Hist_nbins,Hist_ledge,Hist_redge,data_class->get_cluster_reformat());
    cout<<"data, RMS : "<<scattering_hist_data -> GetRMS()<<" RMS error : "<<scattering_hist_data -> GetRMSError()<<" StdDev : "<<scattering_hist_data -> GetStdDev()<<" StdDev error : "<<scattering_hist_data -> GetStdDevError()<<endl;
    // Characterize_Hist1F(scattering_hist_data,1,"#A08144","","((Cl2-Cl1)/dis)-((Cl1-Cl0)/dis)","Events");

    // note : for residual dist
    // TH1F * residual_hist_data = data_class->Get_residual_dist(Hist_nbins,Hist_ledge,Hist_redge,data_class->get_cluster_reformat());
    // Characterize_Hist1F(residual_hist_data,1,"#A08144",set_logY,false,"","Residual [mm]","Events");

    //note : for the track-candidates slope 
    // TH1F * track_candi_slope_data = data_class->Get_track_candidate_slope(Hist_nbins,Hist_ledge,Hist_redge,data_class->get_cluster_reformat());
    // Characterize_Hist1F(track_candi_slope_data,1,"#A08144",set_logY,false,"","Slope [tan(#theta)]","Events");
    // track_candi_slope_data -> Draw("hist");

    // note : for fitting a gaussian
    // TF1 * gaus_fit = new TF1 ("gaus_fit","gaus",-10,10);
    // track_candi_slope_data -> Fit(gaus_fit,"N");
    // gaus_fit -> Draw("lsame");

    // note : //////////// //////////// //////////// //////////// //////////// for  MC //////////// //////////// //////////// //////////// ////////////
    // TString folder_direction_MC = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency/run52_MC_AlFoil/folder_run52_X0_Y-5_1M_WithSciAlFoil_10PercentHDIMetalMaterial_BeamSmear_EMZ_40umCut_Run52DataU9offset_L1_2.65014e-01mm_RoughTrigger";
    // TString folder_direction_MC = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency/run52_MC_AlFoil/folder_run52_X0_Y-5_1M_WithSciAlFoil_10PercentHDIMetalMaterial_BeamSmear_EMZ_40umCut_RoughTrigger";

    //note : EMZ MC5 40 um
    // TString folder_direction_MC = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency/run52_MC_AlFoil/folder_run52_X0_Y-5_1M_WithSciAlFoil_10PercentHDIMetalMaterial_BeamSmear_EMZ_40umCut_Run52DataU8Offset_L1_2.91425e-01mm_Run52DataU8TrackCandidateSlope_9.809e-02deg_RoughTrigger";

    //note : WVI MC5 20 um
    TString folder_direction_MC = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency/run52_MC_AlFoil/folder_run52_X0_Y-5_1M_WithSciAlFoil_10PercentHDIMetalMaterial_BeamSmear_WVI_20umCut_Run52DataU8Offset_L1_2.91425e-01mm_Run52DataU8TrackCandidateSlope_9.809e-02deg_RoughTrigger";

    //note : SS MC5 20 um
    // TString folder_direction_MC = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency/run52_MC_AlFoil/folder_run52_X0_Y-5_1M_WithSciAlFoil_10PercentHDIMetalMaterial_BeamSmear_SS_20umCut_Run52DataU8Offset_L1_2.91425e-01mm_Run52DataU8TrackCandidateSlope_9.809e-02deg_RoughTrigger";
    
    TString cluster_file_name_MC = "cluster_information_offset-0.0000_adcinfo_SingleTrigger";
    int study_chip_MC = 8;

    // double amount_of_alignment_MC = -2.89928e-01; // note : EMZ 40um MC run52 U8 offset, obtained in MC4
    // double slope_correction_MC = -1.56313e-03; // note : EMZ 40um  MC run52 U8 track slope, from MC5

    double amount_of_alignment_MC = -2.90258e-01; // note : WVI 20um MC run52 U8 offset, obtained in MC5
    double slope_correction_MC = 0; // note : WVI 20um MC run52 U8 track slope, from MC5

    // double amount_of_alignment_MC = -2.89824e-01; // note : SS 20um MC run52 U8 offset, obtained in MC5
    // double slope_correction_MC = 0; // note : SS 20um MC run52 U8 track slope, from MC5
    
    // double amount_of_alignment_MC = -2.66304e-01; // note : MC run52 U9 offset
    // double slope_correction_MC = 0.;
    
    // note : test
    // double amount_of_alignment_MC = 0;
    // double slope_correction_MC = 0.;

    data_process * MC_class = new data_process(1, "MC_class", folder_direction_MC, cluster_file_name_MC, study_chip_MC, amount_of_alignment_MC, slope_correction_MC);
    cout<<"class name : "<<MC_class->GetClassName()<<endl;
    MC_class->para_check();

    // note : for cluster size, three layers
    // vector <TH1F *> cluster_size_dist_all_layers_MC = MC_class->Get_cluster_size(with_cut,Hist_nbins,Hist_ledge,Hist_redge,MC_class->get_cluster_reformat());
    // Characterize_Hist1F(cluster_size_dist_all_layers_MC[0],1,"#A08144","","cluster size","Events");

    // note : for cluster adc, three layers
    // vector <TH1F *> cluster_adc_dist_all_layers_MC = MC_class->Get_cluster_adc(with_cut,Hist_nbins,Hist_ledge,Hist_redge,MC_class->get_cluster_reformat());
    // Characterize_Hist1F(cluster_adc_dist_all_layers_MC[2],1,"#A08144",set_logY,false,"","cluster adc","A.U.");
    // cluster_adc_dist_all_layers_MC[2] -> Scale(1./cluster_adc_dist_all_layers_MC[2]->Integral(-1,-1));
    // binbin_uniform(cluster_adc_dist_all_layers_MC[2],bin_uniform_factor);
    // cluster_adc_dist_all_layers_MC[2] -> Draw("hist");

    //note : for the track-candidates slope 
    // TH1F * track_candi_slope_MC = MC_class->Get_track_candidate_slope(Hist_nbins,Hist_ledge,Hist_redge,MC_class->get_cluster_reformat());
    // Characterize_Hist1F(track_candi_slope_MC,1,"#A08144",set_logY,false,"","Slope [tan(#theta)]","Events");
    // track_candi_slope_MC -> GetYaxis() -> SetTitleOffset (1.5);
    // track_candi_slope_MC -> Draw("hist");

    // note : for scattering
    TH1F * scattering_hist_MC = MC_class->Get_scattering_dist(Hist_nbins,Hist_ledge,Hist_redge,MC_class->get_cluster_reformat());
    cout<<"MC, RMS : "<<scattering_hist_MC -> GetRMS()<<" RMS error : "<<scattering_hist_MC -> GetRMSError()<<" StdDev : "<<scattering_hist_MC -> GetStdDev()<<" StdDev error : "<<scattering_hist_MC -> GetStdDevError()<<endl;
    // Characterize_Hist1F(scattering_hist_MC,1,"#A08144","","((Cl2-Cl1)/dis)-((Cl1-Cl0)/dis)","Events");

    // note : for residual distribution
    // TH1F * residual_hist_MC = MC_class->Get_residual_dist(Hist_nbins,Hist_ledge,Hist_redge,MC_class->get_cluster_reformat());
    // Characterize_Hist1F(residual_hist_MC,0,"#A08144",set_logY,false,"","Residual [mm]","Events");

    // note : for alignment dist
    // TH1F * alignment_hist_MC = MC_class->Get_alignment_dist(Hist_nbins,Hist_ledge,Hist_redge,MC_class->get_cluster_reformat());
    // Characterize_Hist1F(alignment_hist_MC,0,"#A08144",set_logY,false,Form("Chip : %i",study_chip_MC),"Residual [mm]","Events",X_label_less,Y_label_far);
    // alignment_hist_MC -> Draw("hist");
    
    // note : for fitting
    // TF1 * gaus_fit = new TF1 ("gaus_fit","gaus",-10,10);
    // alignment_hist_MC -> Fit(gaus_fit,"N");
    // gaus_fit -> Draw("lsame");
    // c1 -> Print(folder_direction_MC+"/MC_WVI_run52U8_alignment_dist.pdf");

    // note : //////////// //////////// //////////// //////////// //////////// for comparison //////////// //////////// //////////// //////////// ////////////
    // TString plot_name = "WVI20um_run52U8_comp_residual_Nbin_"+std::to_string(Hist_nbins);
    TString plot_name = "BinTest_WVI20um_run52U8_comp_scattering_Nbin_"+std::to_string(Hist_nbins); 

    // TString plot_name = "EMZ40um_run52U8_comp_residual_Nbin_"+std::to_string(Hist_nbins);
    // TString plot_name = "EMZ40um_run52U8_comp_scattering_Nbin_"+std::to_string(Hist_nbins); 

    // TString plot_name = "SS20um_run52U8_comp_residual_Nbin_"+std::to_string(Hist_nbins);
    // TString plot_name = "SS20um_run52U8_comp_scattering_Nbin_"+std::to_string(Hist_nbins); 

    vector<TString> titles_vec = {"Scattering slope [tan(#theta)]","A.U."};
    // vector<TString> titles_vec = {"Residual [mm]","A.U."};
     
    dataMC_comp (scattering_hist_data, scattering_hist_MC, folder_direction_MC, plot_name, titles_vec ,set_logY, false);

    

    // note : for the cluster ADC and the cluster size
    // for (int i = 0; i < 3; i++)
    // {   
    //     TString plot_name = "run52U8_comp_clusterADC_Nbin_"+std::to_string(Hist_nbins)+"_LayerID"+std::to_string(i);
    //     // TString plot_name = "run52U8_comp_clustersize_Nbin_"+std::to_string(Hist_nbins)+"_LayerID"+std::to_string(i);

    //     if (with_cut == true)
    //     {
    //         plot_name += "_WithCut";
    //     }
    //     else if (with_cut == false)
    //     {
    //         plot_name += "_WithOutCut";
    //     }
        
    //     vector<TString> titles_vec = {"Cluster ADC","A.U."}; // note : for cluster
    //     // vector<TString> titles_vec = {"Cluster size","A.U."}; // note : for cluster

    //     // note : ADC combination factor cancellation
    //     binbin_uniform(cluster_adc_dist_all_layers_data[i],bin_uniform_factor);
    //     binbin_uniform(cluster_adc_dist_all_layers_MC[i],bin_uniform_factor);

    //     dataMC_comp (cluster_adc_dist_all_layers_data[i], cluster_adc_dist_all_layers_MC[i], folder_direction_MC, plot_name, titles_vec ,set_logY, false);
    // }


    // note : for test
    // scattering_hist_MC -> Draw("hist");
    // scattering_hist_data -> Draw("ep same");
    // c1 -> Print("test.pdf");
    
}