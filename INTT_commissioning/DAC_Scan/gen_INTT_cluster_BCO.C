#include "DAC_Scan_ladder.h"
#include "InttClustering.h"

// todo : the number of number is given by the adc_setting_run !!!
// todo : also the range of the hist.
// todo : the adc follows the following convention
// todo : 1. the increment has to be 4
// todo : remember to check the "adc_conv"
// vector<vector<int>> adc_setting_run = {	
//     // {8  , 12 , 16 , 20 , 24 , 28 , 32 , 36 },
//     // {28 , 32 , 36 , 40 , 44 , 48 , 52 , 56 },
//     {48 , 52 , 56 , 60 , 64 , 68 , 72 , 76 }, // note : 3
//     {68 , 72 , 76 , 80 , 84 , 88 , 92 , 96 }, // note : 4
//     {88 , 92 , 96 , 100, 104, 108, 112, 116}, // note : 5
//     {108, 112, 116, 120, 124, 128, 132, 136}, // note : 6
//     {128, 132, 136, 140, 144, 148, 152, 156}, // note : 7
//     // {148, 152, 156, 160, 164, 168, 172, 176},
//     // {168, 172, 176, 180, 184, 188, 192, 196},
//     // {188, 192, 196, 200, 204, 208, 212, 216}
// };

vector<vector<int>> adc_setting_run = {	
    {15, 30, 60, 90, 120, 150, 180, 210, 240}
    // {8  , 12 , 16 , 20 , 24 , 28 , 32 , 36 },
    // {28 , 32 , 36 , 40 , 44 , 48 , 52 , 56 },
    // {48 , 52 , 56 , 60 , 64 , 68 , 72 , 76 }, // note : 3
    // {68 , 72 , 76 , 80 , 84 , 88 , 92 , 96 }, // note : 4
    // {88 , 92 , 96 , 100, 104, 108, 112, 116}, // note : 5
    // {108, 112, 116, 120, 124, 128, 132, 136}, // note : 6
    // {128, 132, 136, 140, 144, 148, 152, 156}, // note : 7
    // {148, 152, 156, 160, 164, 168, 172, 176},
    // {168, 172, 176, 180, 184, 188, 192, 196},
    // {188, 192, 196, 200, 204, 208, 212, 216}
};

struct full_hit_info {
    int FC;
    int chip_id;
    int chan_id;
    int adc;
};

struct ladder_info {
    int FC;
    TString Port;
    int ROC;
    int Direction; // note : 0 : south, 1 : north 
};


