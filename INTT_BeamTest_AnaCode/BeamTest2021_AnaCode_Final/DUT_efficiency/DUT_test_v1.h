// note : this code is for the test of the DUT method

// note : include the parameter information
// #include "par_init.h"
#include "double_gaus.h"
#include "sPhenixStyle.C"

// note : the structure of cluster info.
struct cluster_str {
    
    int cluster_eID;
    vector<int> cluster_layer;
    vector<int> cluster_chip;
    vector<int> cluster_hit;
    vector<double> cluster_position;
    vector<double> cluster_adc;
};

// note : the structure for vector<double>[13][3]
struct cluster_reformat_str {
    int eID;
    vector<int> cluster_hit[13][3];
    vector<double> cluster_pos[13][3];
    vector<double> cluster_adc[13][3];
};

struct DUT_str{
    int track_111_count;
    int track_101_count;
    vector<double> middle_layer_residual; 
    vector<double> good_combination_slope_hit3;
    vector<double> good_combination_slope_hit2;
    
    // note : mostly for the study of the satellite peak of run89 
    vector<double> good_combination_l0_pos_hit3;
    vector<double> good_combination_l1_pos_hit3;
    vector<double> good_combination_l2_pos_hit3;

    vector<double> l0_l1_slope_vec; // note : this is for new scattering study, 2023_06_30
    vector<double> l0_l1_pos_vec;   // note : this is for new scattering study, 2023_06_30
    vector<double> scattering_vec;   // note : this is for new scattering study, 2023_06_30

    vector<double> passed_middle_pos;
    vector<int> final_event_result;
};

// note : read the root file of cluster_information, and build the event
vector<cluster_str> cluster_read_and_build (TString folder_direction, TString file_name, TString cluster_file_name, int study_chip)
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

// note : change the structure of the data
vector<cluster_reformat_str> cluster_reformat ( vector<cluster_str> input_vec, int study_chip, double amount_of_alignment )
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

// note : version 1 of the efficiency calculation, it's done by the DUT method (Geometry method)
// note : In principle, the results from v1 and v2 should be identical.
// note : the v1 is more flexible, the N of cluster of l0 and l2 don't have to be 1.
// note : and this version is friendly to the run89 satellite-peak study
// note : but the middle layer residual distribution is not that correct (some of the cuts were not applied, which leads to a wider dist.)
DUT_str efficiency_DUT_method (vector<cluster_reformat_str> input_cluster_vec, int study_chip)
{ 
	double edge_exclusion_bottom = (lower_section_initial - INTT_strip_width / 2.) + INTT_strip_width * double(boundary_cut);
	double edge_exclusion_upper = ( INTT_strip_width * 128. ) - INTT_strip_width * double(boundary_cut);

    int layer_hit_count = 0;

    vector<int> receiver_unit_clu[13][3]; // note : for abbreviation, for hit 
    vector<double> receiver_unit_clu_pos[13][3];
    vector<double> receiver_unit_clu_adc[13][3];

    // note : the clusters that pass the pre-selections (cluster cut / event cut) will be saved here.
    vector<int> receiver_unit_clu_size_filter[3]; 
    vector<double> receiver_unit_clu_pos_size_filter[3]; 
    vector<double> receiver_unit_clu_adc_size_filter[3];

    for (int i = 0; i < 3; i++)
    {
        receiver_unit_clu_size_filter[i].clear();
        receiver_unit_clu_pos_size_filter[i].clear(); 
        receiver_unit_clu_adc_size_filter[i].clear(); 
    }

    TF1 * linear_fit;
    TGraph * grr;

    double chi2_register = 0; 
    double cluster_register_l0 = 0;
    double cluster_register_l1 = 0;
    double cluster_register_l2 = 0;
    double hit3_best_fit_picker_info[7]; // note : first 3 : residual by fittting, second 3 : the selected positions, the 7th : the middle-layer residual by the DUT method
    double hit2_best_fit_picker_info[2]; // note : first 2 : the selected positions of the first and last layers.

    int multi_track_count = 0; // note : this is for finding the multi_track, if we find a new good track, then +=1;

    double hit2_slope;

    // note : for the output 
    int track_111_count = 0;
    int track_101_count = 0;
    vector<double> middle_layer_residual; middle_layer_residual.clear();
    vector<double> good_combination_slope_hit3; good_combination_slope_hit3.clear();
    vector<double> good_combination_slope_hit2; good_combination_slope_hit2.clear();

    vector<double> good_combination_l0_pos_hit3; good_combination_l0_pos_hit3.clear();
    vector<double> good_combination_l1_pos_hit3; good_combination_l1_pos_hit3.clear();
    vector<double> good_combination_l2_pos_hit3; good_combination_l2_pos_hit3.clear();


    for (int i = 0; i < input_cluster_vec.size(); i++)
    {
        if (i % 1000 == 0){ cout<<"Doing the DUT test, eID : "<<input_cluster_vec[i].eID<<endl;}

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

        // todo : the LoE
        // todo :       -> right now, what I try is to make sure there is no cluster in the adjacent chips for all three layers (chip 8 and 10, for example)
        // todo :       -> to avoid any suspicious events. 
        // todo :       -> works well !!! 2022/11/23
        // todo : the adc cut
        // todo : cluster size 
        // todo : cluster size combined
        // todo : N cluster in layer 0 and layer 2
        // todo : slope cut
        // todo : the edge exclusion

        // todo : residual cut
        

        // note : no cluster in selected_chip-1 and selected_chip+1, "fixed cut"
        if ( (receiver_unit_clu[study_chip-1-1][0].size()+receiver_unit_clu[study_chip-1-1][1].size()+receiver_unit_clu[study_chip-1-1][2].size()+receiver_unit_clu[study_chip-1+1][0].size()+receiver_unit_clu[study_chip-1+1][1].size()+receiver_unit_clu[study_chip-1+1][2].size()) == 0 )
        {   
            // note : we require the first and the last layers have clusters
            // note : and the total numbr of cluster in single layer (l0 and l2) has to be 1

            // note : in the begining, the cut below is at the next if structure, but it will allow the event that has 2 clusters in single layer, but only one cluster pass the following cut. Not good, not tight.
            // note : therefore, the cut is moved here.
            if (receiver_unit_clu[study_chip-1][0].size() == hit_allowed_in_adjacent_layers && receiver_unit_clu[study_chip-1][2].size() == hit_allowed_in_adjacent_layers )
            {
                // note : filter, reject some clusters or events
                for (int i1 = 0; i1 < 3; i1++)
                {
                    for (int i2 = 0; i2 < receiver_unit_clu[study_chip-1][i1].size(); i2++ )
                    {
                        if (i1 == 1) // note : middle layer, all clusters are included
                        {
                            receiver_unit_clu_size_filter[i1].push_back(receiver_unit_clu[study_chip-1][i1][i2]);
                            receiver_unit_clu_pos_size_filter[i1].push_back(receiver_unit_clu_pos[study_chip-1][i1][i2]); 
                            receiver_unit_clu_adc_size_filter[i1].push_back(receiver_unit_clu_adc[study_chip-1][i1][i2]);
                        }
                        else // note : layer 0 and layer 1
                        {
                            if ( receiver_unit_clu[study_chip-1][i1][i2] > cluster_size_requirement ) // note : cluster size cut
                            {
                                if ( receiver_unit_clu_adc[study_chip-1][i1][i2] > cluster_adc_value_requirement ) // note : cluster adc cut
                                {
                                    receiver_unit_clu_size_filter[i1].push_back(receiver_unit_clu[study_chip-1][i1][i2]);
                                    receiver_unit_clu_pos_size_filter[i1].push_back(receiver_unit_clu_pos[study_chip-1][i1][i2]);
                                    receiver_unit_clu_adc_size_filter[i1].push_back(receiver_unit_clu_adc[study_chip-1][i1][i2]);
                                }
                                
                            }
                        }

                    }
                }
            }   
            


            // note : to check whether the first and the last layers have hits or not
            if (receiver_unit_clu_size_filter[0].size() > 0 && receiver_unit_clu_size_filter[2].size() > 0)
            {
                // todo : the following cut is set to be the first index, if the # of allowed cluster is changed, then the following cut has to be modified
                if (receiver_unit_clu_size_filter[0][0]+receiver_unit_clu_size_filter[2][0] > cluster_size_requirement_combine)
                {
                    if ( receiver_unit_clu_size_filter[1].size() > 0 ) // note : the middle layer has at least one cluster
                    {
                        for (int l0 = 0; l0 < receiver_unit_clu_size_filter[0].size(); l0 ++ ) // note : # of clusters of the chip of the layer 0
                        {
                            for (int l1 = 0; l1 < receiver_unit_clu_size_filter[1].size(); l1 ++ ) // note : # of clusters of the chip of the layer 1
                            {
                                for (int l2 = 0; l2 < receiver_unit_clu_size_filter[2].size(); l2 ++ ) // note : # of clusters of the chip of the layer 2
                                {
                                    double hit3_Y_data[3] = {receiver_unit_clu_pos_size_filter[0][l0], receiver_unit_clu_pos_size_filter[1][l1], receiver_unit_clu_pos_size_filter[2][l2]};

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

                                        // note : the middle-layer residual of the DUT method
                                        hit3_best_fit_picker_info[6] = hit3_Y_data[1] - (hit3_Y_data[0]+hit3_Y_data[2])/2.;

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

                                            // note : the middle-layer residual of the DUT method
                                            hit3_best_fit_picker_info[6] = hit3_Y_data[1] - (hit3_Y_data[0]+hit3_Y_data[2])/2.;
                                            

                                        }
                                    }
                                }
                            }
                        } // note : end of 3 layers track finding and fitting

                        // note : for saving the position of the selected clusters
                        good_combination_l0_pos_hit3.push_back(hit3_best_fit_picker_info[3]);
                        good_combination_l1_pos_hit3.push_back(hit3_best_fit_picker_info[4]);
                        good_combination_l2_pos_hit3.push_back(hit3_best_fit_picker_info[5]);

                        // note : save the residual at the middle layer.
                        middle_layer_residual.push_back( hit3_best_fit_picker_info[4] - ((hit3_best_fit_picker_info[3]+hit3_best_fit_picker_info[5])/2.) );
                        // note : save the slope of the good combination
                        good_combination_slope_hit3.push_back( (hit3_best_fit_picker_info[5]-hit3_best_fit_picker_info[3])/actual_xpos[2] + slope_correction);

                        // note : check the slope
                        if ( fabs( (hit3_best_fit_picker_info[5]-hit3_best_fit_picker_info[3])/actual_xpos[2] + slope_correction ) < slope_cut )
                        {
                            // note : the edge exclusion of the first and last layers
                            if ( hit3_best_fit_picker_info[3] > edge_exclusion_bottom && hit3_best_fit_picker_info[3] < edge_exclusion_upper && hit3_best_fit_picker_info[5] > edge_exclusion_bottom && hit3_best_fit_picker_info[5] < edge_exclusion_upper )
                            {
                                // note : after finding the best combination, let's check it, check the residual. 
                                if ( fabs( hit3_best_fit_picker_info[4] - ((hit3_best_fit_picker_info[3]+hit3_best_fit_picker_info[5])/2.) ) < noise_hit_distance ) // todo : noise_hit_distance may need to be changed
                                {
                                    track_111_count += 1;
                                }
                                else 
                                {
                                    track_101_count += 1;
                                    cout<<"101 event, eID : "<<input_cluster_vec[i].eID<<" -> middle has cluster"<<endl;
                                }
                            }
                                
                        }

                    }   
                    else // note : the middle layer has no cluster 
                    {
                        for (int l0 = 0; l0 < receiver_unit_clu_size_filter[0].size(); l0 ++ ) // note : layer 0
                        {
                            for (int l1 = 0; l1 < receiver_unit_clu_size_filter[2].size(); l1 ++ ) // note : layer 2
                            {
                                
                                if (l0 + l1 == 0) // note : the first one
                                {
                                    hit2_slope = fabs( (receiver_unit_clu_pos_size_filter[2][l1] - receiver_unit_clu_pos_size_filter[0][l0]) / actual_xpos[2] + slope_correction );

                                    hit2_best_fit_picker_info[0] = receiver_unit_clu_pos_size_filter[0][l1]; // note : the first layer
                                    hit2_best_fit_picker_info[1] = receiver_unit_clu_pos_size_filter[2][l1]; // note : the last layer
                                }
                                else 
                                {
                                    if ( fabs( (receiver_unit_clu_pos_size_filter[2][l1] - receiver_unit_clu_pos_size_filter[0][l0]) / actual_xpos[2] + slope_correction ) < hit2_slope )
                                    {
                                        hit2_slope = fabs( (receiver_unit_clu_pos_size_filter[2][l1] - receiver_unit_clu_pos_size_filter[0][l0]) / actual_xpos[2] + slope_correction );

                                        hit2_best_fit_picker_info[0] = receiver_unit_clu_pos_size_filter[0][l1]; // note : the first layer
                                        hit2_best_fit_picker_info[1] = receiver_unit_clu_pos_size_filter[2][l1]; // note : the last layer
                                    }
                                }
            
                            }
                        } // note : end of 3 layers track finding and fitting

                        // note : save the slope of the good combination
                        good_combination_slope_hit2.push_back( (hit2_best_fit_picker_info[1]-hit2_best_fit_picker_info[0])/actual_xpos[2] + slope_correction);

                        if ( hit2_slope < slope_cut )
                        {
                            // note : the edge exclusion of the first and last layers
                            if ( hit2_best_fit_picker_info[0] > edge_exclusion_bottom && hit2_best_fit_picker_info[0] < edge_exclusion_upper && hit2_best_fit_picker_info[1] > edge_exclusion_bottom && hit2_best_fit_picker_info[1] < edge_exclusion_upper )
                            {
                                track_101_count += 1;
                                cout<<"101 event, eID : "<<input_cluster_vec[i].eID<<" -> no cluster"<<endl;
                            }
                            
                        }


                    }
                }
            }
        } // note : end of if, No cluster in adjacent chips


        // note : start clean

        // layer_hit_count = 0;
        for (int i1 = 0; i1 < 3; i1++)
        {
            receiver_unit_clu_size_filter[i1].clear();
            receiver_unit_clu_pos_size_filter[i1].clear();  
            receiver_unit_clu_adc_size_filter[i1].clear();
        }    

    } // note : end of for loop, event


    DUT_str output_space;
    output_space.track_111_count = track_111_count;
    output_space.track_101_count = track_101_count;
    output_space.middle_layer_residual = middle_layer_residual;
    output_space.good_combination_slope_hit3 = good_combination_slope_hit3;
    output_space.good_combination_slope_hit2 = good_combination_slope_hit2;
    output_space.good_combination_l0_pos_hit3 = good_combination_l0_pos_hit3;
    output_space.good_combination_l1_pos_hit3 = good_combination_l1_pos_hit3;
    output_space.good_combination_l2_pos_hit3 = good_combination_l2_pos_hit3;
    
    return output_space;

}

