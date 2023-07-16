
void ch_dist_check()
{

    TString output_directory = "/home/inttdev/work/cwshih/raul_readout/ch_dist_plot";
    
    TString input_directory = "/home/inttdev/data/IR_DAQ_server";

    vector<TString> file_name = {
        // "selftrig_packv5_043023_1343",
        // "selftrig_packv5_043023_1237",
        // "selftrig_packv5_042923_0022",
        // "selftrig_packv5_042823_2345",
        // "selftrig_packv5_042923_0026",
        // "selftrig_packv5_043023_1233",
        // "selftrig_packv5_042823_2349"

        // "selftrig_packv5_043023_1815_RC-2south_RC-3south_DAC0_15_runtime_2min_30sec",
        // "selftrig_packv5_043023_1815_RC-4south_RC-5south_DAC0_15_runtime_2min_30sec",
        // "selftrig_packv5_043023_1815_RC-6south_RC-7south_DAC0_15_runtime_2min_30sec",
        // "selftrig_packv5_043023_1816_RC-0north_RC-1north_DAC0_15_runtime_2min_30sec",
        // "selftrig_packv5_043023_1815_RC-2north_RC-3north_DAC0_15_runtime_2min_30sec",
        // "selftrig_packv5_043023_1815_RC-4north_RC-5north_DAC0_15_runtime_2min_30sec",
        // "selftrig_packv5_043023_1815_RC-6north_RC-7north_DAC0_15_runtime_2min_30sec"
        "selftrig_packv5_050323_1521_intt0_DAC0_17_runtime_2min_0sec"
    };
    
    
    
    

    
    
    
    


    TFile * f1;
    
    TCanvas * c1 = new TCanvas("c1","c1",800,800);

    int module;
    int chan_id;
    int chip_id;

    for (int i = 0; i < file_name.size(); i++) // note : file
    {

        TH1F * ch_hist[14][26];
        for (int i1 = 0; i1 < 14; i1++) // note : ladder
        {
            for (int i2 = 0; i2 < 26; i2++) // note : chip
            {
               ch_hist[i1][i2] = new TH1F("","",128,0,128);
               ch_hist[i1][i2] -> SetTitle(Form("Felix ch %i, chip_id %i",i1, i2+1));
            }
        }

        f1 = TFile::Open(Form("%s/%s.root", input_directory.Data(), file_name[i].Data()));
        TTree * Gettree = (TTree *)f1->Get("tree");
        Gettree->SetBranchAddress("module", &module);
        Gettree->SetBranchAddress("chip_id", &chip_id);
        Gettree->SetBranchAddress("chan_id", &chan_id);

        long long N_hit = Gettree -> GetEntries();
        
        for (int i1 = 0; i1 < N_hit; i1++)
        {
            Gettree -> GetEntry(i1);
            ch_hist[module][chip_id - 1] -> Fill(chan_id);
        }
        
        
        
        c1 -> Print(Form("%s/%s_ch_dist.pdf(",output_directory.Data(),file_name[i].Data()));


        for (int i1 = 0; i1 < 14; i1++) // note : ladder
        {
            for (int i2 = 0; i2 < 26; i2++) // note : chip
            {
                ch_hist[i1][i2] -> Draw("hist");
                c1 -> Print(Form("%s/%s_ch_dist.pdf",output_directory.Data(),file_name[i].Data()));        
            }
        }
        

        c1 -> Print(Form("%s/%s_ch_dist.pdf)",output_directory.Data(),file_name[i].Data()));

    }

}