void gen_INTT_cluster_BCO(string sub_folder_string, string file_name, int DAC_run_ID, int Nhit_cut, int geo_mode_id, int run_Nevent)
{

    // string mother_folder_directory = "/home/phnxrc/INTT/cwshih/DACscan_data/zero_magnet_Takashi_used";
    // string file_name = "beam_inttall-00020869-0000_event_base_ana";

    string mother_folder_directory = "/home/phnxrc/INTT/cwshih/DACscan_data/" + sub_folder_string;
    // int DAC_run_ID = 0;
    // int Nhit_cut = 1500;
    // int run_Nevent = 36000;
    double peek = 3.32405;

    // vector<int> adc_config = {15, 30, 60, 90, 120, 150, 180, 210, 240};
    vector<vector<int>> adc_setting_run = {	
        {25, 30, 60, 90, 120, 150, 180, 210, 240},
        {8  , 12 , 16 , 20 , 24 , 28 , 32 , 36 },
        {28 , 32 , 36 , 40 , 44 , 48 , 52 , 56 }, // note : 2
        {48 , 52 , 56 , 60 , 64 , 68 , 72 , 76 }, // note : 3
        {68 , 72 , 76 , 80 , 84 , 88 , 92 , 96 }, // note : 4
        {88 , 92 , 96 , 100, 104, 108, 112, 116}, // note : 5
        {108, 112, 116, 120, 124, 128, 132, 136}, // note : 6
        {128, 132, 136, 140, 144, 148, 152, 156}, // note : 7
        {148, 152, 156, 160, 164, 168, 172, 176},
        {168, 172, 176, 180, 184, 188, 192, 196},
        {188, 192, 196, 200, 204, 208, 212, 216}
    };

    int N_total_ladder = 14;
    int N_server       = 8;
    string conversion_mode = (geo_mode_id == 0) ? "ideal" : "survey_1_XYAlpha_Peek";
    

    vector<int> adc_convert(8,0);
    for (int i=0; i<8; i++) {adc_convert[i] = (adc_setting_run[DAC_run_ID][i]+adc_setting_run[DAC_run_ID][i+1])/2.;} // todo : change the run setting here

    vector<vector<hit_info>> single_event_hit_ladder_vec(N_total_ladder); // note : [ladder]
    vector<vector<vector<hit_info>>> single_event_hit_vec(N_server, single_event_hit_ladder_vec); // note : [server][ladder] 
    // vector<vector<clu_info>> clu_ladder_vec(N_total_ladder); // note : [ladder]
    vector<clu_info> clu_vec; 

    int N_hits;
    int N_cluster_inner = 0;
    int N_cluster_outer = 0;
    Long64_t bco_full_out;
    vector<int> column_out_vec; column_out_vec.clear();
    vector<double> avg_chan_out_vec; avg_chan_out_vec.clear();
    vector<int> sum_adc_out_vec; sum_adc_out_vec.clear();
    vector<int> sum_adc_conv_out_vec; sum_adc_conv_out_vec.clear();
    vector<int> size_out_vec; size_out_vec.clear();
    vector<double> x_out_vec; x_out_vec.clear();
    vector<double> y_out_vec; y_out_vec.clear();
    vector<double> z_out_vec; z_out_vec.clear();
    vector<int> layer_out_vec; layer_out_vec.clear();
    vector<double> phi_out_vec; phi_out_vec.clear();

    

    TFile * file_in = TFile::Open(Form("%s/%s.root",mother_folder_directory.c_str(),file_name.c_str()));
    TTree * tree = (TTree *)file_in->Get("tree");
    long long N_event = tree -> GetEntries();
    cout<<Form("N_event in file %s : %lli",file_name.c_str(), N_event)<<endl;

    int fNhits;
    int pid[100000];
    int module[100000];
    int chip_id[100000];
    int chan_id[100000];
    int adc[100000];
    int bco[100000];
    Long64_t bco_full[100000];

    
    tree -> SetBranchStatus("*",0);
    tree -> SetBranchStatus("fNhits",1);
    tree -> SetBranchStatus("fhitArray.pid",1);
    tree -> SetBranchStatus("fhitArray.module",1);
    tree -> SetBranchStatus("fhitArray.chip_id",1);
    tree -> SetBranchStatus("fhitArray.chan_id",1);
    tree -> SetBranchStatus("fhitArray.adc",1);
    tree -> SetBranchStatus("fhitArray.bco",1);
    tree -> SetBranchStatus("fhitArray.bco_full",1);

    tree -> SetBranchAddress("fNhits",&fNhits);
    tree -> GetEntry(0); // note : actually I really don't know why this line is necessary.
    tree -> SetBranchAddress("fhitArray.pid",&pid[0]);
    tree -> SetBranchAddress("fhitArray.module",&module[0]);
    tree -> SetBranchAddress("fhitArray.chip_id",&chip_id[0]);
    tree -> SetBranchAddress("fhitArray.chan_id",&chan_id[0]);
    tree -> SetBranchAddress("fhitArray.adc",&adc[0]);
    tree -> SetBranchAddress("fhitArray.bco",&bco[0]);
    tree -> SetBranchAddress("fhitArray.bco_full",&bco_full[0]);

    string out_file_name = Form("%s_cluster_%s",file_name.c_str(),conversion_mode.c_str());
    if (conversion_mode == "survey_1_XYAlpha_Peek") out_file_name += Form("_%.2fmm",peek);
    out_file_name += Form("_excludeR%i",Nhit_cut);
    out_file_name += Form("_%1.fkEvent",run_Nevent/1000.);

    TFile * out_file = new TFile(Form("%s/%s.root",mother_folder_directory.c_str(),out_file_name.c_str()),"RECREATE");

    TTree * tree_out =  new TTree ("tree_clu", "clustering info.");
    tree_out -> Branch("nhits",&N_hits);
    tree_out -> Branch("nclu_inner",&N_cluster_inner);
    tree_out -> Branch("nclu_outer",&N_cluster_outer);
    tree_out -> Branch("bco_full",&bco_full_out);

    tree_out -> Branch("column", &column_out_vec);
    tree_out -> Branch("avg_chan", &avg_chan_out_vec);
    tree_out -> Branch("sum_adc", &sum_adc_out_vec);
    tree_out -> Branch("sum_adc_conv", &sum_adc_conv_out_vec);
    tree_out -> Branch("size", &size_out_vec);
    tree_out -> Branch("x", &x_out_vec);
    tree_out -> Branch("y", &y_out_vec);
    tree_out -> Branch("z", &z_out_vec);
    tree_out -> Branch("layer", &layer_out_vec);
    tree_out -> Branch("phi", &phi_out_vec);


    for (int i = 0; i < run_Nevent; i++ ) // note : event
    {
        tree -> GetEntry(i);
        if (i % 10 == 0){cout<<"clustering : "<<i<<endl;}
        // cout<<"=================================== =================================== ==================================="<<endl;
        // cout<<"event ID : "<<i<<endl;

        N_hits = fNhits;
        bco_full_out = bco_full[0];

        // todo : the fNhits cut is here 
        if (fNhits > Nhit_cut ) {
            clu_vec.clear();
            single_event_hit_vec.clear(); single_event_hit_vec = vector<vector<vector<hit_info>>>(N_server, single_event_hit_ladder_vec);
            

            N_cluster_inner = -1;
            N_cluster_outer = -1;
            tree_out -> Fill();

            N_cluster_inner = 0;
            N_cluster_outer = 0;
            column_out_vec.clear();
            avg_chan_out_vec.clear();
            sum_adc_out_vec.clear();
            sum_adc_conv_out_vec.clear();
            size_out_vec.clear();
            x_out_vec.clear();
            y_out_vec.clear();
            z_out_vec.clear();
            layer_out_vec.clear();
            phi_out_vec.clear();

            continue;
        }

        for (int i1 = 0; i1 < fNhits; i1++) // note : N hit in one event 
        {
            // note : pid 3001 to 3008
            if (pid[i1] > 3000 && pid[i1] < 3009 && module[i1] > -1 && module[i1] < 14 && chip_id[i1] > 0 && chip_id[i1] < 27 && chan_id[i1] > -1 && chan_id[i1] < 128 && adc[i1] > -1 && adc[i1] < 8) 
            {
                // if ( (pid[i1] - 3001) == 0 && module[i1] == 6 ) continue;
                // if ( (pid[i1] - 3001) == 0 && module[i1] == 1 ) continue;
                // if ( (pid[i1] - 3001) == 0 && module[i1] == 12 ) continue;
                
                // if ( (pid[i1] - 3001) == 1 && module[i1] == 7 ) continue;
                // if ( (pid[i1] - 3001) == 1 && module[i1] == 2 ) continue;

                // if ( (pid[i1] - 3001) == 2 && module[i1] == 1 ) continue;

                // if ( (pid[i1] - 3001) == 4 && module[i1] == 13 ) continue;

                // if ( (pid[i1] - 3001) == 5 && module[i1] == 10 ) continue;

                // if ( (pid[i1] - 3001) == 6 && module[i1] == 1 ) continue;
                // if ( (pid[i1] - 3001) == 6 && module[i1] == 12 ) continue;

                // if ( (pid[i1] - 3001) == 7 && module[i1] == 0 ) continue;
                
                if ( pid[i1] == 3002 && module[i1] == 2 && chip_id[i1] == 15 && chan_id[i1] == 0 ) continue;
                
                if ( pid[i1] == 3003 && module[i1] == 1 && chip_id[i1] == 8 && chan_id[i1] == 0 ) continue;
                
                if ( pid[i1] == 3007 && module[i1] == 1 && chan_id[i1] == 0 ) continue;

                if ( pid[i1] == 3007 && module[i1] == 12 && chip_id[i1] == 19 && chan_id[i1] == 0 ) continue;

                
                // int bco_diff = ( (bco_full[i1]&0x7F - bco[i1]) < 0 ) ? (bco_full[i1]&0x7F - bco[i1]) + 128 : (bco_full[i1]&0x7F - bco[i1]);
                // cout<<"test : "<<bco_diff<<endl;
                // if (bco_diff < 58 || bco_diff > 63) continue;

                single_event_hit_vec[ (pid[i1] - 1) % 3000 ][ module[i1] ].push_back({chip_id[i1], chan_id[i1], adc[i1], adc_convert[adc[i1]]});
            }
                
        } // note : end of the hit in one event

        for (int i1 = 0; i1 < N_server; i1++) // note : server, one event 
        {
            for (int i2 = 0; i2 < N_total_ladder; i2++) // note : ladder, one event 
            {
                
                if ( single_event_hit_vec[i1][i2].size() > 0 )
                {
                    clu_vec = InttClustering::clustering(Form("intt%i",i1), i2, single_event_hit_vec[i1][i2],conversion_mode,peek);
                    // cout<<"-------> eID : "<<i<<" Nhit : "<<fNhits<<" serverID : "<<i1<<" ladderID : "<<i2<<" clu_vec size : "<<clu_vec.size()<<" inner_nclu : "<<N_cluster_inner<<" outer_nclu : "<<N_cluster_outer<<endl;

                    for (int clu_i = 0; clu_i < clu_vec.size(); clu_i++)
                    {
                        column_out_vec.push_back( clu_vec[clu_i].column );
                        avg_chan_out_vec.push_back( clu_vec[clu_i].avg_chan );
                        sum_adc_out_vec.push_back( clu_vec[clu_i].sum_adc );
                        sum_adc_conv_out_vec.push_back( clu_vec[clu_i].sum_adc_conv );
                        size_out_vec.push_back( clu_vec[clu_i].size );
                        x_out_vec.push_back( clu_vec[clu_i].x );
                        y_out_vec.push_back( clu_vec[clu_i].y );
                        z_out_vec.push_back( clu_vec[clu_i].z );
                        layer_out_vec.push_back( clu_vec[clu_i].layer );
                        phi_out_vec.push_back( clu_vec[clu_i].phi );

                        if (clu_vec[clu_i].layer == 0) {N_cluster_inner += 1;}
                        else if (clu_vec[clu_i].layer == 1) {N_cluster_outer += 1;}
                    }
                }                

                clu_vec.clear();
            } // note : end of ladder
        } // note : end of server

        // cout<<"test : "<<i<<" Nhit : "<<fNhits<<" nclu_inner : "<<N_cluster_inner<<" nclu_outer : "<<N_cluster_outer<<endl; 

        tree_out -> Fill();

        N_cluster_inner = 0;
        N_cluster_outer = 0;
        column_out_vec.clear();
        avg_chan_out_vec.clear();
        sum_adc_out_vec.clear();
        sum_adc_conv_out_vec.clear();
        size_out_vec.clear();
        x_out_vec.clear();
        y_out_vec.clear();
        z_out_vec.clear();
        layer_out_vec.clear();
        phi_out_vec.clear();

        clu_vec.clear();
        single_event_hit_vec.clear(); single_event_hit_vec = vector<vector<vector<hit_info>>>(N_server, single_event_hit_ladder_vec);
    }// note : end of event


    // file_in -> Close();

    
    tree_out->SetDirectory(out_file);
    tree_out -> Write("", TObject::kOverwrite);

    cout<<"conversion done, file : "<<out_file_name<<endl;
	out_file -> Close();

}