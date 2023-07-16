#include "draw_style.h"

TString color_code [8]={"#343434","#1A3947","#566575","#797983","#EFBD9D","#FCA26E","#F5751D","#F5321D"};
TString color_code_2 [8]={"#CC768D","#19768D","#DDA573","#009193","#6E9193","#941100","#A08144","#517E66"};

void two_hist_comp()
{
    TCanvas * c1 = new TCanvas("c1","c1",850,800);
    c1 -> cd();

    TPad * pad_c1 = new TPad(Form("pad_c1"), "", 0.0, 0.0, 1.0, 1.0);
    Characterize_Pad(pad_c1, 0.15,  0.1,  0.1,  0.12, 0, 0);
    pad_c1 -> Draw();
    pad_c1 -> SetTicks(1,1);
    pad_c1 -> SetLogy();
    pad_c1 -> cd();


    // note : DUT_residual_narrow_U8_pub_FirstTwoCutsOnly.root
    TFile * data_file = new TFile("/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/BeamTest2021_data_MC_file_plot_final/data_file/run52/scattering_l0l1_attempt/DUT_residual_narrow_U8_pub_SelectionLevel_999.root","read");
    TH1F * data_hist = (TH1F *) data_file->Get("run52_U8");
    data_hist -> Rebin(2);

    cout <<"n_bin= "<< data_hist->GetXaxis()->GetNbins() << endl;
    cout <<"X_min= "<< data_hist->GetXaxis()->GetXmin() << endl;
    cout <<"X_Max= "<< data_hist->GetXaxis()->GetXmax() << endl;

    data_hist -> SetLineColor(TColor::GetColor("#1A3947"));
    data_hist -> SetLineWidth(2);
    data_hist -> SetMarkerSize(1);
    data_hist -> Scale(1. / data_hist -> Integral(-1,-1));
    data_hist -> SetStats(0);
    // data_hist -> GetXaxis() -> SetNdivisions  (505);

    data_hist -> GetYaxis() -> SetTitle("A.U.");
    data_hist -> GetXaxis() -> SetTitleSize   (0.05);
	data_hist -> GetXaxis() -> SetTitleOffset (0.8);
	data_hist -> GetYaxis() -> SetTitleSize   (0.05);
	data_hist -> GetYaxis() -> SetTitleOffset (1.3);


    TString file_txt = "20230628_1";
    int selection = 999;

    TFile * MC_file = new TFile(Form("/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/BeamTest2021_data_MC_file_plot_final/MC_file/MC_run52/folder_run52_MC_run_%s/scattering_l0l1_attempt/DUT_residual_narrow_U11_pub_SelectionLevel_%i.root",file_txt.Data(),selection),"read");
    TH1F * MC_hist = (TH1F *) MC_file->Get("MC_run52_U11");
    MC_hist  -> Rebin(2);

    cout <<"MC n_bin= "<< MC_hist->GetXaxis()->GetNbins() << endl;
    cout <<"MC X_min= "<< MC_hist->GetXaxis()->GetXmin() << endl;
    cout <<"MC X_Max= "<< MC_hist->GetXaxis()->GetXmax() << endl;

    MC_hist -> SetLineColor(TColor::GetColor("#A08144"));
    MC_hist -> SetLineWidth(2);
    MC_hist -> Scale(1. / MC_hist -> Integral(-1,-1));
    MC_hist -> SetStats(0);
    // MC_hist -> GetXaxis() -> SetNdivisions  (505);

    MC_hist -> GetYaxis() -> SetTitle("A.U.");
    MC_hist -> GetXaxis() -> SetTitleSize   (0.05);
	MC_hist -> GetXaxis() -> SetTitleOffset (0.8);
	MC_hist -> GetYaxis() -> SetTitleSize   (0.05);
	MC_hist -> GetYaxis() -> SetTitleOffset (1.3);

    vector<TString> titles_vec = {"Residual [mm]","A.U."};

    TString folder_directory = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/BeamTest2021_data_MC_file_plot_final/MC_file/MC_run52/folder_run52_MC_run_20230628_1/scattering_l0l1_attempt";
    TString plot_name = Form("DataRun52U8_MC%s_Selection_%i_4.5",file_txt.Data(),selection);

    dataMC_comp (data_hist, MC_hist, folder_directory , plot_name, titles_vec ,true, false);

    // TLegend *legend = new TLegend(0.63, 0.8, 0.90, 0.90);
    // legend -> SetTextSize(0.027);
    // legend -> AddEntry(data_hist,"Data run52 U8","f");
    // legend -> AddEntry(MC_hist,Form("MC %s",file_txt.Data()),"f");

    // // MC_hist -> Scale(data_hist -> Integral(-1,-1) / MC_hist -> Integral(-1,-1) );
    // // h->Scale(h2->Integral() / h->Integral());

    // data_hist -> Draw("ep ");
    // MC_hist   -> Draw("hist same");
    // legend    -> Draw("same");

    // c1 -> Print(Form("DataRun52U8_MC%s_Selection_%i.pdf",file_txt.Data(),selection));
}