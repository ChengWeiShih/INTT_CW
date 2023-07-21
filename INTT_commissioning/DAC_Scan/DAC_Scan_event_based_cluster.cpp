#include "DAC_Scan_ladder.h"
#include "InttConversion.h"

TString mother_folder_directory = "/home/phnxrc/INTT/cwshih/DACscan_data/test_intt4_zero_magnet";
// todo : the number of number is given by the adc_setting_run !!!
// todo : also the range of the hist.
// todo : the adc follows the following convention
// todo : 1. the increment has to be 4
// todo : remember to check the "adc_conv"
vector<vector<int>> adc_setting_run = {	
    // {8  , 12 , 16 , 20 , 24 , 28 , 32 , 36 },
    // {28 , 32 , 36 , 40 , 44 , 48 , 52 , 56 },
    {48 , 52 , 56 , 60 , 64 , 68 , 72 , 76 }, // note : 3
    {68 , 72 , 76 , 80 , 84 , 88 , 92 , 96 }, // note : 4
    {88 , 92 , 96 , 100, 104, 108, 112, 116}, // note : 5
    {108, 112, 116, 120, 124, 128, 132, 136}, // note : 6
    {128, 132, 136, 140, 144, 148, 152, 156}, // note : 7
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

// struct sPH_clu_info{
//     vector<clu_info> inner[13];
//     vector<clu_info> outer[13];
// };

// struct full_ladder_info {
//     int FC;
//     TString Port;
//     int ROC;
//     int Direction; // note : 0 : south, 1 : north 
//     TString Ladder;
// };

// struct pos_str
// {
//     double x;
//     double y;
//     int layer;
//     double phi;
// };

// note : assumption, in one event, single channel can only be fired once. 
// vector<clu_info> clustering(string server_name, int FC_id, vector<hit_info> single_event)
// {
//     int hit_seat[13][256]; memset(hit_seat, -1, sizeof(hit_seat)); // note : one event, one half-ladder
//     vector<hit_info>hit_vec; hit_vec.clear(); // note : chip_id is not used
//     vector<hit_pro_info>hit_pro_vec; hit_pro_vec.clear(); // note : have the position information

//     clu_info clu_unit;
//     vector<clu_info> output_vec; output_vec.clear();
     

//     double num_hit = 0;
//     double chan_truck = 0;
// 	double chan_truck_denominator = 0;
// 	int standard_channel;
// 	int distance_meter = 0;
//     int sum_adc = 0;
    
//     for (int i = 0; i < single_event.size(); i++) // note : number of hits in this event, for this half-ladder
//     {
//         // note : U1 ~ U13, 0 ~ 127. // U14 ~ U26, -> 128 ~ 255
//         int chip_conv = ( single_event[i].chip_id > 13 ) ? single_event[i].chip_id - 13  : single_event[i].chip_id;
//         int chan_conv = ( single_event[i].chip_id > 13 ) ? 255 - single_event[i].chan_id : single_event[i].chan_id;

//         if (hit_seat[chip_conv - 1][chan_conv]  == -1)
//         {
//             hit_seat[chip_conv - 1][chan_conv] = single_event[i].adc;
//         }
//         else 
//         {
//             cout<<"chip_id : "<<chip_conv<<" chan_id : "<<chan_conv<<" fired more than once ! N_hit in this HL : "<<single_event.size()<<" original : "<<single_event[i].chip_id<<" "<<single_event[i].chan_id<<endl;
//             hit_seat[chip_conv - 1][chan_conv] = single_event[i].adc; // note : take the latest one.   
//         }
//     }

//     for (int col = 0; col < 13; col++) // note : column
//     {
//         for (int ch = 0; ch < 256; ch++) // note : channel 
//         {
//             if (hit_seat[col][ch] != -1)
//             {
                
//                 hit_pro_vec.push_back({
//                     (ch > 127) ? col + 1 + 13 : col + 1,
//                     (ch > 127) ? 255 - ch : ch,
//                     col + 1,
//                     ch,
//                     hit_seat[col][ch],
                    
//                 });


//                 int chip_id;
//                 int chan_id;

//                 int column_id;
//                 int chan_id_conv;

//                 int adc;
//                 int adc_conv;

//                 double x;
//                 double y;
//                 int layer;
                
                
//                 hit_info hit_unit;
//                 hit_unit.chan_id = ch;
//                 hit_unit.adc = hit_seat[col][ch];
                
//                 hit_vec.push_back(hit_unit);
//             }

//         }

//         for ()
//         {

//         }

//         // note : single column
//         for (int i = 0; i < hit_vec.size(); i ++)
//         {
//             standard_channel        = hit_vec[i].chan_id;
//             chan_truck             += hit_vec[i].chan_id;
//             chan_truck_denominator += 1;
//             num_hit                += 1; 
//             sum_adc                += hit_vec[i].adc;

//             if (hit_vec.size() - i == 1) // note : for the single hit
//             {
//                 clu_unit.column   = col + 1;
//                 clu_unit.avg_chan = chan_truck / chan_truck_denominator;
//                 clu_unit.sum_adc  = sum_adc;
//                 clu_unit.size     = num_hit;

//                 output_vec.push_back(clu_unit);
//             }

//             for (int i1 = 0; i1 < hit_vec.size() - (i + 1); i1++) // note : this for loop will be skipped if it's single hit
//             {
//                 if ( fabs(standard_channel - hit_vec[i + i1 + 1].chan_id) == i1 + 1 )
//                 {
//                     chan_truck             += hit_vec[i + i1 + 1].chan_id;
//                     chan_truck_denominator += 1;
//                     num_hit                += 1; 
//                     sum_adc                += hit_vec[i + i1 + 1].adc;

//                     distance_meter += 1;

//                     if ((hit_vec.size() - (i + 1) - i1) == 1) // note : for the last hit
// 					{
// 						clu_unit.column   = col + 1;
//                         clu_unit.avg_chan = chan_truck / chan_truck_denominator;
//                         clu_unit.sum_adc  = sum_adc;
//                         clu_unit.size     = num_hit;

//                         output_vec.push_back(clu_unit);
// 					}
//                 }
//                 else 
//                 {
//                     clu_unit.column   = col + 1;
//                     clu_unit.avg_chan = chan_truck / chan_truck_denominator;
//                     clu_unit.sum_adc  = sum_adc;
//                     clu_unit.size     = num_hit;

//                     output_vec.push_back(clu_unit);

//                     break;
//                 }
//             }

//             i += distance_meter;
// 			distance_meter = 0;
// 			chan_truck = 0;
// 			chan_truck_denominator = 0;
//             num_hit = 0;
//             sum_adc = 0;
//         }

//         // note : start of cleaning 
//         hit_vec.clear();

//     } // note : chip loop

//     return output_vec;

// }

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


map<TString,full_ladder_info> serverFC_toinfo_map{    
    {"intt0_0", {0, "D2", 0, 0, "B1L101S"}}, // note : intt 0
    {"intt0_1", {1, "C1", 0, 0, "B0L101S"}},
    {"intt0_2", {2, "C2", 0, 0, "B1L001S"}},
    {"intt0_3", {3, "B3", 0, 0, "B1L000S"}},
    {"intt0_4", {4, "A2", 0, 0, "B1L100S"}},
    {"intt0_5", {5, "B1", 0, 0, "B0L000S"}},
    {"intt0_6", {6, "A1", 0, 0, "B0L100S"}},

    {"intt0_7", {7, "C2", 1, 0, "B1L103S"}},
    {"intt0_8", {8, "C1", 1, 0, "B0L002S"}},
    {"intt0_9", {9, "A1", 1, 0, "B0L001S"}},
    {"intt0_10", {10, "B3", 1, 0, "B1L002S"}},
    {"intt0_11", {11, "A2", 1, 0, "B1L102S"}},
    {"intt0_12", {12, "B1", 1, 0, "B0L102S"}},
    {"intt0_13", {13, "D2", 1, 0, "B1L003S"}},



    {"intt1_0", {0, "C2", 2, 0, "B1L105S"}},  // note : intt 1
    {"intt1_1", {1, "C1", 2, 0, "B0L104S"}},  
    {"intt1_2", {2, "A2", 2, 0, "B0L103S"}},  
    {"intt1_3", {3, "B3", 2, 0, "B1L004S"}},  
    {"intt1_4", {4, "A1", 2, 0, "B1L104S"}},  
    {"intt1_5", {5, "B1", 2, 0, "B0L003S"}},  
    {"intt1_6", {6, "D2", 2, 0, "B1L005S"}},  

    {"intt1_7", {7, "C2", 3, 0, "B1L107S"}},  
    {"intt1_8", {8, "C1", 3, 0, "B0L005S"}},  
    {"intt1_9", {9, "A1", 3, 0, "B0L004S"}},  
    {"intt1_10", {10, "B2", 3, 0, "B1L006S"}},  
    {"intt1_11", {11, "A2", 3, 0, "B1L106S"}},  
    {"intt1_12", {12, "B1", 3, 0, "B0L105S"}},  
    {"intt1_13", {13, "D1", 3, 0, "B1L007S"}},



    {"intt2_0", {0, "A1", 4, 0, "B0L106S"}},  // note : intt 2
    {"intt2_1", {1, "B1", 4, 0, "B0L006S"}},  
    {"intt2_2", {2, "C1", 4, 0, "B0L107S"}},  
    {"intt2_3", {3, "A2", 4, 0, "B1L108S"}},  
    {"intt2_4", {4, "B2", 4, 0, "B1L008S"}},  
    {"intt2_5", {5, "C2", 4, 0, "B1L109S"}},  
    {"intt2_6", {6, "D1", 4, 0, "B1L009S"}},  

    {"intt2_7", {7, "A1", 5, 0, "B0L007S"}},  
    {"intt2_8", {8, "B3", 5, 0, "B0L108S"}},  
    {"intt2_9", {9, "C1", 5, 0, "B0L008S"}},  
    {"intt2_10", {10, "A2", 5, 0, "B1L110S"}},  
    {"intt2_11", {11, "B2", 5, 0, "B1L010S"}},  
    {"intt2_12", {12, "C2", 5, 0, "B1L111S"}},  
    {"intt2_13", {13, "C3", 5, 0, "B1L011S"}},



    {"intt3_0", {0, "A1", 6, 0, "B0L109S"}},  // note : intt 3
    {"intt3_1", {1, "B1", 6, 0, "B0L009S"}},  
    {"intt3_2", {2, "C1", 6, 0, "B0L110S"}},  
    {"intt3_3", {3, "A2", 6, 0, "B1L112S"}},  
    {"intt3_4", {4, "B3", 6, 0, "B1L012S"}},  
    {"intt3_5", {5, "C2", 6, 0, "B1L113S"}},  
    {"intt3_6", {6, "D1", 6, 0, "B1L013S"}},  

    {"intt3_7", {7, "A1", 7, 0, "B0L010S"}},  
    {"intt3_8", {8, "B1", 7, 0, "B0L111S"}},  
    {"intt3_9", {9, "C1", 7, 0, "B0L011S"}},  
    {"intt3_10", {10, "A2", 7, 0, "B1L114S"}},  
    {"intt3_11", {11, "B3", 7, 0, "B1L014S"}},  
    {"intt3_12", {12, "C3", 7, 0, "B1L115S"}},  
    {"intt3_13", {13, "D2", 7, 0, "B1L015S"}},



    {"intt4_0", {0,	"B1", 0, 1, "B1L101N"}},	// note : intt 4
    {"intt4_1", {1,	"C2", 0, 1, "B0L000N"}},	
    {"intt4_2", {2,	"D1", 0, 1, "B0L100N"}},	
    {"intt4_3", {3,	"A2", 0, 1, "B1L001N"}},	
    {"intt4_4", {4,	"B2", 0, 1, "B0L101N"}},	
    {"intt4_5", {5,	"C3", 0, 1, "B1L000N"}},	
    {"intt4_6", {6,	"D2", 0, 1, "B1L100N"}},	 

    {"intt4_7", {7, "B1", 1, 1, "B0L002N"}},	 
    {"intt4_8", {8, "C2", 1, 1, "B0L102N"}},	
    {"intt4_9", {9, "D1", 1, 1, "B0L001N"}},	
    {"intt4_10", {10, "A2", 1, 1, "B1L003N"}},	 
    {"intt4_11", {11, "B2", 1, 1, "B1L103N"}},	
    {"intt4_12", {12, "C3", 1, 1, "B1L002N"}},	
    {"intt4_13", {13, "B3", 1, 1, "B1L102N"}},



    {"intt5_0", {0, "C1", 2, 1, "B0L003N"}},	// note : intt 5
    {"intt5_1", {1, "B1", 2, 1, "B0L104N"}},	
    {"intt5_2", {2, "D2", 2, 1, "B0L103N"}},	 
    {"intt5_3", {3, "D1", 2, 1, "B1L004N"}},	
    {"intt5_4", {4, "A2", 2, 1, "B1L005N"}},	 
    {"intt5_5", {5, "C2", 2, 1, "B1L104N"}},	
    {"intt5_6", {6, "A1", 2, 1, "B1L105N"}},	

    {"intt5_7", {7, "C3", 3, 1, "B1L107N"}},	
    {"intt5_8", {8, "B1", 3, 1, "B1L007N"}},	
    {"intt5_9", {9, "C1", 3, 1, "B1L006N"}},	
    {"intt5_10", {10, "D2", 3, 1, "B1L106N"}},	
    {"intt5_11", {11, "A1", 3, 1, "B0L005N"}},	
    {"intt5_12", {12, "C2", 3, 1, "B0L105N"}},	
    {"intt5_13", {13, "D1", 3, 1, "B0L004N"}},



    {"intt6_0", {0, "A1", 4, 1, "B0L106N"}},	// note : intt 6
    {"intt6_1", {1, "C3", 4, 1, "B0L006N"}},	
    {"intt6_2", {2, "B2", 4, 1, "B0L107N"}},	
    {"intt6_3", {3, "D1", 4, 1, "B1L108N"}},	
    {"intt6_4", {4, "C2", 4, 1, "B1L008N"}},	
    {"intt6_5", {5, "B3", 4, 1, "B1L109N"}},	
    {"intt6_6", {6, "A2", 4, 1, "B1L009N"}},	

    {"intt6_7", {7, "D1", 5, 1, "B0L007N"}},	
    {"intt6_8", {8, "C3", 5, 1, "B0L108N"}},	
    {"intt6_9", {9, "A2", 5, 1, "B0L008N"}},	
    {"intt6_10", {10, "D2", 5, 1, "B1L110N"}},	  
    {"intt6_11", {11, "C2", 5, 1, "B1L010N"}},	
    {"intt6_12", {12, "B3", 5, 1, "B1L111N"}},	
    {"intt6_13", {13, "A1", 5, 1, "B1L011N"}},  



    {"intt7_0", {0, "B3", 6, 1, "B0L109N"}},	// note : intt 7
    {"intt7_1", {1, "C1", 6, 1, "B0L009N"}},	
    {"intt7_2", {2, "B1", 6, 1, "B0L110N"}},	
    {"intt7_3", {3, "D1", 6, 1, "B1L112N"}},	
    {"intt7_4", {4, "C2", 6, 1, "B1L012N"}},	
    {"intt7_5", {5, "B2", 6, 1, "B1L113N"}},	
    {"intt7_6", {6, "A1", 6, 1, "B1L013N"}},	

    {"intt7_7", {7, "B2", 7, 1, "B0L010N"}},	
    {"intt7_8", {8, "C2", 7, 1, "B0L111N"}},	
    {"intt7_9", {9, "B1", 7, 1, "B0L011N"}},	  
    {"intt7_10", {10, "D2", 7, 1, "B1L114N"}},	  
    {"intt7_11", {11, "C3", 7, 1, "B1L014N"}},	
    {"intt7_12", {12, "B3", 7, 1, "B1L115N"}},	
    {"intt7_13", {13, "A1", 7, 1, "B1L015N"}}
};

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

TCanvas * c1 = new TCanvas("","",800,800);

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
void DAC_Scan_event_based_cluster()
{

    TString set_folder_name = "testing"; 
    TString server_name = "intt4";
    vector<int> FC_id={11};
    double temp_multiplicity_cut[5] = {600,500,500,400,400};
    pair<double,double> beam_origin = {0,0};
    
    int selected_event_id = 54;
    int selected_column   = 11;

    vector<TString> file_name_vec = read_file_list(set_folder_name, server_name);

    // TString set_name = set_folder_name; set_name = set_name.ReplaceAll("folder_","");

    TString plot_folder_dire = Form("%s/plots/results/%i_%s", mother_folder_directory.Data(), FC_id[0], serverFC_toinfo_map[Form("%s_%i",server_name.Data(),FC_id[0])].Ladder.Data());
    system( Form("mkdir %s",plot_folder_dire.Data()) );

    vector< LadderDAC * > ladder(FC_id.size(),NULL);
    for (int i = 0; i < FC_id.size(); i++)
    {
        full_ladder_info single_HF_info = serverFC_toinfo_map[Form("%s_%i",server_name.Data(),FC_id[i])];
        ladder[i] = new LadderDAC("chip", single_HF_info.Ladder, single_HF_info.ROC, single_HF_info.Port, FC_id[i], adc_setting_run, 1);
    }

    int fNhits;
    int module[10000];
    int chip_id[10000];
    int chan_id[10000];
    int adc[10000];

    int N_total_ladder = 14;

    vector<vector<hit_info>> single_event_hit_vec(N_total_ladder); // note : N half_ladder 
    vector<vector<clu_info>> clu_vec(N_total_ladder);              // note : N half_ladder

    vector<double> temp_x_vec; temp_x_vec.clear();
    vector<double> temp_y_vec; temp_y_vec.clear();

    vector<vector<clu_info>> temp_sPH_inner_vec(13);
    vector<vector<clu_info>> temp_sPH_outer_vec(13);

    TH2F * angle_correlation = new TH2F("","",361,0,361,361,0,361);
    TH2F * angle_correlation_inner = new TH2F("","",361,0,361,100,-50,50);
    TH2F * angle_correlation_outer = new TH2F("","",361,0,361,100,-50,50);

    TH2F * DCA_point = new TH2F("","",100,-10,10,100,-10,10);
    TH2F * angle_DCAPointX = new TH2F("","",100,-50,50,100,-10,10);
    TH2F * angle_DCAPointY = new TH2F("","",100,-50,50,100,-10,10);

    TH1F * total_cluster_size = new TH1F("","",40,0,40);

    TH2F * module_cluster_size = new TH2F("","",14,0,14,40,0,40);

    TH2F * DCAdistance_innerPhi = new TH2F("","",100,-1,100,361,0,361);

    TH1F * DCAdistance = new TH1F("","",100,-1,300);

    vector<TLine * > track_line; track_line.clear();

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


        for (int i = 0; i < 1000; i++ ) // note : event
        {
            tree -> GetEntry(i);
            
            // if (fNhits < temp_multiplicity_cut[fid]) {

            //     single_event_hit_vec.clear(); single_event_hit_vec = vector<vector<hit_info>>(N_total_ladder);
            //     clu_vec.clear(); clu_vec = vector<vector<clu_info>>(N_total_ladder);
            //     continue;
            // }
            // cout<<" test Nhit "<<fNhits<<endl;

            for (int i1 = 0 ; i1 < fNhits; i1++)
            {
                // note : module ID 0 to 13, and chip_id 1 to 26, chan_id 0 to 127 adc 0 to 7
                if (module[i1] > -1 && module[i1] < 14 && chip_id[i1] > 0 && chip_id[i1] < 27 && chan_id[i1] > -1 && chan_id[i1] < 128 && adc[i1] > -1 && adc[i1] < 8) 
                    single_event_hit_vec[module[i1]].push_back({chip_id[i1], chan_id[i1], adc[i1], adc_setting_run[fid][adc[i1]]});
            }


            for (int i1 = 0; i1 < N_total_ladder; i1++)
            {
                clu_vec[i1] = clustering(server_name.Data(), i1, single_event_hit_vec[i1]);
                
                cout<<"event : "<<i<<" server : "<<i1<<" N_cluster : "<<clu_vec[i1].size()<<endl;

                for (int i2 = 0; i2 < clu_vec[i1].size(); i2++)
                {
                    

                    total_cluster_size -> Fill(clu_vec[i1][i2].size);
                    module_cluster_size -> Fill(i1,clu_vec[i1][i2].size);
                    if ( clu_vec[i1][i2].layer == 0 )
                        temp_sPH_inner_vec[ clu_vec[i1][i2].column - 1 ].push_back(clu_vec[i1][i2]);
                    else
                        temp_sPH_outer_vec[ clu_vec[i1][i2].column - 1 ].push_back(clu_vec[i1][i2]);

                    // if (clu_vec[i1][i2].column == 11)
                    // {
                    //     temp_x_vec.push_back(clu_vec[i1][i2].x);
                    //     temp_y_vec.push_back(clu_vec[i1][i2].y);
                    //     cout<<"--- test, pos : "<<clu_vec[i1][i2].x<<" "<<clu_vec[i1][i2].y<<endl; 
                    // }
                }
            }

            for (int column_i = 0; column_i < 13; column_i++)
            {
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

                        if (DCA_info_vec[1]>-3 && DCA_info_vec[1] < 3 && DCA_info_vec[2] > 2 && DCA_info_vec[2] < 7)
                        {
                            cout<<"--------- X1 : "<<temp_sPH_inner_vec[column_i][inner_i].x <<" Y1 : "<< temp_sPH_inner_vec[column_i][inner_i].y<<" X2 : "<<temp_sPH_outer_vec[column_i][outer_i].x <<" Y2 : "<< temp_sPH_outer_vec[column_i][outer_i].y<<endl;
                            cout<<"--------- ("<<temp_sPH_inner_vec[column_i][inner_i].x <<","<< temp_sPH_inner_vec[column_i][inner_i].y<<"),("<<temp_sPH_outer_vec[column_i][outer_i].x <<","<< temp_sPH_outer_vec[column_i][outer_i].y<<")"<<endl;
                            cout<<"--------- distance : "<<DCA_info_vec[0]<<" DCAx : "<<DCA_info_vec[1]<<" DCAy : "<<DCA_info_vec[2]<<endl;
                            cout<<" "<<endl;
                        }
                        
                        angle_correlation -> Fill(temp_sPH_inner_vec[column_i][inner_i].phi, temp_sPH_outer_vec[column_i][outer_i].phi);

                        DCA_point -> Fill(
                            DCA_info_vec[1],
                            DCA_info_vec[2]
                        );

                        DCAdistance_innerPhi -> Fill(
                            DCA_info_vec[0],
                            temp_sPH_inner_vec[column_i][inner_i].phi
                        );

                        DCAdistance -> Fill(
                            DCA_info_vec[0]
                        );

                        angle_DCAPointX -> Fill(
                            temp_sPH_inner_vec[column_i][inner_i].phi - temp_sPH_outer_vec[column_i][outer_i].phi,
                            DCA_info_vec[1]
                        );

                        angle_DCAPointY -> Fill(
                            temp_sPH_inner_vec[column_i][inner_i].phi - temp_sPH_outer_vec[column_i][outer_i].phi,
                            DCA_info_vec[2]
                        );
                        // if (temp_sPH_inner_vec[column_i][inner_i].phi < 100)cout<<"what hte fuck"<<endl;
                        angle_correlation_inner -> Fill(temp_sPH_inner_vec[column_i][inner_i].phi, temp_sPH_inner_vec[column_i][inner_i].phi - temp_sPH_outer_vec[column_i][outer_i].phi);
                        angle_correlation_outer -> Fill(temp_sPH_outer_vec[column_i][outer_i].phi, temp_sPH_inner_vec[column_i][inner_i].phi - temp_sPH_outer_vec[column_i][outer_i].phi);
                        
                        
                    }
                }
            }

            single_event_hit_vec.clear(); single_event_hit_vec = vector<vector<hit_info>>(N_total_ladder);
            clu_vec.clear(); clu_vec = vector<vector<clu_info>>(N_total_ladder);
            temp_sPH_inner_vec.clear(); temp_sPH_inner_vec = vector<vector<clu_info>>(13); // note : chip
            temp_sPH_outer_vec.clear(); temp_sPH_outer_vec = vector<vector<clu_info>>(13); // note : chip

        }


        single_event_hit_vec.clear(); single_event_hit_vec = vector<vector<hit_info>>(N_total_ladder);
        clu_vec.clear(); clu_vec = vector<vector<clu_info>>(N_total_ladder);
        temp_sPH_inner_vec.clear(); temp_sPH_inner_vec = vector<vector<clu_info>>(13); // note : chip
        temp_sPH_outer_vec.clear(); temp_sPH_outer_vec = vector<vector<clu_info>>(13); // note : chip
    }


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
        c1 -> SetLogz();
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

    // ladder[0] -> Weight();
    // ladder[0] -> Fill_final();
    // ladder[0] -> Fit();

    // ladder[0] -> Output_bin_plots(plot_folder_dire, set_folder_name, false);
    // ladder[0] -> Output_comb_plots(plot_folder_dire, set_folder_name);
    // ladder[0] -> Output_final_plots(plot_folder_dire, set_folder_name);
    // ladder[0] -> Output_root(plot_folder_dire, set_folder_name);

}


