vector<long long> MVTX_BCO (string mvtx_directory)
{
    long long bco_input;
    vector<long long> out_vec; out_vec.clear();

    ifstream in_mvtx(Form("%s",mvtx_directory.c_str()));
    printf("we ar working on : %s\n", mvtx_directory.c_str());

    while (1)
	{
		in_mvtx >> bco_input;

		if (!in_mvtx.good())
		{
			break;
			cout << "shit" << endl;
		}
		out_vec.push_back(bco_input);
	}

    return out_vec;
}

vector<long long> INTT_BCO (string intt_directory)
{
    vector<long long> out_vec; out_vec.clear();

    TFile * file_in = TFile::Open(Form("%s",intt_directory.c_str()));
    TTree * tree = (TTree *)file_in->Get("tree_clu");
    long long N_event = tree -> GetEntries();
    cout<<Form("N_event in file %s : %lli",intt_directory.c_str(), N_event)<<endl;
    
    long long bco_full;
    tree -> SetBranchAddress("bco_full",&bco_full);

    for (int i = 0; i < N_event; i++)
    {
        tree -> GetEntry(i);
        out_vec.push_back(bco_full);
    }

    return out_vec;
}

void BCO_CrossCheck()
{    
    string mother_folder_directory = "/sphenix/user/ChengWei/INTT/INTT_commissioning/cosmic/25952/folder_cosmics_inttall-00025952-0000_event_base_ana_cluster_survey_1_XYAlpha_Peek_3.32mm_excludeR500_1000kEvent_10HotCut_cosmic";
    string file_name = "INTT_eventdisplay_cluster_fit.root";

    vector<long long> INTT_BCO_vec = INTT_BCO(Form("%s/%s",mother_folder_directory.c_str(),file_name.c_str()));
    vector<long long> MVTX_BCO_vec = MVTX_BCO("/sphenix/user/ChengWei/INTT/INTT_commissioning/cosmic/25952/MVTXBCO_Run25952.txt");

    // for (int i1 = 0; i1 < MVTX_BCO_vec.size(); i1++)
    // {
    //     cout<<MVTX_BCO_vec[i1]<<endl;
    // }

    int match_count = 0;

    for (int i = 0; i < INTT_BCO_vec.size(); i++)
    {
        for (int i1 = 0; i1 < MVTX_BCO_vec.size(); i1++)
        {
            if (INTT_BCO_vec[i] + 1 == MVTX_BCO_vec[i1])
            {
                // cout<<" ------------matched !------------- "<<mvtx_bco[i1] - 1<<endl;
                cout<<INTT_BCO_vec[i]<<endl;
                match_count += 1;
                break;
            }
        }
    }

    cout<<"INTT + MVTX BCO match : "<<match_count<<endl;

}