// note : version 2 of the efficiency calculation, it's done by the DUT method (Geometry method)
// note : In principle, the results from v1 and v2 should be identical.
// note : the structure of version 2 is more compact. which also means that the v2 is not flexible.
// note : the single cluster at l0 and l2 is mandatory
// note : and it's not friendly to the run89 study
// note : but the middle layer residual distribution can be more correct.
// note : it's more compact and smarter one !!!
DUT_str efficiency_DUT_method_v2 (vector<cluster_reformat_str> input_cluster_vec, int study_chip)
{ 
	double edge_exclusion_bottom = (lower_section_initial - INTT_strip_width / 2.) + INTT_strip_width * double(boundary_cut);
	double edge_exclusion_upper = ( INTT_strip_width * 128. ) - INTT_strip_width * double(boundary_cut);

    vector<int> receiver_unit_clu_size[13][3]; // note : for abbreviation, for hit 
    vector<double> receiver_unit_clu_pos[13][3];
    vector<double> receiver_unit_clu_adc[13][3];

    // note : the clusters that pass the pre-selections (cluster cut / event cut) will be saved here.

    TF1 * linear_fit;
    TGraph * grr;

    double chi2_register = 10000000; 
    double cluster_register_l0 = 0;
    double cluster_register_l1 = 0;
    double cluster_register_l2 = 0;
    double hit3_best_fit_picker_info[7]; // note : first 3 : residual by fittting, second 3 : the selected positions, the 7th : the middle-layer residual by the DUT method
    double hit2_best_fit_picker_info[2]; // note : first 2 : the selected positions of the first and last layers.

    int multi_track_count = 0; // note : this is for finding the multi_track, if we find a new good track, then +=1;

    double hit2_slope;
    double event_residual;

    // note : for the output 
    int track_111_count = 0;
    int track_101_count = 0;
    vector<double> middle_layer_residual; middle_layer_residual.clear();
    vector<double> good_combination_slope_hit3; good_combination_slope_hit3.clear();
    vector<double> good_combination_slope_hit2; good_combination_slope_hit2.clear();

    vector<double> good_combination_l0_pos_hit3; good_combination_l0_pos_hit3.clear();
    vector<double> good_combination_l1_pos_hit3; good_combination_l1_pos_hit3.clear();
    vector<double> good_combination_l2_pos_hit3; good_combination_l2_pos_hit3.clear();

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

    double l0_l2_avg_pos;

    // note : in order to make the efficiency plot as function of position
    vector<double> passed_middle_pos; passed_middle_pos.clear();
    // note : middle layer no hits : 0
    // note : middle layer has close hit : 1
    // note : middle layer has a hit far from the prediction pos : 2.
    vector<int> final_event_result; final_event_result.clear(); 


    for (int i = 0; i < input_cluster_vec.size(); i++)
    {
        if (i % 1000 == 0){ cout<<"Doing the DUT test, eID : "<<input_cluster_vec[i].eID<<endl;}

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

        // todo : the LoE
        // todo :       -> right now, what I try is to make sure there is no cluster in the adjacent chips for all three layers (chip 8 and 10, for example)
        // todo :       -> to avoid any suspicious events. 
        // todo :       -> works well !!! 2022/11/23
        // todo : the adc cut
        // todo : cluster size 
        // todo : cluster size combined
        // todo : N cluster in layer 0 and layer 2
        // todo : slope cut
        // todo : the edge exclusion

        // todo : residual cut
        
        event_counting_1 += 1;

        // note : zero cluster in adjacent chips
        if ( (receiver_unit_clu_size[study_chip-1-1][0].size()+receiver_unit_clu_size[study_chip-1-1][1].size()+receiver_unit_clu_size[study_chip-1-1][2].size()+receiver_unit_clu_size[study_chip-1+1][0].size()+receiver_unit_clu_size[study_chip-1+1][1].size()+receiver_unit_clu_size[study_chip-1+1][2].size()) != 0 ) continue;
        event_counting_2 += 1;

        // note : single cluster in l0 and l2
        if (receiver_unit_clu_size[study_chip-1][0].size() != hit_allowed_in_adjacent_layers || receiver_unit_clu_size[study_chip-1][2].size() != hit_allowed_in_adjacent_layers) continue;
        event_counting_3 += 1;

        // note : standalone cluster size cut of l0 and l2
        if ( receiver_unit_clu_size[study_chip-1][0][0] <= cluster_size_requirement || receiver_unit_clu_size[study_chip-1][2][0] <= cluster_size_requirement ) continue;
        event_counting_4 += 1;

        // note : standalone cluster adc cut
        if ( receiver_unit_clu_adc[study_chip-1][0][0] <= cluster_adc_value_requirement || receiver_unit_clu_adc[study_chip-1][2][0] <= cluster_adc_value_requirement ) continue;
        event_counting_5 += 1;

        // note : combined cluster size cut
        if ( receiver_unit_clu_size[study_chip-1][0][0]+receiver_unit_clu_size[study_chip-1][2][0] <= cluster_size_requirement_combine ) continue;
        event_counting_6 += 1;


        // note : edge exclusion cut of the l0
        if ( receiver_unit_clu_pos[study_chip-1][0][0] <= edge_exclusion_bottom || receiver_unit_clu_pos[study_chip-1][0][0] >= edge_exclusion_upper ) continue;
        event_counting_7 += 1;

        // note : edge exclusion cut of the l2
        if ( receiver_unit_clu_pos[study_chip-1][2][0] <= edge_exclusion_bottom || receiver_unit_clu_pos[study_chip-1][2][0] >= edge_exclusion_upper ) continue;
        event_counting_8 += 1;

        hit2_slope = (receiver_unit_clu_pos[study_chip-1][2][0] - receiver_unit_clu_pos[study_chip-1][0][0]) / actual_xpos[2] + slope_correction;
        good_combination_slope_hit2.push_back(hit2_slope);

        // note : the slope cut
        if ( fabs(hit2_slope) >= slope_cut ) continue;
        event_counting_9 += 1;
        
        l0_l2_avg_pos = ( receiver_unit_clu_pos[study_chip-1][0][0] + receiver_unit_clu_pos[study_chip-1][2][0] ) / 2.;

        passed_middle_pos.push_back( l0_l2_avg_pos );

        // note : to check the N clusters of the middle layer
        if ( receiver_unit_clu_pos[study_chip-1][1].size() == 0 ) // note : no hits in the middle
        {
            event_counting_10 += 1;

            final_event_result.push_back(0); // note : in order to make the detection efficiency as function of pos.

            track_101_count += 1;
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
                    
                    cluster_register_l1 = l1;

                    hit3_best_fit_picker_info[0] = ( hit3_Y_data[0] - ( linear_fit -> GetParameter(1) * actual_xpos[0] + linear_fit -> GetParameter(0) ) );
                    hit3_best_fit_picker_info[1] = ( hit3_Y_data[1] - ( linear_fit -> GetParameter(1) * actual_xpos[1] + linear_fit -> GetParameter(0) ) );
                    hit3_best_fit_picker_info[2] = ( hit3_Y_data[2] - ( linear_fit -> GetParameter(1) * actual_xpos[2] + linear_fit -> GetParameter(0) ) );
                    
                    hit3_best_fit_picker_info[3] = hit3_Y_data[0];
                    hit3_best_fit_picker_info[4] = hit3_Y_data[1];
                    hit3_best_fit_picker_info[5] = hit3_Y_data[2];

                    // note : the middle-layer residual of the DUT method
                    hit3_best_fit_picker_info[6] = hit3_Y_data[1] - (hit3_Y_data[0]+hit3_Y_data[2])/2.;

                }
                else 
                {
                    if ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) < chi2_register )
                    {
                        chi2_register = ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) );
                        
                        cluster_register_l1 = l1;

                        hit3_best_fit_picker_info[0] = ( hit3_Y_data[0] - ( linear_fit -> GetParameter(1) * actual_xpos[0] + linear_fit -> GetParameter(0) ) );
                        hit3_best_fit_picker_info[1] = ( hit3_Y_data[1] - ( linear_fit -> GetParameter(1) * actual_xpos[1] + linear_fit -> GetParameter(0) ) );
                        hit3_best_fit_picker_info[2] = ( hit3_Y_data[2] - ( linear_fit -> GetParameter(1) * actual_xpos[2] + linear_fit -> GetParameter(0) ) );
                        
                        hit3_best_fit_picker_info[3] = hit3_Y_data[0];
                        hit3_best_fit_picker_info[4] = hit3_Y_data[1];
                        hit3_best_fit_picker_info[5] = hit3_Y_data[2];

                        // note : the middle-layer residual of the DUT method
                        hit3_best_fit_picker_info[6] = hit3_Y_data[1] - (hit3_Y_data[0]+hit3_Y_data[2])/2.;
                        

                    }
                }

                grr->Delete();
                linear_fit->Delete();

            }

            event_residual = hit3_best_fit_picker_info[4] - ( l0_l2_avg_pos );

            middle_layer_residual.push_back( event_residual );

            good_combination_l0_pos_hit3.push_back( receiver_unit_clu_pos[study_chip-1][0][0] );
            good_combination_l1_pos_hit3.push_back( hit3_best_fit_picker_info[4] );
            good_combination_l2_pos_hit3.push_back( receiver_unit_clu_pos[study_chip-1][2][0] );

            if ( fabs(event_residual) < noise_hit_distance )
            {
                event_counting_11 += 1;

                final_event_result.push_back(1); // note : in order to make the detection efficiency as function of pos.

                track_111_count += 1;
            }
            else 
            {
                event_counting_12 += 1;

                final_event_result.push_back(2); // note : in order to make the detection efficiency as function of pos.

                track_101_count += 1;   
                cout<<"101 event, eID : "<<input_cluster_vec[i].eID<<" -> middle has cluster"<<endl;
            }

        }
        // note : start clean

        chi2_register = 10000000;

    } // note : end of for loop, event


    DUT_str output_space;
    output_space.track_111_count = track_111_count;
    output_space.track_101_count = track_101_count;
    output_space.middle_layer_residual = middle_layer_residual;
    output_space.good_combination_slope_hit3 = good_combination_slope_hit3; // note : empty, 2022/12/03
    output_space.good_combination_slope_hit2 = good_combination_slope_hit2; // note : 3hits or 2hits cases are included
    output_space.good_combination_l0_pos_hit3 = good_combination_l0_pos_hit3; 
    output_space.good_combination_l1_pos_hit3 = good_combination_l1_pos_hit3;
    output_space.good_combination_l2_pos_hit3 = good_combination_l2_pos_hit3;
    
    // note : in order to make the plot of the detection efficiency as a function of position. 
    output_space.passed_middle_pos = passed_middle_pos;
    output_space.final_event_result = final_event_result;

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
    
    return output_space;

}


DUT_str efficiency_DUT_method_residual_test (vector<cluster_reformat_str> input_cluster_vec, int study_chip, int selection_level)
{ 
	double edge_exclusion_bottom = (lower_section_initial - INTT_strip_width / 2.) + INTT_strip_width * double(boundary_cut);
	double edge_exclusion_upper = ( INTT_strip_width * 128. ) - INTT_strip_width * double(boundary_cut);

    vector<int> receiver_unit_clu_size[13][3]; // note : for abbreviation, for hit 
    vector<double> receiver_unit_clu_pos[13][3];
    vector<double> receiver_unit_clu_adc[13][3];

    // note : the clusters that pass the pre-selections (cluster cut / event cut) will be saved here.

    TF1 * linear_fit;
    TGraph * grr;

    double chi2_register = 10000000; 
    double cluster_register_l0 = 0;
    double cluster_register_l1 = 0;
    double cluster_register_l2 = 0;
    double hit3_best_fit_picker_info[7]; // note : first 3 : residual by fittting, second 3 : the selected positions, the 7th : the middle-layer residual by the DUT method
    double hit2_best_fit_picker_info[2]; // note : first 2 : the selected positions of the first and last layers.

    int multi_track_count = 0; // note : this is for finding the multi_track, if we find a new good track, then +=1;

    double hit2_slope;
    double event_residual;

    // note : for the output 
    int track_111_count = 0;
    int track_101_count = 0;
    vector<double> middle_layer_residual; middle_layer_residual.clear();
    vector<double> good_combination_slope_hit3; good_combination_slope_hit3.clear();
    vector<double> good_combination_slope_hit2; good_combination_slope_hit2.clear();

    vector<double> good_combination_l0_pos_hit3; good_combination_l0_pos_hit3.clear();
    vector<double> good_combination_l1_pos_hit3; good_combination_l1_pos_hit3.clear();
    vector<double> good_combination_l2_pos_hit3; good_combination_l2_pos_hit3.clear();

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

    double l0_l2_avg_pos;

    // note : in order to make the efficiency plot as function of position
    vector<double> passed_middle_pos; passed_middle_pos.clear();
    // note : middle layer no hits : 0
    // note : middle layer has close hit : 1
    // note : middle layer has a hit far from the prediction pos : 2.
    vector<int> final_event_result; final_event_result.clear(); 


    for (int i = 0; i < input_cluster_vec.size(); i++)
    {
        if (i % 1000 == 0){ cout<<"Doing the DUT test, eID : "<<input_cluster_vec[i].eID<<endl;}

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

        // todo : the LoE
        // todo :       -> right now, what I try is to make sure there is no cluster in the adjacent chips for all three layers (chip 8 and 10, for example)
        // todo :       -> to avoid any suspicious events. 
        // todo :       -> works well !!! 2022/11/23
        // todo : the adc cut
        // todo : cluster size 
        // todo : cluster size combined
        // todo : N cluster in layer 0 and layer 2
        // todo : slope cut
        // todo : the edge exclusion

        // todo : residual cut
        
        event_counting_1 += 1;

        // note : 2023_06_29 the order of the "zero cluster in adjacent chips" and "single cluster in l0 & l2" is swapped

        // note : single cluster in l0 and l2
        if (receiver_unit_clu_size[study_chip-1][0].size() != hit_allowed_in_adjacent_layers || receiver_unit_clu_size[study_chip-1][2].size() != hit_allowed_in_adjacent_layers) continue;
        event_counting_2 += 1;

        if ( selection_level > 1 )
            // note : zero cluster in adjacent chips
            if ( (receiver_unit_clu_size[study_chip-1-1][0].size()+receiver_unit_clu_size[study_chip-1-1][1].size()+receiver_unit_clu_size[study_chip-1-1][2].size()+receiver_unit_clu_size[study_chip-1+1][0].size()+receiver_unit_clu_size[study_chip-1+1][1].size()+receiver_unit_clu_size[study_chip-1+1][2].size()) != 0 ) continue;
            event_counting_3 += 1;

        // if ( selection_level > 2 )
        //     // note : standalone cluster size cut of l0 and l2
        //     if ( receiver_unit_clu_size[study_chip-1][0][0] <= cluster_size_requirement || receiver_unit_clu_size[study_chip-1][2][0] <= cluster_size_requirement ) continue;
        //     event_counting_4 += 1;

        if ( selection_level > 2 )
            // note : standalone cluster adc cut
            if ( receiver_unit_clu_adc[study_chip-1][0][0] <= cluster_adc_value_requirement || receiver_unit_clu_adc[study_chip-1][2][0] <= cluster_adc_value_requirement ) continue;
            event_counting_4 += 1;

        // if ( selection_level > 4 )
        //     // note : combined cluster size cut
        //     if ( receiver_unit_clu_size[study_chip-1][0][0]+receiver_unit_clu_size[study_chip-1][2][0] <= cluster_size_requirement_combine ) continue;
        //     event_counting_6 += 1;

        if ( selection_level > 3 )
            // note : edge exclusion cut of the l0
            if ( receiver_unit_clu_pos[study_chip-1][0][0] <= edge_exclusion_bottom || receiver_unit_clu_pos[study_chip-1][0][0] >= edge_exclusion_upper ) continue;
            event_counting_5+= 1;

        if ( selection_level > 4 )
            // note : edge exclusion cut of the l2
            if ( receiver_unit_clu_pos[study_chip-1][2][0] <= edge_exclusion_bottom || receiver_unit_clu_pos[study_chip-1][2][0] >= edge_exclusion_upper ) continue;
            event_counting_6 += 1;

        hit2_slope = (receiver_unit_clu_pos[study_chip-1][2][0] - receiver_unit_clu_pos[study_chip-1][0][0]) / actual_xpos[2] + slope_correction;
        good_combination_slope_hit2.push_back(hit2_slope);

        l0_l2_avg_pos = ( receiver_unit_clu_pos[study_chip-1][0][0] + receiver_unit_clu_pos[study_chip-1][2][0] ) / 2.;
        passed_middle_pos.push_back( l0_l2_avg_pos );

        if ( selection_level > 5 )
            // note : the slope cut
            if ( fabs(hit2_slope) >= slope_cut ) continue;
            event_counting_7 += 1;

        // note : to check the N clusters of the middle layer
        if ( receiver_unit_clu_pos[study_chip-1][1].size() == 0 ) // note : no hits in the middle
        {
            event_counting_10 += 1;

            final_event_result.push_back(0); // note : in order to make the detection efficiency as function of pos.

            track_101_count += 1;
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
                    
                    cluster_register_l1 = l1;

                    hit3_best_fit_picker_info[0] = ( hit3_Y_data[0] - ( linear_fit -> GetParameter(1) * actual_xpos[0] + linear_fit -> GetParameter(0) ) );
                    hit3_best_fit_picker_info[1] = ( hit3_Y_data[1] - ( linear_fit -> GetParameter(1) * actual_xpos[1] + linear_fit -> GetParameter(0) ) );
                    hit3_best_fit_picker_info[2] = ( hit3_Y_data[2] - ( linear_fit -> GetParameter(1) * actual_xpos[2] + linear_fit -> GetParameter(0) ) );
                    
                    hit3_best_fit_picker_info[3] = hit3_Y_data[0];
                    hit3_best_fit_picker_info[4] = hit3_Y_data[1];
                    hit3_best_fit_picker_info[5] = hit3_Y_data[2];

                    // note : the middle-layer residual of the DUT method
                    hit3_best_fit_picker_info[6] = hit3_Y_data[1] - (hit3_Y_data[0]+hit3_Y_data[2])/2.;

                }
                else 
                {
                    if ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) < chi2_register )
                    {
                        chi2_register = ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) );
                        
                        cluster_register_l1 = l1;

                        hit3_best_fit_picker_info[0] = ( hit3_Y_data[0] - ( linear_fit -> GetParameter(1) * actual_xpos[0] + linear_fit -> GetParameter(0) ) );
                        hit3_best_fit_picker_info[1] = ( hit3_Y_data[1] - ( linear_fit -> GetParameter(1) * actual_xpos[1] + linear_fit -> GetParameter(0) ) );
                        hit3_best_fit_picker_info[2] = ( hit3_Y_data[2] - ( linear_fit -> GetParameter(1) * actual_xpos[2] + linear_fit -> GetParameter(0) ) );
                        
                        hit3_best_fit_picker_info[3] = hit3_Y_data[0];
                        hit3_best_fit_picker_info[4] = hit3_Y_data[1];
                        hit3_best_fit_picker_info[5] = hit3_Y_data[2];

                        // note : the middle-layer residual of the DUT method
                        hit3_best_fit_picker_info[6] = hit3_Y_data[1] - (hit3_Y_data[0]+hit3_Y_data[2])/2.;
                        

                    }
                }

                grr->Delete();
                linear_fit->Delete();

            }

            event_residual = hit3_best_fit_picker_info[4] - ( l0_l2_avg_pos );

            middle_layer_residual.push_back( event_residual );

            good_combination_l0_pos_hit3.push_back( receiver_unit_clu_pos[study_chip-1][0][0] );
            good_combination_l1_pos_hit3.push_back( hit3_best_fit_picker_info[4] );
            good_combination_l2_pos_hit3.push_back( receiver_unit_clu_pos[study_chip-1][2][0] );

            if ( fabs(event_residual) < noise_hit_distance )
            {
                event_counting_11 += 1;

                final_event_result.push_back(1); // note : in order to make the detection efficiency as function of pos.

                track_111_count += 1;
            }
            else 
            {
                event_counting_12 += 1;

                final_event_result.push_back(2); // note : in order to make the detection efficiency as function of pos.

                track_101_count += 1;   
                cout<<"101 event, eID : "<<input_cluster_vec[i].eID<<" -> middle has cluster"<<endl;
            }

        }
        // note : start clean

        chi2_register = 10000000;

    } // note : end of for loop, event


    DUT_str output_space;
    output_space.track_111_count = track_111_count;
    output_space.track_101_count = track_101_count;
    output_space.middle_layer_residual = middle_layer_residual;
    output_space.good_combination_slope_hit3 = good_combination_slope_hit3; // note : empty, 2022/12/03
    output_space.good_combination_slope_hit2 = good_combination_slope_hit2; // note : 3hits or 2hits cases are included
    output_space.good_combination_l0_pos_hit3 = good_combination_l0_pos_hit3; 
    output_space.good_combination_l1_pos_hit3 = good_combination_l1_pos_hit3;
    output_space.good_combination_l2_pos_hit3 = good_combination_l2_pos_hit3;
    
    // note : in order to make the plot of the detection efficiency as a function of position. 
    output_space.passed_middle_pos = passed_middle_pos;
    output_space.final_event_result = final_event_result;

    cout<<"event counting 1 \t : "<<event_counting_1<<" all "<<endl;
    cout<<"event counting 2 \t : "<<event_counting_2<<" single cluster "<<endl;
    cout<<"event counting 3 \t : "<<event_counting_3<<" adjacent chip, no cluster  "<<endl;
    // cout<<"event counting 4 \t : "<<event_counting_4<<" standalone cluster requirement "<<endl;
    cout<<"event counting 4 \t : "<<event_counting_4<<" cluster adc "<<endl;
    // cout<<"event counting 6 \t : "<<event_counting_6<<" cluster size combine "<<endl;
    cout<<"event counting 5 \t : "<<event_counting_5<<" l0 cluster edge exclusion "<<endl;
    cout<<"event counting 6 \t : "<<event_counting_6<<" l2 cluster edge exclusion "<<endl;
    cout<<"event counting 7 \t : "<<event_counting_7<<" slope cut "<<endl;

    cout<<"event counting 10 \t : "<<event_counting_10<<" no hits in middle layer "<<endl;
    cout<<"event counting 11 \t : "<<event_counting_11<<" have good cluster in middle "<<endl;
    cout<<"event counting 12 \t : "<<event_counting_12<<" no good cluster in middle "<<endl;
    
    return output_space;

}