// map<TString,ladder_info> ladder_toinfo_map{    
//     {"B1L101S", {0, "D2", 0, 0}}, // note : intt 0
//     {"B0L101S", {1, "C1", 0, 0}},
//     {"B1L001S", {2, "C2", 0, 0}},
//     {"B1L000S", {3, "B3", 0, 0}},
//     {"B1L100S", {4, "A2", 0, 0}},
//     {"B0L000S", {5, "B1", 0, 0}},
//     {"B0L100S", {6, "A1", 0, 0}},

//     {"B1L103S", {7, "C2", 1, 0}},
//     {"B0L002S", {8, "C1", 1, 0}},
//     {"B0L001S", {9, "A1", 1, 0}},
//     {"B1L002S", {10, "B3", 1, 0}},
//     {"B1L102S", {11, "A2", 1, 0}},
//     {"B0L102S", {12, "B1", 1, 0}},
//     {"B1L003S", {13, "D2", 1, 0}},



//     {"B1L105S", {0, "C2", 2, 0}},  // note : intt 1
//     {"B0L104S", {1, "C1", 2, 0}},  
//     {"B0L103S", {2, "A2", 2, 0}},  
//     {"B1L004S", {3, "B3", 2, 0}},  
//     {"B1L104S", {4, "A1", 2, 0}},  
//     {"B0L003S", {5, "B1", 2, 0}},  
//     {"B1L005S", {6, "D2", 2, 0}},  

