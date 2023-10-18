void cosmic_bcodiff()
{
    string mother_folder_directory = "/sphenix/user/ChengWei/INTT/INTT_commissioning/cosmic/26960";
    // string file_name = "beam_inttall-00020869-0000_event_base_ana_cluster_ideal_excludeR1500_100kEvent";
    string file_name = "cosmics_inttall-00026960-0000_event_base_ana_cluster_full_survey_3.32_excludeR2000_200kEvent_10HotCut";
    
    TCanvas * c1 = new TCanvas("","",400,1000);
    c1 -> Divide(2,4);

    TH1F * bcodiff_hist[8];
    for (int i = 0; i < 8; i++){
        bcodiff_hist[i] = new TH1F(Form("server %i",i),Form("server %i",i),128,0,128);
        bcodiff_hist[i] -> SetLineColor(i+1);
        bcodiff_hist[i] -> GetXaxis() -> SetTitle("bco difference");
    }

    TFile * file_in = TFile::Open(Form("%s/folder_%s_cosmic/INTT_eventdisplay_cluster_fit.root",mother_folder_directory.c_str(),file_name.c_str()));
    TTree * tree = (TTree *)file_in->Get("tree_clu");
    vector<vector<double>> *bcodiff_vec = 0;
    vector<int>* server_vec = new vector<int>();
    vector<int>* module_vec = new vector<int>();

    tree -> SetBranchAddress("bco_diff_vec",&bcodiff_vec);
    tree -> SetBranchAddress("server",&server_vec);
    tree -> SetBranchAddress("module",&module_vec);


    for (int i = 0; i < tree -> GetEntries(); i++)
    {
        tree -> GetEntry(i);
     
        cout<<"----------------------------"<<endl;

        for (int i1 = 0; i1 < server_vec->size(); i1++){ // note : n cluster
            for (int i2 = 0; i2 < bcodiff_vec->at(i1).size(); i2++)
            {
                bcodiff_hist[server_vec->at(i1) - 3001] -> Fill(bcodiff_vec->at(i1).at(i2));
                // cout<<"test "<<module_vec->at(i1)<<" "<<bcodiff_vec->at(i1).at(i2)<<endl;
            }
        }



        
    }

    for (int i = 0; i < 8; i++){
        c1 -> cd(i+1);

        // c1 -> cd(i+1) -> SetLogy();
        // TPad *pad = new TPad(Form("pad"), "", 0.0, 0.0, 1.0, 1.0);
        // pad -> SetLogy();

        bcodiff_hist[i] -> Draw("hist");
    }

    c1 -> Print( Form("%s/folder_%s_cosmic/bcodiff_hist.pdf",mother_folder_directory.c_str(),file_name.c_str()) );
}