// note : this is kinda for the scattering study or the residual distribution. 
// note : set the slope angle seems to be not sense. 
DUT_str efficiency_DUT_method_residual_test_2 (vector<cluster_reformat_str> input_cluster_vec, int study_chip)
{ 
	double edge_exclusion_bottom = (lower_section_initial - INTT_strip_width / 2.) + INTT_strip_width * double(boundary_cut);
	double edge_exclusion_upper = ( INTT_strip_width * 128. ) - INTT_strip_width * double(boundary_cut);

    vector<int> receiver_unit_clu_size[13][3]; // note : for abbreviation, for hit 
    vector<double> receiver_unit_clu_pos[13][3];
    vector<double> receiver_unit_clu_adc[13][3];

    // note : the clusters that pass the pre-selections (cluster cut / event cut) will be saved here.

    TF1 * linear_fit;
    TGraph * grr;

    double chi2_register = 10000000; 
    double cluster_register_l0 = 0;
    double cluster_register_l1 = 0;
    double cluster_register_l2 = 0;
    double hit3_best_fit_picker_info[7]; // note : first 3 : residual by fittting, second 3 : the selected positions, the 7th : the middle-layer residual by the DUT method
    double hit2_best_fit_picker_info[2]; // note : first 2 : the selected positions of the first and last layers.

    int multi_track_count = 0; // note : this is for finding the multi_track, if we find a new good track, then +=1;

    double hit2_slope;
    double event_residual;
    

    int N_cluster_l0 = 0;
    int N_cluster_l1 = 0;
    int N_cluster_l2 = 0;

    double l0_l1_pos, l0_l1_slope;
    vector<double> l0_l1_pos_vec; l0_l1_pos_vec.clear();
    vector<double> l0_l1_slope_vec; l0_l1_slope_vec.clear();
    vector<double> scattering_vec; scattering_vec.clear();

    // note : for the output 
    int track_111_count = 0;
    int track_101_count = 0;
    vector<double> middle_layer_residual; middle_layer_residual.clear();
    vector<double> good_combination_slope_hit3; good_combination_slope_hit3.clear();
    vector<double> good_combination_slope_hit2; good_combination_slope_hit2.clear();

    vector<double> good_combination_l0_pos_hit3; good_combination_l0_pos_hit3.clear();
    vector<double> good_combination_l1_pos_hit3; good_combination_l1_pos_hit3.clear();
    vector<double> good_combination_l2_pos_hit3; good_combination_l2_pos_hit3.clear();

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

    double l0_l2_avg_pos;

    // note : in order to make the efficiency plot as function of position
    vector<double> passed_middle_pos; passed_middle_pos.clear();
    // note : middle layer no hits : 0
    // note : middle layer has close hit : 1
    // note : middle layer has a hit far from the prediction pos : 2.
    vector<int> final_event_result; final_event_result.clear(); 


    for (int i = 0; i < input_cluster_vec.size(); i++)
    {
        if (i % 1000 == 0){ cout<<"Doing the DUT test, eID : "<<input_cluster_vec[i].eID<<endl;}

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

        // todo : the LoE
        // todo :       -> right now, what I try is to make sure there is no cluster in the adjacent chips for all three layers (chip 8 and 10, for example)
        // todo :       -> to avoid any suspicious events. 
        // todo :       -> works well !!! 2022/11/23
        // todo : the adc cut
        // todo : cluster size 
        // todo : cluster size combined
        // todo : N cluster in layer 0 and layer 2
        // todo : slope cut
        // todo : the edge exclusion

        // todo : residual cut
        
        event_counting_1 += 1;      
        
        // note : calculate the total_N_cluster in each layer
        for (int i1 = 0; i1 < 13; i1++)
        {
            N_cluster_l0 += receiver_unit_clu_size[i1][0].size();
            N_cluster_l1 += receiver_unit_clu_size[i1][1].size();
            N_cluster_l2 += receiver_unit_clu_size[i1][2].size();
        } 

        // cout<<"N_cluster : "<<N_cluster_l0<<" "<<N_cluster_l1<<" "<<N_cluster_l2<<endl;

        // note : only one cluster in each layer
        if ( N_cluster_l0 != 1 || N_cluster_l1 != 1 || N_cluster_l2 != 1 ) 
        {
            N_cluster_l0 = 0;
            N_cluster_l1 = 0;
            N_cluster_l2 = 0;
            continue;
        }
        event_counting_2 += 1;

        // note : single cluster in l0 and l2 AND "l1"
        if (receiver_unit_clu_size[study_chip-1][0].size() != hit_allowed_in_adjacent_layers || receiver_unit_clu_size[study_chip-1][1].size() != hit_allowed_in_adjacent_layers || receiver_unit_clu_size[study_chip-1][2].size() != hit_allowed_in_adjacent_layers) continue;
        event_counting_3 += 1;

        // note : standalone cluster adc cut
        if ( receiver_unit_clu_adc[study_chip-1][0][0] <= cluster_adc_value_requirement_scattering || receiver_unit_clu_adc[study_chip-1][1][0] <= cluster_adc_value_requirement_scattering || receiver_unit_clu_adc[study_chip-1][2][0] <= cluster_adc_value_requirement_scattering ) continue;
        event_counting_4 += 1;


        l0_l1_slope = (receiver_unit_clu_pos[study_chip-1][1][0] - receiver_unit_clu_pos[study_chip-1][0][0]) / actual_xpos[1] + slope_correction_l0_l1;
        l0_l1_slope_vec.push_back( l0_l1_slope );

        l0_l1_pos = ( receiver_unit_clu_pos[study_chip-1][0][0] + receiver_unit_clu_pos[study_chip-1][1][0] ) / 2.;
        l0_l1_pos_vec.push_back( l0_l1_pos );

        
        
        // note : the slope cut
        if ( fabs(l0_l1_slope) >= slope_cut_scattering ) continue;
        event_counting_5 += 1;

        // note : the position cut
        if ( fabs(l0_l1_pos) >= pos_cut_scattering ) continue;
        event_counting_6 += 1;

        

        // note : to check the N clusters of the middle layer
        // if ( receiver_unit_clu_pos[study_chip-1][1].size() == 0 ) // note : no hits in the middle
        // {
        //     event_counting_10 += 1;

        //     final_event_result.push_back(0); // note : in order to make the detection efficiency as function of pos.

        //     track_101_count += 1;
        //     cout<<"101 event, eID : "<<input_cluster_vec[i].eID<<" -> no cluster"<<endl;
        // }
        // else if ( receiver_unit_clu_pos[study_chip-1][1].size() > 0 ) // note : at least one hit in the middle
        // {
            
        //     for ( int l1 = 0; l1 < receiver_unit_clu_pos[study_chip-1][1].size(); l1++ )
        //     {
        //         // if (receiver_unit_clu_adc[study_chip-1][1][l1] <= cluster_adc_value_requirement ) continue; // note : to add the ADC0 cut at L1

        //         double hit3_Y_data[3] = {receiver_unit_clu_pos[study_chip-1][0][0], receiver_unit_clu_pos[study_chip-1][1][l1], receiver_unit_clu_pos[study_chip-1][2][0]};

        //         grr = new TGraph(3,actual_xpos,hit3_Y_data);
        //         linear_fit = new TF1("linear_fit","pol1",-1,actual_xpos[2]+2);
        //         grr -> Fit("linear_fit","NQ");

        //         if (l1 == 0) // note : the first one
        //         {
        //             chi2_register = ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) );
                    
        //             cluster_register_l1 = l1;

        //             hit3_best_fit_picker_info[0] = ( hit3_Y_data[0] - ( linear_fit -> GetParameter(1) * actual_xpos[0] + linear_fit -> GetParameter(0) ) );
        //             hit3_best_fit_picker_info[1] = ( hit3_Y_data[1] - ( linear_fit -> GetParameter(1) * actual_xpos[1] + linear_fit -> GetParameter(0) ) );
        //             hit3_best_fit_picker_info[2] = ( hit3_Y_data[2] - ( linear_fit -> GetParameter(1) * actual_xpos[2] + linear_fit -> GetParameter(0) ) );
                    
        //             hit3_best_fit_picker_info[3] = hit3_Y_data[0];
        //             hit3_best_fit_picker_info[4] = hit3_Y_data[1];
        //             hit3_best_fit_picker_info[5] = hit3_Y_data[2];

        //             // note : the middle-layer residual of the DUT method
        //             hit3_best_fit_picker_info[6] = hit3_Y_data[1] - (hit3_Y_data[0]+hit3_Y_data[2])/2.;

        //         }
        //         else 
        //         {
        //             if ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) < chi2_register )
        //             {
        //                 chi2_register = ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) );
                        
        //                 cluster_register_l1 = l1;

        //                 hit3_best_fit_picker_info[0] = ( hit3_Y_data[0] - ( linear_fit -> GetParameter(1) * actual_xpos[0] + linear_fit -> GetParameter(0) ) );
        //                 hit3_best_fit_picker_info[1] = ( hit3_Y_data[1] - ( linear_fit -> GetParameter(1) * actual_xpos[1] + linear_fit -> GetParameter(0) ) );
        //                 hit3_best_fit_picker_info[2] = ( hit3_Y_data[2] - ( linear_fit -> GetParameter(1) * actual_xpos[2] + linear_fit -> GetParameter(0) ) );
                        
        //                 hit3_best_fit_picker_info[3] = hit3_Y_data[0];
        //                 hit3_best_fit_picker_info[4] = hit3_Y_data[1];
        //                 hit3_best_fit_picker_info[5] = hit3_Y_data[2];

        //                 // note : the middle-layer residual of the DUT method
        //                 hit3_best_fit_picker_info[6] = hit3_Y_data[1] - (hit3_Y_data[0]+hit3_Y_data[2])/2.;
                        

        //             }
        //         }

        //         grr->Delete();
        //         linear_fit->Delete();

        //     }

        //     event_residual = hit3_best_fit_picker_info[4] - ( l0_l2_avg_pos );

        //     good_combination_l0_pos_hit3.push_back( receiver_unit_clu_pos[study_chip-1][0][0] );
        //     good_combination_l1_pos_hit3.push_back( hit3_best_fit_picker_info[4] );
        //     good_combination_l2_pos_hit3.push_back( receiver_unit_clu_pos[study_chip-1][2][0] );

        //     if ( fabs(event_residual) < noise_hit_distance )
        //     {
        //         event_counting_11 += 1;

        //         final_event_result.push_back(1); // note : in order to make the detection efficiency as function of pos.

        //         track_111_count += 1;
        //     }
        //     else 
        //     {
        //         event_counting_12 += 1;

        //         final_event_result.push_back(2); // note : in order to make the detection efficiency as function of pos.

        //         track_101_count += 1;   
        //         cout<<"101 event, eID : "<<input_cluster_vec[i].eID<<" -> middle has cluster"<<endl;
        //     }

        // }
        // note : start clean
        
        middle_layer_residual.push_back( receiver_unit_clu_pos[study_chip-1][1][0] - (( receiver_unit_clu_pos[study_chip-1][0][0] + receiver_unit_clu_pos[study_chip-1][2][0] )/2.) );
        scattering_vec.push_back( ( receiver_unit_clu_pos[study_chip-1][2][0] - receiver_unit_clu_pos[study_chip-1][1][0] )/actual_xpos[1] - ( receiver_unit_clu_pos[study_chip-1][1][0] - receiver_unit_clu_pos[study_chip-1][0][0] )/actual_xpos[1] );

        N_cluster_l0 = 0;
        N_cluster_l1 = 0;
        N_cluster_l2 = 0;
        chi2_register = 10000000;

    } // note : end of for loop, event


    DUT_str output_space;
    // output_space.track_111_count = track_111_count;
    // output_space.track_101_count = track_101_count;
    output_space.middle_layer_residual = middle_layer_residual;
    // output_space.good_combination_slope_hit3 = good_combination_slope_hit3; // note : empty, 2022/12/03
    // output_space.good_combination_slope_hit2 = good_combination_slope_hit2; // note : 3hits or 2hits cases are included
    // output_space.good_combination_l0_pos_hit3 = good_combination_l0_pos_hit3; 
    // output_space.good_combination_l1_pos_hit3 = good_combination_l1_pos_hit3;
    // output_space.good_combination_l2_pos_hit3 = good_combination_l2_pos_hit3;
    
    // note : in order to make the plot of the detection efficiency as a function of position. 
    // output_space.passed_middle_pos = passed_middle_pos;
    // output_space.final_event_result = final_event_result;
    
    // note : for the scattering study, new attempt 
    output_space.l0_l1_pos_vec = l0_l1_pos_vec;
    output_space.l0_l1_slope_vec = l0_l1_slope_vec;
    output_space.scattering_vec = scattering_vec;

    cout<<"event counting 1 \t : "<<event_counting_1<<" all "<<endl;

    cout<<"event counting 2 \t : "<<event_counting_2<<" single cluster each layer "<<endl;    
    cout<<"event counting 3 \t : "<<event_counting_3<<" selected chip 1 cluster, all layers  "<<endl;
    cout<<"event counting 4 \t : "<<event_counting_4<<" cluster adc "<<endl;
    cout<<"event counting 5 \t : "<<event_counting_5<<" l0_l1_slope cut "<<endl;
    cout<<"event counting 6 \t : "<<event_counting_6<<" l0_l1_pos cut "<<endl;
    
    return output_space;

}