//     {"B1L107S", {7, "C2", 3, 0}},  
//     {"B0L005S", {8, "C1", 3, 0}},  
//     {"B0L004S", {9, "A1", 3, 0}},  
//     {"B1L006S", {10, "B2", 3, 0}},  
//     {"B1L106S", {11, "A2", 3, 0}},  
//     {"B0L105S", {12, "B1", 3, 0}},  
//     {"B1L007S", {13, "D1", 3, 0}},



//     {"B0L106S", {0, "A1", 4, 0}},  // note : intt 2
//     {"B0L006S", {1, "B1", 4, 0}},  
//     {"B0L107S", {2, "C1", 4, 0}},  
//     {"B1L108S", {3, "A2", 4, 0}},  
//     {"B1L008S", {4, "B2", 4, 0}},  
//     {"B1L109S", {5, "C2", 4, 0}},  
//     {"B1L009S", {6, "D1", 4, 0}},  

//     {"B0L007S", {7, "A1", 5, 0}},  
//     {"B0L108S", {8, "B3", 5, 0}},  
//     {"B0L008S", {9, "C1", 5, 0}},  
//     {"B1L110S", {10, "A2", 5, 0}},  
//     {"B1L010S", {11, "B2", 5, 0}},  
//     {"B1L111S", {12, "C2", 5, 0}},  
//     {"B1L011S", {13, "C3", 5, 0}},



