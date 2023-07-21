#include "DAC_Scan_ladder.h"
#include "InttConversion.h"

TString mother_folder_directory = "/home/phnxrc/INTT/cwshih/DACscan_data/test_intt4_zero_magnet";
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

struct hit_info {
    int chip_id;
    int chan_id;
    int adc;
    int adc_conv;
};

struct hit_pro_info {
    int chip_id;
    int chan_id;

    int column_id;
    int chan_id_conv;

    int adc;
    int adc_conv;

    double x;
    double y;
    int layer;
};

struct clu_info {
    int column;
    // int chip_id;
    double avg_chan;
    int sum_adc;
    int sum_adc_conv;
    int size;

    double x;
    double y;
    int layer;
    double phi;
};

struct ladder_info {
    int FC;
    TString Port;
    int ROC;
    int Direction; // note : 0 : south, 1 : north 
};


vector<clu_info> clustering(string server_name, int FC_id, vector<hit_info> single_event)
{

    // note : [0] : nominal_chip_id, 
    // note : [1] : nominal_chan_id,
    // note : [2] : adc,
    // note : [3] : adc_conv
    vector<int> nominal_vec = {-1,-1,-1,-1};
    vector<int> hit_seat[13][256]; 
    for (int i = 0; i < 13; i++){
        for (int i1 = 0; i1 < 256; i1++){
            hit_seat[i][i1] = nominal_vec;
        }
    }

    vector<hit_pro_info>hit_pro_vec; hit_pro_vec.clear(); // note : have the position information
    vector<clu_info> output_vec; output_vec.clear();

    int num_hit = 0;
    double chan_truck = 0;
	double chan_truck_denominator = 0;
    double pos_x_truck = 0;
	double pos_x_truck_denominator = 0;
    double pos_y_truck = 0;
	double pos_y_truck_denominator = 0;
	int standard_channel;
	int distance_meter = 0;
    int sum_adc = 0;
    int sum_adc_conv;

    
    for (int i = 0; i < single_event.size(); i++) // note : number of hits in this event, for this half-ladder
    {
        // note : U1 ~ U13, 0 ~ 127. // U14 ~ U26, -> 128 ~ 255
        int chip_conv = ( single_event[i].chip_id > 13 ) ? single_event[i].chip_id - 13  : single_event[i].chip_id;
        int chan_conv = ( single_event[i].chip_id > 13 ) ? 255 - single_event[i].chan_id : single_event[i].chan_id;

        if (hit_seat[chip_conv - 1][chan_conv] == nominal_vec)
        {
            hit_seat[chip_conv - 1][chan_conv] = {single_event[i].chip_id, single_event[i].chan_id, single_event[i].adc, single_event[i].adc_conv};
        }
        else 
        {
            cout<<"chip_id : "<<chip_conv<<" chan_id : "<<chan_conv<<" fired more than once ! N_hit in this HL : "<<single_event.size()<<" nominal_chipid : "<<single_event[i].chip_id<<" chan_id : "<<single_event[i].chan_id<<endl;
            // note : take the latest one.
            hit_seat[chip_conv - 1][chan_conv] = {single_event[i].chip_id, single_event[i].chan_id, single_event[i].adc, single_event[i].adc_conv};    
        }
    }

    for (int col = 0; col < 13; col++) // note : column
    {
        for (int ch = 0; ch < 256; ch++) // note : channel
        {
            if (hit_seat[col][ch] != nominal_vec)
            {
                pos_str hit_pos = InttConversion::Get_XY_all(server_name, FC_id, hit_seat[col][ch][0], hit_seat[col][ch][1]);

                // cout<<"!!!!!!!!!!!!!!!!!! test in clustering func "<<hit_pos.x<<" "<<hit_pos.y<<endl;
                // cout<<"("<<hit_pos.x<<","<<hit_pos.y<<"),";

                hit_pro_vec.push_back({
                    hit_seat[col][ch][0], // note : original chip_id
                    hit_seat[col][ch][1], // note : original chan_id
                    col + 1,              // note : column
                    ch,                   // note : conv_ch_id
                    hit_seat[col][ch][2], // note : adc
                    hit_seat[col][ch][3], // note : conv_adc
                    
                    hit_pos.x,            // note : nominal strip pos in sPHENIX coordinate 
                    hit_pos.y,            // note : nominal strip pos in sPHENIX coordinate 
                    hit_pos.layer         // note : N layer, 0 or 1.
                });
            }
        } // note : end 256 chan check

        for (int hit_i = 0; hit_i < hit_pro_vec.size(); hit_i++)
        {
            standard_channel         = hit_pro_vec[hit_i].chan_id_conv;

            chan_truck              += hit_pro_vec[hit_i].chan_id_conv;
            chan_truck_denominator  += 1;

            pos_x_truck             += hit_pro_vec[hit_i].x * hit_pro_vec[hit_i].adc_conv;
            pos_x_truck_denominator += hit_pro_vec[hit_i].adc_conv;
            
            pos_y_truck             += hit_pro_vec[hit_i].y * hit_pro_vec[hit_i].adc_conv;
            pos_y_truck_denominator += hit_pro_vec[hit_i].adc_conv;
            
            num_hit                 += 1; 
            sum_adc                 += hit_pro_vec[hit_i].adc;
            sum_adc_conv            += hit_pro_vec[hit_i].adc_conv;

            if (hit_pro_vec.size() - hit_i == 1) // note : for the case that only one hit in this column, and the "last single hit" in the column 
            {
                output_vec.push_back({
                    col + 1,  // note : column
                    chan_truck / chan_truck_denominator,
                    sum_adc,
                    sum_adc_conv,
                    num_hit,

                    pos_x_truck / pos_x_truck_denominator,
                    pos_y_truck / pos_y_truck_denominator,
                    hit_pro_vec[0].layer, // note : layer
                    ((pos_y_truck / pos_y_truck_denominator) < 0) ? atan2((pos_y_truck / pos_y_truck_denominator),(pos_x_truck / pos_x_truck_denominator)) * (180./TMath::Pi()) + 360 : atan2((pos_y_truck / pos_y_truck_denominator),(pos_x_truck / pos_x_truck_denominator)) * (180./TMath::Pi())
                });
                // cout<<"~~~~~~ test clustering func, "<<pos_x_truck / pos_x_truck_denominator<<" "<<pos_y_truck / pos_y_truck_denominator<<endl;
                // cout<<"("<<pos_x_truck/pos_x_truck_denominator<<","<<pos_y_truck/pos_y_truck_denominator<<"),";
            }

            // note : this for loop will be skipped if it's single hit
            for (int hit_i1 = 0; hit_i1 < hit_pro_vec.size() - (hit_i + 1); hit_i1++)
            {
                if ( fabs(standard_channel - hit_pro_vec[hit_i + hit_i1 + 1].chan_id_conv) == hit_i1 + 1 )
                {
                    chan_truck              += hit_pro_vec[hit_i + hit_i1 + 1].chan_id_conv;
                    chan_truck_denominator  += 1;

                    pos_x_truck             += hit_pro_vec[hit_i + hit_i1 + 1].x * hit_pro_vec[hit_i + hit_i1 + 1].adc_conv;
                    pos_x_truck_denominator += hit_pro_vec[hit_i + hit_i1 + 1].adc_conv;
                    
                    pos_y_truck             += hit_pro_vec[hit_i + hit_i1 + 1].y * hit_pro_vec[hit_i + hit_i1 + 1].adc_conv;
                    pos_y_truck_denominator += hit_pro_vec[hit_i + hit_i1 + 1].adc_conv;
                    
                    num_hit                 += 1; 
                    sum_adc                 += hit_pro_vec[hit_i + hit_i1 + 1].adc;
                    sum_adc_conv            += hit_pro_vec[hit_i + hit_i1 + 1].adc_conv;

                    distance_meter += 1;

                    if ((hit_pro_vec.size() - (hit_i + 1) - hit_i1) == 1) // note : the last non single-hit cluster
                    {
                        output_vec.push_back({
                            col + 1,  // note : column
                            chan_truck / chan_truck_denominator,
                            sum_adc,
                            sum_adc_conv,
                            num_hit,

                            pos_x_truck / pos_x_truck_denominator,
                            pos_y_truck / pos_y_truck_denominator,
                            hit_pro_vec[0].layer, // note : layer
                            ((pos_y_truck / pos_y_truck_denominator) < 0) ? atan2((pos_y_truck / pos_y_truck_denominator),(pos_x_truck / pos_x_truck_denominator)) * (180./TMath::Pi()) + 360 : atan2((pos_y_truck / pos_y_truck_denominator),(pos_x_truck / pos_x_truck_denominator)) * (180./TMath::Pi())
                        });
                        // cout<<"~~~~~~ test clustering func, "<<pos_x_truck / pos_x_truck_denominator<<" "<<pos_y_truck / pos_y_truck_denominator<<endl;
                        // cout<<"("<<pos_x_truck/pos_x_truck_denominator<<","<<pos_y_truck/pos_y_truck_denominator<<"),";
                    }

                }
                else 
                {
                    output_vec.push_back({
                        col + 1,  // note : column
                        chan_truck / chan_truck_denominator,
                        sum_adc,
                        sum_adc_conv,
                        num_hit,

                        pos_x_truck / pos_x_truck_denominator,
                        pos_y_truck / pos_y_truck_denominator,
                        hit_pro_vec[0].layer, // note : layer
                        ((pos_y_truck / pos_y_truck_denominator) < 0) ? atan2((pos_y_truck / pos_y_truck_denominator),(pos_x_truck / pos_x_truck_denominator)) * (180./TMath::Pi()) + 360 : atan2((pos_y_truck / pos_y_truck_denominator),(pos_x_truck / pos_x_truck_denominator)) * (180./TMath::Pi())
                    });
                    // cout<<"~~~~~~ test clustering func, "<<pos_x_truck / pos_x_truck_denominator<<" "<<pos_y_truck / pos_y_truck_denominator<<endl;
                    // cout<<"("<<pos_x_truck/pos_x_truck_denominator<<","<<pos_y_truck/pos_y_truck_denominator<<"),";

                    break;
                }

            } // note : end : n_hit i1

            hit_i += distance_meter;
			distance_meter = 0;
			chan_truck = 0;
			chan_truck_denominator = 0;

            pos_x_truck = 0;             
            pos_x_truck_denominator = 0; 
            
            pos_y_truck = 0;             
            pos_y_truck_denominator = 0; 
            
            num_hit = 0;                 
            sum_adc = 0;                 
            sum_adc_conv = 0;            

        } // note : end n_hit in single column

        hit_pro_vec.clear();

    } // note : end column

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

std::vector<double> calculateDistanceAndClosestPoint(double x1, double y1, double x2, double y2, double target_x, double target_y) {
    
    if (x1 != x2)
    {
        // Calculate the slope and intercept of the line passing through (x1, y1) and (x2, y2)
        double a = (y2 - y1) / (x2 - x1);
        double b = y1 - a * x1;

        // cout<<"slope : y="<<a<<"x+"<<b<<endl;
        
        // Calculate the closest distance from (target_x, target_y) to the line y = ax + b
        double closest_distance = std::abs(a * target_x - target_y + b) / std::sqrt(a * a + 1);

        // Calculate the coordinates of the closest point (Xc, Yc) on the line y = ax + b
        double Xc = (target_x + a * target_y - a * b) / (a * a + 1);
        double Yc = a * Xc + b;

        return { closest_distance, Xc, Yc };
    }
    else 
    {
        double closest_distance = std::abs(x1 - target_x);
        double Xc = x1;
        double Yc = target_y;

        return { closest_distance, Xc, Yc };
    }
    
    
}

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
void Intt_sPH_Cluster_test()
{

    TCanvas * c1 = new TCanvas("","",800,800);

    TString set_folder_name = "testing"; 
    TString server_name = "intt4";
    vector<int> FC_id={11};
    double temp_multiplicity_cut[5] = {600,500,500,400,400};
    pair<double,double> beam_origin = {0,5};
    
    int selected_event_id = 54;
    int selected_column   = 11;

    vector<TString> file_name_vec = read_file_list(set_folder_name, server_name);

    vector<int> adc_convert(8,0);
    for (int i=0; i<8; i++) {adc_convert[i] = (adc_setting_run[0][i]+adc_setting_run[0][i+1])/2.;}

    // TString set_name = set_folder_name; set_name = set_name.ReplaceAll("folder_","");

    TString plot_folder_dire = Form("%s/plots/results/%i_%s", mother_folder_directory.Data(), FC_id[0], InttConversion::serverFC_toinfo_map[Form("%s_%i",server_name.Data(),FC_id[0])].Ladder.Data());
    system( Form("mkdir %s",plot_folder_dire.Data()) );

    // vector< LadderDAC * > ladder(FC_id.size(),NULL);
    // for (int i = 0; i < FC_id.size(); i++)
    // {
    //     full_ladder_info single_HF_info = InttConversion::serverFC_toinfo_map[Form("%s_%i",server_name.Data(),FC_id[i])];
    //     ladder[i] = new LadderDAC("chip", single_HF_info.Ladder, single_HF_info.ROC, single_HF_info.Port, FC_id[i], adc_setting_run, 1);
    // }

    int fNhits;
    int module[10000];
    int chip_id[10000];
    int chan_id[10000];
    int adc[10000];

    int N_total_ladder = 14;

    vector<vector<hit_info>> single_event_hit_vec(N_total_ladder); // note : N half_ladder 
    vector<vector<clu_info>> clu_vec(N_total_ladder);              // note : N half_ladder

    vector<vector<double>> temp_x_vec(13); // note : 13 columns 
    vector<vector<double>> temp_y_vec(13); // note : 13 columns 

    vector<vector<clu_info>> temp_sPH_inner_vec(13);
    vector<vector<clu_info>> temp_sPH_outer_vec(13);

    TH2F * angle_correlation = new TH2F("","angle_correlation",361,0,361,361,0,361);
    angle_correlation -> GetXaxis() -> SetTitle("Inner Phi (degree)");
    angle_correlation -> GetYaxis() -> SetTitle("Outer Phi (degree)");
    TH2F * angle_correlation_inner = new TH2F("","angle_correlation_inner",361,0,361,100,-50,50);
    angle_correlation_inner -> GetXaxis() -> SetTitle("Inner Phi (degree)");
    angle_correlation_inner -> GetYaxis() -> SetTitle("#Outer_{#Phi} - #Inner_{#Phi}");
    TH2F * angle_correlation_outer = new TH2F("","angle_correlation_outer",361,0,361,100,-50,50);
    angle_correlation_outer -> GetXaxis() -> SetTitle("Outer Phi (degree)");
    angle_correlation_outer -> GetYaxis() -> SetTitle("#Outer_{#Phi} - #Inner_{#Phi}");

    TH2F * DCA_point = new TH2F("","DCA_point",100,-10,10,100,-10,10);
    DCA_point -> GetXaxis() -> SetTitle("X pos (mm)");
    DCA_point -> GetYaxis() -> SetTitle("Y pos (mm)");
    TH2F * angle_DCAPointX = new TH2F("","angle_DCAPointX",100,-50,50,100,-10,10);
    angle_DCAPointX -> GetXaxis() -> SetTitle("#Outer_{#Phi} - #Inner_{#Phi}");
    angle_DCAPointX -> GetYaxis() -> SetTitle("X pos (mm)");
    TH2F * angle_DCAPointY = new TH2F("","angle_DCAPointY",100,-50,50,100,-10,10);
    angle_DCAPointY -> GetXaxis() -> SetTitle("#Outer_{#Phi} - #Inner_{#Phi}");
    angle_DCAPointY -> GetYaxis() -> SetTitle("Y pos (mm)");

    TH1F * total_cluster_size = new TH1F("","total_cluster_size",40,0,40);
    total_cluster_size -> GetXaxis() -> SetTitle("Nhit in cluster");
    total_cluster_size -> GetYaxis() -> SetTitle("Entry");
    TH2F * module_cluster_size = new TH2F("","module_cluster_size",14,0,14,40,0,40);
    module_cluster_size -> GetXaxis() -> SetTitle("Felix channel (module)");
    module_cluster_size -> GetYaxis() -> SetTitle("Cluster size");

    TH2F * DCAdistance_innerPhi = new TH2F("","DCAdistance_innerPhi",100,-1,100,361,0,361);
    DCAdistance_innerPhi -> GetXaxis() -> SetTitle("DCA distance (mm)");
    DCAdistance_innerPhi -> GetYaxis() -> SetTitle("Innter phi");
    TH1F * DCAdistance = new TH1F("","DCAdistance",100,-1,300);
    DCAdistance -> GetXaxis() -> SetTitle("DCA distance");
    DCAdistance -> GetYaxis() -> SetTitle("entry");

    // vector<TLine * > track_line; track_line.clear();

    TLatex *draw_text = new TLatex();
    draw_text -> SetNDC();
    draw_text -> SetTextSize(0.02);

    c1 -> Print(plot_folder_dire + "/" + Form("%s_eventdisplay_origin_%.2f_%.2f.pdf(",server_name.Data(),beam_origin.first,beam_origin.second));

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


        for (int i = 0; i < 5000; i++ ) // note : event
        {
            tree -> GetEntry(i);
            
            if (fNhits > 250 || fNhits < 100) {
                temp_x_vec.clear(); temp_x_vec = vector<vector<double>>(13);
                temp_y_vec.clear(); temp_y_vec = vector<vector<double>>(13);
                single_event_hit_vec.clear(); single_event_hit_vec = vector<vector<hit_info>>(N_total_ladder);
                clu_vec.clear(); clu_vec = vector<vector<clu_info>>(N_total_ladder);
                temp_sPH_inner_vec.clear(); temp_sPH_inner_vec = vector<vector<clu_info>>(13); // note : chip
                temp_sPH_outer_vec.clear(); temp_sPH_outer_vec = vector<vector<clu_info>>(13); // note : chip
                continue;
            }
            cout<<"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
            cout<<"event ID : "<<i<<" Nhits : "<<fNhits<<endl;

            for (int i1 = 0 ; i1 < fNhits; i1++)
            {
                // note : module ID 0 to 13, and chip_id 1 to 26, chan_id 0 to 127 adc 0 to 7
                if (module[i1] > -1 && module[i1] < 14 && chip_id[i1] > 0 && chip_id[i1] < 27 && chan_id[i1] > -1 && chan_id[i1] < 128 && adc[i1] > -1 && adc[i1] < 8) 
                    single_event_hit_vec[module[i1]].push_back({chip_id[i1], chan_id[i1], adc[i1], adc_convert[adc[i1]]});
            }


            for (int i1 = 0; i1 < N_total_ladder; i1++)
            {
                clu_vec[i1] = clustering(server_name.Data(), i1, single_event_hit_vec[i1]);
                
                cout<<"-------- event : "<<i<<" server : "<<i1<<" N_cluster : "<<clu_vec[i1].size()<<endl;

                for (int i2 = 0; i2 < clu_vec[i1].size(); i2++)
                {
                    

                    total_cluster_size -> Fill(clu_vec[i1][i2].size);
                    module_cluster_size -> Fill(i1,clu_vec[i1][i2].size);
                    if ( clu_vec[i1][i2].layer == 0 )
                        temp_sPH_inner_vec[ clu_vec[i1][i2].column - 1 ].push_back(clu_vec[i1][i2]);
                    else
                        temp_sPH_outer_vec[ clu_vec[i1][i2].column - 1 ].push_back(clu_vec[i1][i2]);

                    temp_x_vec[ clu_vec[i1][i2].column - 1 ].push_back(clu_vec[i1][i2].x);
                    temp_y_vec[ clu_vec[i1][i2].column - 1 ].push_back(clu_vec[i1][i2].y);

                    // if (clu_vec[i1][i2].column == 11)
                    // {
                    //     temp_x_vec.push_back(clu_vec[i1][i2].x);
                    //     temp_y_vec.push_back(clu_vec[i1][i2].y);
                    //     cout<<"--- test, pos : "<<clu_vec[i1][i2].x<<" "<<clu_vec[i1][i2].y<<endl; 
                    // }
                }
            }
            
            for (int column_i = 12; column_i < 13; column_i++)
            {
                cout<<"~~~~~~~~~~~~~~~~~ event : "<<i<<" column : "<<column_i + 1<<" Inner Nclusters : "<<temp_sPH_inner_vec[column_i].size()<<" Outer Nclusters : "<<temp_sPH_outer_vec[column_i].size()<<endl;
                
                for (int inner_i = 0; inner_i < temp_sPH_inner_vec[column_i].size(); inner_i++)
                {
                    for (int outer_i = 0; outer_i < temp_sPH_outer_vec[column_i].size(); outer_i++)
                    {
                        // track_line.push_back(
                        //     new TLine(
                        //         temp_sPH_outer_vec[column_i][outer_i].x,
                        //         temp_sPH_outer_vec[column_i][outer_i].y,

                        //         calculateDistanceAndClosestPoint(
                        //             temp_sPH_inner_vec[column_i][inner_i].x, temp_sPH_inner_vec[column_i][inner_i].y,
                        //             temp_sPH_outer_vec[column_i][outer_i].x, temp_sPH_outer_vec[column_i][outer_i].y,
                        //             beam_origin.first, beam_origin.second
                        //         )[1],
                        //         calculateDistanceAndClosestPoint(
                        //             temp_sPH_inner_vec[column_i][inner_i].x, temp_sPH_inner_vec[column_i][inner_i].y,
                        //             temp_sPH_outer_vec[column_i][outer_i].x, temp_sPH_outer_vec[column_i][outer_i].y,
                        //             beam_origin.first, beam_origin.second
                        //         )[2]

                        //     )
                        // );

                        vector<double> DCA_info_vec = calculateDistanceAndClosestPoint(
                            temp_sPH_inner_vec[column_i][inner_i].x, temp_sPH_inner_vec[column_i][inner_i].y,
                            temp_sPH_outer_vec[column_i][outer_i].x, temp_sPH_outer_vec[column_i][outer_i].y,
                            beam_origin.first, beam_origin.second
                        );

                        if (/*DCA_info_vec[1]>-3 && DCA_info_vec[1] < 3 && DCA_info_vec[2] > 2 && DCA_info_vec[2] < 7*/ DCA_info_vec[0] < 5)
                        {
                            cout<<"--------- X1 : "<<temp_sPH_inner_vec[column_i][inner_i].x <<" Y1 : "<< temp_sPH_inner_vec[column_i][inner_i].y<<" X2 : "<<temp_sPH_outer_vec[column_i][outer_i].x <<" Y2 : "<< temp_sPH_outer_vec[column_i][outer_i].y<<endl;
                            cout<<"--------- ("<<temp_sPH_inner_vec[column_i][inner_i].x <<","<< temp_sPH_inner_vec[column_i][inner_i].y<<"),("<<temp_sPH_outer_vec[column_i][outer_i].x <<","<< temp_sPH_outer_vec[column_i][outer_i].y<<")"<<endl;
                            cout<<"--------- distance : "<<DCA_info_vec[0]<<" DCAx : "<<DCA_info_vec[1]<<" DCAy : "<<DCA_info_vec[2]<<endl;
                            
                            temp_bkg(c1);

                            TLine * track_line = new TLine(
                                temp_sPH_outer_vec[column_i][outer_i].x,
                                temp_sPH_outer_vec[column_i][outer_i].y,

                                calculateDistanceAndClosestPoint(
                                    temp_sPH_inner_vec[column_i][inner_i].x, temp_sPH_inner_vec[column_i][inner_i].y,
                                    temp_sPH_outer_vec[column_i][outer_i].x, temp_sPH_outer_vec[column_i][outer_i].y,
                                    beam_origin.first, beam_origin.second
                                )[1],
                                calculateDistanceAndClosestPoint(
                                    temp_sPH_inner_vec[column_i][inner_i].x, temp_sPH_inner_vec[column_i][inner_i].y,
                                    temp_sPH_outer_vec[column_i][outer_i].x, temp_sPH_outer_vec[column_i][outer_i].y,
                                    beam_origin.first, beam_origin.second
                                )[2]
                            );

                            track_line -> SetLineColor(1);
                            track_line -> SetLineWidth(1);
                            track_line -> Draw("l same");

                            TGraph * event_display = new TGraph(temp_x_vec[column_i].size(),&temp_x_vec[column_i][0],&temp_y_vec[column_i][0]);
                            event_display -> SetMarkerStyle(20);
                            event_display -> SetMarkerColor(2);
                            event_display -> SetMarkerSize(1);
                            event_display -> Draw("p same");
                            cout<<" "<<endl;

                            draw_text -> DrawLatex(0.12, 0.91, Form("Server : %s, event : %i, column : %i",server_name.Data(),i,column_i + 1));
                            draw_text -> DrawLatex(0.15, 0.87, Form("Total event hit : %i, innter Ncluster : %i, outer Ncluster : %i",fNhits,temp_sPH_inner_vec[column_i].size(),temp_sPH_outer_vec[column_i].size()));
                            draw_text -> DrawLatex(0.15, 0.84, Form("Inner pos : (%.3f,%.3f) mm, size : %i, sum_adc_conv : %d, avg_chan : %.1f",temp_sPH_inner_vec[column_i][inner_i].x, temp_sPH_inner_vec[column_i][inner_i].y,temp_sPH_inner_vec[column_i][inner_i].size,temp_sPH_inner_vec[column_i][inner_i].sum_adc_conv,temp_sPH_inner_vec[column_i][inner_i].avg_chan));
                            draw_text -> DrawLatex(0.15, 0.81, Form("Outer pos : (%.3f,%.3f) mm, size : %i, sum_adc_conv : %d, avg_chan : %.1f" ,temp_sPH_outer_vec[column_i][outer_i].x, temp_sPH_outer_vec[column_i][outer_i].y,temp_sPH_outer_vec[column_i][outer_i].size,temp_sPH_outer_vec[column_i][outer_i].sum_adc_conv,temp_sPH_outer_vec[column_i][outer_i].avg_chan));
                            draw_text -> DrawLatex(0.15, 0.78, Form("DCA point pos : (%.3f,%.3f) mm, distance : %.3f",DCA_info_vec[1],DCA_info_vec[2],DCA_info_vec[0]));

                            draw_text -> Draw("same");

                            c1 -> Print(plot_folder_dire + "/" + Form("%s_eventdisplay_origin_%.2f_%.2f.pdf",server_name.Data(),beam_origin.first,beam_origin.second));
                            c1 -> Clear();
                            event_display -> Clear();

                        }
                        

                        angle_correlation -> Fill(temp_sPH_inner_vec[column_i][inner_i].phi, temp_sPH_outer_vec[column_i][outer_i].phi);
                        DCA_point -> Fill( DCA_info_vec[1], DCA_info_vec[2] );
                        DCAdistance_innerPhi -> Fill( DCA_info_vec[0], temp_sPH_inner_vec[column_i][inner_i].phi );
                        DCAdistance -> Fill( DCA_info_vec[0] );

                        angle_DCAPointX -> Fill(
                            temp_sPH_inner_vec[column_i][inner_i].phi - temp_sPH_outer_vec[column_i][outer_i].phi,
                            DCA_info_vec[1]
                        );

                        angle_DCAPointY -> Fill(
                            temp_sPH_inner_vec[column_i][inner_i].phi - temp_sPH_outer_vec[column_i][outer_i].phi,
                            DCA_info_vec[2]
                        );

                        angle_correlation_inner -> Fill(temp_sPH_inner_vec[column_i][inner_i].phi, temp_sPH_inner_vec[column_i][inner_i].phi - temp_sPH_outer_vec[column_i][outer_i].phi);
                        angle_correlation_outer -> Fill(temp_sPH_outer_vec[column_i][outer_i].phi, temp_sPH_inner_vec[column_i][inner_i].phi - temp_sPH_outer_vec[column_i][outer_i].phi);
                        
                        
                    }
                }
            }

            temp_x_vec.clear(); temp_x_vec = vector<vector<double>>(13);
            temp_y_vec.clear(); temp_y_vec = vector<vector<double>>(13);

            single_event_hit_vec.clear(); single_event_hit_vec = vector<vector<hit_info>>(N_total_ladder);
            clu_vec.clear(); clu_vec = vector<vector<clu_info>>(N_total_ladder);
            temp_sPH_inner_vec.clear(); temp_sPH_inner_vec = vector<vector<clu_info>>(13); // note : chip
            temp_sPH_outer_vec.clear(); temp_sPH_outer_vec = vector<vector<clu_info>>(13); // note : chip

        }

        temp_x_vec.clear(); temp_x_vec = vector<vector<double>>(13);
        temp_y_vec.clear(); temp_y_vec = vector<vector<double>>(13);
        single_event_hit_vec.clear(); single_event_hit_vec = vector<vector<hit_info>>(N_total_ladder);
        clu_vec.clear(); clu_vec = vector<vector<clu_info>>(N_total_ladder);
        temp_sPH_inner_vec.clear(); temp_sPH_inner_vec = vector<vector<clu_info>>(13); // note : chip
        temp_sPH_outer_vec.clear(); temp_sPH_outer_vec = vector<vector<clu_info>>(13); // note : chip
    }

    c1 -> Print(plot_folder_dire + "/" + Form("%s_eventdisplay_origin_%.2f_%.2f.pdf)",server_name.Data(),beam_origin.first,beam_origin.second));


    // c1 -> cd();
    // temp_bkg(c1);
    // TGraph * bbb = new TGraph(temp_x_vec.size(), &temp_x_vec[0], &temp_y_vec[0]);
    // bbb -> SetMarkerStyle(20);
    // bbb -> SetMarkerColor(2);
    // bbb -> SetMarkerSize(1);
    
    // bbb -> Draw("p same");

    // for (int line_i = 0; line_i < track_line.size(); line_i++)
    // {
    //     // track_line[line_i] -> SetLineStyle(7);
    //     track_line[line_i] -> SetLineColor(1);
    //     track_line[line_i] -> SetLineWidth(1);
    //     track_line[line_i] -> Draw("l same");
    // }
    // bbb -> Draw("p same");
    
    

    
    
    if (true){
        angle_correlation -> Draw("colz0");
        // c1 -> SetLogz();
        c1 -> Print(plot_folder_dire + "/" + "inner_outer_angle_correlation.pdf");
        c1 -> Clear();

        DCA_point -> Draw("colz0");
        c1 -> Print(plot_folder_dire + "/" + "DCA_point.pdf");
        c1 -> Clear();

        angle_DCAPointX -> Draw("colz0");
        c1 -> Print(plot_folder_dire + "/" + "angle_DCAPointX.pdf");
        c1 -> Clear();


        angle_DCAPointY -> Draw("colz0");
        c1 -> Print(plot_folder_dire + "/" + "angle_DCAPointY.pdf");
        c1 -> Clear();


        angle_correlation_inner -> Draw("colz0");
        c1 -> Print(plot_folder_dire + "/" + "angle_correlation_inner.pdf");
        c1 -> Clear();


        angle_correlation_outer -> Draw("colz0");
        c1 -> Print(plot_folder_dire + "/" + "angle_correlation_outer.pdf");
        c1 -> Clear();


        total_cluster_size -> Draw("hist");
        c1 -> Print(plot_folder_dire + "/" + "total_cluster_size.pdf");
        c1 -> Clear();

        module_cluster_size -> Draw("colz0");
        c1 -> Print(plot_folder_dire + "/" + "module_cluster_size.pdf");
        c1 -> Clear();

        DCAdistance_innerPhi -> Draw("colz0");
        c1 -> Print(plot_folder_dire + "/" + "DCAdistance_innerPhi.pdf");
        c1 -> Clear();

        DCAdistance -> Draw("colz0");
        c1 -> Print(plot_folder_dire + "/" + "DCAdistance.pdf");
        c1 -> Clear();
    }

}