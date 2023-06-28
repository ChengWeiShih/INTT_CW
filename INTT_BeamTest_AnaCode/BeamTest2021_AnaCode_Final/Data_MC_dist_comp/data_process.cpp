#include "data_process.h"

// data_process::data_process(TString class_name, TString folder_direction, TString cluster_file_name, int study_chip, double amount_of_alignment, double slope_correction)
//     :class_name(class_name),
//     folder_direction(folder_direction),
//     cluster_file_name(cluster_file_name),
//     study_chip(study_chip),
//     amount_of_alignment(amount_of_alignment),
//     slope_correction(slope_correction),
//     //input_vec(),
//     file_name("not_used"),
//     cluster_adc_value_requirement(22.5), // note : " > the_value"
//     noise_hit_distance(0.234), // note : currently, the width of three channels
//     boundary_cut(5), 
//     cluster_size_requirement_combine (1),
//     cluster_size_requirement (0),
//     hit_allowed_in_adjacent_layers (1)
// {
//     cout<<"the class : "<<class_name<<" has been initiated "<<endl;
// }    

// note : merge case
data_process::data_process(int Initialize_mode, TString class_name, TString folder_direction, TString cluster_file_name, int study_chip, double amount_of_alignment, double slope_correction)
    :class_name(class_name),
    folder_direction(folder_direction),
    cluster_file_name(cluster_file_name),
    study_chip(study_chip),
    amount_of_alignment(amount_of_alignment),
    slope_correction(slope_correction),
    //input_vec(),
    file_name("not_used"),
    cluster_adc_value_requirement(22.5), // note : " > the_value"
    noise_hit_distance(0.234), // note : currently, the width of three channels
    boundary_cut(5), 
    slope_cut(0.01),
    cluster_size_requirement_combine (1),
    cluster_size_requirement (0),
    hit_allowed_in_adjacent_layers (1)
{


    if ( Initialize_mode == 1 )
    {
        cout<<"Now you are using the initialize mode : 1"<<endl;
        cout<<"The misalignment correction is applied to single column of L1"<<endl;

        // note : Now the amount of the alignment correction can be given here, don't need to reproduce a new file.
        cluster_str_vec_predefined = data_process::cluster_read_and_build();   
        cluster_reformat_str_vec_predefined = data_process::cluster_reformat(cluster_str_vec_predefined);
        // scattering_hist_predefined = data_process::scattering_dist(cluster_reformat_str_vec_predefined);

        cout<<"the class : "<<class_name<<" has been initiated by using the mode 1 "<<endl;
    }
    else if ( Initialize_mode == 2 )
    {
        cout<<"Now you are using the initialize mode : 2"<<endl;
        cout<<"The misalignment correction is applied to all columns of L1"<<endl;
        // note : Now the amount of the alignment correction can be given here, don't need to reproduce a new file.
        cluster_str_vec_predefined = data_process::cluster_read_and_build();   
        cluster_reformat_str_vec_predefined = data_process::cluster_reformat_all_column_correct(cluster_str_vec_predefined);
        // scattering_hist_predefined = data_process::scattering_dist(cluster_reformat_str_vec_predefined);

        cout<<"the class : "<<class_name<<" has been initiated by using the mode 2 "<<endl;
    }
    
}

// // note : this initiater is for MC
// data_process::data_process(TString class_name, TString folder_direction, TString MC_raw_file_name, TString cluster_file_name, int study_chip, double amount_of_alignment, double slope_correction)
//     :class_name(class_name),
//     folder_direction(folder_direction),
//     MC_raw_file_name(MC_raw_file_name),
//     cluster_file_name(cluster_file_name),
//     study_chip(study_chip),
//     amount_of_alignment(amount_of_alignment),
//     slope_correction(slope_correction),
//     //input_vec(),
//     file_name("not_used"),
//     cluster_adc_value_requirement(22.5), // note : " > the_value"
//     noise_hit_distance(0.234), // note : currently, the width of three channels
//     boundary_cut(5), 
//     cluster_size_requirement_combine (1),
//     cluster_size_requirement (0),
//     hit_allowed_in_adjacent_layers (1)
// {
//     cout<<"=========== =========== =========== =========== Note : =========== =========== =========== ==========="<<endl;
//     cout<<"this is for MC !!!"<<endl;
//     cout<<"the class : "<<class_name<<" has been initiated "<<endl;
// }


void data_process::para_check()
{   

    cout<<"=========== =========== =========== =========== para check =========== =========== =========== ==========="<<endl;
    cout<<"actual_xpos : "<<actual_xpos[0]<<" "<<actual_xpos[1]<<" "<<actual_xpos[2]<<endl;

    cout<<"INTT_strip_width : "<<INTT_strip_width<<endl;
    cout<<"lower_section_initial : "<<lower_section_initial<<endl;
    cout<<"upper_section_initial : "<<upper_section_initial<<endl;

    cout<<"folder_direction : "<<folder_direction<<endl;
    cout<<"file_name : "<<file_name<<endl;
    cout<<"cluster_file_name : "<<cluster_file_name<<endl;

    cout<<"study_chip : "<<study_chip<<endl;;
    cout<<"amount_of_alignment : "<<amount_of_alignment<<endl;;
    cout<<"slope_correction : "<<slope_correction<<endl;;
    
    cout<< "cluster_adc_value_requirement : "<<cluster_adc_value_requirement<<endl; 
    cout<< "noise_hit_distance : "<<noise_hit_distance<<endl;
    cout<< "boundary_cut : "<<boundary_cut<<endl; 
    cout<< "cluster_size_requirement_combine : "<<cluster_size_requirement_combine<<endl; 
    cout<< "cluster_size_requirement : "<<cluster_size_requirement<<endl; 
    cout<< "hit_allowed_in_adjacent_layers : "<<hit_allowed_in_adjacent_layers<<endl;
    cout<<"=========== =========== =========== =========== para check =========== =========== =========== ==========="<<endl;
}

TString data_process::GetClassName()
{   
    return class_name;
}

vector<cluster_str> data_process::cluster_read_and_build ()
{
    // note : the root file of the cluster information, chip, layer, Nhits, position.
    // note : it is in different file, no "_test"
    // TFile* cluster_file = TFile::Open (Form("%s/folder_%s_chip_%i/%s.root", folder_direction.Data(), file_name.Data(),study_chip,cluster_file_name.Data()), "read");
    TFile* cluster_file = TFile::Open (Form("%s/%s.root", folder_direction.Data(),cluster_file_name.Data()), "read");
    TTree *cluster_tree = (TTree *)cluster_file->Get("cluster_info");
    int cluster_tree_size = cluster_tree->GetEntriesFast();
    cout<<"Tree cluster size : "<<cluster_tree_size<<endl;

    int cluster_eID;
    vector<int> * cluster_layer = new vector<int>();
    vector<int> * cluster_chip = new vector<int>();
    vector<int> * cluster_hit = new vector<int>();
    vector<double> * cluster_position = new vector<double>();
    vector<double> * cluster_adc = new vector<double>();

    cluster_tree -> SetBranchAddress("eID",&cluster_eID);
    cluster_tree -> SetBranchAddress("layer",&cluster_layer);
    cluster_tree -> SetBranchAddress("chip",&cluster_chip);
    cluster_tree -> SetBranchAddress("Nhit",&cluster_hit);
    cluster_tree -> SetBranchAddress("position",&cluster_position);
    cluster_tree -> SetBranchAddress("cluster_adc",&cluster_adc);

    vector<cluster_str> output_vec; output_vec.clear();
    cluster_str fill_unit;

    fill_unit.cluster_layer.clear();
    fill_unit.cluster_chip.clear();
    fill_unit.cluster_hit.clear();
    fill_unit.cluster_position.clear();
    fill_unit.cluster_adc.clear();

    for (int i = 0; i < cluster_tree_size; i++)
    {
        cluster_tree -> GetEntry(i);

        output_vec.push_back(fill_unit);

        output_vec[output_vec.size()-1].cluster_eID = cluster_eID;
        for ( int i1 = 0; i1 < cluster_layer->size(); i1++ )
        {
            output_vec[output_vec.size()-1].cluster_layer.push_back(cluster_layer->at(i1));
            output_vec[output_vec.size()-1].cluster_chip.push_back(cluster_chip->at(i1));
            output_vec[output_vec.size()-1].cluster_hit.push_back(cluster_hit->at(i1));
            output_vec[output_vec.size()-1].cluster_position.push_back(cluster_position->at(i1));
            output_vec[output_vec.size()-1].cluster_adc.push_back(cluster_adc->at(i1));
        }


        // cout<<"-------------- -------------- -------------- -------------- --------------"<<endl;
        // if (1==1) {cout<<i<<" event check : "<<cluster_chip->size()<<endl;}
        // if (1==1) {cout<<i<<" event check : "<< output_vec[0].cluster_chip.size()<< " "<<output_vec[0].cluster_position[0] <<endl;}

    }

    
    // cout<<"event check out but in : "<< output_vec[0].cluster_chip.size() <<endl;
    cout<<"cluster size check : "<<output_vec.size()<<endl;
    return output_vec;
}

