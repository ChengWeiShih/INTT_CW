void open_root_file_test()
{
    TFile * file_in = new TFile("/home/phnxrc/INTT/cwshih/DACscan_data/test_intt4/beam_intt4-00021535-0000_event_base.root","read");
    TTree * tree = (TTree *)file_in->Get("tree");

    long long N_event = tree -> GetEntries();
    cout<<N_event<<endl;

    int fNhits;
    int chan_id[100000];
    // int* chan_id = NULL;
    tree -> SetBranchAddress("fNhits"      ,&fNhits);
    tree -> GetEntry(0);
    
    tree -> SetBranchAddress("fhitArray.chan_id"      ,&chan_id[0]);
    
    cout<<"hi"<<endl;

    for (int i = 1; i < 3; i++)
    {
        tree -> GetEntry(i);
        cout<<"test : "<<fNhits<<endl;
        // for(int i1 = 0; i1 < fNhits; i1++)
        // {
        //     cout<<i<<" "<<chan_id[i1]<<endl;
        // }
        // cout<<" "<<endl;
    }

    file_in -> Close();
}