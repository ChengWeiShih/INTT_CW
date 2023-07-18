#include "DAC_Scan_ladder.h"

TString mother_folder_directory = "/home/phnxrc/INTT/cwshih/DACscan_data/test_intt4";
// todo : the number of number is given by the adc_setting_run !!!
// todo : also the range of the hist.
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

struct full_ladder_info {
    int FC;
    TString Port;
    int ROC;
    int Direction; // note : 0 : south, 1 : north 
    TString Ladder;
};

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




// note : use "ls *.root > file_list.txt" to create the list of the file in the folder, full directory in the file_list.txt
// note : set_folder_name = "folder_xxxx"
// note : server_name = "inttx"
void DAC_Scan_event_based()
{

    TString set_folder_name = "testing"; 
    TString server_name = "intt4";
    vector<int> FC_id={11};
    double temp_multiplicity_cut[5] = {600,500,500,400,400};

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
            
            if (fNhits < temp_multiplicity_cut[fid]) {

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
                if ( (N_clu_column_FC7[i1] + N_clu_column_FC8[i1]) == 0 ) continue;

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

    ladder[0] -> Output_bin_plots(plot_folder_dire, set_folder_name, true);
    ladder[0] -> Output_comb_plots(plot_folder_dire, set_folder_name);
    ladder[0] -> Output_final_plots(plot_folder_dire, set_folder_name);
    ladder[0] -> Output_root(plot_folder_dire, set_folder_name);

}