// note : this is just for the alignment plots
// note : after tracking, only apply two criteria
DUT_str Alignment_DUT (vector<cluster_reformat_str> input_cluster_vec, int study_chip)
{ 
	double edge_exclusion_bottom = (lower_section_initial - INTT_strip_width / 2.) + INTT_strip_width * double(boundary_cut);
	double edge_exclusion_upper = ( INTT_strip_width * 128. ) - INTT_strip_width * double(boundary_cut);

    vector<int> receiver_unit_clu_size[13][3]; // note : for abbreviation, for hit 
    vector<double> receiver_unit_clu_pos[13][3];
    vector<double> receiver_unit_clu_adc[13][3];

    // note : the clusters that pass the pre-selections (cluster cut / event cut) will be saved here.

    TF1 * linear_fit;
    TGraph * grr;

    double chi2_register = 0; 
    double cluster_register_l0 = 0;
    double cluster_register_l1 = 0;
    double cluster_register_l2 = 0;
    double hit3_best_fit_picker_info[7]; // note : first 3 : residual by fittting, second 3 : the selected positions, the 7th : the middle-layer residual by the DUT method
    double hit2_best_fit_picker_info[2]; // note : first 2 : the selected positions of the first and last layers.

    int multi_track_count = 0; // note : this is for finding the multi_track, if we find a new good track, then +=1;

    double hit2_slope;
    double event_residual;

    // note : for the output 
    int track_111_count = 0;
    int track_101_count = 0;
    vector<double> middle_layer_residual; middle_layer_residual.clear();
    vector<double> good_combination_slope_hit3; good_combination_slope_hit3.clear();
    vector<double> good_combination_slope_hit2; good_combination_slope_hit2.clear();

    vector<double> good_combination_l0_pos_hit3; good_combination_l0_pos_hit3.clear();
    vector<double> good_combination_l1_pos_hit3; good_combination_l1_pos_hit3.clear();
    vector<double> good_combination_l2_pos_hit3; good_combination_l2_pos_hit3.clear();

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

    double l0_l2_avg_pos;

    // note : in order to make the efficiency plot as function of position
    vector<double> passed_middle_pos; passed_middle_pos.clear();
    // note : middle layer no hits : 0
    // note : middle layer has close hit : 1
    // note : middle layer has a hit far from the prediction pos : 2.
    vector<int> final_event_result; final_event_result.clear(); 


    for (int i = 0; i < input_cluster_vec.size(); i++)
    {
        if (i % 1000 == 0){ cout<<"Doing the DUT test, eID : "<<input_cluster_vec[i].eID<<endl;}

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

        // todo : the LoE
        // todo :       -> right now, what I try is to make sure there is no cluster in the adjacent chips for all three layers (chip 8 and 10, for example)
        // todo :       -> to avoid any suspicious events. 
        // todo :       -> works well !!! 2022/11/23
        // todo : the adc cut
        // todo : cluster size 
        // todo : cluster size combined
        // todo : N cluster in layer 0 and layer 2
        // todo : slope cut
        // todo : the edge exclusion

        // todo : residual cut
        
        event_counting_1 += 1;

        // note : zero cluster in adjacent chips
        if ( (receiver_unit_clu_size[study_chip-1-1][0].size()+receiver_unit_clu_size[study_chip-1-1][1].size()+receiver_unit_clu_size[study_chip-1-1][2].size()+receiver_unit_clu_size[study_chip-1+1][0].size()+receiver_unit_clu_size[study_chip-1+1][1].size()+receiver_unit_clu_size[study_chip-1+1][2].size()) != 0 ) continue;
        event_counting_2 += 1;

        // note : single cluster in l0 and l2
        if (receiver_unit_clu_size[study_chip-1][0].size() != hit_allowed_in_adjacent_layers || receiver_unit_clu_size[study_chip-1][2].size() != hit_allowed_in_adjacent_layers) continue;
        event_counting_3 += 1;

        // note : standalone cluster size cut of l0 and l2
        // if ( receiver_unit_clu_size[study_chip-1][0][0] <= cluster_size_requirement || receiver_unit_clu_size[study_chip-1][2][0] <= cluster_size_requirement ) continue;
        // event_counting_4 += 1;

        // note : standalone cluster adc cut
        // if ( receiver_unit_clu_adc[study_chip-1][0][0] <= cluster_adc_value_requirement || receiver_unit_clu_adc[study_chip-1][2][0] <= cluster_adc_value_requirement ) continue;
        // event_counting_5 += 1;

        // note : combined cluster size cut
        // if ( receiver_unit_clu_size[study_chip-1][0][0]+receiver_unit_clu_size[study_chip-1][2][0] <= cluster_size_requirement_combine ) continue;
        // event_counting_6 += 1;


        // note : edge exclusion cut of the l0
        // if ( receiver_unit_clu_pos[study_chip-1][0][0] <= edge_exclusion_bottom || receiver_unit_clu_pos[study_chip-1][0][0] >= edge_exclusion_upper ) continue;
        // event_counting_7 += 1;

        // note : edge exclusion cut of the l2
        // if ( receiver_unit_clu_pos[study_chip-1][2][0] <= edge_exclusion_bottom || receiver_unit_clu_pos[study_chip-1][2][0] >= edge_exclusion_upper ) continue;
        // event_counting_8 += 1;

        hit2_slope = (receiver_unit_clu_pos[study_chip-1][2][0] - receiver_unit_clu_pos[study_chip-1][0][0]) / actual_xpos[2] + slope_correction;
        good_combination_slope_hit2.push_back(hit2_slope);

        // note : the slope cut
        // if ( fabs(hit2_slope) >= slope_cut ) continue;
        // event_counting_9 += 1;
        
        l0_l2_avg_pos = ( receiver_unit_clu_pos[study_chip-1][0][0] + receiver_unit_clu_pos[study_chip-1][2][0] ) / 2.;

        passed_middle_pos.push_back( l0_l2_avg_pos );

        // note : to check the N clusters of the middle layer
        if ( receiver_unit_clu_pos[study_chip-1][1].size() == 0 ) // note : no hits in the middle
        {
            event_counting_10 += 1;

            final_event_result.push_back(0); // note : in order to make the detection efficiency as function of pos.

            track_101_count += 1;
            // cout<<"101 event, eID : "<<input_cluster_vec[i].eID<<" -> no cluster"<<endl;
        }
        else if ( receiver_unit_clu_pos[study_chip-1][1].size() > 0 ) // note : at least one hit in the middle
        {
            
            for ( int l1 = 0; l1 < receiver_unit_clu_pos[study_chip-1][1].size(); l1++ )
            {
                double hit3_Y_data[3] = {receiver_unit_clu_pos[study_chip-1][0][0], receiver_unit_clu_pos[study_chip-1][1][l1], receiver_unit_clu_pos[study_chip-1][2][0]};

                grr = new TGraph(3,actual_xpos,hit3_Y_data);
                linear_fit = new TF1("linear_fit","pol1",-1,actual_xpos[2]+2);
                grr -> Fit("linear_fit","NQ");

                if (l1 == 0) // note : the first one
                {
                    chi2_register = ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) );
                    
                    cluster_register_l1 = l1;

                    hit3_best_fit_picker_info[0] = ( hit3_Y_data[0] - ( linear_fit -> GetParameter(1) * actual_xpos[0] + linear_fit -> GetParameter(0) ) );
                    hit3_best_fit_picker_info[1] = ( hit3_Y_data[1] - ( linear_fit -> GetParameter(1) * actual_xpos[1] + linear_fit -> GetParameter(0) ) );
                    hit3_best_fit_picker_info[2] = ( hit3_Y_data[2] - ( linear_fit -> GetParameter(1) * actual_xpos[2] + linear_fit -> GetParameter(0) ) );
                    
                    hit3_best_fit_picker_info[3] = hit3_Y_data[0];
                    hit3_best_fit_picker_info[4] = hit3_Y_data[1];
                    hit3_best_fit_picker_info[5] = hit3_Y_data[2];

                    // note : the middle-layer residual of the DUT method
                    hit3_best_fit_picker_info[6] = hit3_Y_data[1] - (hit3_Y_data[0]+hit3_Y_data[2])/2.;

                }
                else 
                {
                    if ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) < chi2_register )
                    {
                        chi2_register = ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) );
                        
                        cluster_register_l1 = l1;

                        hit3_best_fit_picker_info[0] = ( hit3_Y_data[0] - ( linear_fit -> GetParameter(1) * actual_xpos[0] + linear_fit -> GetParameter(0) ) );
                        hit3_best_fit_picker_info[1] = ( hit3_Y_data[1] - ( linear_fit -> GetParameter(1) * actual_xpos[1] + linear_fit -> GetParameter(0) ) );
                        hit3_best_fit_picker_info[2] = ( hit3_Y_data[2] - ( linear_fit -> GetParameter(1) * actual_xpos[2] + linear_fit -> GetParameter(0) ) );
                        
                        hit3_best_fit_picker_info[3] = hit3_Y_data[0];
                        hit3_best_fit_picker_info[4] = hit3_Y_data[1];
                        hit3_best_fit_picker_info[5] = hit3_Y_data[2];

                        // note : the middle-layer residual of the DUT method
                        hit3_best_fit_picker_info[6] = hit3_Y_data[1] - (hit3_Y_data[0]+hit3_Y_data[2])/2.;
                        

                    }
                }

                grr->Delete();
                linear_fit->Delete();

            }

            event_residual = hit3_best_fit_picker_info[4] - ( l0_l2_avg_pos );

            middle_layer_residual.push_back( event_residual );

            good_combination_l0_pos_hit3.push_back( receiver_unit_clu_pos[study_chip-1][0][0] );
            good_combination_l1_pos_hit3.push_back( hit3_best_fit_picker_info[4] );
            good_combination_l2_pos_hit3.push_back( receiver_unit_clu_pos[study_chip-1][2][0] );

            if ( fabs(event_residual) < noise_hit_distance )
            {
                event_counting_11 += 1;

                final_event_result.push_back(1); // note : in order to make the detection efficiency as function of pos.

                track_111_count += 1;
            }
            else 
            {
                event_counting_12 += 1;

                final_event_result.push_back(2); // note : in order to make the detection efficiency as function of pos.

                track_101_count += 1;   
                // cout<<"101 event, eID : "<<input_cluster_vec[i].eID<<" -> middle has cluster"<<endl;
            }

        }
        // note : start clean

    } // note : end of for loop, event


    DUT_str output_space;
    output_space.track_111_count = track_111_count;
    output_space.track_101_count = track_101_count;
    output_space.middle_layer_residual = middle_layer_residual;
    output_space.good_combination_slope_hit3 = good_combination_slope_hit3; // note : empty, 2022/12/03
    output_space.good_combination_slope_hit2 = good_combination_slope_hit2; // note : 3hits or 2hits cases are included
    output_space.good_combination_l0_pos_hit3 = good_combination_l0_pos_hit3; 
    output_space.good_combination_l1_pos_hit3 = good_combination_l1_pos_hit3;
    output_space.good_combination_l2_pos_hit3 = good_combination_l2_pos_hit3;
    
    // note : in order to make the plot of the detection efficiency as a function of position. 
    output_space.passed_middle_pos = passed_middle_pos;
    output_space.final_event_result = final_event_result;

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
    
    return output_space;

}


