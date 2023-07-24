#ifndef __INTTCONVERSION_H_
#define __INTTCONVERSION_H_

struct full_ladder_info {
    int FC;
    TString Port;
    int ROC;
    int Direction; // note : 0 : south, 1 : north 
    TString Ladder;
};
struct pos_str{
    
    double x;
    double y;
    double z;
    int layer;
    double phi;
};


namespace InttConversion
{
    double layer_raduis[4] = {71.844000, 77.284000, 96.764000, 102.58400}; // note : unit : mm
    int N_ladder[4] = {12, 12, 16, 16};
    double angle_increment[4] = {30,30,22.5,22.5}; // note : ladder rotation
    double ladder_location_angle_ini_correction[4] = {255,270,258.75,270};
    double ladder_self_angle_ini_correction[4] = {345,360,348.75,360};
    double pos_ini_correction[4];
    double channel_width = 0.078; // note : unit : mm
    // note : It's different from the sPHENIX-INTT official conversion, but it's 
    double ideal_z_pos[13] = {
        9.55, 25.55, 41.55, 57.55, 73.55, 89.55, 105.55, 121.55, // note : type A
        141.75, 161.75, 181.75, 201.75, 221.75 // note : type B
    };
    string ladder_index_string[16] = {"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15"};

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

    pair<double,double> Get_dummy_ladder_XY(int chip_id, int chan_id)
    {
        // note : chip_id 1 ~ 13, 14 ~ 26
        int row_index = (chip_id > 0 && chip_id < 14) ? 0 : 1;

        double dummy_X = 0;
        double dummy_Y = (row_index == 0) ? -0.039 - (127 - chan_id) * 0.078 : 0.039 + (127 - chan_id) * 0.078;

        return {dummy_X,dummy_Y};
    }

    pair<double,double> Get_self_rotation( pair<double,double> chan_pos, double angle )
    {
        return {chan_pos.second * cos(angle / (180/TMath::Pi())), chan_pos.second * sin(angle / (180/TMath::Pi()))};
    }

    pos_str Get_XY_all(/*string ladder_name*/string server, int module, int chip_id, int chan_id)
    {
        // string ladder_pos_str = ladder_name;
        string ladder_pos_str = string(serverFC_toinfo_map[Form("%s_%i",server.c_str(),module)].Ladder);
        
        int layer_index; 
        if      (ladder_pos_str.substr(0,4) == "B0L0") layer_index = 0;
        else if (ladder_pos_str.substr(0,4) == "B0L1") layer_index = 1;
        else if (ladder_pos_str.substr(0,4) == "B1L0") layer_index = 2;
        else if (ladder_pos_str.substr(0,4) == "B1L1") layer_index = 3;
        else 
        {
            cout<<"error happens, wrong name"<<endl;
            exit(1);
        }

        int ladder_index = stoi(ladder_pos_str.substr(4,2));

        // note : 0, 1, 2, 3 : south, id 0
        // note : 4, 5, 6, 7 : north, id 1
        int ladder_side = ( stoi(server.substr(4,1)) < 4 ) ? 0 : 1;

        
        double ladder_location_angle = ladder_location_angle_ini_correction[layer_index] -  (angle_increment[layer_index] * ladder_index);

        double ring_pos_x = layer_raduis[layer_index] * cos(ladder_location_angle / (180/TMath::Pi()));
        double ring_pos_y = layer_raduis[layer_index] * sin(ladder_location_angle / (180/TMath::Pi()));

        double ladder_self_pos_X = ( fabs(Get_self_rotation(Get_dummy_ladder_XY(chip_id,chan_id),( ladder_self_angle_ini_correction[layer_index] - (angle_increment[layer_index] * ladder_index) )).first) < 0.0000001) ? 0 : Get_self_rotation(Get_dummy_ladder_XY(chip_id,chan_id),( ladder_self_angle_ini_correction[layer_index] - (angle_increment[layer_index] * ladder_index) )).first;
        double ladder_self_pos_Y = ( fabs(Get_self_rotation(Get_dummy_ladder_XY(chip_id,chan_id),( ladder_self_angle_ini_correction[layer_index] - (angle_increment[layer_index] * ladder_index) )).second) < 0.0000001) ? 0 : Get_self_rotation(Get_dummy_ladder_XY(chip_id,chan_id),( ladder_self_angle_ini_correction[layer_index] - (angle_increment[layer_index] * ladder_index) )).second;


        double final_pos_X = ring_pos_x + ladder_self_pos_X;
        double final_pos_Y = ring_pos_y + ladder_self_pos_Y;
        int    final_pos_layer = (layer_index == 0 || layer_index == 1) ? 0 : 1;
        double final_pos_phi = (final_pos_Y < 0) ? atan2(final_pos_Y,final_pos_X) * (180./TMath::Pi()) + 360 : atan2(final_pos_Y,final_pos_X) * (180./TMath::Pi());

        double final_pos_Z = ideal_z_pos[(chip_id - 1) % 13] * pow(-1, 1 - ladder_side);

        return {final_pos_X,final_pos_Y,final_pos_Z,final_pos_layer,final_pos_phi};
    }

}

