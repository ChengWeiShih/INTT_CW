#include "InttConversion.h"

// struct hit_info {
//     int chip_id;
//     int chan_id;
//     int adc;
// };

// struct clu_info {
//     int column;
//     int chip_id;
//     double avg_chan;
//     int sum_adc;
//     int size;

//     double x;
//     double y;
//     int layer;
//     double phi;
// };

// // note : assumption, in one event, single channel can only be fired once. 
// vector<clu_info> clustering(vector<hit_info> single_event)
// {
//     int hit_seat[13][256]; memset(hit_seat, -1, sizeof(hit_seat)); // note : one event, one half-ladder
//     vector<hit_info>hit_vec; hit_vec.clear(); // note : chip_id is not used

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
//                 hit_info hit_unit;
//                 hit_unit.chan_id = ch;
//                 hit_unit.adc = hit_seat[col][ch];
                
//                 hit_vec.push_back(hit_unit);
//             }

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

void Clustering_testing()
{
    // vector<hit_info> aaa_vec = {
    //     {1,127,0},
    //     {14,127,0}
    // };


    // vector<clu_info> out_vec = clustering(aaa_vec);

    // for (int i = 0; i < out_vec.size(); i++)
    // {
    //     cout<<i<<" column "<<out_vec[i].column<<" chip "<<out_vec[i].chip_id<<" chan "<<out_vec[i].avg_chan<<" adc "<<out_vec[i].sum_adc<<" size "<<out_vec[i].size<<endl;
    // }










        

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
    bbb -> SetMarkerColor(2);
    
    
    aaa -> Draw("ap");
    bbb -> Draw("p same");


}