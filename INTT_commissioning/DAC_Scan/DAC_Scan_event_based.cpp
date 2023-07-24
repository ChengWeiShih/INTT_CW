#include "DAC_Scan_ladder.h"
#include "InttConversion.h"

TString mother_folder_directory = "/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/intt4";
// todo : the number of number is given by the adc_setting_run !!!
// todo : also the range of the hist.
vector<vector<int>> adc_setting_run = {	
    // {8  , 12 , 16 , 20 , 24 , 28 , 32 , 36 },
    {28 , 32 , 36 , 40 , 44 , 48 , 52 , 56 },
    {48 , 52 , 56 , 60 , 64 , 68 , 72 , 76 }, // note : 3
    {68 , 72 , 76 , 80 , 84 , 88 , 92 , 96 }, // note : 4
    {88 , 92 , 96 , 100, 104, 108, 112, 116}, // note : 5
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

struct hit_info {
    int chip_id;
    int chan_id;
    int adc;
};

struct clu_info {
    int column;
    int chip_id;
    double avg_chan;
    int sum_adc;
    int size;
};

struct ladder_info {
    int FC;
    TString Port;
    int ROC;
    int Direction; // note : 0 : south, 1 : north 
};

// struct full_ladder_info {
//     int FC;
//     TString Port;
//     int ROC;
//     int Direction; // note : 0 : south, 1 : north 
//     TString Ladder;
// };

// note : assumption, in one event, single channel can only be fired once. 
vector<clu_info> clustering(vector<hit_info> single_event)
{
    int hit_seat[13][256]; memset(hit_seat, -1, sizeof(hit_seat)); // note : one event, one half-ladder
    vector<hit_info>hit_vec; hit_vec.clear(); // note : chip_id is not used

    clu_info clu_unit;
    vector<clu_info> output_vec; output_vec.clear();

    double num_hit = 0;
    double chan_truck = 0;
	double chan_truck_denominator = 0;
	int standard_channel;
	int distance_meter = 0;
    int sum_adc = 0;
    
    for (int i = 0; i < single_event.size(); i++) // note : number of hits in this event, for this half-ladder
    {
        // note : U1 ~ U13, 0 ~ 127. // U14 ~ U26, -> 128 ~ 255
        int chip_conv = ( single_event[i].chip_id > 13 ) ? single_event[i].chip_id - 13  : single_event[i].chip_id;
        int chan_conv = ( single_event[i].chip_id > 13 ) ? 255 - single_event[i].chan_id : single_event[i].chan_id;

        if (hit_seat[chip_conv - 1][chan_conv]  == -1)
        {
            hit_seat[chip_conv - 1][chan_conv] = single_event[i].adc;
        }
        else 
        {
            cout<<"chip_id : "<<chip_conv<<" chan_id : "<<chan_conv<<" fired more than once ! N_hit in this HL : "<<single_event.size()<<" original : "<<single_event[i].chip_id<<" "<<single_event[i].chan_id<<endl;
            hit_seat[chip_conv - 1][chan_conv] = single_event[i].adc; // note : take the latest one.   
        }
    }

    for (int col = 0; col < 13; col++) // note : column
    {
        for (int ch = 0; ch < 256; ch++) // note : channel 
        {
            if (hit_seat[col][ch] != -1)
            {
                hit_info hit_unit;
                hit_unit.chan_id = ch;
                hit_unit.adc = hit_seat[col][ch];
                
                hit_vec.push_back(hit_unit);
            }

        }

        // note : single column
        for (int i = 0; i < hit_vec.size(); i ++)
        {
            standard_channel        = hit_vec[i].chan_id;
            chan_truck             += hit_vec[i].chan_id;
            chan_truck_denominator += 1;
            num_hit                += 1; 
            sum_adc                += hit_vec[i].adc;

            if (hit_vec.size() - i == 1) // note : for the single hit
            {
                clu_unit.column   = col + 1;
                clu_unit.avg_chan = chan_truck / chan_truck_denominator;
                clu_unit.sum_adc  = sum_adc;
                clu_unit.size     = num_hit;

                output_vec.push_back(clu_unit);
            }

            for (int i1 = 0; i1 < hit_vec.size() - (i + 1); i1++) // note : this for loop will be skipped if it's single hit
            {
                if ( fabs(standard_channel - hit_vec[i + i1 + 1].chan_id) == i1 + 1 )
                {
                    chan_truck             += hit_vec[i + i1 + 1].chan_id;
                    chan_truck_denominator += 1;
                    num_hit                += 1; 
                    sum_adc                += hit_vec[i + i1 + 1].adc;

                    distance_meter += 1;

                    if ((hit_vec.size() - (i + 1) - i1) == 1) // note : for the last hit
					{
						clu_unit.column   = col + 1;
                        clu_unit.avg_chan = chan_truck / chan_truck_denominator;
                        clu_unit.sum_adc  = sum_adc;
                        clu_unit.size     = num_hit;

                        output_vec.push_back(clu_unit);
					}
                }
                else 
                {
                    clu_unit.column   = col + 1;
                    clu_unit.avg_chan = chan_truck / chan_truck_denominator;
                    clu_unit.sum_adc  = sum_adc;
                    clu_unit.size     = num_hit;

                    output_vec.push_back(clu_unit);

                    break;
                }
            }

            i += distance_meter;
			distance_meter = 0;
			chan_truck = 0;
			chan_truck_denominator = 0;
            num_hit = 0;
            sum_adc = 0;
        }

        // note : start of cleaning 
        hit_vec.clear();

    } // note : chip loop

    return output_vec;

}

// todo : one very important key: the order has to be run0 -> runN
// todo : FULL directory of the root files
vector<TString> read_file_list (TString set_folder_name, TString server_name)
{

    // TString file_list_directory = mother_folder_directory + "/data_analysis/" + set_folder_name + "/" + server_name + "/" + "file_list.txt";
    TString file_list_directory = mother_folder_directory + "/file_list.txt";
    vector<TString> file_list_vec; file_list_vec.clear();

    unsigned long len_unsigned_long = -1;

    fstream list_file;
    list_file.open(Form("%s", file_list_directory.Data()), ios::in);
    if (list_file.is_open()) 
    {
        string data;
        while (getline(list_file, data)) 
        {
            if ( TString(data[0]) == "#"  )  { continue; }
            if ( TString(data[0]) == " "  )  { continue; }
            if ( TString(data[0]) == ""   )  { continue; }
            if ( TString(data[0]) == "  " )  { continue; }
            if (TString(data).Length() < 2)  { continue; }
            if ( data.find(".root")   == len_unsigned_long)     { continue; }

            TString data_TString(data);

            file_list_vec.push_back(data_TString);
        }
        list_file.close();
    }

    
    return file_list_vec;
}




// note : use "ls *.root > file_list.txt" to create the list of the file in the folder, full directory in the file_list.txt
// note : set_folder_name = "folder_xxxx"
// note : server_name = "inttx"
void DAC_Scan_event_based()
{

    TString set_folder_name = "testing"; 
    TString server_name = "intt4";
    vector<int> FC_id={11};
    double temp_multiplicity_cut_L[5] = {300,300,300,300};
    double temp_multiplicity_cut_R[5] = {800,800,800,800};

    vector<TString> file_name_vec = read_file_list(set_folder_name, server_name);

    // TString set_name = set_folder_name; set_name = set_name.ReplaceAll("folder_","");

    TString plot_folder_dire = Form("%s/plots/results/%i_%s", mother_folder_directory.Data(), FC_id[0], InttConversion::serverFC_toinfo_map[Form("%s_%i",server_name.Data(),FC_id[0])].Ladder.Data());
    system( Form("mkdir %s",plot_folder_dire.Data()) );

    vector< LadderDAC * > ladder(FC_id.size(),NULL);
    for (int i = 0; i < FC_id.size(); i++)
    {
        full_ladder_info single_HF_info = InttConversion::serverFC_toinfo_map[Form("%s_%i",server_name.Data(),FC_id[i])];
        ladder[i] = new LadderDAC("chip", single_HF_info.Ladder, single_HF_info.ROC, single_HF_info.Port, FC_id[i], adc_setting_run, 1);
    }

    int fNhits;
    int module[10000];
    int chip_id[10000];
    int chan_id[10000];
    int adc[10000];

    // vector<vector<hit_info>> event_hit_vec;                              // note : event_hit_vec[event][hit]
  

    vector<hit_info> single_event_hit_vec_FC11;  single_event_hit_vec_FC11.clear(); // note : single_event_hit_vec[hit]
    vector<clu_info> clu_vec_FC11; clu_vec_FC11.clear();

    vector<hit_info> single_event_hit_vec_FC7;  single_event_hit_vec_FC7.clear(); // note : single_event_hit_vec[hit]
    vector<clu_info> clu_vec_FC7; clu_vec_FC7.clear();

    vector<hit_info> single_event_hit_vec_FC8;  single_event_hit_vec_FC8.clear(); // note : single_event_hit_vec[hit]
    vector<clu_info> clu_vec_FC8; clu_vec_FC8.clear();

    int N_clu_column_FC11[13]; memset(N_clu_column_FC11, 0, sizeof(N_clu_column_FC11));
    int N_clu_column_FC7[13]; memset(N_clu_column_FC7, 0, sizeof(N_clu_column_FC7));
    int N_clu_column_FC8[13]; memset(N_clu_column_FC8, 0, sizeof(N_clu_column_FC8));

    for (int fid = 0; fid < file_name_vec.size(); fid++)
    {
        string file_name_short = string(file_name_vec[fid].Data()); file_name_short = std::regex_replace(file_name_short, std::regex("^.*/"), "");
        
        TFile * file_in = new TFile(file_name_vec[fid],"read");
        TTree * tree = (TTree *)file_in->Get("tree");

        long long N_event = tree -> GetEntries();

        cout<<set_folder_name<<", "<<server_name<<"_"<<FC_id[0]<<endl;
        cout<<Form("N_event in file %s : %lli",file_name_short.c_str(), N_event)<<endl;
        cout<<" "<<endl;
        
        tree -> SetBranchAddress("fNhits",&fNhits);
        tree -> GetEntry(0); // note : actually I really don't know why this line is necessary.
        tree -> SetBranchAddress("fhitArray.module",&module[0]);
        tree -> SetBranchAddress("fhitArray.chip_id",&chip_id[0]);
        tree -> SetBranchAddress("fhitArray.chan_id",&chan_id[0]);
        tree -> SetBranchAddress("fhitArray.adc",&adc[0]);


        for (int i = 0; i < N_event; i++ ) // note : event
        {
            tree -> GetEntry(i);
            
            if (temp_multiplicity_cut_L[fid] < fNhits && fNhits < temp_multiplicity_cut_R[fid] ) {

                single_event_hit_vec_FC11.clear();
                single_event_hit_vec_FC7.clear();
                single_event_hit_vec_FC8.clear();
                clu_vec_FC11.clear();
                clu_vec_FC7.clear();
                clu_vec_FC8.clear();

                memset(N_clu_column_FC11, 0, sizeof(N_clu_column_FC11));
                memset(N_clu_column_FC7, 0, sizeof(N_clu_column_FC7));
                memset(N_clu_column_FC8, 0, sizeof(N_clu_column_FC8));
                continue;
            }
            // cout<<"running the event : "<<i<<" fNhits : "<<fNhits<<endl;
            for (int i1 = 0 ; i1 < fNhits; i1++)
            {
                if (module[i1] == 11) single_event_hit_vec_FC11.push_back({chip_id[i1],chan_id[i1],adc[i1]});
                else if (module[i1] == 7) single_event_hit_vec_FC7.push_back({chip_id[i1],chan_id[i1],adc[i1]});
                else if (module[i1] == 8) single_event_hit_vec_FC8.push_back({chip_id[i1],chan_id[i1],adc[i1]});
            }
            clu_vec_FC11 = clustering(single_event_hit_vec_FC11);
            clu_vec_FC7 = clustering(single_event_hit_vec_FC7);
            clu_vec_FC8 = clustering(single_event_hit_vec_FC8);

            for (int i1 = 0; i1 < clu_vec_FC11.size(); i1++) { N_clu_column_FC11[ clu_vec_FC11[i1].column - 1 ] += 1; }
            for (int i1 = 0; i1 < clu_vec_FC7.size(); i1++) { N_clu_column_FC7[ clu_vec_FC7[i1].column - 1 ] += 1; }
            for (int i1 = 0; i1 < clu_vec_FC8.size(); i1++) { N_clu_column_FC8[ clu_vec_FC8[i1].column - 1 ] += 1; }

            for (int i1 = 0; i1 < 13; i1++ ) // note : column
            {
                if ( N_clu_column_FC11[i1] == 0 ) continue;
                // if ( (N_clu_column_FC7[i1] + N_clu_column_FC8[i1]) == 0 ) continue;
                if ( (clu_vec_FC7.size() + clu_vec_FC8.size()) == 0 ) continue;

                for (int i2 = 0 ; i2 < clu_vec_FC11.size(); i2++) // note : hit
                {
                    if (clu_vec_FC11[i2].column != i1 + 1 || clu_vec_FC11[i2].size != 1) {continue;}

                    ladder[0] -> Fill( // todo : single ladder mode
                        fid, 
                        (clu_vec_FC11[i2].avg_chan > 127) ? clu_vec_FC11[i2].column + 13 : clu_vec_FC11[i2].column, 
                        (clu_vec_FC11[i2].avg_chan > 127) ? 127 - int(clu_vec_FC11[i2].avg_chan)%128 : int(clu_vec_FC11[i2].avg_chan), 
                        clu_vec_FC11[i2].sum_adc
                    );

                }
                


            }
            
            single_event_hit_vec_FC11.clear();
            single_event_hit_vec_FC7.clear();
            single_event_hit_vec_FC8.clear();
            clu_vec_FC11.clear();
            clu_vec_FC7.clear();
            clu_vec_FC8.clear();

            memset(N_clu_column_FC11, 0, sizeof(N_clu_column_FC11));
            memset(N_clu_column_FC7, 0, sizeof(N_clu_column_FC7));
            memset(N_clu_column_FC8, 0, sizeof(N_clu_column_FC8));
        }

        // int BCO_buffer;
        // long long BCO_full_buffer;

        // for (long long i = 0; i < N_event; i++)
        // {
        //     tree -> GetEntry(i);
        //     // if (module > -1 && module < 14) // note : module, Felix channel, 0 to 13
        //     if (module == FC_id[0])
        //     {
        //         BCO_buffer = bco;
        //         BCO_full_buffer = bco_full;
        //         break;
        //     }
        // }

        // for (long long i = 0; i < N_event; i++)
        // {
        //     tree -> GetEntry(i);

        //     if (chip_id > 0 && chip_id < 27 && chan_id > -1 && chan_id < 128 && module == FC_id[0]) // todo : single ladder mode
		//     {
        //         if(bco == BCO_buffer && bco_full == BCO_full_buffer)
        //         {
        //             single_event_hit_vec.push_back( {chip_id, chan_id, adc} );
        //         }
        //         else if (bco != BCO_buffer || bco_full != BCO_full_buffer)
        //         {
        //             BCO_buffer = bco;
        //             BCO_full_buffer = bco_full;

        //             clu_vec = clustering(single_event_hit_vec);
        //             for (int i1 = 0; i1 < clu_vec.size(); i1++)
        //             {
        //                 if (clu_vec[i1].size == 1)
        //                 {
        //                     // todo : the chan_id 0 and chan_id 127 are all excluded
        //                     if ( clu_vec[i1].avg_chan == 0 || clu_vec[i1].avg_chan == 127 || clu_vec[i1].avg_chan == 128 || clu_vec[i1].avg_chan == 255 ) continue;

        //                     ladder[0] -> Fill( // todo : single ladder mode
        //                         fid, 
        //                         (clu_vec[i1].avg_chan > 127) ? clu_vec[i1].column + 13 : clu_vec[i1].column, 
        //                         (clu_vec[i1].avg_chan > 127) ? 127 - int(clu_vec[i1].avg_chan)%128 : int(clu_vec[i1].avg_chan), 
        //                         clu_vec[i1].sum_adc
        //                     );
        //                 }
        //             }
        //             // event_hit_vec.push_back(single_event_hit_vec);
        //             single_event_hit_vec.clear();
        //             single_event_hit_vec.push_back( {chip_id, chan_id, adc} );
        //         }
        //     }
        // } 

        // clu_vec = clustering(single_event_hit_vec);
        // for (int i1 = 0; i1 < clu_vec.size(); i1++)
        // {
        //     if (clu_vec[i1].size == 1)
        //     {
        //         // todo : the chan_id 0 and chan_id 127 are all excluded
        //         if ( clu_vec[i1].avg_chan == 0 || clu_vec[i1].avg_chan == 127 || clu_vec[i1].avg_chan == 128 || clu_vec[i1].avg_chan == 255 ) continue;

        //         ladder[0] -> Fill( // todo : single ladder mode
        //             fid, 
        //             (clu_vec[i1].avg_chan > 127) ? clu_vec[i1].column + 13 : clu_vec[i1].column, 
        //             (clu_vec[i1].avg_chan > 127) ? 127 - int(clu_vec[i1].avg_chan)%128 : int(clu_vec[i1].avg_chan), 
        //             clu_vec[i1].sum_adc
        //         );
        //     }
        // }
        // // event_hit_vec.push_back(single_event_hit_vec);
        // single_event_hit_vec.clear();

        single_event_hit_vec_FC11.clear();
        single_event_hit_vec_FC7.clear();
        single_event_hit_vec_FC8.clear();
        clu_vec_FC11.clear();
        clu_vec_FC7.clear();
        clu_vec_FC8.clear();

        memset(N_clu_column_FC11, 0, sizeof(N_clu_column_FC11));
        memset(N_clu_column_FC7, 0, sizeof(N_clu_column_FC7));
        memset(N_clu_column_FC8, 0, sizeof(N_clu_column_FC8));

        // // note : cleaning 
        file_in -> Close();
        // event_hit_vec.clear();
        // single_event_hit_vec.clear();

    } // note : loop file

    ladder[0] -> Weight();
    ladder[0] -> Fill_final();
    ladder[0] -> Fit();

    ladder[0] -> Output_bin_plots(plot_folder_dire, set_folder_name, false);
    ladder[0] -> Output_comb_plots(plot_folder_dire, set_folder_name);
    ladder[0] -> Output_final_plots(plot_folder_dire, set_folder_name);
    ladder[0] -> Output_root(plot_folder_dire, set_folder_name);

}