#endif



// class InttClutering
// {
//     public:
//         InttClutering();
//         pair<double,double> Get_XY_all(string ladder_name,/*string server, int module,*/ int chip_id, int chan_id);
    
//     private: 
//         double layer_raduis[4] = {71.844000, 77.284000, 96.764000, 102.58400}; // note : unit : mm
//         int N_ladder[4] = {12, 12, 16, 16};
//         double angle_increment[4] = {30,30,22.5,22.5}; // note : ladder rotation
//         double ladder_location_angle_ini_correction[4] = {255,270,258.75,270};
//         double ladder_self_angle_ini_correction[4] = {345,360,348.75,360};
//         double pos_ini_correction[4];
//         double channel_width = 0.078; // note : unit : mm
//         string ladder_index_string[16] = {"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15"};

//         map<TString,full_ladder_info> serverFC_toinfo_map{    
//             {"intt0_0", {0, "D2", 0, 0, "B1L101S"}}, // note : intt 0
//             {"intt0_1", {1, "C1", 0, 0, "B0L101S"}},
//             {"intt0_2", {2, "C2", 0, 0, "B1L001S"}},
//             {"intt0_3", {3, "B3", 0, 0, "B1L000S"}},
//             {"intt0_4", {4, "A2", 0, 0, "B1L100S"}},
//             {"intt0_5", {5, "B1", 0, 0, "B0L000S"}},
//             {"intt0_6", {6, "A1", 0, 0, "B0L100S"}},

//             {"intt0_7", {7, "C2", 1, 0, "B1L103S"}},
//             {"intt0_8", {8, "C1", 1, 0, "B0L002S"}},
//             {"intt0_9", {9, "A1", 1, 0, "B0L001S"}},
//             {"intt0_10", {10, "B3", 1, 0, "B1L002S"}},
//             {"intt0_11", {11, "A2", 1, 0, "B1L102S"}},
//             {"intt0_12", {12, "B1", 1, 0, "B0L102S"}},
//             {"intt0_13", {13, "D2", 1, 0, "B1L003S"}},



//             {"intt1_0", {0, "C2", 2, 0, "B1L105S"}},  // note : intt 1
//             {"intt1_1", {1, "C1", 2, 0, "B0L104S"}},  
//             {"intt1_2", {2, "A2", 2, 0, "B0L103S"}},  
//             {"intt1_3", {3, "B3", 2, 0, "B1L004S"}},  
//             {"intt1_4", {4, "A1", 2, 0, "B1L104S"}},  
//             {"intt1_5", {5, "B1", 2, 0, "B0L003S"}},  
//             {"intt1_6", {6, "D2", 2, 0, "B1L005S"}},  

//             {"intt1_7", {7, "C2", 3, 0, "B1L107S"}},  
//             {"intt1_8", {8, "C1", 3, 0, "B0L005S"}},  
//             {"intt1_9", {9, "A1", 3, 0, "B0L004S"}},  
//             {"intt1_10", {10, "B2", 3, 0, "B1L006S"}},  
//             {"intt1_11", {11, "A2", 3, 0, "B1L106S"}},  
//             {"intt1_12", {12, "B1", 3, 0, "B0L105S"}},  
//             {"intt1_13", {13, "D1", 3, 0, "B1L007S"}},



//             {"intt2_0", {0, "A1", 4, 0, "B0L106S"}},  // note : intt 2
//             {"intt2_1", {1, "B1", 4, 0, "B0L006S"}},  
//             {"intt2_2", {2, "C1", 4, 0, "B0L107S"}},  
//             {"intt2_3", {3, "A2", 4, 0, "B1L108S"}},  
//             {"intt2_4", {4, "B2", 4, 0, "B1L008S"}},  
//             {"intt2_5", {5, "C2", 4, 0, "B1L109S"}},  
//             {"intt2_6", {6, "D1", 4, 0, "B1L009S"}},  

//             {"intt2_7", {7, "A1", 5, 0, "B0L007S"}},  
//             {"intt2_8", {8, "B3", 5, 0, "B0L108S"}},  
//             {"intt2_9", {9, "C1", 5, 0, "B0L008S"}},  
//             {"intt2_10", {10, "A2", 5, 0, "B1L110S"}},  
//             {"intt2_11", {11, "B2", 5, 0, "B1L010S"}},  
//             {"intt2_12", {12, "C2", 5, 0, "B1L111S"}},  
//             {"intt2_13", {13, "C3", 5, 0, "B1L011S"}},