// void data_process::MC_cluster_file_generator ()
// {
//     TFile *f1 = new TFile (Form("%s/%s.root", folder_direction.Data(), file_name.Data()), "read");
// 	TTree *tree_both_in = (TTree *)f1->Get("tree_both");

//     long event_N = tree_both_in -> GetEntriesFast();
// 	printf("# of event in file %s.root : %li \n",file_name.Data(),event_N);



// }

// note : change the structure of the data and apply the single column offset
vector<cluster_reformat_str> data_process::cluster_reformat ( vector<cluster_str> input_vec )
{

    vector<cluster_reformat_str> output_vec; output_vec.clear();
    cluster_reformat_str fill_unit;

    for (int i = 0; i < 13; i++)
    {
        fill_unit.cluster_hit[0][i].clear(); fill_unit.cluster_hit[1][i].clear(); fill_unit.cluster_hit[2][i].clear();
        fill_unit.cluster_pos[0][i].clear(); fill_unit.cluster_pos[1][i].clear(); fill_unit.cluster_pos[2][i].clear();
        fill_unit.cluster_adc[0][i].clear(); fill_unit.cluster_adc[1][i].clear(); fill_unit.cluster_adc[2][i].clear();
    }

    for (int i = 0; i < input_vec.size(); i++ )
    {
        output_vec.push_back(fill_unit);
        
        // note : fill the eID
        output_vec[ output_vec.size() - 1 ].eID = input_vec[i].cluster_eID;
        
        for (int i1 = 0; i1 < input_vec[i].cluster_layer.size(); i1++)
        {
            output_vec[ output_vec.size() - 1 ].cluster_hit[ input_vec[i].cluster_chip[i1] - 1 ][ input_vec[i].cluster_layer[i1] ].push_back( input_vec[i].cluster_hit[i1] );
            output_vec[ output_vec.size() - 1 ].cluster_adc[ input_vec[i].cluster_chip[i1] - 1 ][ input_vec[i].cluster_layer[i1] ].push_back( input_vec[i].cluster_adc[i1] );
            
            if (input_vec[i].cluster_chip[i1] == study_chip && input_vec[i].cluster_layer[i1] == 1) // note : the study_chip of the middle layer is aligned.
            {
                output_vec[ output_vec.size() - 1 ].cluster_pos[ input_vec[i].cluster_chip[i1] - 1 ][ input_vec[i].cluster_layer[i1] ].push_back( input_vec[i].cluster_position[i1] + amount_of_alignment);
            }
            else 
            {
                output_vec[ output_vec.size() - 1 ].cluster_pos[ input_vec[i].cluster_chip[i1] - 1 ][ input_vec[i].cluster_layer[i1] ].push_back( input_vec[i].cluster_position[i1] );
            }
            
        }
    }

    cout<<"reformat output check : "<<output_vec.size()<<endl;
    return output_vec;
    
}

// note : change the structure of the data and apply the misalignment correction to L1, all columns
vector<cluster_reformat_str> data_process::cluster_reformat_all_column_correct ( vector<cluster_str> input_vec )
{

    vector<cluster_reformat_str> output_vec; output_vec.clear();
    cluster_reformat_str fill_unit;

    for (int i = 0; i < 13; i++)
    {
        fill_unit.cluster_hit[0][i].clear(); fill_unit.cluster_hit[1][i].clear(); fill_unit.cluster_hit[2][i].clear();
        fill_unit.cluster_pos[0][i].clear(); fill_unit.cluster_pos[1][i].clear(); fill_unit.cluster_pos[2][i].clear();
        fill_unit.cluster_adc[0][i].clear(); fill_unit.cluster_adc[1][i].clear(); fill_unit.cluster_adc[2][i].clear();
    }

    for (int i = 0; i < input_vec.size(); i++ )
    {
        output_vec.push_back(fill_unit);
        
        // note : fill the eID
        output_vec[ output_vec.size() - 1 ].eID = input_vec[i].cluster_eID;
        
        for (int i1 = 0; i1 < input_vec[i].cluster_layer.size(); i1++)
        {
            output_vec[ output_vec.size() - 1 ].cluster_hit[ input_vec[i].cluster_chip[i1] - 1 ][ input_vec[i].cluster_layer[i1] ].push_back( input_vec[i].cluster_hit[i1] );
            output_vec[ output_vec.size() - 1 ].cluster_adc[ input_vec[i].cluster_chip[i1] - 1 ][ input_vec[i].cluster_layer[i1] ].push_back( input_vec[i].cluster_adc[i1] );
            
            if (input_vec[i].cluster_layer[i1] == 1) // note : the study_chip of the middle layer is aligned.
            {
                output_vec[ output_vec.size() - 1 ].cluster_pos[ input_vec[i].cluster_chip[i1] - 1 ][ input_vec[i].cluster_layer[i1] ].push_back( input_vec[i].cluster_position[i1] + amount_of_alignment);
            }
            else 
            {
                output_vec[ output_vec.size() - 1 ].cluster_pos[ input_vec[i].cluster_chip[i1] - 1 ][ input_vec[i].cluster_layer[i1] ].push_back( input_vec[i].cluster_position[i1] );
            }
            
        }
    }

    cout<<"reformat output check : "<<output_vec.size()<<endl;
    return output_vec;
    
}

TH1F * data_process::Get_scattering_dist (int SD_Nbins, double SD_Ledge, double SD_Redge, vector<cluster_reformat_str> input_cluster_vec)
{
    vector<int> receiver_unit_clu_size[13][3]; // note : for abbreviation, for hit 
    vector<double> receiver_unit_clu_pos[13][3];
    vector<double> receiver_unit_clu_adc[13][3];

    int event_counting_1  = 0;
    int event_counting_2  = 0;
    int event_counting_3  = 0;
    int event_counting_4  = 0;
    int event_counting_5  = 0;
    int event_counting_6  = 0;
    int event_counting_7  = 0;
    int event_counting_8  = 0;
    int event_counting_9  = 0;
    int event_counting_10 = 0;
    int event_counting_11 = 0;
    int event_counting_12 = 0;

    double event_scattering;

    TH1F * scattering_hist = new TH1F ("","",SD_Nbins,SD_Ledge,SD_Redge);

    for (int i = 0; i < input_cluster_vec.size(); i++)
    {
        if (i % 10000 == 0){ cout<<"Doing the scattering_dist, eID : "<<input_cluster_vec[i].eID<<endl;}

        // note : abbreviation
        for (int i1 = 0; i1 < 13; i1++)
        {
            for (int i2 = 0; i2 < 3; i2++)
            {
                receiver_unit_clu_size[i1][i2] = input_cluster_vec[i].cluster_hit[ i1 ][i2];
                receiver_unit_clu_pos[i1][i2] = input_cluster_vec[i].cluster_pos[ i1 ][i2];
                receiver_unit_clu_adc[i1][i2] = input_cluster_vec[i].cluster_adc[ i1 ][i2];

            }
        }

        event_counting_1 += 1;
        
        // note : zero cluster in adjacent chips
        if ( (receiver_unit_clu_size[study_chip-1-1][0].size()+receiver_unit_clu_size[study_chip-1-1][1].size()+receiver_unit_clu_size[study_chip-1-1][2].size()+receiver_unit_clu_size[study_chip-1+1][0].size()+receiver_unit_clu_size[study_chip-1+1][1].size()+receiver_unit_clu_size[study_chip-1+1][2].size()) != 0 ) continue;
        event_counting_2 += 1;

        // note : single cluster in l0, l1 and l2
        if (receiver_unit_clu_size[study_chip-1][0].size() != hit_allowed_in_adjacent_layers || receiver_unit_clu_size[study_chip-1][1].size() != hit_allowed_in_adjacent_layers || receiver_unit_clu_size[study_chip-1][2].size() != hit_allowed_in_adjacent_layers) continue;
        event_counting_3 += 1;
        
        // note : single-hit cluster required
        if ( receiver_unit_clu_size[study_chip-1][0][0] != 1 || receiver_unit_clu_size[study_chip-1][1][0] != 1 || receiver_unit_clu_size[study_chip-1][2][0] != 1 ) continue;
        event_counting_4 += 1;

        // // note : standalone cluster adc cut
        // if ( receiver_unit_clu_adc[study_chip-1][0][0] <= cluster_adc_value_requirement || receiver_unit_clu_adc[study_chip-1][1][0] <= cluster_adc_value_requirement || receiver_unit_clu_adc[study_chip-1][2][0] <= cluster_adc_value_requirement ) continue;
        // event_counting_5 += 1;

        event_scattering = ((receiver_unit_clu_pos[study_chip-1][2][0] - receiver_unit_clu_pos[study_chip-1][1][0]) / actual_xpos[1]) - ((receiver_unit_clu_pos[study_chip-1][1][0] - receiver_unit_clu_pos[study_chip-1][0][0]) / actual_xpos[1]);
        
        scattering_hist -> Fill(event_scattering);
    }


    cout<<"event counting 1 \t : "<<event_counting_1<<endl;
    cout<<"event counting 2 \t : "<<event_counting_2<<endl;
    cout<<"event counting 3 \t : "<<event_counting_3<<endl;
    cout<<"event counting 4 \t : "<<event_counting_4<<endl;

    return scattering_hist;
}

