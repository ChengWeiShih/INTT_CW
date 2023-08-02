void simple_comp_2()
{

    vector<string> file_name = {
	    "/home/phnxrc/INTT/cwshih/DACscan_data/zero_magnet_Takashi_used/beam_inttall-00020869-0000_event_base_ana_cluster_ideal_excludeR1500_100kEvent.root",
	    "/home/phnxrc/INTT/cwshih/DACscan_data/2023_08_01/24767/beam_inttall-00024767-0000_event_base_ana_cluster_ideal_excludeR2000_100kEvent.root",
        "/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/AllServer/DAC2/beam_inttall-00023058-0000_event_base_ana_cluster_ideal_excludeR2000_100kEvent.root",
        "/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/AllServer/DAC3/beam_inttall-00023055-0000_event_base_ana_cluster_ideal_excludeR2000_100kEvent.root",
        "/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/AllServer/DAC4/beam_inttall-00023053-0000_event_base_ana_cluster_ideal_excludeR2000_100kEvent.root",
        "/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/AllServer/DAC5/beam_inttall-00023052-0000_event_base_ana_cluster_ideal_excludeR2000_100kEvent.root",
        "/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/AllServer/DAC5/beam_inttall-00023051-0000_event_base_ana_cluster_ideal_excludeR2000_100kEvent.root"
    };

    vector<string>DACO_string = {
	    "15",
	    "15",
	    "DAC2",
	    "DAC3",
	    "DAC4",
	    "DAC5",
	    "DAC5"
    };

    TCanvas * c1 = new TCanvas("c1","c1",800,800);
    TCanvas * c2 = new TCanvas("c2","c2",800,800);
    TCanvas * c3 = new TCanvas("c3","c3",800,800);
    TCanvas * c4 = new TCanvas("c4","c4",800,800);
    
    TH2F * hist_2D = new TH2F("hist_2D","hist_2D",200,0,2000,200,0,2000);
    TH2F * hist_2D_hit_clu = new TH2F("","",200,0,2000,200,0,2000);
    TH1F * nhits_hist = new TH1F("nhits_hist","nhits_hist",200,0,15000);
    TH1F * nclu_hist = new TH1F("nclu_hist","nclu_hist",200,0,3500);
    int fNhits1;
    int nclu_inner1, nclu_outer1;

    c1 -> Print("/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/AllServer/nclus.pdf(");
    c2 -> Print("/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/AllServer/nhits.pdf(");
    c3 -> Print("/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/AllServer/clu_2D.pdf(");
    c4 -> Print("/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/AllServer/nhit_nclu.pdf(");
    
    for ( int i = 0; i < file_name.size(); i++ )
    {
        TFile * f1 = TFile::Open(file_name[i].c_str());
        TTree * tree1 = (TTree *)f1->Get("tree_clu");
    
        long long N_event1 = tree1 -> GetEntries();
        cout<<Form("N_event in file1 : ")<<N_event1<<endl;
        
        tree1 -> SetBranchStatus("*",0);
        tree1 -> SetBranchStatus("nhits",1);
        tree1 -> SetBranchStatus("nclu_inner",1);
        tree1 -> SetBranchStatus("nclu_outer",1);
        tree1 -> SetBranchAddress("nhits",&fNhits1);
        tree1 -> SetBranchAddress("nclu_inner",&nclu_inner1);
        tree1 -> SetBranchAddress("nclu_outer",&nclu_outer1);

        for (int i1 = 0; i1 < N_event1; i1++)
        {
            tree1 -> GetEntry(i1);

	    nhits_hist -> Fill(fNhits1);
            nclu_hist -> Fill(nclu_inner1 + nclu_outer1);
	    hist_2D -> Fill(nclu_inner1,nclu_outer1);
	    hist_2D_hit_clu -> Fill(fNhits1,nclu_inner1 + nclu_outer1);

            //if (nclu_inner1 != -1 && nclu_outer1 != -1)
            //  hist_2D -> Fill(nclu_inner1, nclu_outer1);
        }

	c1 -> cd();
	nclu_hist -> SetTitle(DACO_string[i].c_str());
	nclu_hist -> Draw("hist");
	c1 -> Print("/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/AllServer/nclus.pdf");
	c1 -> Clear();
	nclu_hist -> Reset("ICESM");

	c2 -> cd();
	nhits_hist -> SetTitle(DACO_string[i].c_str());
	nhits_hist -> Draw("hist");
	c2 -> Print("/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/AllServer/nhits.pdf");
	c2 -> Clear();
	nhits_hist -> Reset("ICESM");

	c3 -> cd();
	hist_2D -> SetTitle(DACO_string[i].c_str());
	hist_2D -> Draw("colz0");
	c3 -> Print("/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/AllServer/clu_2D.pdf");
	c3 -> Clear();
	hist_2D -> Reset("ICESM");


	c4 -> cd();
	hist_2D_hit_clu -> SetTitle(DACO_string[i].c_str());
	hist_2D_hit_clu -> Draw("colz0");
	c4 -> Print("/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/AllServer/nhit_nclu.pdf");
	c4 -> Clear();
	hist_2D_hit_clu -> Reset("ICESM");
	
	// hist_2D -> Draw("colz0");
        //nhits_hist -> Draw("hist");
        //c1 -> Print("/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/AllServer/nclus.pdf");
        
        //hist_2D -> Reset("ICESM");
        //nhits_hist -> Reset("ICESM");
        //c1 -> Clear();
        //cout<<"test clear : "<<hist_2D -> GetEntries()<<endl;

    }
    
    //c1 -> Print("/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/AllServer/nclus.pdf)");
    c1 -> Print("/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/AllServer/nclus.pdf)");
    c2 -> Print("/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/AllServer/nhits.pdf)");
    c3 -> Print("/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/AllServer/clu_2D.pdf)");
    c4 -> Print("/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/AllServer/nhit_nclu.pdf)");

}