//             {"intt3_0", {0, "A1", 6, 0, "B0L109S"}},  // note : intt 3
//             {"intt3_1", {1, "B1", 6, 0, "B0L009S"}},  
//             {"intt3_2", {2, "C1", 6, 0, "B0L110S"}},  
//             {"intt3_3", {3, "A2", 6, 0, "B1L112S"}},  
//             {"intt3_4", {4, "B3", 6, 0, "B1L012S"}},  
//             {"intt3_5", {5, "C2", 6, 0, "B1L113S"}},  
//             {"intt3_6", {6, "D1", 6, 0, "B1L013S"}},  

//             {"intt3_7", {7, "A1", 7, 0, "B0L010S"}},  
//             {"intt3_8", {8, "B1", 7, 0, "B0L111S"}},  
//             {"intt3_9", {9, "C1", 7, 0, "B0L011S"}},  
//             {"intt3_10", {10, "A2", 7, 0, "B1L114S"}},  
//             {"intt3_11", {11, "B3", 7, 0, "B1L014S"}},  
//             {"intt3_12", {12, "C3", 7, 0, "B1L115S"}},  
//             {"intt3_13", {13, "D2", 7, 0, "B1L015S"}},



//             {"intt4_0", {0,	"B1", 0, 1, "B1L101N"}},	// note : intt 4
//             {"intt4_1", {1,	"C2", 0, 1, "B0L000N"}},	
//             {"intt4_2", {2,	"D1", 0, 1, "B0L100N"}},	
//             {"intt4_3", {3,	"A2", 0, 1, "B1L001N"}},	
//             {"intt4_4", {4,	"B2", 0, 1, "B0L101N"}},	
//             {"intt4_5", {5,	"C3", 0, 1, "B1L000N"}},	
//             {"intt4_6", {6,	"D2", 0, 1, "B1L100N"}},	 

//             {"intt4_7", {7, "B1", 1, 1, "B0L002N"}},	 
//             {"intt4_8", {8, "C2", 1, 1, "B0L102N"}},	
//             {"intt4_9", {9, "D1", 1, 1, "B0L001N"}},	
//             {"intt4_10", {10, "A2", 1, 1, "B1L003N"}},	 
//             {"intt4_11", {11, "B2", 1, 1, "B1L103N"}},	
//             {"intt4_12", {12, "C3", 1, 1, "B1L002N"}},	
//             {"intt4_13", {13, "B3", 1, 1, "B1L102N"}},



//             {"intt5_0", {0, "C1", 2, 1, "B0L003N"}},	// note : intt 5
//             {"intt5_1", {1, "B1", 2, 1, "B0L104N"}},	
//             {"intt5_2", {2, "D2", 2, 1, "B0L103N"}},	 
//             {"intt5_3", {3, "D1", 2, 1, "B1L004N"}},	
//             {"intt5_4", {4, "A2", 2, 1, "B1L005N"}},	 
//             {"intt5_5", {5, "C2", 2, 1, "B1L104N"}},	
//             {"intt5_6", {6, "A1", 2, 1, "B1L105N"}},	

//             {"intt5_7", {7, "C3", 3, 1, "B1L107N"}},	
//             {"intt5_8", {8, "B1", 3, 1, "B1L007N"}},	
//             {"intt5_9", {9, "C1", 3, 1, "B1L006N"}},	
//             {"intt5_10", {10, "D2", 3, 1, "B1L106N"}},	
//             {"intt5_11", {11, "A1", 3, 1, "B0L005N"}},	
//             {"intt5_12", {12, "C2", 3, 1, "B0L105N"}},	
//             {"intt5_13", {13, "D1", 3, 1, "B0L004N"}},



//             {"intt6_0", {0, "A1", 4, 1, "B0L106N"}},	// note : intt 6
//             {"intt6_1", {1, "C3", 4, 1, "B0L006N"}},	
//             {"intt6_2", {2, "B2", 4, 1, "B0L107N"}},	
//             {"intt6_3", {3, "D1", 4, 1, "B1L108N"}},	
//             {"intt6_4", {4, "C2", 4, 1, "B1L008N"}},	
//             {"intt6_5", {5, "B3", 4, 1, "B1L109N"}},	
//             {"intt6_6", {6, "A2", 4, 1, "B1L009N"}},	

//             {"intt6_7", {7, "D1", 5, 1, "B0L007N"}},	
//             {"intt6_8", {8, "C3", 5, 1, "B0L108N"}},	
//             {"intt6_9", {9, "A2", 5, 1, "B0L008N"}},	
//             {"intt6_10", {10, "D2", 5, 1, "B1L110N"}},	  
//             {"intt6_11", {11, "C2", 5, 1, "B1L010N"}},	
//             {"intt6_12", {12, "B3", 5, 1, "B1L111N"}},	
//             {"intt6_13", {13, "A1", 5, 1, "B1L011N"}},  