TH1F * data_process::Get_track_candidate_slope (int SD_Nbins, double SD_Ledge, double SD_Redge, vector<cluster_reformat_str> input_cluster_vec)
{
    vector<int> receiver_unit_clu_size[13][3]; // note : for abbreviation, for hit 
    vector<double> receiver_unit_clu_pos[13][3];
    vector<double> receiver_unit_clu_adc[13][3];

    int event_counting_1  = 0;
    int event_counting_2  = 0;
    int event_counting_3  = 0;
    int event_counting_4  = 0;
    int event_counting_5  = 0;
    int event_counting_6  = 0;
    int event_counting_7  = 0;
    int event_counting_8  = 0;
    int event_counting_9  = 0;
    int event_counting_10 = 0;
    int event_counting_11 = 0;
    int event_counting_12 = 0;

    // double event_scattering;
    double hit2_slope;

    TH1F * candi_track_slope_hist = new TH1F ("","",SD_Nbins,SD_Ledge,SD_Redge);

    for (int i = 0; i < input_cluster_vec.size(); i++)
    {
        if (i % 10000 == 0){ cout<<"Doing the track candidate slope dist, eID : "<<input_cluster_vec[i].eID<<endl;}

        // note : abbreviation
        for (int i1 = 0; i1 < 13; i1++)
        {
            for (int i2 = 0; i2 < 3; i2++)
            {
                receiver_unit_clu_size[i1][i2] = input_cluster_vec[i].cluster_hit[ i1 ][i2];
                receiver_unit_clu_pos[i1][i2] = input_cluster_vec[i].cluster_pos[ i1 ][i2];
                receiver_unit_clu_adc[i1][i2] = input_cluster_vec[i].cluster_adc[ i1 ][i2];

            }
        }

        event_counting_1 += 1;
        
        // note : zero cluster in adjacent chips
        if ( (receiver_unit_clu_size[study_chip-1-1][0].size()+receiver_unit_clu_size[study_chip-1-1][1].size()+receiver_unit_clu_size[study_chip-1-1][2].size()+receiver_unit_clu_size[study_chip-1+1][0].size()+receiver_unit_clu_size[study_chip-1+1][1].size()+receiver_unit_clu_size[study_chip-1+1][2].size()) != 0 ) continue;
        event_counting_2 += 1;

        // note : single cluster in l0 and l2
        if (receiver_unit_clu_size[study_chip-1][0].size() != hit_allowed_in_adjacent_layers || receiver_unit_clu_size[study_chip-1][2].size() != hit_allowed_in_adjacent_layers) continue;
        event_counting_3 += 1;

        hit2_slope = (receiver_unit_clu_pos[study_chip-1][2][0] - receiver_unit_clu_pos[study_chip-1][0][0]) / actual_xpos[2] + slope_correction;
        
        candi_track_slope_hist -> Fill(hit2_slope);
    }


    cout<<"event counting 1 \t : "<<event_counting_1<<endl;
    cout<<"event counting 2 \t : "<<event_counting_2<<endl;
    cout<<"event counting 3 \t : "<<event_counting_3<<endl;
    // cout<<"event counting 4 \t : "<<event_counting_4<<endl;

    return candi_track_slope_hist;
}

TH1F * data_process::Get_residual_dist (int SD_Nbins, double SD_Ledge, double SD_Redge, vector<cluster_reformat_str> input_cluster_vec)
{
    vector<int> receiver_unit_clu_size[13][3]; // note : for abbreviation, for hit 
    vector<double> receiver_unit_clu_pos[13][3];
    vector<double> receiver_unit_clu_adc[13][3];

    int event_counting_1  = 0;
    int event_counting_2  = 0;
    int event_counting_3  = 0;
    int event_counting_4  = 0;
    int event_counting_5  = 0;
    int event_counting_6  = 0;
    int event_counting_7  = 0;
    int event_counting_8  = 0;
    int event_counting_9  = 0;
    int event_counting_10 = 0;
    int event_counting_11 = 0;
    int event_counting_12 = 0;

    double event_residual;

    TH1F * residual_hist = new TH1F ("","",SD_Nbins,SD_Ledge,SD_Redge);

    for (int i = 0; i < input_cluster_vec.size(); i++)
    {
        if (i % 10000 == 0){ cout<<"Doing the residual_dist, eID : "<<input_cluster_vec[i].eID<<endl;}

        // note : abbreviation
        for (int i1 = 0; i1 < 13; i1++)
        {
            for (int i2 = 0; i2 < 3; i2++)
            {
                receiver_unit_clu_size[i1][i2] = input_cluster_vec[i].cluster_hit[ i1 ][i2];
                receiver_unit_clu_pos[i1][i2] = input_cluster_vec[i].cluster_pos[ i1 ][i2];
                receiver_unit_clu_adc[i1][i2] = input_cluster_vec[i].cluster_adc[ i1 ][i2];

            }
        }

        event_counting_1 += 1;
        
        // note : zero cluster in adjacent chips
        if ( (receiver_unit_clu_size[study_chip-1-1][0].size()+receiver_unit_clu_size[study_chip-1-1][1].size()+receiver_unit_clu_size[study_chip-1-1][2].size()+receiver_unit_clu_size[study_chip-1+1][0].size()+receiver_unit_clu_size[study_chip-1+1][1].size()+receiver_unit_clu_size[study_chip-1+1][2].size()) != 0 ) continue;
        event_counting_2 += 1;

        // note : single cluster in l0, l1 and l2
        if (receiver_unit_clu_size[study_chip-1][0].size() != hit_allowed_in_adjacent_layers || receiver_unit_clu_size[study_chip-1][1].size() != hit_allowed_in_adjacent_layers || receiver_unit_clu_size[study_chip-1][2].size() != hit_allowed_in_adjacent_layers) continue;
        event_counting_3 += 1;
        
        // note : single-hit cluster required
        if ( receiver_unit_clu_size[study_chip-1][0][0] != 1 || receiver_unit_clu_size[study_chip-1][1][0] != 1 || receiver_unit_clu_size[study_chip-1][2][0] != 1 ) continue;
        event_counting_4 += 1;

        // // note : standalone cluster adc cut
        // if ( receiver_unit_clu_adc[study_chip-1][0][0] <= cluster_adc_value_requirement || receiver_unit_clu_adc[study_chip-1][1][0] <= cluster_adc_value_requirement || receiver_unit_clu_adc[study_chip-1][2][0] <= cluster_adc_value_requirement ) continue;
        // event_counting_5 += 1;
        
        event_residual = receiver_unit_clu_pos[study_chip-1][1][0] - (receiver_unit_clu_pos[study_chip-1][2][0] + receiver_unit_clu_pos[study_chip-1][0][0])/2.;

        // event_residual = ((receiver_unit_clu_pos[study_chip-1][2][0] - receiver_unit_clu_pos[study_chip-1][1][0]) / actual_xpos[1]) - ((receiver_unit_clu_pos[study_chip-1][1][0] - receiver_unit_clu_pos[study_chip-1][0][0]) / actual_xpos[1]);
        
        residual_hist -> Fill(event_residual);
    }


    cout<<"event counting 1 \t : "<<event_counting_1<<endl;
    cout<<"event counting 2 \t : "<<event_counting_2<<endl;
    cout<<"event counting 3 \t : "<<event_counting_3<<endl;
    cout<<"event counting 4 \t : "<<event_counting_4<<endl;

    return residual_hist;
}