// note : try to study the systematic uncertainty, by checking the 
// note : residual cut 
// note : slope cut
// note : exclusion cut
// todo : the cut range is fixed.
vector<vector<double>> efficiency_DUT_method_v2_SU (vector<cluster_reformat_str> input_cluster_vec, int study_chip, int run_scan)
{ 
	double edge_exclusion_bottom;
	double edge_exclusion_upper;

    vector<int> receiver_unit_clu_size[13][3]; // note : for abbreviation, for hit 
    vector<double> receiver_unit_clu_pos[13][3];
    vector<double> receiver_unit_clu_adc[13][3];

    // note : the clusters that pass the pre-selections (cluster cut / event cut) will be saved here.

    TF1 * linear_fit;
    TGraph * grr;

    double chi2_register = 0; 
    double cluster_register_l0 = 0;
    double cluster_register_l1 = 0;
    double cluster_register_l2 = 0;
    double hit3_best_fit_picker_info[7]; // note : first 3 : residual by fittting, second 3 : the selected positions, the 7th : the middle-layer residual by the DUT method
    double hit2_best_fit_picker_info[2]; // note : first 2 : the selected positions of the first and last layers.

    int multi_track_count = 0; // note : this is for finding the multi_track, if we find a new good track, then +=1;

    double hit2_slope;
    double event_residual;

    // note : for the output 
    int track_111_count = 0;
    int track_101_count_2 = 0; // note : middle layer doesn't have hit.
    int track_101_count_3 = 0; // note : middle layer has hit far from the prediction position.
    // vector<double> middle_layer_residual; middle_layer_residual.clear();
    // vector<double> good_combination_slope_hit3; good_combination_slope_hit3.clear();
    // vector<double> good_combination_slope_hit2; good_combination_slope_hit2.clear();

    // vector<double> good_combination_l0_pos_hit3; good_combination_l0_pos_hit3.clear();
    // vector<double> good_combination_l1_pos_hit3; good_combination_l1_pos_hit3.clear();
    // vector<double> good_combination_l2_pos_hit3; good_combination_l2_pos_hit3.clear();

    double l0_l2_avg_pos;



    // note : in order to make the efficiency plot as function of position
    // vector<double> passed_middle_pos; passed_middle_pos.clear();
    // note : middle layer no hits : 0
    // note : middle layer has close hit : 1
    // note : middle layer has a hit far from the prediction pos : 2.
    // vector<int> final_event_result; final_event_result.clear(); 

    vector<vector<double>> Scan_main; Scan_main.clear();
    vector<double> Scan_each; Scan_each.clear(); //note : {cut_1, cut_2, cut_3, pass, fail_2, fail_3}

    double cut_scan_edge_exclusion;
    double cut_scan_slope_cut;
    double cut_scan_residual_cut;

    // note : cut_1 is for residual
    // note : cut_2 is for slope cut
    // note : cut_3 is for edge exclusion

    if (run_scan == 1)
    {
        for (int cut_3 = 0; cut_3 < 200; cut_3++) // note : residual cut
        {
            cut_scan_edge_exclusion = boundary_cut;
            cut_scan_slope_cut = slope_cut;
            cut_scan_residual_cut =  0.1 + 0.002 * cut_3;

            edge_exclusion_bottom = (lower_section_initial - INTT_strip_width / 2.) + INTT_strip_width * double(cut_scan_edge_exclusion);
            edge_exclusion_upper = ( INTT_strip_width * 128. ) - INTT_strip_width * double(cut_scan_edge_exclusion);
            
            cout<<"doing scan, edge exclusion chan : "<<cut_scan_edge_exclusion<<" slope cut : "<<cut_scan_slope_cut<<" residual cut : "<< cut_scan_residual_cut <<endl;
            for (int i = 0; i < input_cluster_vec.size(); i++)
            {
                if (i % 10000 == 0){ cout<<"Doing the DUT test, eID : "<<input_cluster_vec[i].eID<<endl;}

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

                // todo : the LoE
                // todo :       -> right now, what I try is to make sure there is no cluster in the adjacent chips for all three layers (chip 8 and 10, for example)
                // todo :       -> to avoid any suspicious events. 
                // todo :       -> works well !!! 2022/11/23
                // todo : the adc cut
                // todo : cluster size 
                // todo : cluster size combined
                // todo : N cluster in layer 0 and layer 2
                // todo : slope cut
                // todo : the edge exclusion

                // todo : residual cut
                

                // note : zero cluster in adjacent chips
                if ( (receiver_unit_clu_size[study_chip-1-1][0].size()+receiver_unit_clu_size[study_chip-1-1][1].size()+receiver_unit_clu_size[study_chip-1-1][2].size()+receiver_unit_clu_size[study_chip-1+1][0].size()+receiver_unit_clu_size[study_chip-1+1][1].size()+receiver_unit_clu_size[study_chip-1+1][2].size()) != 0 ) continue;

                // note : single cluster in l0 and l2
                if (receiver_unit_clu_size[study_chip-1][0].size() != hit_allowed_in_adjacent_layers || receiver_unit_clu_size[study_chip-1][2].size() != hit_allowed_in_adjacent_layers) continue;

                // note : standalone cluster size cut of l0 and l2
                if ( receiver_unit_clu_size[study_chip-1][0][0] <= cluster_size_requirement || receiver_unit_clu_size[study_chip-1][2][0] <= cluster_size_requirement ) continue;

                // note : standalone cluster adc cut
                if ( receiver_unit_clu_adc[study_chip-1][0][0] <= cluster_adc_value_requirement || receiver_unit_clu_adc[study_chip-1][2][0] <= cluster_adc_value_requirement ) continue;

                // note : combined cluster size cut
                if ( receiver_unit_clu_size[study_chip-1][0][0]+receiver_unit_clu_size[study_chip-1][2][0] <= cluster_size_requirement_combine ) continue;

                // note : edge exclusion cut of the l0
                if ( receiver_unit_clu_pos[study_chip-1][0][0] <= edge_exclusion_bottom || receiver_unit_clu_pos[study_chip-1][0][0] >= edge_exclusion_upper ) continue;

                // note : edge exclusion cut of the l2
                if ( receiver_unit_clu_pos[study_chip-1][2][0] <= edge_exclusion_bottom || receiver_unit_clu_pos[study_chip-1][2][0] >= edge_exclusion_upper ) continue;

                hit2_slope = (receiver_unit_clu_pos[study_chip-1][2][0] - receiver_unit_clu_pos[study_chip-1][0][0]) / actual_xpos[2] + slope_correction;

                // note : the slope cut
                if ( fabs(hit2_slope) >= cut_scan_slope_cut ) continue;
                
                l0_l2_avg_pos = ( receiver_unit_clu_pos[study_chip-1][0][0] + receiver_unit_clu_pos[study_chip-1][2][0] ) / 2.;

                

                // note : to check the N clusters of the middle layer
                if ( receiver_unit_clu_pos[study_chip-1][1].size() == 0 ) // note : no hits in the middle
                {
                    track_101_count_2 += 1;
                    // cout<<"101 event, eID : "<<input_cluster_vec[i].eID<<" -> no cluster"<<endl;
                }
                else if ( receiver_unit_clu_pos[study_chip-1][1].size() > 0 ) // note : at least one hit in the middle
                {
                    
                    for ( int l1 = 0; l1 < receiver_unit_clu_pos[study_chip-1][1].size(); l1++ )
                    {
                        double hit3_Y_data[3] = {receiver_unit_clu_pos[study_chip-1][0][0], receiver_unit_clu_pos[study_chip-1][1][l1], receiver_unit_clu_pos[study_chip-1][2][0]};

                        grr = new TGraph(3,actual_xpos,hit3_Y_data);
                        linear_fit = new TF1("linear_fit","pol1",-1,actual_xpos[2]+2);
                        grr -> Fit("linear_fit","NQ");

                        if (l1 == 0) // note : the first one
                        {
                            chi2_register = ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) );
                            
                            cluster_register_l1 = l1;

                            hit3_best_fit_picker_info[0] = ( hit3_Y_data[0] - ( linear_fit -> GetParameter(1) * actual_xpos[0] + linear_fit -> GetParameter(0) ) );
                            hit3_best_fit_picker_info[1] = ( hit3_Y_data[1] - ( linear_fit -> GetParameter(1) * actual_xpos[1] + linear_fit -> GetParameter(0) ) );
                            hit3_best_fit_picker_info[2] = ( hit3_Y_data[2] - ( linear_fit -> GetParameter(1) * actual_xpos[2] + linear_fit -> GetParameter(0) ) );
                            
                            hit3_best_fit_picker_info[3] = hit3_Y_data[0];
                            hit3_best_fit_picker_info[4] = hit3_Y_data[1];
                            hit3_best_fit_picker_info[5] = hit3_Y_data[2];

                            // note : the middle-layer residual of the DUT method
                            hit3_best_fit_picker_info[6] = hit3_Y_data[1] - (hit3_Y_data[0]+hit3_Y_data[2])/2.;

                        }
                        else 
                        {
                            if ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) < chi2_register )
                            {
                                chi2_register = ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) );
                                
                                cluster_register_l1 = l1;

                                hit3_best_fit_picker_info[0] = ( hit3_Y_data[0] - ( linear_fit -> GetParameter(1) * actual_xpos[0] + linear_fit -> GetParameter(0) ) );
                                hit3_best_fit_picker_info[1] = ( hit3_Y_data[1] - ( linear_fit -> GetParameter(1) * actual_xpos[1] + linear_fit -> GetParameter(0) ) );
                                hit3_best_fit_picker_info[2] = ( hit3_Y_data[2] - ( linear_fit -> GetParameter(1) * actual_xpos[2] + linear_fit -> GetParameter(0) ) );
                                
                                hit3_best_fit_picker_info[3] = hit3_Y_data[0];
                                hit3_best_fit_picker_info[4] = hit3_Y_data[1];
                                hit3_best_fit_picker_info[5] = hit3_Y_data[2];

                                // note : the middle-layer residual of the DUT method
                                hit3_best_fit_picker_info[6] = hit3_Y_data[1] - (hit3_Y_data[0]+hit3_Y_data[2])/2.;
                                

                            }
                        }

                        grr->Delete();
                        linear_fit->Delete();
                    }

                    event_residual = hit3_best_fit_picker_info[4] - ( l0_l2_avg_pos );

                    if ( fabs(event_residual) < cut_scan_residual_cut )
                    {
                        track_111_count += 1;
                    }
                    else 
                    {
                        track_101_count_3 += 1;   
                        // cout<<"101 event, eID : "<<input_cluster_vec[i].eID<<" -> middle has cluster"<<endl;
                    }

                }
                // note : start clean

            } // note : end of for loop, event

            Scan_each.push_back(cut_scan_edge_exclusion);
            Scan_each.push_back(cut_scan_slope_cut);
            Scan_each.push_back(cut_scan_residual_cut);
            Scan_each.push_back(track_111_count);
            Scan_each.push_back(track_101_count_2);
            Scan_each.push_back(track_101_count_3);

            Scan_main.push_back(Scan_each);

            Scan_each.clear();
            track_111_count = 0;
            track_101_count_2 = 0;
            track_101_count_3 = 0;
        } // note : end of cut_3
    }
    else if (run_scan == 2)
    {
        for (int cut_3 = 0; cut_3 < 150; cut_3++) // note : for the slope cut
        {
            cut_scan_edge_exclusion = boundary_cut;
            cut_scan_slope_cut = 0.005 + 0.0002 * cut_3;
            cut_scan_residual_cut =  noise_hit_distance;

            edge_exclusion_bottom = (lower_section_initial - INTT_strip_width / 2.) + INTT_strip_width * double(cut_scan_edge_exclusion);
            edge_exclusion_upper = ( INTT_strip_width * 128. ) - INTT_strip_width * double(cut_scan_edge_exclusion);
            
            cout<<"doing scan, edge exclusion chan : "<<cut_scan_edge_exclusion<<" slope cut : "<<cut_scan_slope_cut<<" residual cut : "<< cut_scan_residual_cut <<endl;
            for (int i = 0; i < input_cluster_vec.size(); i++)
            {
                if (i % 10000 == 0){ cout<<"Doing the DUT test, eID : "<<input_cluster_vec[i].eID<<endl;}

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

                // todo : the LoE
                // todo :       -> right now, what I try is to make sure there is no cluster in the adjacent chips for all three layers (chip 8 and 10, for example)
                // todo :       -> to avoid any suspicious events. 
                // todo :       -> works well !!! 2022/11/23
                // todo : the adc cut
                // todo : cluster size 
                // todo : cluster size combined
                // todo : N cluster in layer 0 and layer 2
                // todo : slope cut
                // todo : the edge exclusion

                // todo : residual cut
                

                // note : zero cluster in adjacent chips
                if ( (receiver_unit_clu_size[study_chip-1-1][0].size()+receiver_unit_clu_size[study_chip-1-1][1].size()+receiver_unit_clu_size[study_chip-1-1][2].size()+receiver_unit_clu_size[study_chip-1+1][0].size()+receiver_unit_clu_size[study_chip-1+1][1].size()+receiver_unit_clu_size[study_chip-1+1][2].size()) != 0 ) continue;

                // note : single cluster in l0 and l2
                if (receiver_unit_clu_size[study_chip-1][0].size() != hit_allowed_in_adjacent_layers || receiver_unit_clu_size[study_chip-1][2].size() != hit_allowed_in_adjacent_layers) continue;

                // note : standalone cluster size cut of l0 and l2
                if ( receiver_unit_clu_size[study_chip-1][0][0] <= cluster_size_requirement || receiver_unit_clu_size[study_chip-1][2][0] <= cluster_size_requirement ) continue;

                // note : standalone cluster adc cut
                if ( receiver_unit_clu_adc[study_chip-1][0][0] <= cluster_adc_value_requirement || receiver_unit_clu_adc[study_chip-1][2][0] <= cluster_adc_value_requirement ) continue;

                // note : combined cluster size cut
                if ( receiver_unit_clu_size[study_chip-1][0][0]+receiver_unit_clu_size[study_chip-1][2][0] <= cluster_size_requirement_combine ) continue;

                // note : edge exclusion cut of the l0
                if ( receiver_unit_clu_pos[study_chip-1][0][0] <= edge_exclusion_bottom || receiver_unit_clu_pos[study_chip-1][0][0] >= edge_exclusion_upper ) continue;

                // note : edge exclusion cut of the l2
                if ( receiver_unit_clu_pos[study_chip-1][2][0] <= edge_exclusion_bottom || receiver_unit_clu_pos[study_chip-1][2][0] >= edge_exclusion_upper ) continue;

                hit2_slope = (receiver_unit_clu_pos[study_chip-1][2][0] - receiver_unit_clu_pos[study_chip-1][0][0]) / actual_xpos[2] + slope_correction;

                // note : the slope cut
                if ( fabs(hit2_slope) >= cut_scan_slope_cut ) continue;
                
                l0_l2_avg_pos = ( receiver_unit_clu_pos[study_chip-1][0][0] + receiver_unit_clu_pos[study_chip-1][2][0] ) / 2.;

                

                // note : to check the N clusters of the middle layer
                if ( receiver_unit_clu_pos[study_chip-1][1].size() == 0 ) // note : no hits in the middle
                {
                    track_101_count_2 += 1;
                    // cout<<"101 event, eID : "<<input_cluster_vec[i].eID<<" -> no cluster"<<endl;
                }
                else if ( receiver_unit_clu_pos[study_chip-1][1].size() > 0 ) // note : at least one hit in the middle
                {
                    
                    for ( int l1 = 0; l1 < receiver_unit_clu_pos[study_chip-1][1].size(); l1++ )
                    {
                        double hit3_Y_data[3] = {receiver_unit_clu_pos[study_chip-1][0][0], receiver_unit_clu_pos[study_chip-1][1][l1], receiver_unit_clu_pos[study_chip-1][2][0]};

                        grr = new TGraph(3,actual_xpos,hit3_Y_data);
                        linear_fit = new TF1("linear_fit","pol1",-1,actual_xpos[2]+2);
                        grr -> Fit("linear_fit","NQ");

                        if (l1 == 0) // note : the first one
                        {
                            chi2_register = ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) );
                            
                            cluster_register_l1 = l1;

                            hit3_best_fit_picker_info[0] = ( hit3_Y_data[0] - ( linear_fit -> GetParameter(1) * actual_xpos[0] + linear_fit -> GetParameter(0) ) );
                            hit3_best_fit_picker_info[1] = ( hit3_Y_data[1] - ( linear_fit -> GetParameter(1) * actual_xpos[1] + linear_fit -> GetParameter(0) ) );
                            hit3_best_fit_picker_info[2] = ( hit3_Y_data[2] - ( linear_fit -> GetParameter(1) * actual_xpos[2] + linear_fit -> GetParameter(0) ) );
                            
                            hit3_best_fit_picker_info[3] = hit3_Y_data[0];
                            hit3_best_fit_picker_info[4] = hit3_Y_data[1];
                            hit3_best_fit_picker_info[5] = hit3_Y_data[2];

                            // note : the middle-layer residual of the DUT method
                            hit3_best_fit_picker_info[6] = hit3_Y_data[1] - (hit3_Y_data[0]+hit3_Y_data[2])/2.;

                        }
                        else 
                        {
                            if ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) < chi2_register )
                            {
                                chi2_register = ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) );
                                
                                cluster_register_l1 = l1;

                                hit3_best_fit_picker_info[0] = ( hit3_Y_data[0] - ( linear_fit -> GetParameter(1) * actual_xpos[0] + linear_fit -> GetParameter(0) ) );
                                hit3_best_fit_picker_info[1] = ( hit3_Y_data[1] - ( linear_fit -> GetParameter(1) * actual_xpos[1] + linear_fit -> GetParameter(0) ) );
                                hit3_best_fit_picker_info[2] = ( hit3_Y_data[2] - ( linear_fit -> GetParameter(1) * actual_xpos[2] + linear_fit -> GetParameter(0) ) );
                                
                                hit3_best_fit_picker_info[3] = hit3_Y_data[0];
                                hit3_best_fit_picker_info[4] = hit3_Y_data[1];
                                hit3_best_fit_picker_info[5] = hit3_Y_data[2];

                                // note : the middle-layer residual of the DUT method
                                hit3_best_fit_picker_info[6] = hit3_Y_data[1] - (hit3_Y_data[0]+hit3_Y_data[2])/2.;
                                

                            }
                        }

                        grr->Delete();
                        linear_fit->Delete();
                    }

                    event_residual = hit3_best_fit_picker_info[4] - ( l0_l2_avg_pos );

                    if ( fabs(event_residual) < cut_scan_residual_cut )
                    {
                        track_111_count += 1;
                    }
                    else 
                    {
                        track_101_count_3 += 1;   
                        // cout<<"101 event, eID : "<<input_cluster_vec[i].eID<<" -> middle has cluster"<<endl;
                    }

                }
                // note : start clean

            } // note : end of for loop, event

            Scan_each.push_back(cut_scan_edge_exclusion);
            Scan_each.push_back(cut_scan_slope_cut);
            Scan_each.push_back(cut_scan_residual_cut);
            Scan_each.push_back(track_111_count);
            Scan_each.push_back(track_101_count_2);
            Scan_each.push_back(track_101_count_3);

            Scan_main.push_back(Scan_each);

            Scan_each.clear();
            track_111_count = 0;
            track_101_count_2 = 0;
            track_101_count_3 = 0;
        } // note : end of cut_3
    }
    else if (run_scan == 3)
    {
        for (int cut_3 = 0; cut_3 < 71; cut_3++) // note : for edge exclusion
        {
            cut_scan_edge_exclusion = cut_3;
            cut_scan_slope_cut = slope_cut;
            cut_scan_residual_cut =  noise_hit_distance;

            edge_exclusion_bottom = (lower_section_initial - INTT_strip_width / 2.) + INTT_strip_width * double(cut_scan_edge_exclusion);
            edge_exclusion_upper = ( INTT_strip_width * 128. ) - INTT_strip_width * double(cut_scan_edge_exclusion);
            
            cout<<"doing scan, edge exclusion chan : "<<cut_scan_edge_exclusion<<" slope cut : "<<cut_scan_slope_cut<<" residual cut : "<< cut_scan_residual_cut <<endl;
            for (int i = 0; i < input_cluster_vec.size(); i++)
            {
                if (i % 10000 == 0){ cout<<"Doing the DUT test, eID : "<<input_cluster_vec[i].eID<<endl;}

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

                // todo : the LoE
                // todo :       -> right now, what I try is to make sure there is no cluster in the adjacent chips for all three layers (chip 8 and 10, for example)
                // todo :       -> to avoid any suspicious events. 
                // todo :       -> works well !!! 2022/11/23
                // todo : the adc cut
                // todo : cluster size 
                // todo : cluster size combined
                // todo : N cluster in layer 0 and layer 2
                // todo : slope cut
                // todo : the edge exclusion

                // todo : residual cut
                

                // note : zero cluster in adjacent chips
                if ( (receiver_unit_clu_size[study_chip-1-1][0].size()+receiver_unit_clu_size[study_chip-1-1][1].size()+receiver_unit_clu_size[study_chip-1-1][2].size()+receiver_unit_clu_size[study_chip-1+1][0].size()+receiver_unit_clu_size[study_chip-1+1][1].size()+receiver_unit_clu_size[study_chip-1+1][2].size()) != 0 ) continue;

                // note : single cluster in l0 and l2
                if (receiver_unit_clu_size[study_chip-1][0].size() != hit_allowed_in_adjacent_layers || receiver_unit_clu_size[study_chip-1][2].size() != hit_allowed_in_adjacent_layers) continue;

                // note : standalone cluster size cut of l0 and l2
                if ( receiver_unit_clu_size[study_chip-1][0][0] <= cluster_size_requirement || receiver_unit_clu_size[study_chip-1][2][0] <= cluster_size_requirement ) continue;

                // note : standalone cluster adc cut
                if ( receiver_unit_clu_adc[study_chip-1][0][0] <= cluster_adc_value_requirement || receiver_unit_clu_adc[study_chip-1][2][0] <= cluster_adc_value_requirement ) continue;

                // note : combined cluster size cut
                if ( receiver_unit_clu_size[study_chip-1][0][0]+receiver_unit_clu_size[study_chip-1][2][0] <= cluster_size_requirement_combine ) continue;

                // note : edge exclusion cut of the l0
                if ( receiver_unit_clu_pos[study_chip-1][0][0] <= edge_exclusion_bottom || receiver_unit_clu_pos[study_chip-1][0][0] >= edge_exclusion_upper ) continue;

                // note : edge exclusion cut of the l2
                if ( receiver_unit_clu_pos[study_chip-1][2][0] <= edge_exclusion_bottom || receiver_unit_clu_pos[study_chip-1][2][0] >= edge_exclusion_upper ) continue;

                hit2_slope = (receiver_unit_clu_pos[study_chip-1][2][0] - receiver_unit_clu_pos[study_chip-1][0][0]) / actual_xpos[2] + slope_correction;

                // note : the slope cut
                if ( fabs(hit2_slope) >= cut_scan_slope_cut ) continue;
                
                l0_l2_avg_pos = ( receiver_unit_clu_pos[study_chip-1][0][0] + receiver_unit_clu_pos[study_chip-1][2][0] ) / 2.;

                

                // note : to check the N clusters of the middle layer
                if ( receiver_unit_clu_pos[study_chip-1][1].size() == 0 ) // note : no hits in the middle
                {
                    track_101_count_2 += 1;
                    // cout<<"101 event, eID : "<<input_cluster_vec[i].eID<<" -> no cluster"<<endl;
                }
                else if ( receiver_unit_clu_pos[study_chip-1][1].size() > 0 ) // note : at least one hit in the middle
                {
                    
                    for ( int l1 = 0; l1 < receiver_unit_clu_pos[study_chip-1][1].size(); l1++ )
                    {
                        double hit3_Y_data[3] = {receiver_unit_clu_pos[study_chip-1][0][0], receiver_unit_clu_pos[study_chip-1][1][l1], receiver_unit_clu_pos[study_chip-1][2][0]};

                        grr = new TGraph(3,actual_xpos,hit3_Y_data);
                        linear_fit = new TF1("linear_fit","pol1",-1,actual_xpos[2]+2);
                        grr -> Fit("linear_fit","NQ");

                        if (l1 == 0) // note : the first one
                        {
                            chi2_register = ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) );
                            
                            cluster_register_l1 = l1;

                            hit3_best_fit_picker_info[0] = ( hit3_Y_data[0] - ( linear_fit -> GetParameter(1) * actual_xpos[0] + linear_fit -> GetParameter(0) ) );
                            hit3_best_fit_picker_info[1] = ( hit3_Y_data[1] - ( linear_fit -> GetParameter(1) * actual_xpos[1] + linear_fit -> GetParameter(0) ) );
                            hit3_best_fit_picker_info[2] = ( hit3_Y_data[2] - ( linear_fit -> GetParameter(1) * actual_xpos[2] + linear_fit -> GetParameter(0) ) );
                            
                            hit3_best_fit_picker_info[3] = hit3_Y_data[0];
                            hit3_best_fit_picker_info[4] = hit3_Y_data[1];
                            hit3_best_fit_picker_info[5] = hit3_Y_data[2];

                            // note : the middle-layer residual of the DUT method
                            hit3_best_fit_picker_info[6] = hit3_Y_data[1] - (hit3_Y_data[0]+hit3_Y_data[2])/2.;

                        }
                        else 
                        {
                            if ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) < chi2_register )
                            {
                                chi2_register = ( linear_fit->GetChisquare()/double (linear_fit->GetNDF()) );
                                
                                cluster_register_l1 = l1;

                                hit3_best_fit_picker_info[0] = ( hit3_Y_data[0] - ( linear_fit -> GetParameter(1) * actual_xpos[0] + linear_fit -> GetParameter(0) ) );
                                hit3_best_fit_picker_info[1] = ( hit3_Y_data[1] - ( linear_fit -> GetParameter(1) * actual_xpos[1] + linear_fit -> GetParameter(0) ) );
                                hit3_best_fit_picker_info[2] = ( hit3_Y_data[2] - ( linear_fit -> GetParameter(1) * actual_xpos[2] + linear_fit -> GetParameter(0) ) );
                                
                                hit3_best_fit_picker_info[3] = hit3_Y_data[0];
                                hit3_best_fit_picker_info[4] = hit3_Y_data[1];
                                hit3_best_fit_picker_info[5] = hit3_Y_data[2];

                                // note : the middle-layer residual of the DUT method
                                hit3_best_fit_picker_info[6] = hit3_Y_data[1] - (hit3_Y_data[0]+hit3_Y_data[2])/2.;
                                

                            }
                        }

                        grr->Delete();
                        linear_fit->Delete();
                    }

                    event_residual = hit3_best_fit_picker_info[4] - ( l0_l2_avg_pos );

                    if ( fabs(event_residual) < cut_scan_residual_cut )
                    {
                        track_111_count += 1;
                    }
                    else 
                    {
                        track_101_count_3 += 1;   
                        // cout<<"101 event, eID : "<<input_cluster_vec[i].eID<<" -> middle has cluster"<<endl;
                    }

                }
                // note : start clean

            } // note : end of for loop, event

            Scan_each.push_back(cut_scan_edge_exclusion);
            Scan_each.push_back(cut_scan_slope_cut);
            Scan_each.push_back(cut_scan_residual_cut);
            Scan_each.push_back(track_111_count);
            Scan_each.push_back(track_101_count_2);
            Scan_each.push_back(track_101_count_3);

            Scan_main.push_back(Scan_each);

            Scan_each.clear();
            track_111_count = 0;
            track_101_count_2 = 0;
            track_101_count_3 = 0;
        } // note : end of cut_3
    }
    
    return Scan_main;

}


