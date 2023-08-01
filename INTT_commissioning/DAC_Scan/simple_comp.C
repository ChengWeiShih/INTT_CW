void simple_comp()
{
    TFile * f1 = TFile::Open("/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/AllServer/DAC2/beam_inttall-00023058-0000_event_base_ana_cluster_ideal_excludeR2000.root");
    TTree * tree1 = (TTree *)f1->Get("tree_clu");
    TH1F * hist1 = new TH1F("hist1","hist1",200,0,10000);
    hist1 -> SetLineColor(2);

    long long N_event1 = tree1 -> GetEntries();
    cout<<Form("N_event in file1 : ")<<N_event1<<endl;

    int fNhits1;
    int nclu_inner1, nclu_outer1;
    
    tree1 -> SetBranchStatus("*",0);
    tree1 -> SetBranchStatus("nhits",1);
    tree1 -> SetBranchStatus("nclu_inner",1);
    tree1 -> SetBranchStatus("nclu_outer",1);
    tree1 -> SetBranchAddress("nhits",&fNhits1);
    tree1 -> SetBranchAddress("nclu_inner",&nclu_inner1);
    tree1 -> SetBranchAddress("nclu_outer",&nclu_outer1);


    for (int i = 0; i < N_event1; i++)
    {   
        tree1 -> GetEntry(i);

        if (nclu_inner1 != -1 && nclu_outer1 != -1)
            hist1 -> Fill(nclu_inner1 + nclu_outer1);
    }




    TFile * f2 = TFile::Open("/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/AllServer/DAC5/beam_inttall-00023051-0000_event_base_ana_cluster_ideal_excludeR2000.root");
    TTree * tree2 = (TTree *)f2->Get("tree_clu");
    TH1F * hist2 = new TH1F("hist2","hist2",200,0,10000);
    // hist2 -> SetLineColor(2);

    long long N_event2 = tree2 -> GetEntries();
    cout<<Form("N_event in file2 : ")<<N_event2<<endl;

    int fNhits2;
    int nclu_inner2;
    int nclu_outer2;

    tree2 -> SetBranchStatus("*",0);
    tree2 -> SetBranchStatus("nhits",1);
    tree2 -> SetBranchStatus("nclu_inner",1);
    tree2 -> SetBranchStatus("nclu_outer",1);
    tree2 -> SetBranchAddress("nhits",&fNhits2);
    tree2 -> SetBranchAddress("nclu_inner",&nclu_inner2);
    tree2 -> SetBranchAddress("nclu_outer",&nclu_outer2);

    for (int i = 0; i < N_event2; i++)
    {
        tree2 -> GetEntry(i);
        if (nclu_inner2 != -1 && nclu_outer2 != -1)
            hist2 -> Fill(nclu_inner2 + nclu_outer2);
        // hist2 -> Fill(fNhits2);
    }

    hist2 -> Scale(1./hist2->Integral(-1,-1));
    hist1 -> Scale(1./hist1->Integral(-1,-1));

    hist2 -> Draw("hist ");
    hist1 -> Draw("hist same");

}