TH1F * data_process::Get_alignment_dist (int SD_Nbins, double SD_Ledge, double SD_Redge, vector<cluster_reformat_str> input_cluster_vec)
{
    vector<int> receiver_unit_clu_size[13][3]; // note : for abbreviation, for hit 
    vector<double> receiver_unit_clu_pos[13][3];
    vector<double> receiver_unit_clu_adc[13][3];

    int event_counting_1  = 0;
    int event_counting_2  = 0;
    int event_counting_3  = 0;
    int event_counting_4  = 0;
    int event_counting_5  = 0;
    int event_counting_6  = 0;
    int event_counting_7  = 0;
    int event_counting_8  = 0;
    int event_counting_9  = 0;
    int event_counting_10 = 0;
    int event_counting_11 = 0;
    int event_counting_12 = 0;

    double event_residual;

    TH1F * residual_hist = new TH1F ("","",SD_Nbins,SD_Ledge,SD_Redge);

    for (int i = 0; i < input_cluster_vec.size(); i++)
    {
        if (i % 10000 == 0){ cout<<"Doing the residual_dist, eID : "<<input_cluster_vec[i].eID<<endl;}

        // note : abbreviation
        for (int i1 = 0; i1 < 13; i1++)
        {
            for (int i2 = 0; i2 < 3; i2++)
            {
                receiver_unit_clu_size[i1][i2] = input_cluster_vec[i].cluster_hit[ i1 ][i2];
                receiver_unit_clu_pos[i1][i2] = input_cluster_vec[i].cluster_pos[ i1 ][i2];
                receiver_unit_clu_adc[i1][i2] = input_cluster_vec[i].cluster_adc[ i1 ][i2];

            }
        }

        event_counting_1 += 1;
        
        // note : zero cluster in adjacent chips
        if ( (receiver_unit_clu_size[study_chip-1-1][0].size()+receiver_unit_clu_size[study_chip-1-1][1].size()+receiver_unit_clu_size[study_chip-1-1][2].size()+receiver_unit_clu_size[study_chip-1+1][0].size()+receiver_unit_clu_size[study_chip-1+1][1].size()+receiver_unit_clu_size[study_chip-1+1][2].size()) != 0 ) continue;
        event_counting_2 += 1;

        // note : single cluster in l0, l1 and l2
        if (receiver_unit_clu_size[study_chip-1][0].size() != hit_allowed_in_adjacent_layers || receiver_unit_clu_size[study_chip-1][1].size() != hit_allowed_in_adjacent_layers || receiver_unit_clu_size[study_chip-1][2].size() != hit_allowed_in_adjacent_layers) continue;
        event_counting_3 += 1;
        
        // note : single-hit cluster required
        // if ( receiver_unit_clu_size[study_chip-1][0][0] != 1 || receiver_unit_clu_size[study_chip-1][1][0] != 1 || receiver_unit_clu_size[study_chip-1][2][0] != 1 ) continue;
        // event_counting_4 += 1;

        // // note : standalone cluster adc cut
        // if ( receiver_unit_clu_adc[study_chip-1][0][0] <= cluster_adc_value_requirement || receiver_unit_clu_adc[study_chip-1][1][0] <= cluster_adc_value_requirement || receiver_unit_clu_adc[study_chip-1][2][0] <= cluster_adc_value_requirement ) continue;
        // event_counting_5 += 1;
        
        event_residual = receiver_unit_clu_pos[study_chip-1][1][0] - (receiver_unit_clu_pos[study_chip-1][2][0] + receiver_unit_clu_pos[study_chip-1][0][0])/2.;

        // event_residual = ((receiver_unit_clu_pos[study_chip-1][2][0] - receiver_unit_clu_pos[study_chip-1][1][0]) / actual_xpos[1]) - ((receiver_unit_clu_pos[study_chip-1][1][0] - receiver_unit_clu_pos[study_chip-1][0][0]) / actual_xpos[1]);
        
        residual_hist -> Fill(event_residual);
    }


    cout<<"event counting 1 \t : "<<event_counting_1<<endl;
    cout<<"event counting 2 \t : "<<event_counting_2<<endl;
    cout<<"event counting 3 \t : "<<event_counting_3<<endl;
    // cout<<"event counting 4 \t : "<<event_counting_4<<endl;

    return residual_hist;
}