//     {"B0L109S", {0, "A1", 6, 0}},  // note : intt 3
//     {"B0L009S", {1, "B1", 6, 0}},  
//     {"B0L110S", {2, "C1", 6, 0}},  
//     {"B1L112S", {3, "A2", 6, 0}},  
//     {"B1L012S", {4, "B3", 6, 0}},  
//     {"B1L113S", {5, "C2", 6, 0}},  
//     {"B1L013S", {6, "D1", 6, 0}},  

//     {"B0L010S", {7, "A1", 7, 0}},  
//     {"B0L111S", {8, "B1", 7, 0}},  
//     {"B0L011S", {9, "C1", 7, 0}},  
//     {"B1L114S", {10, "A2", 7, 0}},  
//     {"B1L014S", {11, "B3", 7, 0}},  
//     {"B1L115S", {12, "C3", 7, 0}},  
//     {"B1L015S", {13, "D2", 7, 0}},



//     {"B1L101N", {0,	"B1", 0, 1}},	// note : intt 4
//     {"B0L000N", {1,	"C2", 0, 1}},	
//     {"B0L100N", {2,	"D1", 0, 1}},	
//     {"B1L001N", {3,	"A2", 0, 1}},	
//     {"B0L101N", {4,	"B2", 0, 1}},	
//     {"B1L000N", {5,	"C3", 0, 1}},	
//     {"B1L100N", {6,	"D2", 0, 1}},	 

