#include "ladder_cali.cpp"

void ladder_cali_test()
{   
    TString folder_name = "/home/inttdev/data/IR";
    TString file_name    = "calib_packv5_033123_0516.root"; // B1L105S, 0
    int ROC_index = 0; // todo : not sure about the number 
    // TString file_name    = "calib_packv5_040423_1923.root";   // B1L001S, 2

    // int threshold = 15; 
    // int ampl_cut = 50; 
    // double used_sigma = 4.0; 
    // int classify_base = 0; 
    // bool output_basic = 0; 
    // bool output_dead_channel_tree = false;

    TString plot_folder_dire = "/home/inttdev/work/cwshih/calib_db/ladder_cali_analyzer/test_folder";
    // vector<int> draw_plot_range = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26};
    vector<int> draw_plot_range = {1,2,5};

    TFile *f1 = TFile::Open(Form("%s/%s", folder_name.Data(), file_name.Data()));
	TTree *Gettree = (TTree *)f1->Get("tree");

    int adc = 0;
	int ampl = 0;
	int chip_id = 0;
	int fpga_id = 0;
	int module = 0;
	int chan_id = 0;
	int fem_id = 0;
	int bco_full = 0;
	int bco = 0;
	int event = 0;

    Gettree->SetBranchAddress("adc", &adc);
	Gettree->SetBranchAddress("ampl", &ampl);
	Gettree->SetBranchAddress("chip_id", &chip_id);
	Gettree->SetBranchAddress("fpga_id", &fpga_id);
	Gettree->SetBranchAddress("module", &module);
	Gettree->SetBranchAddress("chan_id", &chan_id);
	Gettree->SetBranchAddress("fem_id", &fem_id);
	Gettree->SetBranchAddress("bco", &bco);
	Gettree->SetBranchAddress("bco_full", &bco_full);
	Gettree->SetBranchAddress("event", &event);

    int total_size = Gettree->GetEntriesFast();
	cout << " total event : " << total_size << endl;
    
    ladder_cali * test_class = new ladder_cali("B1L105S", ROC_index, 0, 30, 52, 4.0,0, 0, 0);
    cout<<"ladder name : "<<test_class -> GetLadderName()<<endl;
    cout<<"ampl cut : "<<test_class -> GetAmplCut()<<endl;

    for (int i = 0; i < total_size; i++)
	{
		Gettree->GetEntry(i);
        if (module == test_class -> GetFelixCh())
        {
            test_class -> Fill(chip_id, chan_id, ampl, adc);
        }

        if (i%100000 == 0) cout<<"running : "<<i<<endl;
    }

    

    test_class -> Fit();
    test_class -> Overall_info();
    test_class -> Classifier();
    test_class -> Draw_bad_ch_plot(plot_folder_dire, file_name);
    test_class -> Output_bad_ch_root(plot_folder_dire, file_name );
    test_class -> Output_ch_property_root(plot_folder_dire, file_name );
    test_class -> Draw_ch_plot(draw_plot_range, plot_folder_dire, file_name, "ampl_total ampl_single");
    test_class -> Draw_overall_info(plot_folder_dire, file_name);

    cout<<"Entry : "<<test_class->ampl_total[0][3]->GetEntries()<<endl;
    cout<<"Entry : "<<test_class->ampl_single[0][3][5]->GetTitle()<<endl;
}