vector<TH1F *> data_process::Get_cluster_size (bool with_cut, int SD_Nbins, double SD_Ledge, double SD_Redge, vector<cluster_reformat_str> input_cluster_vec)
{
    double edge_exclusion_bottom = (lower_section_initial - INTT_strip_width / 2.) + INTT_strip_width * double(boundary_cut);
	double edge_exclusion_upper = ( INTT_strip_width * 128. ) - INTT_strip_width * double(boundary_cut);

    vector<int> receiver_unit_clu_size[13][3]; // note : for abbreviation, for hit 
    vector<double> receiver_unit_clu_pos[13][3];
    vector<double> receiver_unit_clu_adc[13][3];

    int event_counting_1  = 0;
    int event_counting_2  = 0;
    int event_counting_3  = 0;
    int event_counting_4  = 0;
    int event_counting_5  = 0;
    int event_counting_6  = 0;
    int event_counting_7  = 0;
    int event_counting_8  = 0;
    int event_counting_9  = 0;
    int event_counting_10 = 0;
    int event_counting_11 = 0;
    int event_counting_12 = 0;

    TF1 * linear_fit;
    TGraph * grr;

    double hit2_slope;
    double event_residual;
    double l0_l2_avg_pos;
    double chi2_register = 10000000;

    double hit3_best_fit_picker_info[2]; // note : 0 : residual, 1 : the cluster size

    TH1F * cluster_size_unit[3];
    for (int i = 0; i < 3; i++) { cluster_size_unit[i] = new TH1F ("","",SD_Nbins,SD_Ledge,SD_Redge); }

    if (with_cut == true)
    {
        for (int i = 0; i < input_cluster_vec.size(); i++)
        {
            if (i % 10000 == 0){ cout<<"Doing the cluster_size_dist, eID : "<<input_cluster_vec[i].eID<<endl;}

            // note : abbreviation
            for (int i1 = 0; i1 < 13; i1++)
            {
                for (int i2 = 0; i2 < 3; i2++)
                {
                    receiver_unit_clu_size[i1][i2] = input_cluster_vec[i].cluster_hit[ i1 ][i2];
                    receiver_unit_clu_pos[i1][i2] = input_cluster_vec[i].cluster_pos[ i1 ][i2];
                    receiver_unit_clu_adc[i1][i2] = input_cluster_vec[i].cluster_adc[ i1 ][i2];

                }
            }

            event_counting_1 += 1;
        
            // note : zero cluster in adjacent chips
            if ( (receiver_unit_clu_size[study_chip-1-1][0].size()+receiver_unit_clu_size[study_chip-1-1][1].size()+receiver_unit_clu_size[study_chip-1-1][2].size()+receiver_unit_clu_size[study_chip-1+1][0].size()+receiver_unit_clu_size[study_chip-1+1][1].size()+receiver_unit_clu_size[study_chip-1+1][2].size()) != 0 ) continue;
            event_counting_2 += 1;

            // note : single cluster in l0 and l2
            if (receiver_unit_clu_size[study_chip-1][0].size() != hit_allowed_in_adjacent_layers || receiver_unit_clu_size[study_chip-1][2].size() != hit_allowed_in_adjacent_layers) continue;
            event_counting_3 += 1;

            // note : standalone cluster size cut of l0 and l2, currently, the cut value is 0
            if ( receiver_unit_clu_size[study_chip-1][0][0] <= cluster_size_requirement || receiver_unit_clu_size[study_chip-1][2][0] <= cluster_size_requirement ) continue;
            event_counting_4 += 1;

            // note : standalone cluster adc cut, currently, the cut value is 22.5
            if ( receiver_unit_clu_adc[study_chip-1][0][0] <= cluster_adc_value_requirement || receiver_unit_clu_adc[study_chip-1][2][0] <= cluster_adc_value_requirement ) continue;
            event_counting_5 += 1;

            // note : combined cluster size cut, currently, the cut value is 1
            if ( receiver_unit_clu_size[study_chip-1][0][0]+receiver_unit_clu_size[study_chip-1][2][0] <= cluster_size_requirement_combine ) continue;
            event_counting_6 += 1;


            // note : edge exclusion cut of the l0
            if ( receiver_unit_clu_pos[study_chip-1][0][0] <= edge_exclusion_bottom || receiver_unit_clu_pos[study_chip-1][0][0] >= edge_exclusion_upper ) continue;
            event_counting_7 += 1;

            // note : edge exclusion cut of the l2
            if ( receiver_unit_clu_pos[study_chip-1][2][0] <= edge_exclusion_bottom || receiver_unit_clu_pos[study_chip-1][2][0] >= edge_exclusion_upper ) continue;
            event_counting_8 += 1;

            hit2_slope = (receiver_unit_clu_pos[study_chip-1][2][0] - receiver_unit_clu_pos[study_chip-1][0][0]) / actual_xpos[2] + slope_correction;

            // note : the slope cut
            if ( fabs(hit2_slope) >= slope_cut ) continue;
            event_counting_9 += 1;
            
            // note : fill the cluster size of the good tracks
            cluster_size_unit[0]->Fill(receiver_unit_clu_size[study_chip-1][0][0]);
            cluster_size_unit[2]->Fill(receiver_unit_clu_size[study_chip-1][2][0]);

            l0_l2_avg_pos = ( receiver_unit_clu_pos[study_chip-1][0][0] + receiver_unit_clu_pos[study_chip-1][2][0] ) / 2.;

            // note : to check the N clusters of the middle layer
            if ( receiver_unit_clu_pos[study_chip-1][1].size() == 0 ) // note : no hits in the middle
            {
                event_counting_10 += 1;
                cout<<"101 event, eID : "<<input_cluster_vec[i].eID<<" -> no cluster"<<endl;
            }
            else if ( receiver_unit_clu_pos[study_chip-1][1].size() > 0 ) // note : at least one hit in the middle
            {
                
                for ( int l1 = 0; l1 < receiver_unit_clu_pos[study_chip-1][1].size(); l1++ )
                {
                    // if (receiver_unit_clu_adc[study_chip-1][1][l1] <= cluster_adc_value_requirement ) continue; // note : to add the ADC0 cut at L1

                    double hit3_Y_data[3] = {receiver_unit_clu_pos[study_chip-1][0][0], receiver_unit_clu_pos[study_chip-1][1][l1], receiver_unit_clu_pos[study_chip-1][2][0]};

                    grr = new TGraph(3,actual_xpos,hit3_Y_data);
                    linear_fit = new TF1("linear_fit","pol1",-1,actual_xpos[2]+2);
                    grr -> Fit("linear_fit","NQ");

                    if (l1 == 0) // note : the first one
                    {
                        chi2_register = ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) );
                        
                        // cluster_register_l1 = l1;

                        // hit3_best_fit_picker_info[0] = ( hit3_Y_data[0] - ( linear_fit -> GetParameter(1) * actual_xpos[0] + linear_fit -> GetParameter(0) ) );
                        // hit3_best_fit_picker_info[1] = ( hit3_Y_data[1] - ( linear_fit -> GetParameter(1) * actual_xpos[1] + linear_fit -> GetParameter(0) ) );
                        // hit3_best_fit_picker_info[2] = ( hit3_Y_data[2] - ( linear_fit -> GetParameter(1) * actual_xpos[2] + linear_fit -> GetParameter(0) ) );
                        
                        // hit3_best_fit_picker_info[3] = hit3_Y_data[0];
                        // hit3_best_fit_picker_info[4] = hit3_Y_data[1];
                        // hit3_best_fit_picker_info[5] = hit3_Y_data[2];

                        // note : the middle-layer residual of the DUT method
                        hit3_best_fit_picker_info[0] = hit3_Y_data[1];
                        hit3_best_fit_picker_info[1] = receiver_unit_clu_size[study_chip-1][1][l1];

                    }
                    else 
                    {
                        if ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) < chi2_register )
                        {
                            chi2_register = ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) );
                            
                            // cluster_register_l1 = l1;

                            // hit3_best_fit_picker_info[0] = ( hit3_Y_data[0] - ( linear_fit -> GetParameter(1) * actual_xpos[0] + linear_fit -> GetParameter(0) ) );
                            // hit3_best_fit_picker_info[1] = ( hit3_Y_data[1] - ( linear_fit -> GetParameter(1) * actual_xpos[1] + linear_fit -> GetParameter(0) ) );
                            // hit3_best_fit_picker_info[2] = ( hit3_Y_data[2] - ( linear_fit -> GetParameter(1) * actual_xpos[2] + linear_fit -> GetParameter(0) ) );
                            
                            // hit3_best_fit_picker_info[3] = hit3_Y_data[0];
                            // hit3_best_fit_picker_info[4] = hit3_Y_data[1];
                            // hit3_best_fit_picker_info[5] = hit3_Y_data[2];

                            // note : the middle-layer residual of the DUT method
                            hit3_best_fit_picker_info[0] = hit3_Y_data[1];
                            hit3_best_fit_picker_info[1] = receiver_unit_clu_size[study_chip-1][1][l1];  

                        }
                    }

                    grr->Delete();
                    linear_fit->Delete();

                }

                chi2_register = 1000000;

                event_residual = hit3_best_fit_picker_info[0] - ( l0_l2_avg_pos );

                if ( fabs(event_residual) < noise_hit_distance )
                {
                    event_counting_11 += 1;

                    // final_event_result.push_back(1); // note : in order to make the detection efficiency as function of pos.

                    // track_111_count += 1;

                    cluster_size_unit[1]->Fill(hit3_best_fit_picker_info[1]);
                }
                else 
                {
                    event_counting_12 += 1;

                    // final_event_result.push_back(2); // note : in order to make the detection efficiency as function of pos.

                    // track_101_count += 1;   
                    cout<<"101 event, eID : "<<input_cluster_vec[i].eID<<" -> middle has cluster"<<endl;
                }

            }
            // note : start clean

        } // note : end of event for loop in the "with cut if"

        cout<<"event counting 1 \t : "<<event_counting_1<<" all "<<endl;
        cout<<"event counting 2 \t : "<<event_counting_2<<" adjacent chip, no cluster  "<<endl;
        cout<<"event counting 3 \t : "<<event_counting_3<<" single cluster "<<endl;
        cout<<"event counting 4 \t : "<<event_counting_4<<" standalone cluster requirement "<<endl;
        cout<<"event counting 5 \t : "<<event_counting_5<<" cluster adc "<<endl;
        cout<<"event counting 6 \t : "<<event_counting_6<<" cluster size combine "<<endl;
        cout<<"event counting 7 \t : "<<event_counting_7<<" l0 cluster edge exclusion "<<endl;
        cout<<"event counting 8 \t : "<<event_counting_8<<" l2 cluster edge exclusion "<<endl;
        cout<<"event counting 9 \t : "<<event_counting_9<<" slope cut "<<endl;
        cout<<"event counting 10 \t : "<<event_counting_10<<" no hits in middle layer "<<endl;
        cout<<"event counting 11 \t : "<<event_counting_11<<" have good cluster in middle "<<endl;
        cout<<"event counting 12 \t : "<<event_counting_12<<" no good cluster in middle "<<endl;


    } // note : end of with_cut if

    else if (with_cut == false)
    {
        for (int i = 0; i < input_cluster_vec.size(); i++)
        {
            if (i % 10000 == 0){ cout<<"Doing the cluster_size_dist, eID : "<<input_cluster_vec[i].eID<<endl;}

            // note : abbreviation
            for (int i1 = 0; i1 < 13; i1++)
            {
                for (int i2 = 0; i2 < 3; i2++)
                {
                    receiver_unit_clu_size[i1][i2] = input_cluster_vec[i].cluster_hit[ i1 ][i2];
                    receiver_unit_clu_pos[i1][i2] = input_cluster_vec[i].cluster_pos[ i1 ][i2];
                    receiver_unit_clu_adc[i1][i2] = input_cluster_vec[i].cluster_adc[ i1 ][i2];

                }
            }

            // note : layer
            for (int i1 = 0; i1 < 3; i1++)
            {
                // note : number of clusters in the "study_chip-1" of layer i1
                for (int i2 = 0; i2 < receiver_unit_clu_size[study_chip-1][i1].size(); i2++)
                {
                    cluster_size_unit[i1]->Fill(receiver_unit_clu_size[study_chip-1][i1][i2]);
                }
            }

        } // note : end of event for loop in the WITHOUT cut if
    } // note : end of WITHOUT cut if

    vector <TH1F *> vec_out = {cluster_size_unit[0], cluster_size_unit[1], cluster_size_unit[2]};
    return vec_out;
}