void Characterize_Pad (TPad *pad, float left = 0.2, float right = 0.05, float top = 0.1, float bottom = 0.15, int logY = 0, int setgrid_bool = 1)
{
	if (setgrid_bool == true) {pad -> SetGrid (1, 1);}
	pad -> SetLeftMargin   (left);
	pad -> SetRightMargin  (right);
	pad -> SetTopMargin    (top);
	pad -> SetBottomMargin (bottom);
	pad -> SetLogy (logY);
}

void Characterize_Rate1F (TH1F *hist,  int hcolour)
{
	float ratio = 7.0/3.0;
	hist -> SetTitle       ("");
	hist -> SetLineColor   (hcolour);
	hist -> SetMarkerColor (hcolour);
	hist -> SetMarkerStyle (20);
	hist -> SetMarkerSize  (0.8);
	hist -> SetFillColor   (hcolour);

	hist -> GetXaxis() -> SetTitleSize   (0.052*ratio);
	hist -> GetXaxis() -> SetTitleOffset (0.940);
	hist -> GetXaxis() -> SetLabelSize   (0.042*ratio);
	hist -> GetXaxis() -> SetLabelOffset (0.004*ratio);

	hist -> GetYaxis() -> SetTitle       ("Data/MC");
	hist -> GetYaxis() -> SetTitleSize   (0.052*ratio);
	hist -> GetYaxis() -> SetTitleOffset (0.360);
	hist -> GetYaxis() -> SetLabelSize   (0.042*ratio);
	hist -> GetYaxis() -> SetLabelOffset (0.006);
	hist -> GetYaxis() -> SetRangeUser   (0.4, 3);
	hist -> GetYaxis() -> SetNdivisions  (505);

    hist -> SetStats(0);
}

void Characterize_Hist1F (TH1F *hist,  int statsbox = 0, TString color_ID = "#1A3947")
{
	float ratio = 1.;
	hist -> SetTitle       ("");

	hist -> SetLineColor   (TColor::GetColor(color_ID));
    hist -> SetLineWidth   (3);

	hist -> SetMarkerColor (TColor::GetColor(color_ID));
	hist -> SetMarkerStyle (20);
	hist -> SetMarkerSize  (0.8);
	// hist -> SetFillColor   (TColor::GetColor(color_ID));

	hist -> GetXaxis() -> SetTitleSize   (0.05);
	hist -> GetXaxis() -> SetTitleOffset (0.8);

	hist -> GetXaxis() -> SetLabelSize   (0.042*ratio);
	hist -> GetXaxis() -> SetLabelOffset (0.004*ratio);

	// hist -> GetYaxis() -> SetTitle       ("Data/MC");
	hist -> GetYaxis() -> SetTitleSize   (0.05);
	hist -> GetYaxis() -> SetTitleOffset (1.5);

	hist -> GetYaxis() -> SetLabelSize   (0.042*ratio);
	hist -> GetYaxis() -> SetLabelOffset (0.006);
	// hist -> GetYaxis() -> SetRangeUser   (0.4, 3);

	hist -> GetYaxis() -> SetNdivisions  (505);

    if (statsbox == 0) {hist -> SetStats(0);}
}


// note : make the plot of the middle-layer residual distribution, full range
TH1F* plot_residual (vector<double> input_vec, TString folder_direction, int study_chip)
{
    TCanvas * c1 = new TCanvas("c1","c1",800,800);
    c1 -> cd();
    c1 -> SetLogy();

    int N_in_range = 0;

    TH1F * l1_residual_hist = new TH1F("","",100,-10,10);
    for (int i = 0; i < input_vec.size(); i++)
    {
        l1_residual_hist -> Fill(input_vec[i]);

        if ( input_vec[i] > -1 * noise_hit_distance && input_vec[i] < noise_hit_distance )
        {
            N_in_range += 1;
        }
    }

    l1_residual_hist -> SetTitle(Form("DUT layer 1 residual, U%i",study_chip));
    l1_residual_hist -> GetXaxis() -> SetTitle("Unit : mm");
    l1_residual_hist -> GetYaxis() -> SetTitle("Entry");

    l1_residual_hist -> Draw("hist");

    cout<<"N event in the range  #pm"<<noise_hit_distance<<" mm : "<< N_in_range<<endl;    

    c1 -> Print( Form("%s/DUT_residual_full_range_U%i.pdf",folder_direction.Data(),study_chip) );
    c1 -> Clear();

    return l1_residual_hist;
}

// note : make the plot of the middle-layer residual distribution, narrow range, and fit with double-gaus function and single-gaus function
TH1F* plot_residual_narrow (vector<double> input_vec, TString folder_direction, int study_chip)
{
    TCanvas * c1 = new TCanvas("c1","c1",800,800);
    c1 -> cd();
    c1 -> SetLogy();

    TF1 * gaus_fit = new TF1 ("gaus_fit","gaus",-0.5,0.5);
		
    TF1 * D_gaus_fit = new TF1 ("D_gaus_fit",double_gaus,-10,10,5);
    // note : par[0] could be 10000 for data
    // note : par[0] could be 40000 for MC
    D_gaus_fit -> SetParameters(10000,0.987,0,0.03,0.1);
    // D_gaus_fit -> SetParLimits  (1, 0.5,      1); // fraction
    // D_gaus_fit -> SetParLimits  (3, 0.025,      1); // note : ON for MC
    // D_gaus_fit -> SetParLimits  (1, 0.97,      1);  // note : ON for MC
    D_gaus_fit -> SetLineColor(TColor::GetColor("#F5751D"));

    TH1F * l1_residual_hist = new TH1F("","",100,-1.5,1.5);
    for (int i = 0; i < input_vec.size(); i++)
    {
        l1_residual_hist -> Fill(input_vec[i]);
    }

    l1_residual_hist -> SetTitle(Form("DUT layer 1 residual, U%i",study_chip));
    l1_residual_hist -> GetXaxis() -> SetTitle("Unit : mm");
    l1_residual_hist -> GetYaxis() -> SetTitle("Entry");

    l1_residual_hist -> Draw("hist");

    TLatex *gaus_fit_latex = new TLatex();
    gaus_fit_latex -> SetNDC();
    gaus_fit_latex -> SetTextSize(0.028);

    double D_gaus_xmin = -10;
    double D_gaus_xmax =  10;
    double the_portion = 0.9973;

    l1_residual_hist -> Fit("gaus_fit","NQ");
    // note : data for -0.5 ~ 0.5
    // note : MC could be -0.3 ~ 0.3
    l1_residual_hist -> Fit(D_gaus_fit,"N","",-1,1);
    bool run_fit_effisig = double_gaus_getsigmarange (D_gaus_fit,the_portion,D_gaus_xmin,D_gaus_xmax);
    
    gaus_fit -> SetNpx(10000);
    D_gaus_fit -> SetNpx(10000);

    gaus_fit->Draw("lsame");	
    D_gaus_fit->Draw("lsame");	
    
    gaus_fit_latex -> DrawLatex(0.12, 0.750, Form("fit gaus mean :  %.4f, fit gaus width %.4f", gaus_fit->GetParameter(1),gaus_fit->GetParameter(2)));
    gaus_fit_latex -> DrawLatex(0.12, 0.720, Form("#chi^{2} : %.2f, NDF : %d, #chi^{2}/NDF : %.4f", gaus_fit->GetChisquare(),gaus_fit->GetNDF(),gaus_fit->GetChisquare()/double(gaus_fit->GetNDF())));
    
    gaus_fit_latex -> DrawLatex(0.12, 0.660, Form("double gaussian fit"));
    gaus_fit_latex -> DrawLatex(0.12, 0.630, Form("mean : %.4f, fraction : %.3f",D_gaus_fit->GetParameter(2),D_gaus_fit->GetParameter(1)));
    gaus_fit_latex -> DrawLatex(0.12, 0.600, Form("first width : %.4f, second width : %.4f",D_gaus_fit->GetParameter(3),D_gaus_fit->GetParameter(4)));
    gaus_fit_latex -> DrawLatex(0.12, 0.570, Form("3 sigma width : %.4f %.4f = %.4f",D_gaus_xmax,D_gaus_xmin,D_gaus_xmax-D_gaus_xmin));
    gaus_fit_latex -> DrawLatex(0.12, 0.540, Form("#chi^{2} : %.2f, NDF : %d, #chi^{2}/NDF : %.4f", D_gaus_fit->GetChisquare(),D_gaus_fit->GetNDF(),D_gaus_fit->GetChisquare()/double(D_gaus_fit->GetNDF())));
    
    gaus_fit_latex -> Draw("same");

    c1 -> Print( Form("%s/DUT_residual_narrow_U%i.pdf",folder_direction.Data(),study_chip) );
    c1 -> Clear();

    return l1_residual_hist;
}

// note : the function to print the plot for publish
// note : make the plot of the middle-layer residual distribution, narrow range, No fitting.
TH1F* plot_residual_narrow_publish (vector<double> input_vec, TString folder_direction, int study_chip, int selection_level)
{
    TCanvas * c1 = new TCanvas("c1","c1",850 ,800);
    c1 -> cd();
    // c1 -> SetLogy();

    TPad * pad_c1 = new TPad(Form("pad_c1"), "", 0.0, 0.0, 1.0, 1.0);
    Characterize_Pad(pad_c1, 0.15,  0.1,  0.1,  0.12, 1, 0);
    pad_c1 -> SetTicks(1,1);
    pad_c1 -> Draw();
    pad_c1 -> cd();

    TF1 * gaus_fit = new TF1 ("gaus_fit","gaus",-0.5,0.5);
		
    TF1 * D_gaus_fit = new TF1 ("D_gaus_fit",double_gaus,-10,10,5);
    // note : par[0] could be 10000 for data
    // note : par[0] could be 40000 for MC
    D_gaus_fit -> SetParameters(10000,0.987,0,0.03,0.1);
    // D_gaus_fit -> SetParLimits  (1, 0.5,      1); // fraction
    // D_gaus_fit -> SetParLimits  (3, 0.025,      1); // note : ON for MC
    // D_gaus_fit -> SetParLimits  (1, 0.97,      1);  // note : ON for MC
    // D_gaus_fit -> SetLineColor(TColor::GetColor("#F5751D"));

    TH1F * l1_residual_hist = new TH1F("","",100,-1.5,1.5);
    Characterize_Hist1F(l1_residual_hist);

    for (int i = 0; i < input_vec.size(); i++)
    {
        l1_residual_hist -> Fill(input_vec[i]);
    }

    // l1_residual_hist -> SetTitle(Form("DUT layer 1 residual, U%i",study_chip));
    l1_residual_hist -> SetTitle("");
    l1_residual_hist -> GetXaxis() -> SetTitle("Residual [mm]");
    l1_residual_hist -> GetYaxis() -> SetTitle("Events");

    l1_residual_hist -> Draw("hist");

    TLatex *gaus_fit_latex = new TLatex();
    gaus_fit_latex -> SetNDC();
    gaus_fit_latex -> SetTextSize(0.035);

    double D_gaus_xmin = -10;
    double D_gaus_xmax =  10;
    double the_portion = 0.9973;

    l1_residual_hist -> Fit("gaus_fit","NQ");
    // note : data for -0.5 ~ 0.5
    // note : MC could be -0.3 ~ 0.3
    l1_residual_hist -> Fit(D_gaus_fit,"N","",-1,1);
    bool run_fit_effisig = double_gaus_getsigmarange (D_gaus_fit,the_portion,D_gaus_xmin,D_gaus_xmax);
    
    gaus_fit -> SetNpx(10000);
    D_gaus_fit -> SetNpx(10000);

    // gaus_fit->Draw("lsame");	
    // D_gaus_fit->Draw("lsame");	
    
    l1_residual_hist -> SetMaximum(1000000.);

    TLine * noise_hit_distance_positive = new TLine(noise_hit_distance,l1_residual_hist -> GetMinimum(),noise_hit_distance,l1_residual_hist -> GetMaximum());
    noise_hit_distance_positive -> SetLineWidth(5);
    noise_hit_distance_positive -> SetLineColor(TColor::GetColor("#A08144"));
    noise_hit_distance_positive -> SetLineStyle(2);

    TLine * noise_hit_distance_negative = new TLine(-1 * noise_hit_distance,l1_residual_hist -> GetMinimum(),-1 * noise_hit_distance,l1_residual_hist -> GetMaximum());
    noise_hit_distance_negative -> SetLineWidth(5);
    noise_hit_distance_negative -> SetLineColor(TColor::GetColor("#A08144"));
    noise_hit_distance_negative -> SetLineStyle(2);

    // gaus_fit_latex -> DrawLatex(0.12, 0.750, Form("fit gaus mean :  %.4f, fit gaus width %.4f", gaus_fit->GetParameter(1),gaus_fit->GetParameter(2)));
    // gaus_fit_latex -> DrawLatex(0.12, 0.720, Form("#chi^{2} : %.2f, NDF : %d, #chi^{2}/NDF : %.4f", gaus_fit->GetChisquare(),gaus_fit->GetNDF(),gaus_fit->GetChisquare()/double(gaus_fit->GetNDF())));
    
    // gaus_fit_latex -> DrawLatex(0.12, 0.660, Form("double gaussian fit"));
    gaus_fit_latex -> DrawLatex(0.12+0.05, 0.74, Form("Mean : %.3f mm",D_gaus_fit->GetParameter(2)));
    // gaus_fit_latex -> DrawLatex(0.12, 0.600, Form("first width : %.4f, second width : %.4f",D_gaus_fit->GetParameter(3),D_gaus_fit->GetParameter(4)));
    // gaus_fit_latex -> DrawLatex(0.12, 0.570, Form("3 sigma width : %.4f %.4f = %.4f",D_gaus_xmax,D_gaus_xmin,D_gaus_xmax-D_gaus_xmin));
    // gaus_fit_latex -> DrawLatex(0.12, 0.540, Form("#chi^{2} : %.2f, NDF : %d, #chi^{2}/NDF : %.4f", D_gaus_fit->GetChisquare(),D_gaus_fit->GetNDF(),D_gaus_fit->GetChisquare()/double(D_gaus_fit->GetNDF())));
    
    // gaus_fit_latex -> Draw("same");

    TLegend *legend1 = new TLegend (0.6, 0.7, 0.82, 0.78);
	legend1 -> SetTextSize (0.04 );
	// legend1 -> SetNColumns (4);
    legend1 -> SetBorderSize(0);
    legend1 -> AddEntry (noise_hit_distance_positive, Form("Residual cut"),  "l");

    noise_hit_distance_positive -> Draw("lsame");
    noise_hit_distance_negative -> Draw("lsame");
    legend1 -> Draw("same");


    c1 -> Print( Form("%s/DUT_residual_narrow_U%i_pub_SelectionLevel_%i.pdf",folder_direction.Data(),study_chip,selection_level) );
    c1 -> Clear();

    return l1_residual_hist;
}

// note : make the plot of the middle-layer residual distribution, full range
// TH1F* plot_residual_align (vector<double> input_vec, TString folder_direction, int study_chip)
// {
//     TCanvas * c1 = new TCanvas("c1","c1",800,800);
//     c1 -> cd();

//     TPad * pad_c1 = new TPad(Form("pad_c1"), "", 0.0, 0.0, 1.0, 1.0);
//     Characterize_Pad(pad_c1, 0.2,  0.05,  0.1,  0.15, 1, 0);
//     pad_c1 -> SetTicks(1,1);
//     pad_c1 -> Draw();
//     pad_c1 -> cd();

//     // c1 -> SetLogy();

//     int N_in_range = 0;

//     TH1F * l1_residual_hist = new TH1F("","",100,-10,10);
//     Characterize_Hist1F(l1_residual_hist);

//     for (int i = 0; i < input_vec.size(); i++)
//     {
//         l1_residual_hist -> Fill(input_vec[i]);

//         if ( input_vec[i] > -1 * noise_hit_distance && input_vec[i] < noise_hit_distance )
//         {
//             N_in_range += 1;
//         }
//     }

//     // l1_residual_hist -> SetTitle(Form("DUT layer 1 residual, U%i",study_chip));
//     l1_residual_hist -> SetTitle("");
//     l1_residual_hist -> GetXaxis() -> SetTitle("Residual [mm]");
//     l1_residual_hist -> GetYaxis() -> SetTitle("Entry");

//     l1_residual_hist -> Draw("hist");

//     cout<<"N event in the range  #pm"<<noise_hit_distance<<" mm : "<< N_in_range<<endl;    

//     c1 -> Print( Form("%s/DUT_residual_full_range_U%i.pdf",folder_direction.Data(),study_chip) );
//     c1 -> Clear();

//     return l1_residual_hist;
// }

