#include "DAC_Scan_ladder.h"
//#include "InttConversion.h"
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
    // {15, 30, 50, 70, 90, 110, 130, 150,170}
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

void temp_bkg(TCanvas * c1)
{
    c1 -> cd();

    int N_ladder[4] = {12, 12, 16, 16};
    string ladder_index_string[16] = {"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15"};

    vector<double> x_vec; x_vec.clear();
    vector<double> y_vec; y_vec.clear();

    vector<double> x_vec_2; x_vec_2.clear();
    vector<double> y_vec_2; y_vec_2.clear();

    for (int server_i = 0; server_i < 4; server_i++)
    {
        for (int FC_i = 0; FC_i < 14; FC_i++)
        {
            for (int chan_i = 0; chan_i < 128; chan_i++)
            {

                x_vec.push_back(InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,14,chan_i).x);
                y_vec.push_back(InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,14,chan_i).y);

                x_vec_2.push_back(InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,1,chan_i).x);
                y_vec_2.push_back(InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,1,chan_i).y);
                // cout<<"("<<InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,14,chan_i).first<<","<<InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,14,chan_i).second<<"),";

                // cout<<"("<<Get_XY_all(Form("%s%s",layer_text[i].c_str(),ladder_index_string[i1].c_str()),used_chip,127).first<<","<<Get_XY_all(Form("%s%s",layer_text[i].c_str(),ladder_index_string[i1].c_str()),used_chip,127).second<<")";
            }
        }
    }

    TGraph * aaa = new TGraph(x_vec.size(),&x_vec[0],&y_vec[0]);
    aaa -> SetMarkerStyle(20);
    aaa -> SetMarkerSize(0.1);

    aaa -> GetXaxis() -> SetTitle("X axis (mm)");
    aaa -> GetYaxis() -> SetTitle("Y axis (mm)");

    TGraph * bbb = new TGraph(x_vec_2.size(),&x_vec_2[0],&y_vec_2[0]);
    bbb -> SetMarkerStyle(20);
    bbb -> SetMarkerSize(0.1);
    bbb -> SetMarkerColor(4);

    vector<double> origin_x_vec={0,0};
    vector<double> origin_y_vec={0,5};
    TGraph * origin_g = new TGraph(origin_x_vec.size(),&origin_x_vec[0],&origin_y_vec[0]);
    origin_g -> SetMarkerStyle(20);
    origin_g -> SetMarkerSize(0.5);
    origin_g -> SetMarkerColor(4);

    
    
    aaa -> Draw("ap");
    bbb -> Draw("p same");
    origin_g -> Draw("psame");
}

// note : use "ls *.root > file_list.txt" to create the list of the file in the folder, full directory in the file_list.txt
// note : set_folder_name = "folder_xxxx"
// note : server_name = "inttx"
void check_event_xy_new()
{
    TCanvas * c1 = new TCanvas("","",1000,800);

    string mother_folder_directory = "/home/phnxrc/INTT/cwshih/DACscan_data/zero_magnet_Takashi_used";
    // string file_name = "beam_inttall-00020869-0000_event_base_ana_cluster_survey_rotation_excludeR500";
    string file_name = "beam_inttall-00020869-0000_event_base_ana_cluster_100K_excludeR500";

    TFile * file_in = new TFile(Form("%s/%s.root",mother_folder_directory.c_str(),file_name.c_str()),"read");
    TTree * tree = (TTree *)file_in->Get("tree_clu");
    
    long long N_event = tree -> GetEntries();
    cout<<Form("N_event in file %s : %lli",file_name.c_str(), N_event)<<endl;

    int N_hits;
    int N_cluster_inner;
    int N_cluster_outer;
    vector<int>* column_vec = new vector<int>();
    vector<double>* avg_chan_vec = new vector<double>();
    vector<int>* sum_adc_vec = new vector<int>();
    vector<int>* sum_adc_conv_vec = new vector<int>();
    vector<int>* size_vec = new vector<int>();
    vector<double>* x_vec = new vector<double>();
    vector<double>* y_vec = new vector<double>();
    vector<double>* z_vec = new vector<double>();
    vector<int>* layer_vec = new vector<int>();
    vector<double>* phi_vec = new vector<double>();

    tree -> SetBranchAddress("nhits",&N_hits);
    tree -> SetBranchAddress("nclu_inner",&N_cluster_inner);
    tree -> SetBranchAddress("nclu_outer",&N_cluster_outer);

    tree -> SetBranchAddress("column", &column_vec);
    tree -> SetBranchAddress("avg_chan", &avg_chan_vec);
    tree -> SetBranchAddress("sum_adc", &sum_adc_vec);
    tree -> SetBranchAddress("sum_adc_conv", &sum_adc_conv_vec);
    tree -> SetBranchAddress("size", &size_vec);
    tree -> SetBranchAddress("x", &x_vec);
    tree -> SetBranchAddress("y", &y_vec);
    tree -> SetBranchAddress("z", &z_vec);
    tree -> SetBranchAddress("layer", &layer_vec);
    tree -> SetBranchAddress("phi", &phi_vec);

    vector<double> temp_x_nocolumn_vec; temp_x_nocolumn_vec.clear();
    vector<double> temp_y_nocolumn_vec; temp_y_nocolumn_vec.clear();

    int N_cluster = 0;

    for (int i = 12; i < 13; i++)
    {
        tree -> GetEntry(i);
        unsigned int length = column_vec -> size();

        for (int hit_i = 0; hit_i < length; hit_i++)
        {
            if ( z_vec -> at(hit_i) < 0)
            {   
                N_cluster += 1;
                // if (size_vec -> at(hit_i) > 4) continue;
                temp_x_nocolumn_vec.push_back(x_vec -> at(hit_i));
                temp_y_nocolumn_vec.push_back(y_vec -> at(hit_i));
                cout<<"hit pos "<<x_vec -> at(hit_i)<<" "<<y_vec -> at(hit_i)<<" "<<z_vec -> at(hit_i)<<" adc_sum "<<sum_adc_conv_vec -> at(hit_i)<<" size : "<<size_vec -> at(hit_i)<<endl;
            }
        }
    }

    cout<<"N_cluster : "<<N_cluster<<endl;


    temp_bkg(c1);

    TGraph * event_display = new TGraph(temp_x_nocolumn_vec.size(),&temp_x_nocolumn_vec[0],&temp_y_nocolumn_vec[0]);
    event_display -> SetMarkerStyle(20);
    event_display -> SetMarkerColor(2);
    event_display -> SetMarkerSize(0.7);
    event_display -> Draw("p same");

}