multi_track_str data_process::Get_Ntrack(vector<cluster_reformat_str> input_cluster_vec, bool print_all_track)
{
    vector<noise_event_str> output_vec; output_vec.clear();
    noise_event_str noise_container; noise_container.noise_event.clear();

    // note : to study the correlation between the first and the second track
    vector<vector<pol1_info>> track_fit_container; track_fit_container.clear();
    vector<pol1_info> track_fit_vec; track_fit_vec.clear(); 

    int chip_track_count[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};

    vector<int> N_track; N_track.clear(); // note : the vector is for recording the # of tracks each event has.

    // int good_track_count = 0; // note : this is for two data match, if "event_profile" == 14, good_track_count += 1;
    vector<int> receiver_unit_clu[13][3]; // note : for abbreviation, for # of hit of the cluster. 
    vector<double> receiver_unit_clu_pos[13][3]; // note : for abbreviation, for the position of the cluster.
    vector<double> receiver_unit_clu_adc[13][3]; // note : for abbreviation, for the adc value of the cluster.

    TF1 * linear_fit;
    TGraph * grr;

    double chi2_register = 0; 
    double cluster_register_l0 = 0;
    double cluster_register_l1 = 0;
    double cluster_register_l2 = 0;
    double hit3_best_fit_picker_info[8]; // note : first 3 : residual, second 3 : the selected positions
    int multi_track_count = 0; // note : this is for finding the multi_track, if we find a new good track, then +=1;

    for (int i = 0; i < input_cluster_vec.size(); i++)
    {
        if (i % 1000 == 0) {cout<<"multi-track removal "<<i<<endl;}

        // note : abbreviation
        for (int i1 = 0; i1 < 13; i1++)
        {
            for (int i2 = 0; i2 < 3; i2++)
            {
                receiver_unit_clu[i1][i2] = input_cluster_vec[i].cluster_hit[ i1 ][i2];
                receiver_unit_clu_pos[i1][i2] = input_cluster_vec[i].cluster_pos[ i1 ][i2];
                receiver_unit_clu_adc[i1][i2] = input_cluster_vec[i].cluster_adc[ i1 ][i2];
            }
        }

        // note : start for searching for the 2nd track.
        for (int i1 = 0; i1 < 13; i1++) // note : 13 chips or you can call it slot
        {
            bool find_track = true;

            // note : to keep the loop running, at least the chip of all layers should have at least one cluster to form a good track.
            // note : and also if there is no good track after search, the loop should stop.
            while ( receiver_unit_clu[i1][0].size() > 0 && receiver_unit_clu[i1][1].size() > 0 && receiver_unit_clu[i1][2].size() > 0 && find_track == true )
            {
                
                // note : try to find the good track in the loops
                for (int l0 = 0; l0 < receiver_unit_clu[i1][0].size(); l0 ++ ) // note : # of clusters of the chip of the layer 0
                {
                    for (int l1 = 0; l1 < receiver_unit_clu[i1][1].size(); l1 ++ ) // note : # of clusters of the chip of the layer 1
                    {
                        for (int l2 = 0; l2 < receiver_unit_clu[i1][2].size(); l2 ++ ) // note : # of clusters of the chip of the layer 2
                        {
                            double hit3_Y_data[3] = {receiver_unit_clu_pos[i1][0][l0], receiver_unit_clu_pos[i1][1][l1], receiver_unit_clu_pos[i1][2][l2]};

                            grr = new TGraph(3,actual_xpos,hit3_Y_data);
                            linear_fit = new TF1("linear_fit","pol1",-1,actual_xpos[2]+2);
                            grr -> Fit("linear_fit","NQ");

                            if (l0 + l1 + l2 == 0) // note : the first one
                            {
                                chi2_register = ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) );
                                cluster_register_l0 = l0;
                                cluster_register_l1 = l1;
                                cluster_register_l2 = l2;

                                hit3_best_fit_picker_info[0] = ( hit3_Y_data[0] - ( linear_fit -> GetParameter(1) * actual_xpos[0] + linear_fit -> GetParameter(0) ) );
                                hit3_best_fit_picker_info[1] = ( hit3_Y_data[1] - ( linear_fit -> GetParameter(1) * actual_xpos[1] + linear_fit -> GetParameter(0) ) );
                                hit3_best_fit_picker_info[2] = ( hit3_Y_data[2] - ( linear_fit -> GetParameter(1) * actual_xpos[2] + linear_fit -> GetParameter(0) ) );
                                
                                hit3_best_fit_picker_info[3] = hit3_Y_data[0];
                                hit3_best_fit_picker_info[4] = hit3_Y_data[1];
                                hit3_best_fit_picker_info[5] = hit3_Y_data[2];

                                hit3_best_fit_picker_info[6] = linear_fit -> GetParameter(0); // note : offset
                                hit3_best_fit_picker_info[7] = linear_fit -> GetParameter(1); // note : slope

                            }
                            else 
                            {
                                if ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) < chi2_register )
                                {
                                    chi2_register = ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) );
                                    cluster_register_l0 = l0;
                                    cluster_register_l1 = l1;
                                    cluster_register_l2 = l2;

                                    hit3_best_fit_picker_info[0] = ( hit3_Y_data[0] - ( linear_fit -> GetParameter(1) * actual_xpos[0] + linear_fit -> GetParameter(0) ) );
                                    hit3_best_fit_picker_info[1] = ( hit3_Y_data[1] - ( linear_fit -> GetParameter(1) * actual_xpos[1] + linear_fit -> GetParameter(0) ) );
                                    hit3_best_fit_picker_info[2] = ( hit3_Y_data[2] - ( linear_fit -> GetParameter(1) * actual_xpos[2] + linear_fit -> GetParameter(0) ) );
                                    
                                    hit3_best_fit_picker_info[3] = hit3_Y_data[0];
                                    hit3_best_fit_picker_info[4] = hit3_Y_data[1];
                                    hit3_best_fit_picker_info[5] = hit3_Y_data[2];

                                    hit3_best_fit_picker_info[6] = linear_fit -> GetParameter(0); // note : offset
                                    hit3_best_fit_picker_info[7] = linear_fit -> GetParameter(1); // note : slope
                                    

                                }
                            }
                        }
                    }
                } // note : end of 3 layers track finding and fitting

                // note : after finding the good tracking, let's check it. 
                if ( fabs(hit3_best_fit_picker_info[0]) > noise_hit_distance || fabs(hit3_best_fit_picker_info[1]) > noise_hit_distance || fabs(hit3_best_fit_picker_info[2]) > noise_hit_distance )
                {
                    find_track = false; // note : which means, even if we have at least one cluster in each layer in this event. they can not form a good track.
                }
                else 
                {
                    multi_track_count += 1;

                    if (print_all_track == true)
                    {
                        cout<<"find the "<< multi_track_count <<"th track !! chip ID : "<< i1+1<<" eID : "<<input_cluster_vec[i].eID<<" position : "<<hit3_best_fit_picker_info[3]<<" "<<hit3_best_fit_picker_info[4]<<" "<<hit3_best_fit_picker_info[5]<<endl;
                    }
                    else if (print_all_track != true)
                    {
                        if ( multi_track_count > 1 ) {cout<<"find the "<< multi_track_count <<"th track !! chip ID : "<< i1+1<<" eID : "<<input_cluster_vec[i].eID<<" position : "<<hit3_best_fit_picker_info[3]<<" "<<hit3_best_fit_picker_info[4]<<" "<<hit3_best_fit_picker_info[5]<<endl;}
                        else if ( i % 1000 == 0 ) {cout<<"find the "<< multi_track_count <<"th track !! chip ID : "<< i1+1<<" eID : "<<input_cluster_vec[i].eID<<" position : "<<hit3_best_fit_picker_info[3]<<" "<<hit3_best_fit_picker_info[4]<<" "<<hit3_best_fit_picker_info[5]<<endl;}
                    }
                    

                    
                    

                    chip_track_count[i1]+=1;

                    // note : this is for deleting the track information of the nth track.
                    receiver_unit_clu[i1][0].erase( receiver_unit_clu[i1][0].begin() + cluster_register_l0 );
                    receiver_unit_clu_pos[i1][0].erase( receiver_unit_clu_pos[i1][0].begin() + cluster_register_l0 );
                    receiver_unit_clu_adc[i1][0].erase( receiver_unit_clu_adc[i1][0].begin() + cluster_register_l0 );

                    receiver_unit_clu[i1][1].erase( receiver_unit_clu[i1][1].begin() + cluster_register_l1 );
                    receiver_unit_clu_pos[i1][1].erase( receiver_unit_clu_pos[i1][1].begin() + cluster_register_l1 );
                    receiver_unit_clu_adc[i1][1].erase( receiver_unit_clu_adc[i1][1].begin() + cluster_register_l1 );
                    
                    receiver_unit_clu[i1][2].erase( receiver_unit_clu[i1][2].begin() + cluster_register_l2 );
                    receiver_unit_clu_pos[i1][2].erase( receiver_unit_clu_pos[i1][2].begin() + cluster_register_l2 );
                    receiver_unit_clu_adc[i1][2].erase( receiver_unit_clu_adc[i1][2].begin() + cluster_register_l2 );

                    // note : for the track correlation study
                    pol1_info pol1_info_unit;
                    pol1_info_unit.eID = input_cluster_vec[i].eID;
                    pol1_info_unit.chip_id = i1 + 1;
                    pol1_info_unit.slope = hit3_best_fit_picker_info[7];
                    pol1_info_unit.offset = hit3_best_fit_picker_info[6]; // note : offset

                    track_fit_vec.push_back(pol1_info_unit);

                }
            } // note : end of while loop

        } // note : end of scan chip 

        if (multi_track_count > 0) 
        { 
            N_track.push_back(multi_track_count); // todo : maybe this line can be moved outside the bracket.

            track_fit_container.push_back(track_fit_vec);

            noise_container.noise_event.clear();
            output_vec.push_back( noise_container );
            output_vec[output_vec.size() - 1].eID = input_cluster_vec[i].eID;

            // note : delete the cluster that is formed by more than one hit.
            for (int i1 = 0; i1 < 13; i1++) // note : scan the chip
            {
                for (int i2 = 0; i2 < 3; i2 ++) // note : scan the layer
                {
                    for (int i3 = 0; i3 < receiver_unit_clu[i1][i2].size(); i3++) //note : # of cluster in the selected chip and layer.
                    {
                        if (receiver_unit_clu[i1][i2][i3] == 1 && receiver_unit_clu_adc[i1][i2][i3] == 22.5 ) // note : the cluster size of noise has to be 1. for the cluster whose size is bigger than 1, it should be a true hit. 
                        {
                            noise_hit_str hit_info;
                            hit_info.noise_chip = i1+1;
                            hit_info.noise_layer = i2;
                            hit_info.noise_pos = receiver_unit_clu_pos[i1][i2][i3];
                            hit_info.noise_adc = receiver_unit_clu_adc[i1][i2][i3];
                            
                            output_vec[ output_vec.size() - 1 ].noise_event.push_back(hit_info);
                        }
                    }
                }
            }// note : end of deleting the cluster which is made of more than one hits.


        }

        multi_track_count = 0;
        track_fit_vec.clear();

    } // note : end of for loop i, the event

    

    multi_track_str output_container;
    output_container.noise_vec = output_vec;
    output_container.N_track = N_track;
    output_container.track_chip_count.clear();
    output_container.track_fit_info_vec = track_fit_container;
    for (int i = 0; i < 13; i++)
    {
        output_container.track_chip_count.push_back(chip_track_count[i]);
    }

    return output_container;

} // note : end of function