//     {"B0L002N", {7, "B1", 1, 1}},	 
//     {"B0L102N", {8, "C2", 1, 1}},	
//     {"B0L001N", {9, "D1", 1, 1}},	
//     {"B1L003N", {10, "A2", 1, 1}},	 
//     {"B1L103N", {11, "B2", 1, 1}},	
//     {"B1L002N", {12, "C3", 1, 1}},	
//     {"B1L102N", {13, "B3", 1, 1}},



//     {"B0L003N", {0, "C1", 2, 1}},	// note : intt 5
//     {"B0L104N", {1, "B1", 2, 1}},	
//     {"B0L103N", {2, "D2", 2, 1}},	 
//     {"B1L004N", {3, "D1", 2, 1}},	
//     {"B1L005N", {4, "A2", 2, 1}},	 
//     {"B1L104N", {5, "C2", 2, 1}},	
//     {"B1L105N", {6, "A1", 2, 1}},	

//     {"B1L107N", {7, "C3", 3, 1}},	
//     {"B1L007N", {8, "B1", 3, 1}},	
//     {"B1L006N", {9, "C1", 3, 1}},	
//     {"B1L106N", {10, "D2", 3, 1}},	
//     {"B0L005N", {11, "A1", 3, 1}},	
//     {"B0L105N", {12, "C2", 3, 1}},	
//     {"B0L004N", {13, "D1", 3, 1}},