// note : to make the misalignment plot
// note : the function to print the plot for publish
TH1F* plot_align_narrow_publish (vector<double> input_vec, TString folder_direction, int study_chip)
{
    TCanvas * c1 = new TCanvas("c1","c1",850,800);
    c1 -> cd();

    TPad * pad_c1 = new TPad(Form("pad_c1"), "", 0.0, 0.0, 1.0, 1.0);
    Characterize_Pad(pad_c1, 0.15,  0.1,  0.1,  0.12, 0, 0);
    pad_c1 -> SetTicks(1,1);
    pad_c1 -> Draw();

    TF1 * gaus_fit = new TF1 ("gaus_fit","gaus",-1,1);
		
    TF1 * D_gaus_fit = new TF1 ("D_gaus_fit",double_gaus,-10,10,5);
    // note : par[0] could be 10000 for data
    // note : par[0] could be 40000 for MC
    D_gaus_fit -> SetParameters(10000,0.987,0,0.03,0.1);
    // D_gaus_fit -> SetParLimits  (1, 0.5,      1); // fraction
    // D_gaus_fit -> SetParLimits  (3, 0.025,      1); // note : ON for MC
    // D_gaus_fit -> SetParLimits  (1, 0.97,      1);  // note : ON for MC
    D_gaus_fit -> SetLineColor(TColor::GetColor("#F5751D"));

    TH1F * l1_residual_hist = new TH1F("","",50,-0.4,1);
    l1_residual_hist -> SetStats(0);
    for (int i = 0; i < input_vec.size(); i++)
    {
        l1_residual_hist -> Fill(input_vec[i]);
    }

    // l1_residual_hist -> SetTitle(Form("DUT layer 1 residual, U%i",study_chip));
    l1_residual_hist -> GetXaxis() -> SetTitle("Residual [mm]");
    l1_residual_hist -> GetYaxis() -> SetTitle("Events");
    l1_residual_hist -> SetLineWidth(3);
    l1_residual_hist -> SetLineColor(TColor::GetColor("#1A3947"));

    // l1_residual_hist -> GetXaxis() -> SetNdivisions  (505);

    l1_residual_hist -> GetXaxis() -> SetTitleSize   (0.05);
	l1_residual_hist -> GetXaxis() -> SetTitleOffset (0.8);

	l1_residual_hist -> GetYaxis() -> SetTitleSize   (0.05);
	l1_residual_hist -> GetYaxis() -> SetTitleOffset (1.5);


    pad_c1 -> cd();

    l1_residual_hist -> Draw("hist");

    TLatex *gaus_fit_latex = new TLatex();
    gaus_fit_latex -> SetNDC();
    gaus_fit_latex -> SetTextSize(0.028);

    TLatex * Column_ID_tex = new TLatex();
    Column_ID_tex -> SetNDC();
    Column_ID_tex -> SetTextSize(0.028);

    double D_gaus_xmin = -10;
    double D_gaus_xmax =  10;
    double the_portion = 0.9973;

    l1_residual_hist -> Fit("gaus_fit","N");
    // note : data for -0.5 ~ 0.5
    // note : MC could be -0.3 ~ 0.3
    l1_residual_hist -> Fit(D_gaus_fit,"N","",-1,1);
    bool run_fit_effisig = double_gaus_getsigmarange (D_gaus_fit,the_portion,D_gaus_xmin,D_gaus_xmax);
    
    gaus_fit -> SetNpx(10000);
    D_gaus_fit -> SetNpx(10000);

    gaus_fit->Draw("lsame");	
    // D_gaus_fit->Draw("lsame");	 
    
    Column_ID_tex -> DrawLatex(0.2, 0.750, Form("Column ID : %i",study_chip));
    gaus_fit_latex -> DrawLatex(0.2, 0.710, Form("Mean :  %.4f mm", gaus_fit->GetParameter(1)));
    gaus_fit_latex -> DrawLatex(0.2, 0.670, Form("Mean error :  %.4f mm", gaus_fit->GetParError(1)));
    // gaus_fit_latex -> DrawLatex(0.25, 0.710, Form("Width %.4f",gaus_fit->GetParameter(2)));
    // gaus_fit_latex -> DrawLatex(0.25, 0.680, Form("#chi^{2}/NDF = %.2f/%i", gaus_fit->GetChisquare(),int(gaus_fit->GetNDF())));
    
    // gaus_fit_latex -> DrawLatex(0.12, 0.660, Form("double gaussian fit"));
    // gaus_fit_latex -> DrawLatex(0.12, 0.630, Form("mean : %.4f, fraction : %.3f",D_gaus_fit->GetParameter(2),D_gaus_fit->GetParameter(1)));
    // gaus_fit_latex -> DrawLatex(0.12, 0.600, Form("first width : %.4f, second width : %.4f",D_gaus_fit->GetParameter(3),D_gaus_fit->GetParameter(4)));
    // gaus_fit_latex -> DrawLatex(0.12, 0.570, Form("3 sigma width : %.4f %.4f = %.4f",D_gaus_xmax,D_gaus_xmin,D_gaus_xmax-D_gaus_xmin));
    // gaus_fit_latex -> DrawLatex(0.12, 0.540, Form("#chi^{2} : %.2f, NDF : %d, #chi^{2}/NDF : %.4f", D_gaus_fit->GetChisquare(),D_gaus_fit->GetNDF(),D_gaus_fit->GetChisquare()/double(D_gaus_fit->GetNDF())));
    
    gaus_fit_latex -> Draw("same");

    c1 -> Print( Form("%s/DUT_align_narrow_U%i_pub.pdf",folder_direction.Data(),study_chip) );
    c1 -> Clear();

    cout<<"=============================Alignment narrow information==============================="<<endl;
    cout<<"chip : "<<study_chip<<endl;
    cout<<"fit mean : "<<gaus_fit -> GetParameter(1)<<" width : "<<gaus_fit -> GetParameter(2)<<endl;
    cout<<"fit mean error : "<<gaus_fit->GetParError(1)<<endl;
    cout<<"CHi2 : "<<gaus_fit->GetChisquare()<<" NDF "<<gaus_fit->GetNDF()<<" = "<<gaus_fit->GetChisquare()/double(gaus_fit->GetNDF())<<endl;
    cout<<"stat mean : "<<l1_residual_hist -> GetMean()<<" stat width : "<<l1_residual_hist -> GetStdDev()<<endl;
    cout<<"stat entry : "<<l1_residual_hist -> GetEntries()<<endl;
    cout<<"=============================Alignment narrow information==============================="<<endl;

    return l1_residual_hist;
}






// note : for the comparison of data and MC
void dataMC_comp (TH1F* hist_data, TH1F* hist_MC, TString folder_direction, TString plot_name, bool linear_or_log = false, bool statsbox_bool = false)
{

    TCanvas * c3 = new TCanvas("c3","c3",800,800);
    c3 -> cd();

    // note : 0) is for SetGrid
    // note : 0,0) is for logY
    TPad *pad_obj = new TPad(Form("pad_obj"), "", 0.0, 0.30, 1.0, 1.0);
    Characterize_Pad(pad_obj, 0.15, 0.05, 0.1, 0.005, 0, 0);
    pad_obj -> Draw();

    TPad *pad_ratio = new TPad(Form("pad_ratio"), "", 0.0, 0.0, 1.0, 0.30);
    Characterize_Pad(pad_ratio, 0.15, 0.05, 0.02, 0.350, 0, 1);
    pad_ratio -> Draw();

    TLegend *legend1 = new TLegend (0.75, 0.7, 0.9, 0.85);
	legend1 -> SetTextSize (0.050);
	// legend1 -> SetNColumns (4);

    if (linear_or_log == true) // note : linear
    {
        plot_name += "_Linear";
    }
    else if (linear_or_log == false) // note : log
    {   
        pad_obj -> SetLogy();
        plot_name += "_Log";
    }

    hist_data -> SetLineColor( TColor::GetColor("#1A3947") );
    hist_data -> SetLineWidth(3);
    hist_data -> SetMarkerColor(TColor::GetColor("#1A3947"));
    hist_data -> SetMarkerSize(1);
    hist_data -> SetMarkerStyle(20);

    hist_data -> GetXaxis() -> SetTitle("Unit : mm");
    hist_data -> GetYaxis() -> SetTitle("A.U.");
    // hist_data -> GetYaxis() -> SetRangeUser(0,1);

    hist_MC -> SetLineColor( TColor::GetColor("#A08144") );
    hist_MC -> SetLineWidth(3);
    hist_MC -> SetTitle(plot_name);
    if (statsbox_bool == false)
    {
        hist_MC -> SetStats(0); // note : remove the box
        hist_data -> SetStats(0);
    } 

    // hist_MC -> SetTitle(Form("Run61, cluster size, all clusters, layer : %i",i));
    hist_MC -> GetXaxis() -> SetTitle("Unit : mm");
    hist_MC -> GetYaxis() -> SetTitle("A.U.");
    hist_MC -> GetYaxis() -> SetTitleSize   (0.06);
    hist_MC -> GetYaxis() -> SetTitleOffset (0.70);
    hist_MC -> GetYaxis() -> SetRangeUser(0,1);

    // note : normalize
    hist_data->Scale(1./hist_data->Integral(-1,-1));
    hist_MC->Scale(1./hist_MC->Integral(-1,-1));
    

    legend1 -> AddEntry (hist_data, Form("Data"),  "pl");
    legend1 -> AddEntry (hist_MC, Form("MC"),  "f");
    
     
    double Y_axis_max = (linear_or_log) ? 0.4 : 10;

    hist_MC -> SetMaximum(Y_axis_max);


    TH1F * hist_ratio = (TH1F*)hist_data -> Clone(); 
    hist_ratio -> Divide (hist_MC);

    Characterize_Rate1F(hist_ratio,1);
    
    pad_obj -> cd();
    hist_MC -> Draw("hist");
    hist_data -> Draw("ep same");
    legend1 -> Draw("same");

    pad_ratio -> cd();
    hist_ratio -> Draw("ep");

    TString output_plot_name = plot_name.ReplaceAll(" ","_");
    output_plot_name = output_plot_name.ReplaceAll(",","");

    c3 -> Print( Form("%s/%s.pdf",folder_direction.Data(),output_plot_name.Data()) );
    c3 -> Clear();
    
}

// note : plot the slope distribution, the (l2-l0)/distance
TH1F* plot_angle (vector<double> input_vec_hit3, vector<double> input_vec_hit2, TString folder_direction, int study_chip, bool draw_fitting)
{
    TCanvas * c1 = new TCanvas("c1","c1",850,800);
    c1 -> cd();

    TPad * pad_c1 = new TPad(Form("pad_c1"), "", 0.0, 0.0, 1.0, 1.0);
    Characterize_Pad(pad_c1, 0.15,  0.1,  0.1,  0.12, 0, 0);
    pad_c1 -> Draw();
    pad_c1 -> SetTicks(1,1);
    pad_c1 -> cd();

    cout<<"--------------------------------------------------------------------------------------------------------"<<endl;
    cout<<"----- Now we are doing the plot_angle "<<endl;
    cout<<"input_vec_hit3 size : "<<input_vec_hit3.size()<<endl;
    cout<<"input_vec_hit2 size : "<<input_vec_hit2.size()<<endl;
    cout<<"--------------------------------------------------------------------------------------------------------"<<endl;

    TH1F * angle_hist = new TH1F("","",50,-0.05,0.05);

    angle_hist -> SetLineWidth(3);
    angle_hist -> SetLineColor(TColor::GetColor("#1A3947")); 

    angle_hist -> GetXaxis() -> SetNdivisions  (505);

    angle_hist -> GetXaxis() -> SetTitleSize   (0.05);
	angle_hist -> GetXaxis() -> SetTitleOffset (0.8);

	angle_hist -> GetYaxis() -> SetTitleSize   (0.05);
	angle_hist -> GetYaxis() -> SetTitleOffset (1.5);

    for (int i = 0; i < input_vec_hit3.size(); i++)
    {
        angle_hist -> Fill(input_vec_hit3[i]);
    }

    for (int i = 0; i < input_vec_hit2.size(); i++)
    {
        angle_hist -> Fill(input_vec_hit2[i]);
    }

    // angle_hist -> SetTitle(Form("DUT candidate track slope, U%i",study_chip));
    angle_hist -> SetStats(0);
    angle_hist -> GetXaxis() -> SetTitle("Track slope");
    angle_hist -> GetYaxis() -> SetTitle("Events");

    angle_hist -> Draw("hist");

    TF1 * gaus_fit = new TF1 ("gaus_fit","gaus",-0.1,0.1);
    gaus_fit -> SetNpx(10000);

    angle_hist -> Fit("gaus_fit","NQ");

    if ( draw_fitting == true ) { gaus_fit->Draw("lsame"); } // todo : void for the fitting

    TLine * slope_cut_positive = new TLine(slope_cut,angle_hist -> GetMinimum(),slope_cut,angle_hist -> GetMaximum()*1.05);
    slope_cut_positive -> SetLineWidth(3);
    slope_cut_positive -> SetLineColor(TColor::GetColor("#A08144"));
    slope_cut_positive -> SetLineStyle(2);

    TLine * slope_cut_negative = new TLine(-1 * slope_cut,angle_hist -> GetMinimum(),-1 * slope_cut,angle_hist -> GetMaximum()*1.05);
    slope_cut_negative -> SetLineWidth(3);
    slope_cut_negative -> SetLineColor(TColor::GetColor("#A08144"));
    slope_cut_negative -> SetLineStyle(2);

    TLatex *gaus_fit_latex = new TLatex();
    gaus_fit_latex -> SetNDC();
    gaus_fit_latex -> SetTextSize(0.028);

    cout<<"Fit slope mean : "<<gaus_fit->GetParameter(1)<<endl;
    cout<<"Fit slope width : "<<gaus_fit->GetParameter(2)<<endl;
    cout<<"stat slope mean : "<<angle_hist -> GetMean()<<endl;
    cout<<"stat slope width : "<<angle_hist -> GetStdDev()<<endl;
    cout<<"stat entry : "<<angle_hist -> GetEntries()<<endl;

    // todo : void for the fitting
    if ( draw_fitting == true )
    {
        gaus_fit_latex -> DrawLatex(0.12+0.05, 0.750, Form("Mean :  %.8f ", gaus_fit->GetParameter(1)));
        gaus_fit_latex -> DrawLatex(0.12+0.05, 0.710, Form("Width : %.4f", gaus_fit->GetParameter(2)));
        gaus_fit_latex -> DrawLatex(0.12+0.05, 0.670, Form("#chi^{2} : %.2f, NDF : %d", gaus_fit->GetChisquare(),gaus_fit->GetNDF()));
        gaus_fit_latex -> DrawLatex(0.12+0.05, 0.630, Form("#chi^{2}/NDF : %.4f",gaus_fit->GetChisquare()/double(gaus_fit->GetNDF())));
        gaus_fit_latex -> Draw("same");  
    }


    TLegend *legend1 = new TLegend (0.62, 0.7, 0.85, 0.78);
	legend1 -> SetTextSize (0.03);
	// legend1 -> SetNColumns (4);
    legend1 -> SetBorderSize(0);
    legend1 -> AddEntry (slope_cut_positive, Form("Slope cut %.2f",slope_cut),  "l");


    slope_cut_negative -> Draw("l same");
    slope_cut_positive -> Draw("l same");

    legend1 -> Draw("l same");

    TString output_file_name = Form("DUT_candidate_track_slope_U%i",study_chip);
    output_file_name += ( slope_correction == 0. ) ? "_before" : "_after";
    output_file_name += ( draw_fitting == true ) ? "" : "_nofitting";

    c1 -> Print( Form("%s/%s.pdf",folder_direction.Data(),output_file_name.Data()) );
    c1 -> Clear();

    return angle_hist;
}


// note : plot the correction of event slope [(l2-l0)/distance] and the middle pos [(l2+l0)/2]
TH2F* plot_angle_pos_2D (vector<double> input_slope, vector<double> input_pos, TString folder_direction, int study_chip, int selection_level)
{
    TCanvas * c1 = new TCanvas("c1","c1",850,800);
    c1 -> cd();

    TPad * pad_c1 = new TPad(Form("pad_c1"), "", 0.0, 0.0, 1.0, 1.0);
    Characterize_Pad(pad_c1, 0.15,  0.15,  0.1,  0.12, 0, 0);
    pad_c1 -> Draw();
    pad_c1 -> SetTicks(1,1);
    pad_c1 -> cd();

    cout<<"--------------------------------------------------------------------------------------------------------"<<endl;
    cout<<"----- Now we are doing the plot_angle&pos plot "<<endl;
    cout<<"input_slope size : "<<input_slope.size()<<endl;
    cout<<"input_pos size   : "<<input_pos.size()<<endl;
    cout<<"--------------------------------------------------------------------------------------------------------"<<endl;

    TH2F * angle_pos_hist = new TH2F("","",50,-0.05,0.05,50,-10,10);

    // angle_pos_hist -> SetLineWidth(3);
    // angle_pos_hist -> SetLineColor(TColor::GetColor("#1A3947")); 

    angle_pos_hist -> GetXaxis() -> SetNdivisions  (505);

    angle_pos_hist -> GetXaxis() -> SetTitleSize   (0.05);
	angle_pos_hist -> GetXaxis() -> SetTitleOffset (0.8);

	angle_pos_hist -> GetYaxis() -> SetTitleSize   (0.05);
	angle_pos_hist -> GetYaxis() -> SetTitleOffset (1.5);

    for (int i = 0; i < input_slope.size(); i++)
    {
        angle_pos_hist -> Fill(input_slope[i],input_pos[i]);
    }

    // angle_pos_hist -> SetTitle(Form("DUT candidate track slope, U%i",study_chip));
    angle_pos_hist -> SetStats(0);
    angle_pos_hist -> GetXaxis() -> SetTitle("Track slope");
    angle_pos_hist -> GetYaxis() -> SetTitle("Track mid pos [mm]");

    angle_pos_hist -> Draw("colz0");

    TString output_file_name = Form("DUT_candidate_track_slope_Pos_U%i_SelectionLevel_%i",study_chip,selection_level);
    output_file_name += ( slope_correction == 0. ) ? "_before" : "_after";

    c1 -> Print( Form("%s/%s.pdf",folder_direction.Data(),output_file_name.Data()) );
    c1 -> Clear();

    return angle_pos_hist;
}