TH1F * data_process::Get_Ntrack_extrapolation_dist(int SD_Nbins, double SD_Ledge, double SD_Redge, multi_track_str input_vec)
{       
    // int good_tracking_counting = 0;

    TH1F * extra_pos_hist = new TH1F("","",SD_Nbins,SD_Ledge,SD_Redge);

    for (int i = 0; i < input_vec.track_fit_info_vec.size(); i++) // note : number of event which has at least one good track 
    {   
        // cout<<"test size : "<<input_vec.track_fit_info_vec[i].size()<<endl;
        // if (input_vec.track_fit_info_vec[i].size() != 1) continue;

        for ( int i1 = 0; i1 < input_vec.track_fit_info_vec[i].size(); i1++ ) // note : number of good track (criteria by fitting) in single event.
        {
            extra_pos_hist -> Fill( input_vec.track_fit_info_vec[i][i1].slope * distance_to_sci + input_vec.track_fit_info_vec[i][i1].offset );
        }

    }

    return extra_pos_hist;
}

TH1F * data_process::Get_Ntrack_dist(int SD_Nbins, double SD_Ledge, double SD_Redge, double track_extrapol_low, double track_extrapol_high, multi_track_str input_vec)
{
    TH1F * Ntrack_dist = new TH1F("","",SD_Nbins,SD_Ledge,SD_Redge);

    int good_tracking_counting = 0;
    double extrapolation_pos;

    for (int i = 0; i < input_vec.track_fit_info_vec.size(); i++) // note : number of event which has at least one good track 
    {
        for ( int i1 = 0; i1 < input_vec.track_fit_info_vec[i].size(); i1++ ) // note : number of good track (criteria by fitting) in single event.
        {

            extrapolation_pos = input_vec.track_fit_info_vec[i][i1].slope * distance_to_sci + input_vec.track_fit_info_vec[i][i1].offset; 

            if ( extrapolation_pos > track_extrapol_low && extrapolation_pos < track_extrapol_high )
            {
                good_tracking_counting += 1;
            }
        }

        Ntrack_dist -> Fill(good_tracking_counting);

        if (good_tracking_counting > 2) // note : at least 3 tracks in the event
        {
            cout<<"In the Ntrack_dist, the choosen one with "<<good_tracking_counting<<" events, the eID : "<<input_vec.track_fit_info_vec[i][0].eID<<endl;
        }

        good_tracking_counting = 0;
    }
    return Ntrack_dist;
}

