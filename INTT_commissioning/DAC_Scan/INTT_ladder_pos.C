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

double layer_raduis[4] = {71.844000, 77.284000, 96.764000, 102.58400}; // note : unit : mm
int N_ladder[4] = {12, 12, 16, 16};
double angle_increment[4] = {30,30,22.5,22.5}; // note : ladder rotation
double ladder_location_angle_ini_correction[4] = {255,270,258.75,270};
double ladder_self_angle_ini_correction[4] = {345,360,348.75,360};
double pos_ini_correction[4];
double channel_width = 0.078; // note : unit : mm

double Get_XY_all(string ladder_name, int ladder_index/*string server, int module, int chip_id, int chan_id*/)
{
    string ladder_pos_str = ladder_name;
    // string ladder_pos_str = string(serverFC_toinfo_map[Form("%s_%i",server.c_str(),module)].Ladder);
    
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

    // int ladder_index = stoi(ladder_pos_str.substr(4,2));

    // note : 0, 1, 2, 3 : south, id 0
    // note : 4, 5, 6, 7 : north, id 1
    // int ladder_SN = ( stoi(server.substr(4,1)) < 4 ) ? 0 : 1;
    // cout<<"test : ladder_SN : "<<ladder_SN<<endl;

    
    double ladder_location_angle = ladder_location_angle_ini_correction[layer_index] -  (angle_increment[layer_index] * ladder_index);

    double ring_pos_x = layer_raduis[layer_index] * cos(ladder_location_angle / (180/TMath::Pi()));
    double ring_pos_y = layer_raduis[layer_index] * sin(ladder_location_angle / (180/TMath::Pi()));


    double final_pos_X = ( fabs(ring_pos_x) < 0.000001 ) ? 0 : ring_pos_x;
    double final_pos_Y = ( fabs(ring_pos_y) < 0.000001 ) ? 0 : ring_pos_y;
    int    final_pos_layer = (layer_index == 0 || layer_index == 1) ? 0 : 1;
    // double final_pos_phi = (final_pos_Y < 0) ? atan2(final_pos_Y,final_pos_X) * (180./TMath::Pi()) + 360 : atan2(final_pos_Y,final_pos_X) * (180./TMath::Pi());

    // double final_pos_Z = ideal_z_pos[(chip_id - 1) % 13] * pow(-1, 1 - ladder_SN);

    // cout<<"("<<setprecision(8)<<final_pos_X<<","<<final_pos_Y<<"),";

    // return {final_pos_X,final_pos_Y,final_pos_Z,final_pos_layer,final_pos_phi};

    return ladder_self_angle_ini_correction[layer_index] - (angle_increment[layer_index] * ladder_index);
}

void INTT_ladder_pos()
{
    string ladder_string[4] = {"B0L0","B0L1","B1L0","B1L1"};
    int N_ladder[4] = {12,12,16,16};
    string ladder_index_string[16] = {"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15"};

    for (int i = 0; i < 4; i++)
    {
        for (int i1 = 0; i1 < N_ladder[i]; i1++)
        {
            cout<<ladder_string[i]<<ladder_index_string[i1]<<" "<<Get_XY_all(ladder_string[i],i1)<<endl;
        }
        // cout<<" "<<endl;
        cout<<" "<<endl;
        // cout<<"-------------"<<endl;

    }
}