// note : to understand the behavior of run89
void hit3_plot_residual_slope_2D (vector<double> input_vec_1, vector<double> input_vec_2, TString folder_direction)
{
    TCanvas * c3 = new TCanvas("c3","c3",1000,800);
    // c3 -> cd();

    TPad *pad = new TPad(Form("pad1"), "", 0.0, 0.0, 1.0, 1.0);
	//pad->SetTopMargin(0.12);
	//pad->SetBottomMargin(0.120);
	pad->SetLeftMargin(0.15);
	pad->SetRightMargin(0.15);
	//pad->SetGrid(1, 1);
	pad->Draw("same");
	pad->cd();

    // c3 -> SetLogy();

    cout<<"--------------------------------------------------------------------------------------------------------"<<endl;
    cout<<"----- Now we are doing the hit3_plot_residual_slope_2D "<<endl;
    // cout<<"----- Don't forget to turn it on"<<endl;
    cout<<"input_vec_1 size : "<<input_vec_1.size()<<endl;
    cout<<"input_vec_2 size : "<<input_vec_2.size()<<endl;
    cout<<"--------------------------------------------------------------------------------------------------------"<<endl;

    TH2F * residual_slope_2Dhist = new TH2F("","",100,-10,10,100,-0.1,0.1);
    residual_slope_2Dhist -> SetStats(0);
    for (int i = 0; i < input_vec_1.size(); i++)
    {
        residual_slope_2Dhist -> Fill(input_vec_1[i],input_vec_2[i]);
    }

    residual_slope_2Dhist -> SetTitle("Correlation of residual and slope");
    residual_slope_2Dhist -> GetXaxis() -> SetTitle("Residual, unit : mm");
    residual_slope_2Dhist -> GetYaxis() -> SetTitle("(l2-l0)/distance");

    residual_slope_2Dhist -> Draw("colz0");

    c3 -> Print( Form("%s/DUT_residual_slope_2Dhist.pdf",folder_direction.Data()) );
    c3 -> Clear();
}

// note : to understand the behavior of run89
void hit3_plot_residual_position_2D (vector<double> input_vec_1, vector<double> input_vec_2, TString folder_direction)
{
    TCanvas * c3 = new TCanvas("c3","c3",1000,800);
    // c3 -> cd();
    TPad *pad = new TPad(Form("pad1"), "", 0.0, 0.0, 1.0, 1.0);
	//pad->SetTopMargin(0.12);
	//pad->SetBottomMargin(0.120);
	pad->SetLeftMargin(0.15);
	pad->SetRightMargin(0.15);
	//pad->SetGrid(1, 1);
	pad->Draw("same");
	pad->cd();
    // c3 -> SetLogy();

    cout<<"--------------------------------------------------------------------------------------------------------"<<endl;
    cout<<"----- Now we are doing the hit3_plot_residual_position_2D "<<endl;
    // cout<<"----- Don't forget to turn it on"<<endl;
    cout<<"input_vec_1 size : "<<input_vec_1.size()<<endl;
    cout<<"input_vec_2 size : "<<input_vec_2.size()<<endl;
    cout<<"--------------------------------------------------------------------------------------------------------"<<endl;

    TH2F * residual_slope_2Dhist = new TH2F("","",100,-10,10,100,-10,10);
    residual_slope_2Dhist -> SetStats(0);
    for (int i = 0; i < input_vec_1.size(); i++)
    {
        residual_slope_2Dhist -> Fill(input_vec_1[i],input_vec_2[i]);
    }

    residual_slope_2Dhist -> SetTitle("Correlation of residual and l0 position");
    residual_slope_2Dhist -> GetXaxis() -> SetTitle("Residual, unit : mm");
    residual_slope_2Dhist -> GetYaxis() -> SetTitle("L0 cluster pos, unit : mm");

    residual_slope_2Dhist -> Draw("colz0");

    c3 -> Print( Form("%s/DUT_residual_l0pos_2Dhist.pdf",folder_direction.Data()) );
    c3 -> Clear();
}

void effi_pos_plot (vector<double> input_vec_pos, vector<int> input_vec_2, TString folder_direction, int study_chip, double beam_spot_range[2],double effi_pos_legend_offset)
{
    double edge_exclusion_bottom = (lower_section_initial - INTT_strip_width / 2.) + INTT_strip_width * double(boundary_cut);
	double edge_exclusion_upper = ( INTT_strip_width * 128. ) - INTT_strip_width * double(boundary_cut);

    cout<<"============== we are making the detection efficiency plot ============== "<<endl;
    sleep(2);
    TCanvas * c6 = new TCanvas("c6","c6",900,800);
    c6 -> cd();

    TPad *pad_obj = new TPad(Form("pad_obj"), "", 0.0, 0.0, 1.0, 1.0);
    Characterize_Pad(pad_obj, 0.18,  0.1,  0.1,  0.12, 0, 0);
    pad_obj -> SetTicks(1,1);
    pad_obj -> Draw();

    cout<<"size of vec 1 : "<<input_vec_pos.size()<<endl;
    cout<<"size of vec 2 : "<<input_vec_2.size()<<endl;

    double left_pos_edge = -10.;
    double right_pos_edge = 10;
    int N_bins_pos_hist = 20;

    vector<double> grX; grX.clear();
    vector<double> grY; grY.clear();
    vector<double> grXeh; grXeh.clear(); // note : half of the X error
    vector<double> grYeU; grYeU.clear(); // note : Y error up
    vector<double> grYeL; grYeL.clear(); // note : Y error down

    TH1F * good_event_hist = new TH1F ("","",N_bins_pos_hist,left_pos_edge,right_pos_edge);
    TH1F * total_event_hist = new TH1F ("","",N_bins_pos_hist,left_pos_edge,right_pos_edge);

    for (int i = 0; i < good_event_hist -> GetNbinsX(); i++){
        
        if (i < 3 || i > 15 ) continue;

        grX.push_back(good_event_hist -> GetBinCenter(i + 1));
        grXeh.push_back( (good_event_hist -> GetBinWidth(i + 1))/2. );
    }

    double plot_high = 102;
    double plot_low  = 95;

    Characterize_Hist1F(good_event_hist,0);
    Characterize_Hist1F(total_event_hist,0);

    good_event_hist -> GetXaxis() -> SetTitle("Track-interpolated position [mm]");
    good_event_hist -> GetYaxis() -> SetTitle("Detection efficiency");

    total_event_hist -> GetXaxis() -> SetTitle("Track-interpolated position [mm]");
    total_event_hist -> GetYaxis() -> SetTitle("Detection efficiency");

    for (int i = 0; i < input_vec_pos.size(); i++)
    {
        total_event_hist -> Fill(input_vec_pos[i]);

        if( input_vec_2[i] == 1 )
        {
            good_event_hist -> Fill(input_vec_pos[i]);
        }
    }

    SetsPhenixStyle();

    TEfficiency * detection_effi_pos = new TEfficiency (*good_event_hist,*total_event_hist);
    for (int i = 0; i < good_event_hist -> GetNbinsX(); i++){

        if (i < 3 || i > 15 ) continue;

        grY.push_back(100. * detection_effi_pos -> GetEfficiency(i + 1));
        grYeU.push_back(100. * detection_effi_pos -> GetEfficiencyErrorUp(i + 1));
        grYeL.push_back(100. * detection_effi_pos -> GetEfficiencyErrorLow(i + 1));
    }

    for (int i = 0; i < grX.size(); i++){
        cout<<"effi. bin : "<<i+1<<" "<<grX[i]<<" "<<grXeh[i]<<" Y : "<<grY[i]<<" YeU "<<grYeU[i]<<" YeL "<<grYeL[i]<<endl;
    }

    TGraphAsymmErrors * effi_pos_gr = new  TGraphAsymmErrors(grX.size(), &grX[0], &grY[0], &grXeh[0], &grXeh[0], &grYeL[0], &grYeU[0]);
    effi_pos_gr -> SetMarkerStyle(20);
    effi_pos_gr -> SetMarkerSize(0.8);
    effi_pos_gr -> SetMarkerColor(TColor::GetColor("#1A3947"));
    effi_pos_gr -> SetLineColor(TColor::GetColor("#1A3947"));
    effi_pos_gr -> SetLineWidth(2);
    effi_pos_gr -> GetYaxis() -> SetTitle("Detection efficiency [%]");
    effi_pos_gr -> GetXaxis() -> SetTitle("Track position [mm]");
    effi_pos_gr -> GetYaxis() -> SetRangeUser(plot_low,plot_high);
    effi_pos_gr -> GetXaxis() -> SetLimits(-10,10);
    effi_pos_gr -> GetXaxis() -> SetNdivisions  (505);

    // detection_effi_pos -> SetMarkerColor(TColor::GetColor("#1A3947"));
    // detection_effi_pos -> SetMarkerSize(1);
    // detection_effi_pos -> SetMarkerStyle(4);
    // detection_effi_pos -> SetLineColor(TColor::GetColor("#1A3947"));
    // detection_effi_pos -> SetLineWidth(2);

    // detection_effi_pos -> SetTitle("Detection efficiency vs Position");
    
    
	// detection_effi_pos -> GetPaintedGraph() -> GetYaxis() -> SetRangeUser   (0.4, 3);
    // detection_effi_pos -> GetXaxis() -> SetTitleOffset(1.1);

	// detection_effi_pos -> GetPaintedGraph() -> GetYaxis() -> SetNdivisions  (505);


    TLine * line_99 = new TLine(-10,99,10,99);
    line_99 -> SetLineColor(TColor::GetColor("#941100"));
    line_99 -> SetLineWidth(5);
    line_99 -> SetLineStyle(7);

    pad_obj -> cd();
    effi_pos_gr -> Draw("ap");

    line_99 -> Draw("lsame");

    TLatex *ltx = new TLatex();
    ltx->SetNDC();
    ltx->SetTextSize(0.045);
    ltx->DrawLatex(gPad->GetLeftMargin(), 1 - gPad->GetTopMargin() + 0.01, "#it{#bf{sPHENIX INTT}} Beam Test 2021");
    ltx->DrawLatex(0.4, 0.155, "*Beam spot region shown only");
    ltx->DrawLatex(0.53, 0.835, "Run 52, Dec 10, 2021");

    // gPad->Update(); 
    // auto graph = detection_effi_pos->GetPaintedGraph(); 
    // graph->SetMinimum(plot_low);
    // graph->SetMaximum(plot_high); 
    // gPad->Update(); 

    // TLine * edge_exclusion_line_pos = new TLine(edge_exclusion_upper, plot_low, edge_exclusion_upper, plot_high);
    // edge_exclusion_line_pos -> SetLineWidth(3);
    // edge_exclusion_line_pos -> SetLineColor(TColor::GetColor("#A08144"));
    // edge_exclusion_line_pos -> SetLineStyle(2);

    // TLine * edge_exclusion_line_neg = new TLine(edge_exclusion_bottom, plot_low, edge_exclusion_bottom, plot_high);
    // edge_exclusion_line_neg -> SetLineWidth(3);
    // edge_exclusion_line_neg -> SetLineColor(TColor::GetColor("#A08144"));
    // edge_exclusion_line_neg -> SetLineStyle(2);

    double beam_spot_line_height = 1.007;
    double beam_spot_edge_width = 0.002;

    // note : for the beam spot region
    // TLine * beam_spot_hor = new TLine(beam_spot_range[0], beam_spot_line_height, beam_spot_range[1], beam_spot_line_height);
    // beam_spot_hor -> SetLineWidth(3);
    // beam_spot_hor -> SetLineColor(TColor::GetColor("#009193"));
    // beam_spot_hor -> SetLineStyle(2);
    // // note : for the beam spot region
    // TLine * beam_spot_ver_l = new TLine(beam_spot_range[0], beam_spot_line_height - beam_spot_edge_width, beam_spot_range[0], beam_spot_line_height + beam_spot_edge_width);
    // beam_spot_ver_l -> SetLineWidth(3);
    // beam_spot_ver_l -> SetLineColor(TColor::GetColor("#009193"));
    // beam_spot_ver_l -> SetLineStyle(2);
    // // note : for the beam spot region
    // TLine * beam_spot_ver_r = new TLine(beam_spot_range[1], beam_spot_line_height - beam_spot_edge_width, beam_spot_range[1], beam_spot_line_height + beam_spot_edge_width);
    // beam_spot_ver_r -> SetLineWidth(3);
    // beam_spot_ver_r -> SetLineColor(TColor::GetColor("#009193 "));
    // beam_spot_ver_r -> SetLineStyle(2);


    TLegend *legend1 = new TLegend (0.33-effi_pos_legend_offset, 0.35, 0.6-effi_pos_legend_offset, 0.5);
	legend1 -> SetTextSize (0.045);
	// legend1 -> SetNColumns (4);
    legend1 -> SetBorderSize(0);
    // legend1 -> AddEntry (edge_exclusion_line_pos, Form("Edge exclusion cut"),  "l");
    legend1 -> AddEntry (line_99, Form("99%% efficiency line"),  "l");
    // legend1 -> AddEntry (beam_spot_hor, Form("Beam-spot region"),  "l");


    // edge_exclusion_line_pos -> Draw("lsame");
    // edge_exclusion_line_neg -> Draw("lsame");
    // beam_spot_hor -> Draw("lsame");
    // beam_spot_ver_l -> Draw("lsame");
    // beam_spot_ver_r -> Draw("lsame");
    legend1 -> Draw("same");

    c6 -> Print( Form("%s/Effi_Pos_U%i_pub_gr.pdf",folder_direction.Data(),study_chip) );
    c6 -> Clear();

}

// note : print the cut value
void print_used_par ()
{
    
    cout<<"=========================== used parameters =============================="<<endl;
    cout<<"the distance of 1st and 3rd ladder :       "<<actual_xpos[2]<<endl;
    cout<<"the INTT_strip_width :                     "<<INTT_strip_width<<endl;
    cout<<"the lower_initial :                        "<<lower_section_initial<<endl;
    cout<<"the upper_initial :                        "<<upper_section_initial<<endl;
    cout<<"study chip :                               "<<study_chip<<endl;
    cout<<"l1 alignment correction :                  "<<amount_of_alignment<<endl;
    cout<<" "<<endl;
    cout<<"the residual tolerance :                   "<<noise_hit_distance<<endl;
    cout<<"tge slope cut :                            "<<slope_cut<<endl;
    cout<<"the slope correction :                     "<<slope_correction<<endl;
    cout<<"the cluster size requirement :             "<<cluster_size_requirement<<endl;
    cout<<"the cluster size requirement combined :    "<<cluster_size_requirement_combine<<endl;
    cout<<"the cluster adc value requirement :        "<<cluster_adc_value_requirement<<endl;
    cout<<"the clusters allowed in adjacent layers :  "<<hit_allowed_in_adjacent_layers<<endl;
    cout<<"the edge exclusion :                       "<<boundary_cut<<endl;
    cout<<"=========================== used parameters =============================="<<endl;
}

void output_SU_root_file (vector<vector<double>>input_2D_vec, TString folder_direction, int run_scan, int study_chip)
{
    cout<<"======================= making the root file ======================="<<endl;
    sleep(2);

    TFile * output_file = new TFile(Form("%s/DUT_v2_cut_scan_%i_U%i.root",folder_direction.Data(),run_scan,study_chip), "RECREATE");
    output_file -> cd();

    double edge_exclusion;
    double slope_cut;
    double residual_cut;
    double pass_event;
    double missing_101_2;
    double missing_101_3;

    TTree * output_tree = new TTree("cut_scan", "cut scan for systematic uncertainty");
    output_tree -> Branch("edge_exclusion",&edge_exclusion);
    output_tree -> Branch("slope_cut",&slope_cut);
    output_tree -> Branch("residual_cut",&residual_cut);
    output_tree -> Branch("pass_event",&pass_event);
    output_tree -> Branch("missing_101_2",&missing_101_2);
    output_tree -> Branch("missing_101_3",&missing_101_3);


    cout<<"size of input file : "<<input_2D_vec.size()<<endl;
    for (int i = 0; i < input_2D_vec.size(); i++)
    {
        edge_exclusion = input_2D_vec[i][0];
        slope_cut      = input_2D_vec[i][1];
        residual_cut   = input_2D_vec[i][2];
        pass_event     = input_2D_vec[i][3];
        missing_101_2  = input_2D_vec[i][4];
        missing_101_3  = input_2D_vec[i][5];

        output_tree -> Fill();
    }


    output_file -> cd();
    output_tree -> Write("", TObject::kOverwrite);
    output_file -> Close();
}

// note : print the efficiency, the statistic error is given by the TEfficiency
double print_effi ( DUT_str input_DUT_data )
{
    cout<<"=========================== Detection efficiency ==================================="<<endl;
    cout<<"# of 111 : "<<input_DUT_data.track_111_count<<endl;
    cout<<"# of 101 : "<<input_DUT_data.track_101_count<<endl;
    cout<<"Efficiency : "<<double(input_DUT_data.track_111_count) / double(input_DUT_data.track_111_count + input_DUT_data.track_101_count)<<endl;


    TH1F * total_hist = new TH1F ("","",1,0,1);
    TH1F * pass_hist = new TH1F ("","",1,0,1);
    
    // note : layer - 1 efficiency
    total_hist->SetBinContent(1,(input_DUT_data.track_111_count + input_DUT_data.track_101_count)); 
    pass_hist ->SetBinContent(1,input_DUT_data.track_111_count);

    TEfficiency * detection_effi = new TEfficiency (*pass_hist,*total_hist);
    printf("Efficiency by TEfficiency \n");
    printf("%.5f\t+%.5f\t-%.5f \n", detection_effi->GetEfficiency(1)*100.,detection_effi->GetEfficiencyErrorUp(1)*100.,detection_effi->GetEfficiencyErrorLow(1)*100.);

    cout<<"=========================== Detection efficiency ==================================="<<endl;

    double output_effi = detection_effi->GetEfficiency(1);

    return output_effi;
}