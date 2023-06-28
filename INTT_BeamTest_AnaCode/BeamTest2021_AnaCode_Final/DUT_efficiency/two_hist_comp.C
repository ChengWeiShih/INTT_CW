TString color_code [8]={"#343434","#1A3947","#566575","#797983","#EFBD9D","#FCA26E","#F5751D","#F5321D"};
TString color_code_2 [8]={"#CC768D","#19768D","#DDA573","#009193","#6E9193","#941100","#A08144","#517E66"};

void Characterize_Pad (TPad *pad, float left = 0.2, float right = 0.05, float top = 0.1, float bottom = 0.15, int logY = 0, int setgrid_bool = 1)
{
	if (setgrid_bool == true) {pad -> SetGrid (1, 1);}
	pad -> SetLeftMargin   (left);
	pad -> SetRightMargin  (right);
	pad -> SetTopMargin    (top);
	pad -> SetBottomMargin (bottom);
	pad -> SetLogy (logY);
}

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



    TFile * data_file = new TFile("/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/BeamTest2021_data_MC_file_plot_final/MC_file/MC_hinako/DUT_residual_narrow_U11_pub_tightest.root","read");
    TH1F * data_hist = (TH1F *) data_file->Get("MC_hinako_U11");
    data_hist -> Rebin(2);

    cout <<"n_bin= "<< data_hist->GetXaxis()->GetNbins() << endl;
    cout <<"X_min= "<< data_hist->GetXaxis()->GetXmin() << endl;
    cout <<"X_Max= "<< data_hist->GetXaxis()->GetXmax() << endl;

    data_hist -> SetLineColor(TColor::GetColor("#A08144"));
    data_hist -> SetLineWidth(2);
    data_hist -> Scale(1. / data_hist -> Integral(-1,-1));
    data_hist -> SetStats(0);
    // data_hist -> GetXaxis() -> SetNdivisions  (505);

    data_hist -> GetYaxis() -> SetTitle("A.U.");
    data_hist -> GetXaxis() -> SetTitleSize   (0.05);
	data_hist -> GetXaxis() -> SetTitleOffset (0.8);
	data_hist -> GetYaxis() -> SetTitleSize   (0.05);
	data_hist -> GetYaxis() -> SetTitleOffset (1.3);


    TFile * MC_file = new TFile("/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/BeamTest2021_data_MC_file_plot_final/MC_file/MC_hinako/outfile_10.root","read");
    TH1F * MC_hist = (TH1F *) MC_file->Get("h18");
    // MC_hist  -> Rebin(2);

    cout <<"MC n_bin= "<< MC_hist->GetXaxis()->GetNbins() << endl;
    cout <<"MC X_min= "<< MC_hist->GetXaxis()->GetXmin() << endl;
    cout <<"MC X_Max= "<< MC_hist->GetXaxis()->GetXmax() << endl;

    MC_hist -> SetLineColor(TColor::GetColor("#1A3947"));
    MC_hist -> SetLineWidth(2);
    MC_hist -> Scale(1. / MC_hist -> Integral(-1,-1));
    MC_hist -> SetStats(0);
    // MC_hist -> GetXaxis() -> SetNdivisions  (505);

    MC_hist -> GetYaxis() -> SetTitle("A.U.");
    MC_hist -> GetXaxis() -> SetTitleSize   (0.05);
	MC_hist -> GetXaxis() -> SetTitleOffset (0.8);
	MC_hist -> GetYaxis() -> SetTitleSize   (0.05);
	MC_hist -> GetYaxis() -> SetTitleOffset (1.3);



    TLegend *legend = new TLegend(0.7, 0.8, 0.90, 0.90);
    legend -> SetTextSize(0.027);
    legend -> AddEntry(data_hist,"Cheng-Wei","f");
    legend -> AddEntry(MC_hist,"Hinako","f");

    // MC_hist -> Scale(data_hist -> Integral(-1,-1) / MC_hist -> Integral(-1,-1) );
    // h->Scale(h2->Integral() / h->Integral());

    data_hist -> Draw("hist");
    MC_hist   -> Draw("hist same");
    legend    -> Draw("same");

    c1 -> Print("/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/BeamTest2021_data_MC_file_plot_final/MC_file/MC_hinako/CW_tightest_hinako_h18.pdf");
}