//     {"B0L106N", {0, "A1", 4, 1}},	// note : intt 6
//     {"B0L006N", {1, "C3", 4, 1}},	
//     {"B0L107N", {2, "B2", 4, 1}},	
//     {"B1L108N", {3, "D1", 4, 1}},	
//     {"B1L008N", {4, "C2", 4, 1}},	
//     {"B1L109N", {5, "B3", 4, 1}},	
//     {"B1L009N", {6, "A2", 4, 1}},	

//     {"B0L007N", {7, "D1", 5, 1}},	
//     {"B0L108N", {8, "C3", 5, 1}},	
//     {"B0L008N", {9, "A2", 5, 1}},	
//     {"B1L110N", {10, "D2", 5, 1}},	  
//     {"B1L010N", {11, "C2", 5, 1}},	
//     {"B1L111N", {12, "B3", 5, 1}},	
//     {"B1L011N", {13, "A1", 5, 1}},  



//     {"B0L109N", {0, "B3", 6, 1}},	// note : intt 7
//     {"B0L009N", {1, "C1", 6, 1}},	
//     {"B0L110N", {2, "B1", 6, 1}},	
//     {"B1L112N", {3, "D1", 6, 1}},	
//     {"B1L012N", {4, "C2", 6, 1}},	
//     {"B1L113N", {5, "B2", 6, 1}},	
//     {"B1L013N", {6, "A1", 6, 1}},	

//     {"B0L010N", {7, "B2", 7, 1}},	
//     {"B0L111N", {8, "C2", 7, 1}},	
//     {"B0L011N", {9, "B1", 7, 1}},	  
//     {"B1L114N", {10, "D2", 7, 1}},	  
//     {"B1L014N", {11, "C3", 7, 1}},	
//     {"B1L115N", {12, "B3", 7, 1}},	
//     {"B1L015N", {13, "A1", 7, 1}}
// };