vector<TH1F *> data_process::Get_cluster_adc (bool with_cut, int SD_Nbins, double SD_Ledge, double SD_Redge, vector<cluster_reformat_str> input_cluster_vec)
{
    double edge_exclusion_bottom = (lower_section_initial - INTT_strip_width / 2.) + INTT_strip_width * double(boundary_cut);
	double edge_exclusion_upper = ( INTT_strip_width * 128. ) - INTT_strip_width * double(boundary_cut);
    
    vector<int> receiver_unit_clu_size[13][3]; // note : for abbreviation, for hit 
    vector<double> receiver_unit_clu_pos[13][3];
    vector<double> receiver_unit_clu_adc[13][3];

    int event_counting_1  = 0;
    int event_counting_2  = 0;
    int event_counting_3  = 0;
    int event_counting_4  = 0;
    int event_counting_5  = 0;
    int event_counting_6  = 0;
    int event_counting_7  = 0;
    int event_counting_8  = 0;
    int event_counting_9  = 0;
    int event_counting_10 = 0;
    int event_counting_11 = 0;
    int event_counting_12 = 0;

    TF1 * linear_fit;
    TGraph * grr;

    double hit2_slope;
    double event_residual;
    double l0_l2_avg_pos;
    double chi2_register = 10000000;

    double hit3_best_fit_picker_info[2]; // note : 0 : residual, 1 : the cluster adc

    TH1F * adc_size_unit[3];
    for (int i = 0; i < 3; i++) { adc_size_unit[i] = new TH1F ("","",SD_Nbins,SD_Ledge,SD_Redge); }

    if (with_cut == true) // note : L0 and L2, all cuts applied, same as the efficiency DUT study. L1, pass the residual cut
    {
        for (int i = 0; i < input_cluster_vec.size(); i++)
        {
            if (i % 10000 == 0){ cout<<"Doing the adc_dist, eID : "<<input_cluster_vec[i].eID<<endl;}

            // note : abbreviation
            for (int i1 = 0; i1 < 13; i1++)
            {
                for (int i2 = 0; i2 < 3; i2++)
                {
                    receiver_unit_clu_size[i1][i2] = input_cluster_vec[i].cluster_hit[ i1 ][i2];
                    receiver_unit_clu_pos[i1][i2] = input_cluster_vec[i].cluster_pos[ i1 ][i2];
                    receiver_unit_clu_adc[i1][i2] = input_cluster_vec[i].cluster_adc[ i1 ][i2];
                }
            }
            event_counting_1 += 1;

            // note : zero cluster in adjacent chips
            if ( (receiver_unit_clu_size[study_chip-1-1][0].size()+receiver_unit_clu_size[study_chip-1-1][1].size()+receiver_unit_clu_size[study_chip-1-1][2].size()+receiver_unit_clu_size[study_chip-1+1][0].size()+receiver_unit_clu_size[study_chip-1+1][1].size()+receiver_unit_clu_size[study_chip-1+1][2].size()) != 0 ) continue;
            event_counting_2 += 1;

            // note : single cluster in l0 and l2
            if (receiver_unit_clu_size[study_chip-1][0].size() != hit_allowed_in_adjacent_layers || receiver_unit_clu_size[study_chip-1][2].size() != hit_allowed_in_adjacent_layers) continue;
            event_counting_3 += 1;

            // note : standalone cluster size cut of l0 and l2, currently, the cut value is 0
            if ( receiver_unit_clu_size[study_chip-1][0][0] <= cluster_size_requirement || receiver_unit_clu_size[study_chip-1][2][0] <= cluster_size_requirement ) continue;
            event_counting_4 += 1;

            // note : standalone cluster adc cut, currently, the cut value is 22.5
            if ( receiver_unit_clu_adc[study_chip-1][0][0] <= cluster_adc_value_requirement || receiver_unit_clu_adc[study_chip-1][2][0] <= cluster_adc_value_requirement ) continue;
            event_counting_5 += 1;

            // note : combined cluster size cut, currently, the cut value is 1
            if ( receiver_unit_clu_size[study_chip-1][0][0]+receiver_unit_clu_size[study_chip-1][2][0] <= cluster_size_requirement_combine ) continue;
            event_counting_6 += 1;


            // note : edge exclusion cut of the l0
            if ( receiver_unit_clu_pos[study_chip-1][0][0] <= edge_exclusion_bottom || receiver_unit_clu_pos[study_chip-1][0][0] >= edge_exclusion_upper ) continue;
            event_counting_7 += 1;

            // note : edge exclusion cut of the l2
            if ( receiver_unit_clu_pos[study_chip-1][2][0] <= edge_exclusion_bottom || receiver_unit_clu_pos[study_chip-1][2][0] >= edge_exclusion_upper ) continue;
            event_counting_8 += 1;

            hit2_slope = (receiver_unit_clu_pos[study_chip-1][2][0] - receiver_unit_clu_pos[study_chip-1][0][0]) / actual_xpos[2] + slope_correction;

            // note : the slope cut
            if ( fabs(hit2_slope) >= slope_cut ) continue;
            event_counting_9 += 1;
            
            // note : fill the cluster adc of the good tracks
            adc_size_unit[0]->Fill(receiver_unit_clu_adc[study_chip-1][0][0]);
            adc_size_unit[2]->Fill(receiver_unit_clu_adc[study_chip-1][2][0]);

            l0_l2_avg_pos = ( receiver_unit_clu_pos[study_chip-1][0][0] + receiver_unit_clu_pos[study_chip-1][2][0] ) / 2.;

            // note : to check the N clusters of the middle layer
            if ( receiver_unit_clu_pos[study_chip-1][1].size() == 0 ) // note : no hits in the middle
            {
                event_counting_10 += 1;
                cout<<"101 event, eID : "<<input_cluster_vec[i].eID<<" -> no cluster"<<endl;
            }
            else if ( receiver_unit_clu_pos[study_chip-1][1].size() > 0 ) // note : at least one hit in the middle
            {
                
                for ( int l1 = 0; l1 < receiver_unit_clu_pos[study_chip-1][1].size(); l1++ )
                {
                    // if (receiver_unit_clu_adc[study_chip-1][1][l1] <= cluster_adc_value_requirement ) continue; // note : to add the ADC0 cut at L1

                    double hit3_Y_data[3] = {receiver_unit_clu_pos[study_chip-1][0][0], receiver_unit_clu_pos[study_chip-1][1][l1], receiver_unit_clu_pos[study_chip-1][2][0]};

                    grr = new TGraph(3,actual_xpos,hit3_Y_data);
                    linear_fit = new TF1("linear_fit","pol1",-1,actual_xpos[2]+2);
                    grr -> Fit("linear_fit","NQ");

                    if (l1 == 0) // note : the first one
                    {
                        chi2_register = ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) );
                        
                        // cluster_register_l1 = l1;

                        // hit3_best_fit_picker_info[0] = ( hit3_Y_data[0] - ( linear_fit -> GetParameter(1) * actual_xpos[0] + linear_fit -> GetParameter(0) ) );
                        // hit3_best_fit_picker_info[1] = ( hit3_Y_data[1] - ( linear_fit -> GetParameter(1) * actual_xpos[1] + linear_fit -> GetParameter(0) ) );
                        // hit3_best_fit_picker_info[2] = ( hit3_Y_data[2] - ( linear_fit -> GetParameter(1) * actual_xpos[2] + linear_fit -> GetParameter(0) ) );
                        
                        // hit3_best_fit_picker_info[3] = hit3_Y_data[0];
                        // hit3_best_fit_picker_info[4] = hit3_Y_data[1];
                        // hit3_best_fit_picker_info[5] = hit3_Y_data[2];

                        // note : the middle-layer residual of the DUT method
                        hit3_best_fit_picker_info[0] = hit3_Y_data[1];
                        hit3_best_fit_picker_info[1] = receiver_unit_clu_adc[study_chip-1][1][l1];

                    }
                    else 
                    {
                        if ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) < chi2_register )
                        {
                            chi2_register = ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) );
                            
                            // cluster_register_l1 = l1;

                            // hit3_best_fit_picker_info[0] = ( hit3_Y_data[0] - ( linear_fit -> GetParameter(1) * actual_xpos[0] + linear_fit -> GetParameter(0) ) );
                            // hit3_best_fit_picker_info[1] = ( hit3_Y_data[1] - ( linear_fit -> GetParameter(1) * actual_xpos[1] + linear_fit -> GetParameter(0) ) );
                            // hit3_best_fit_picker_info[2] = ( hit3_Y_data[2] - ( linear_fit -> GetParameter(1) * actual_xpos[2] + linear_fit -> GetParameter(0) ) );
                            
                            // hit3_best_fit_picker_info[3] = hit3_Y_data[0];
                            // hit3_best_fit_picker_info[4] = hit3_Y_data[1];
                            // hit3_best_fit_picker_info[5] = hit3_Y_data[2];

                            // note : the middle-layer residual of the DUT method
                            hit3_best_fit_picker_info[0] = hit3_Y_data[1];
                            hit3_best_fit_picker_info[1] = receiver_unit_clu_adc[study_chip-1][1][l1];  

                        }
                    }

                    grr->Delete();
                    linear_fit->Delete();

                }

                chi2_register = 1000000;

                event_residual = hit3_best_fit_picker_info[0] - ( l0_l2_avg_pos );

                if ( fabs(event_residual) < noise_hit_distance )
                {
                    event_counting_11 += 1;

                    // final_event_result.push_back(1); // note : in order to make the detection efficiency as function of pos.

                    // track_111_count += 1;

                    adc_size_unit[1]->Fill(hit3_best_fit_picker_info[1]);
                }
                else 
                {
                    event_counting_12 += 1;

                    // final_event_result.push_back(2); // note : in order to make the detection efficiency as function of pos.

                    // track_101_count += 1;   
                    cout<<"101 event, eID : "<<input_cluster_vec[i].eID<<" -> middle has cluster"<<endl;
                }

            }
            // note : start clean

        } // note : end of event for loop in the "with cut if"

        cout<<"event counting 1 \t : "<<event_counting_1<<" all "<<endl;
        cout<<"event counting 2 \t : "<<event_counting_2<<" adjacent chip, no cluster  "<<endl;
        cout<<"event counting 3 \t : "<<event_counting_3<<" single cluster "<<endl;
        cout<<"event counting 4 \t : "<<event_counting_4<<" standalone cluster requirement "<<endl;
        cout<<"event counting 5 \t : "<<event_counting_5<<" cluster adc "<<endl;
        cout<<"event counting 6 \t : "<<event_counting_6<<" cluster size combine "<<endl;
        cout<<"event counting 7 \t : "<<event_counting_7<<" l0 cluster edge exclusion "<<endl;
        cout<<"event counting 8 \t : "<<event_counting_8<<" l2 cluster edge exclusion "<<endl;
        cout<<"event counting 9 \t : "<<event_counting_9<<" slope cut "<<endl;
        cout<<"event counting 10 \t : "<<event_counting_10<<" no hits in middle layer "<<endl;
        cout<<"event counting 11 \t : "<<event_counting_11<<" have good cluster in middle "<<endl;
        cout<<"event counting 12 \t : "<<event_counting_12<<" no good cluster in middle "<<endl;


    } // note : end of the "with cut" if

    else if (with_cut == false) // note : no cut
    {
        for (int i = 0; i < input_cluster_vec.size(); i++)
        {
            if (i % 10000 == 0){ cout<<"Doing the adc_dist, eID : "<<input_cluster_vec[i].eID<<endl;}

            // note : abbreviation
            for (int i1 = 0; i1 < 13; i1++)
            {
                for (int i2 = 0; i2 < 3; i2++)
                {
                    receiver_unit_clu_size[i1][i2] = input_cluster_vec[i].cluster_hit[ i1 ][i2];
                    receiver_unit_clu_pos[i1][i2] = input_cluster_vec[i].cluster_pos[ i1 ][i2];
                    receiver_unit_clu_adc[i1][i2] = input_cluster_vec[i].cluster_adc[ i1 ][i2];

                }
            }

            // note : layer
            for (int i1 = 0; i1 < 3; i1++)
            {
                // note : number of clusters in the "study_chip-1" of layer i1
                for (int i2 = 0; i2 < receiver_unit_clu_adc[study_chip-1][i1].size(); i2++)
                {
                    adc_size_unit[i1]->Fill(receiver_unit_clu_adc[study_chip-1][i1][i2]);
                }
            }

        } // note : end of the event for loop in the WITHOUT cut if
    }// note : end of the WITHOUT cut if

    vector <TH1F *> vec_out = {adc_size_unit[0], adc_size_unit[1], adc_size_unit[2]};
    return vec_out;
}

vector<cluster_str> data_process::get_cluster_read_and_build ()
{
    return cluster_str_vec_predefined;
}
vector<cluster_reformat_str> data_process::get_cluster_reformat ()
{
    return cluster_reformat_str_vec_predefined;
}

// TH1F * data_process::residual_dist (vector<cluster_reformat_str> input_cluster_vec)
// {

// }