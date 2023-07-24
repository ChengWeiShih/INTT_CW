void simple_comp()
{
    TString color_code [8]={"#343434","#1A3947","#566575","#797983","#EFBD9D","#FCA26E","#F5751D","#F5321D"};
    TString color_code_2 [8]={"#CC768D","#19768D","#DDA573","#009193","#6E9193","#941100","#A08144","#517E66"};

    vector<string> file_name = {
        // "/home/phnxrc/INTT/cwshih/DACscan_data/test_intt4/beam_intt4-00021527-0000_event_base.root",
        // "/home/phnxrc/INTT/cwshih/DACscan_data/test_intt4/beam_intt4-00021040-0000_event_base.root",
        // "/home/phnxrc/INTT/cwshih/DACscan_data/test_intt4/beam_intt4-00021035-0000_event_base.root",
        // "/home/phnxrc/INTT/cwshih/DACscan_data/test_intt4/beam_intt4-00021025-0000_event_base.root",
        // "/home/phnxrc/INTT/cwshih/DACscan_data/test_intt4/beam_intt4-00021019-0000_event_base.root"

        "/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/intt4/beam_intt4-00023058-0000_event_base.root"
        ,"/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/intt4/beam_intt4-00023055-0000_event_base.root"
        ,"/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/intt4/beam_intt4-00023053-0000_event_base.root"
        ,"/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/intt4/beam_intt4-00023051-0000_event_base.root"
    };
    
    vector<TH1F *> hist_vec; hist_vec.clear();

    int selected_module = 5;

    int fNhits;
    int module[10000];
    int chip_id[10000];
    int chan_id[10000];
    int adc[10000];

    TCanvas * c1 = new TCanvas("","",800,800);

    for (int i = 0; i < file_name.size(); i++)
    {
        hist_vec.push_back( new TH1F(Form("hist_%i",i),"",8,0,8) );
        hist_vec[i] -> SetMinimum(0);
        hist_vec[i] -> SetMaximum(0.6);
        hist_vec[i] -> SetLineColor( TColor::GetColor( color_code_2[i ] ) );

        TFile * file_in = new TFile(file_name[i].c_str());
        TTree * tree = (TTree *)file_in -> Get("tree");

        tree -> SetBranchAddress("fNhits",&fNhits);
        tree -> GetEntry(0); // note : actually I really don't know why this line is necessary.
        tree -> SetBranchAddress("fhitArray.module",&module[0]);
        tree -> SetBranchAddress("fhitArray.chip_id",&chip_id[0]);
        tree -> SetBranchAddress("fhitArray.chan_id",&chan_id[0]);
        tree -> SetBranchAddress("fhitArray.adc",&adc[0]);

        // tree -> Draw(Form("fhitArray.adc>>hist_%i",i),Form("fhitArray.module==%i && fhitArray.adc != 7",selected_module),"",100000);


        for (int iev = 0; iev < 5000; iev++)
        {
            tree -> GetEntry(iev);

            for ( int i1 = 0; i1 < fNhits; i1++ )
            {
                if (module[i1] == selected_module && adc[i1] != 7)
                {
                    hist_vec[i] -> Fill(adc[i1]);
                }                   
            }
        }

        hist_vec[i] -> Scale( 1. / hist_vec[i] -> Integral(-1,-1));   

        hist_vec[i] -> SetMinimum(0);
        hist_vec[i] -> SetMaximum(0.25);

        // hist_vec[i] -> SetBinContent(3,100);
        cout<<"test "<<i<<" "<<hist_vec[i] -> GetBinContent(3)<<" "<<hist_vec[i] -> GetNbinsX()<<endl;


        TString draw_text = (i == 0) ? "ep" : "ep same";
        hist_vec[i] -> Draw(draw_text);

        

        file_in -> Close();
    }

    c1 -> Print("test.pdf");
    

}