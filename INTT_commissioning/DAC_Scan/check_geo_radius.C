#include "../DAC_Scan/InttClustering.h"
void check_geo_radius()
{
    int geo_mode_id = 1;
    string conversion_mode = (geo_mode_id == 0) ? "ideal" : "survey_1_XYAlpha_Peek";
    double peek = 3.32405;

    int N_ladder[4] = {12, 12, 16, 16};
    string ladder_index_string[16] = {"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15"};

    vector<double> x_vec; x_vec.clear();
    vector<double> y_vec; y_vec.clear();

    vector<double> x_vec_2; x_vec_2.clear();
    vector<double> y_vec_2; y_vec_2.clear();

    vector<double> inner_all_r; inner_all_r.clear();
    vector<double> outer_all_r; outer_all_r.clear();

    // for (int server_i = 0; server_i < 4; server_i++)
    // {
    //     for (int FC_i = 0; FC_i < 14; FC_i++)
    //     {
    //         ladder_line -> DrawLine(
    //             InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,14,0,conversion_mode,peek).x, InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,14,0,conversion_mode,peek).y,
    //             InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,1,0,conversion_mode,peek).x, InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,1,0,conversion_mode,peek).y
    //         );
            
    //         x1 = InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,14,0,conversion_mode,peek).x;
    //         y1 = InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,14,0,conversion_mode,peek).y;

    //         x2 = InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,1,0,conversion_mode,peek).x;
    //         y2 = InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,1,0,conversion_mode,peek).y;

    //         cout<<"("<<x1<<", "<<y1<<", "<<x2<<", "<<y2<<")"<<endl;

    //         // tree_out -> Fill();

    //     }
    // }

    pos_str ch_pos_14;
    pos_str ch_pos_1;

    for (int server_i = 0; server_i < 4; server_i++)
    {
        for (int FC_i = 0; FC_i < 14; FC_i++)
        {
            for (int chan_i = 0; chan_i < 128; chan_i++)
            {
                
                ch_pos_14 = InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,14,chan_i,conversion_mode,peek);
                if (ch_pos_14.layer == 0)
                {
                    inner_all_r.push_back(sqrt(pow(ch_pos_14.x,2)+pow(ch_pos_14.y,2)));
                }
                else if (ch_pos_14.layer == 1)
                {
                    outer_all_r.push_back(sqrt(pow(ch_pos_14.x,2)+pow(ch_pos_14.y,2)));
                }
                

                ch_pos_1 = InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,1,chan_i,conversion_mode,peek);
                if (ch_pos_1.layer == 0)
                {
                    inner_all_r.push_back(sqrt(pow(ch_pos_1.x,2)+pow(ch_pos_1.y,2)));
                }
                else if (ch_pos_1.layer == 1)
                {
                    outer_all_r.push_back(sqrt(pow(ch_pos_1.x,2)+pow(ch_pos_1.y,2)));
                }

                // x_vec.push_back(InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,14,chan_i,conversion_mode,peek).x);
                // y_vec.push_back(InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,14,chan_i,conversion_mode,peek).y);

                // x_vec_2.push_back(InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,1,chan_i,conversion_mode,peek).x);
                // y_vec_2.push_back(InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,1,chan_i,conversion_mode,peek).y);
                // cout<<"("<<InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,14,chan_i).first<<","<<InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,14,chan_i).second<<"),";

                // cout<<"("<<Get_XY_all(Form("%s%s",layer_text[i].c_str(),ladder_index_string[i1].c_str()),used_chip,127).first<<","<<Get_XY_all(Form("%s%s",layer_text[i].c_str(),ladder_index_string[i1].c_str()),used_chip,127).second<<")";
            }
        }
    }

    cout<<"inner : "<<*max_element(inner_all_r.begin(), inner_all_r.end())<<" "<<*min_element(inner_all_r.begin(), inner_all_r.end())<<endl;
    cout<<"outer : "<<*max_element(outer_all_r.begin(), outer_all_r.end())<<" "<<*min_element(outer_all_r.begin(), outer_all_r.end())<<endl;


}