//             {"intt7_0", {0, "B3", 6, 1, "B0L109N"}},	// note : intt 7
//             {"intt7_1", {1, "C1", 6, 1, "B0L009N"}},	
//             {"intt7_2", {2, "B1", 6, 1, "B0L110N"}},	
//             {"intt7_3", {3, "D1", 6, 1, "B1L112N"}},	
//             {"intt7_4", {4, "C2", 6, 1, "B1L012N"}},	
//             {"intt7_5", {5, "B2", 6, 1, "B1L113N"}},	
//             {"intt7_6", {6, "A1", 6, 1, "B1L013N"}},	

//             {"intt7_7", {7, "B2", 7, 1, "B0L010N"}},	
//             {"intt7_8", {8, "C2", 7, 1, "B0L111N"}},	
//             {"intt7_9", {9, "B1", 7, 1, "B0L011N"}},	  
//             {"intt7_10", {10, "D2", 7, 1, "B1L114N"}},	  
//             {"intt7_11", {11, "C3", 7, 1, "B1L014N"}},	
//             {"intt7_12", {12, "B3", 7, 1, "B1L115N"}},	
//             {"intt7_13", {13, "A1", 7, 1, "B1L015N"}}
//         };

//         pair<double,double> Get_dummy_ladder_XY(int chip_id, int chan_id);
//         pair<double,double> Get_self_rotation( pair<double,double> chan_pos, double angle );
        
// };

// InttClutering::InttClutering()
// {

// }

// pair<double,double> InttClutering::Get_XY_all(string ladder_name,/*string server, int module,*/ int chip_id, int chan_id)
// {
//     string ladder_pos_str = ladder_name;
//     // string ladder_pos_str = string(serverFC_toinfo_map[Form("%s_%i",server.c_str(),module)].Ladder);
    
//     int layer_index; 
//     if      (ladder_pos_str.substr(0,4) == "B0L0") layer_index = 0;
//     else if (ladder_pos_str.substr(0,4) == "B0L1") layer_index = 1;
//     else if (ladder_pos_str.substr(0,4) == "B1L0") layer_index = 2;
//     else if (ladder_pos_str.substr(0,4) == "B1L1") layer_index = 3;

//     int ladder_index = stoi(ladder_pos_str.substr(4,2));
    
    
//     double ladder_location_angle = ladder_location_angle_ini_correction[layer_index] -  (angle_increment[layer_index] * ladder_index);

//     double ring_pos_x = layer_raduis[layer_index] * cos(ladder_location_angle / (180/TMath::Pi()));
//     double ring_pos_y = layer_raduis[layer_index] * sin(ladder_location_angle / (180/TMath::Pi()));

//     double ladder_self_pos_X = ( fabs(Get_self_rotation(Get_dummy_ladder_XY(chip_id,chan_id),( ladder_self_angle_ini_correction[layer_index] - (angle_increment[layer_index] * ladder_index) )).first) < 0.0000001) ? 0 : Get_self_rotation(Get_dummy_ladder_XY(chip_id,chan_id),( ladder_self_angle_ini_correction[layer_index] - (angle_increment[layer_index] * ladder_index) )).first;
//     double ladder_self_pos_Y = ( fabs(Get_self_rotation(Get_dummy_ladder_XY(chip_id,chan_id),( ladder_self_angle_ini_correction[layer_index] - (angle_increment[layer_index] * ladder_index) )).second) < 0.0000001) ? 0 : Get_self_rotation(Get_dummy_ladder_XY(chip_id,chan_id),( ladder_self_angle_ini_correction[layer_index] - (angle_increment[layer_index] * ladder_index) )).second;

//     return {ring_pos_x +  ladder_self_pos_X, ring_pos_y + ladder_self_pos_Y};
// }

// pair<double,double> InttClutering::Get_dummy_ladder_XY(int chip_id, int chan_id)
// {
//     // note : chip_id 1 ~ 13, 14 ~ 26
//     int row_index = (chip_id > 0 && chip_id < 14) ? 0 : 1;

//     double dummy_X = 0;
//     double dummy_Y = (row_index == 0) ? -0.039 - (127 - chan_id) * 0.078 : 0.039 + (127 - chan_id) * 0.078;

//     return {dummy_X,dummy_Y};
// }

// pair<double,double> InttClutering::Get_self_rotation( pair<double,double> chan_pos, double angle )
// {
//     return {chan_pos.second * cos(angle / (180/TMath::Pi())), chan_pos.second * sin(angle / (180/TMath::Pi()))};
// }