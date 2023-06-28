void run52_data_MC_residual_comp()
{
    TFile * data_file = new TFile("/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/MC_data_residual_comp/run52_U8_data/DUT_residual_narrow_U8_test.root","read");
    TH1F * data_hist = (TH1F *) data_file->Get("run52_U8");
    data_hist -> Rebin(2);

    cout <<"n_bin= "<< data_hist->GetXaxis()->GetNbins() << endl;
    cout <<"X_min= "<< data_hist->GetXaxis()->GetXmin() << endl;
    cout <<"X_Max= "<< data_hist->GetXaxis()->GetXmax() << endl;

    data_hist -> SetLineColor(1);
    data_hist -> SetLineWidth(2);
    data_hist -> Scale(1. / data_hist -> Integral(-1,-1));
    data_hist -> SetStats(0);


    TFile * MC_file = new TFile("/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/MC_data_residual_comp/folder_e+_934MeV_L1offset_1M_filter_all_int_1000_chip_9/DUT_residual_narrow_U11_test.root","read");
    TH1F * MC_hist = (TH1F *) MC_file->Get("run52_U11");
    MC_hist  -> Rebin(2);

    cout <<"MC n_bin= "<< MC_hist->GetXaxis()->GetNbins() << endl;
    cout <<"MC X_min= "<< MC_hist->GetXaxis()->GetXmin() << endl;
    cout <<"MC X_Max= "<< MC_hist->GetXaxis()->GetXmax() << endl;

    MC_hist -> SetLineColor(3);
    MC_hist -> SetLineWidth(2);
    MC_hist -> Scale(1. / MC_hist -> Integral(-1,-1));
    MC_hist -> SetStats(0);

    TLegend *legend = new TLegend(0.75, 0.75, 0.90, 0.90);

    legend -> AddEntry(data_hist,"data U8","f");
    legend -> AddEntry(MC_hist,"MC","f");

    // MC_hist -> Scale(data_hist -> Integral(-1,-1) / MC_hist -> Integral(-1,-1) );
    // h->Scale(h2->Integral() / h->Integral());

    data_hist -> Draw("hist");
    MC_hist   -> Draw("hist same");
    legend    -> Draw("same");
}