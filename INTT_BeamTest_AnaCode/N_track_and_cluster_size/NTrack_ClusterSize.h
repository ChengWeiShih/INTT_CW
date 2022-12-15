// note : the features of the v4 version 
// note : the event doesn't have to have the good track in the first place. We do the tracking here
// note : all the good track can be removed
// note : in principle, the macro should work with the "cluster_info_slope_adc.root" (file made by the track_single_cluster_v2.C)

// note : we can have the adc information to have a better seapration on the delta-ray electron and the noise, 2022/11/05

// todo : find a way to improve the noise_hit_distance ? Right now what I am using now is from the second layer residual distribution of layer-1

TString color_code [8]={"#343434","#1A3947","#566575","#797983","#EFBD9D","#FCA26E","#F5751D","#F5321D"};
TString color_code_2 [8]={"#CC768D","#19768D","#DDA573","#009193","#6E9193","#941100","#A08144","#517E66"};
double noise_hit_distance = 0.234; // note : 3 sigma of the l1 residual width
double actual_xpos [3] = {0,29.552,59.104}; 
double distance_to_sci = 183.5;

#include "vector_stddev.h"

// double slope_cut = 0.4; // note : the parameter for the DUT

// note : the structure for event profile
struct profile_str {
    int profile_eID;
    int profile_chip;
    int profile;
    int final_pickup;
    int clusterID_0;
    int clusterID_1;
    int clusterID_2;
};

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

struct noise_hit_str{
    int noise_chip;
    int noise_layer;
    double noise_pos;
    double noise_adc;
};

struct noise_event_str{
    int eID;
    vector<noise_hit_str> noise_event;
};

struct pol1_info{
    int eID;
    int chip_id;
    double slope;
    double offset;
};

struct multi_track_str{
    // note : in order to output multiple things.

    // note : in order to take out the araay of the # of track of each event.
    vector<noise_event_str> noise_vec;
    vector<int> N_track;

    vector<int> track_chip_count;

    vector<vector<pol1_info>> track_fit_info_vec;
    
};

struct edge_finder_str{
    double hist_left_edge;
    double hist_right_edge;
};


struct DUT_str{
    int track_111_count;
    int track_101_count;
    vector<double> middle_layer_residual; 
    vector<double> good_combination_slope; 
};


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
    hist -> SetLineWidth   (2);

	hist -> SetMarkerColor (TColor::GetColor(color_ID));
	hist -> SetMarkerStyle (20);
	hist -> SetMarkerSize  (0.8);
	// hist -> SetFillColor   (TColor::GetColor(color_ID));

	hist -> GetXaxis() -> SetTitleSize   (0.052*ratio);
	hist -> GetXaxis() -> SetTitleOffset (0.940);

	hist -> GetXaxis() -> SetLabelSize   (0.042*ratio);
	hist -> GetXaxis() -> SetLabelOffset (0.004*ratio);

	hist -> GetYaxis() -> SetTitle       ("Data/MC");
	hist -> GetYaxis() -> SetTitleSize   (0.052*ratio);
	hist -> GetYaxis() -> SetTitleOffset (1.8);

	hist -> GetYaxis() -> SetLabelSize   (0.042*ratio);
	hist -> GetYaxis() -> SetLabelOffset (0.006);
	// hist -> GetYaxis() -> SetRangeUser   (0.4, 3);

	hist -> GetYaxis() -> SetNdivisions  (505);

    if (statsbox == 0) {hist -> SetStats(0);}
}

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
            output_vec[output_vec.size()-1].cluster_layer.push_back( cluster_layer -> at(i1) );
            output_vec[output_vec.size()-1].cluster_chip.push_back( cluster_chip -> at(i1) );
            output_vec[output_vec.size()-1].cluster_hit.push_back( cluster_hit -> at(i1) );
            output_vec[output_vec.size()-1].cluster_position.push_back( cluster_position -> at(i1) );
            output_vec[output_vec.size()-1].cluster_adc.push_back( cluster_adc -> at(i1) );
        }


        // cout<<"-------------- -------------- -------------- -------------- --------------"<<endl;
        // if (1==1) {cout<<i<<" event check : "<<cluster_chip->size()<<endl;}
        // if (1==1) {cout<<i<<" event check : "<< output_vec[0].cluster_chip.size()<< " "<<output_vec[0].cluster_position[0] <<endl;}

    }

    
    // cout<<"event check out but in : "<< output_vec[0].cluster_chip.size() <<endl;
    cout<<"cluster size check : "<<output_vec.size()<<endl;
    return output_vec;
}

vector<cluster_reformat_str> cluster_reformat ( vector<cluster_str> input_vec )
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
            output_vec[ output_vec.size() - 1 ].cluster_pos[ input_vec[i].cluster_chip[i1] - 1 ][ input_vec[i].cluster_layer[i1] ].push_back( input_vec[i].cluster_position[i1] );
            output_vec[ output_vec.size() - 1 ].cluster_adc[ input_vec[i].cluster_chip[i1] - 1 ][ input_vec[i].cluster_layer[i1] ].push_back( input_vec[i].cluster_adc[i1] );
        }
    }

    cout<<"reformat output check : "<<output_vec.size()<<endl;
    return output_vec;
    
}

// note : to remove the multi-tracks in the event
multi_track_str multiple_track_removal ( vector<cluster_reformat_str> input_cluster_vec, bool print_all_track = false )
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

void post_multi_N_track (multi_track_str input_vec, TString folder_direction)
{
    TCanvas * c1 = new TCanvas("c1","c1",800,800);
    c1 -> cd();
    c1 -> SetLogy();

    TH1F * N_track_hist = new TH1F("","",10,0-2,10-2);
    for (int i = 0; i < input_vec.N_track.size(); i++)
    {
        N_track_hist -> Fill(input_vec.N_track[i]);
    }

    N_track_hist -> SetTitle("(multrack) Number of tracks in event");
    N_track_hist -> GetXaxis() -> SetTitle("N track");
    N_track_hist -> GetYaxis() -> SetTitle("Entry");

    N_track_hist -> Draw("hist");

    c1 -> Print( Form("%s/PostTrack_removal_N_track.pdf",folder_direction.Data()) );
    c1 -> Clear();
}

edge_finder_str hist_edge_finder_hist (TH1F * input_hist)
{   
    int N_bins = input_hist -> GetNbinsX();
    vector<double> bin_content_vec; bin_content_vec.clear();

    int sort_bin_content_index[N_bins];

    for (int i = 0; i < N_bins; i++)
    {
        bin_content_vec.push_back( input_hist -> GetBinContent(i+1) );
        cout<<i+1<<" "<<input_hist -> GetBinContent(i+1)<<endl;
    }

    TMath::Sort(N_bins, &bin_content_vec[0], sort_bin_content_index);
    
    // note : to calculate average of the top N bins, in order to find the edges 
    int top_N = 45;
    vector<double> top_N_vec; top_N_vec.clear(); 
    for (int i = 0; i < top_N; i++)
    {
        top_N_vec.push_back( bin_content_vec[ sort_bin_content_index[i] ] );
    }

    double top_N_avg = vector_average(top_N_vec); // note : calculate the average of the top 20 bins
    double top_N_stddev = vector_stddev(top_N_vec); // note : calculate the stddev of the top 20 bins

    double left_edge = 1; // note : first bin for func "BinContent"
    double right_edge = N_bins; 

    cout<<"the mean entry of top "<<top_N<<" : "<<top_N_avg<<" std : "<<top_N_stddev<<endl;
    
    // note : run when condition is achieved.
    while ( input_hist -> GetBinContent(left_edge) < top_N_avg ) // note : left edge
    {
        left_edge += 1;
    }

    while ( input_hist -> GetBinContent(right_edge) < top_N_avg ) // note : right edge
    {
        right_edge -= 1; 
    }

    cout<<"extrapolation edge finder, left : "<<left_edge<<" right edge : "<<right_edge<<endl;

    edge_finder_str output_str;
    output_str.hist_left_edge = left_edge;
    output_str.hist_right_edge = right_edge;

    return output_str;

}

// note : this is the more advanced version of the N_track_distribution
// note : because some of the track may not from the beam
// note : what we can do is to check the extrapolation position, and to see whether the position is within the scintillator area
edge_finder_str post_multi_N_track_extrapolation (multi_track_str input_vec, TString folder_direction, TString file_name)
{
    TCanvas * c1 = new TCanvas("c1","c1",800,800);
    c1 -> cd();

    TPad *pad_obj = new TPad(Form("pad_obj"), "", 0.0, 0.0, 1.0, 1.0);
    Characterize_Pad(pad_obj);
    pad_obj -> Draw();

    cout<<"making the extrapolation hist"<<endl;
    sleep(2);
        
    int good_tracking_counting = 0;

    TH1F * extra_pos_hist = new TH1F("","",100,-20,20);
    Characterize_Hist1F(extra_pos_hist, 1);

    extra_pos_hist -> SetTitle("extrapolation position of the tracks");
    extra_pos_hist -> GetXaxis() -> SetTitle("position (mm)");
    extra_pos_hist -> GetYaxis() -> SetTitle("Entry");

    for (int i = 0; i < input_vec.track_fit_info_vec.size(); i++) // note : number of event which has at least one good track 
    {
        for ( int i1 = 0; i1 < input_vec.track_fit_info_vec[i].size(); i1++ ) // note : number of good track (criteria by fitting) in single event.
        {
            extra_pos_hist -> Fill( input_vec.track_fit_info_vec[i][i1].slope * distance_to_sci + input_vec.track_fit_info_vec[i][i1].offset );
        }

    }

    pad_obj -> cd();
    extra_pos_hist -> Draw("hist");

    c1 -> Print( Form("%s/%s_PostTrack_track_extrapolation.pdf",folder_direction.Data(),file_name.Data()) );
    c1 -> Clear();

    edge_finder_str output_str = hist_edge_finder_hist(extra_pos_hist);

    return output_str;
}

vector<double> fill_value()
{

    double left_edge;
    double right_edge;
    cout<<"the lefe edge : ";
    cin>>left_edge;

    cout<<" "<<endl;
    cout<<"the right edge : ";
    cin>>right_edge;
    
    vector<double> output_vec; output_vec.clear();
    output_vec.push_back(left_edge);
    output_vec.push_back(right_edge);
    return output_vec;
}

TH1F* post_multi_N_track_check (multi_track_str input_vec, TString run_ID, TString folder_direction, TString file_name)
{

    TCanvas * c1 = new TCanvas("c1","c1",800,800);
    c1 -> cd();

    TPad *pad_obj = new TPad(Form("pad_obj"), "", 0.0, 0.0, 1.0, 1.0);
    Characterize_Pad(pad_obj, 0.2,  0.05,  0.1,  0.15, 1, 1);
    pad_obj -> Draw();

    vector<double> left_right_edge_vec = fill_value();

    double left_edge = left_right_edge_vec[0];
    double right_edge = left_right_edge_vec[1];

    cout<<"working on checking the extrapolation position"<<endl;
    sleep(3);
    cout<<"size of the imput vec : "<<input_vec.track_fit_info_vec.size()<<endl;

    double extrapolation_pos;
    int event_track_counting = 0;

    TH1F * event_N_track_hist = new TH1F("","",20,0-3,20-3);
    Characterize_Hist1F(event_N_track_hist,0);

    event_N_track_hist -> SetTitle(Form("%s, N track distribution",run_ID.Data()));
    event_N_track_hist -> GetXaxis() -> SetTitle("N track");
    event_N_track_hist -> GetYaxis() -> SetTitle("Entry");


    for ( int i = 0; i < input_vec.track_fit_info_vec.size(); i++ )
    {
        for ( int i1 = 0; i1 < input_vec.track_fit_info_vec[i].size(); i1++  )
        {

            extrapolation_pos = input_vec.track_fit_info_vec[i][i1].slope * distance_to_sci + input_vec.track_fit_info_vec[i][i1].offset;

            if ( extrapolation_pos > left_edge && extrapolation_pos < right_edge )
            {
                event_track_counting += 1;
            }
        }
        
        if (event_track_counting == 3) cout<<"Post-check, 3-tracks event found !, eID : "<<input_vec.track_fit_info_vec[i][0].eID<<endl;

        if ( event_track_counting != 0 ) event_N_track_hist -> Fill(event_track_counting);
        event_track_counting = 0;
    }

    pad_obj -> cd();

    event_N_track_hist -> Draw("hist");
    c1 -> Print( Form("%s/%s_NTrack_PostCheck_Ledge_%.2f_Redge_%.2f.pdf",folder_direction.Data(),file_name.Data(),left_edge,right_edge) );
    c1 -> Clear();

    return event_N_track_hist;
    
}

void N_track_dataMC_comp (TH1F* hist_data, TH1F* hist_MC, TString folder_direction, TString run_ID, bool linear_or_log = false, bool statsbox_bool = false)
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

    TLegend *legend1 = new TLegend (0.5, 0.7, 0.75, 0.85);
	legend1 -> SetTextSize (0.050);
	// legend1 -> SetNColumns (4);

    TString rest_plot_name;
    if (linear_or_log == true) // note : linear
    {
        rest_plot_name = "PostCheck_NTrack_Linear";
    }
    else if (linear_or_log == false) // note : log
    {   
        pad_obj -> SetLogy();
        rest_plot_name = "PostCheck_NTrack_Log";
    }

    hist_data -> SetLineColor( TColor::GetColor("#1A3947") );
    hist_data -> SetLineWidth(3);
    hist_data -> SetMarkerColor(TColor::GetColor("#1A3947"));
    hist_data -> SetMarkerSize(1);
    hist_data -> SetMarkerStyle(20);

    hist_data -> GetXaxis() -> SetTitle("N tracks");
    hist_data -> GetYaxis() -> SetTitle("A.U.");
    // hist_data -> GetYaxis() -> SetRangeUser(0,1);

    hist_MC -> SetLineColor( TColor::GetColor("#A08144") );
    hist_MC -> SetLineWidth(3);
    hist_MC -> SetTitle(Form("%s, Post extrapolation check",run_ID.Data()));
    if (statsbox_bool == false) hist_MC -> SetStats(0); // note : remove the box

    // hist_MC -> SetTitle(Form("Run61, cluster size, all clusters, layer : %i",i));
    hist_MC -> GetXaxis() -> SetTitle("N tracks");
    hist_MC -> GetYaxis() -> SetTitle("A.U.");
    hist_MC -> GetYaxis() -> SetTitleSize   (0.06);
    hist_MC -> GetYaxis() -> SetTitleOffset (0.70);
    hist_MC -> GetYaxis() -> SetRangeUser(0,1);

    // note : normalize
    hist_data->Scale(1./hist_data->Integral(-1,-1));
    hist_MC->Scale(1./hist_MC->Integral(-1,-1));
    

    legend1 -> AddEntry (hist_data, Form("%s, Data",run_ID.Data()),  "pl");
    legend1 -> AddEntry (hist_MC, Form("%s, MC",run_ID.Data()),  "f");
    
     
    double Y_axis_max = (linear_or_log) ? 1.1 : 10;

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


    c3 -> Print( Form("%s/%s_%s.pdf",folder_direction.Data(),run_ID.Data(),rest_plot_name.Data()) );
    c3 -> Clear();
    
}

void post_multi_noise_hitmap (vector<noise_event_str> input_noise_vec, TString folder_direction)
{

    TCanvas *c4 = new TCanvas("c4", "noise_hitmap", 0, 700, 1625, 250);
    // c4 -> SetLogz();
    TH2D* hitmap[3];
    for (int i = 0; i < 3; i++)
    {
        hitmap[i] = new TH2D("hitmap", "", 13, 0, 13, 200, -10.5, 10.5);
    }

    c4->cd();
    c4->Print( Form("%s/PostTrack_noise_hitmap.pdf(",folder_direction.Data()) );

    for (int i = 0; i < input_noise_vec.size(); i++)
    {
        for (int i1 = 0; i1 < input_noise_vec[i].noise_event.size(); i1++)
        {
            int chip_conversion = -input_noise_vec[i].noise_event[i1].noise_chip + (1 + int(input_noise_vec[i].noise_event[i1].noise_chip / 14)) * 13;

            hitmap[ input_noise_vec[i].noise_event[i1].noise_layer ] -> Fill( chip_conversion, input_noise_vec[i].noise_event[i1].noise_pos );
        }
        
    }

    for (int i = 0; i < 3; i++)
    {
        hitmap[i]->Draw("colz");
    
        hitmap[i]->GetXaxis()->SetNdivisions(15);
        hitmap[i]->GetXaxis()->SetLabelSize(0.08);
        hitmap[i]->GetXaxis()->SetTitleSize(0.08);
        hitmap[i]->GetYaxis()->SetLabelSize(0.08);
        // gStyle->SetOptStat(111111);
        hitmap[i]->SetStats(0);
        // draw chip_id on the hist
        for (int i1 = 0; i1 < 26; i1++) {
            TLatex* tex = new TLatex();
            double x = i1 < 13 ? 12.25 - i1 : 12.25 - i1 + 13;
            double y = i1 < 13 ? -5 : 5;
            tex->DrawLatex(x, y, Form("Chip%d", i1 + 1));
        }


        c4->Print( Form("%s/PostTrack_noise_hitmap.pdf",folder_direction.Data()) );
        
        c4->Clear();
    }

    c4->cd();
    c4->Print( Form("%s/PostTrack_noise_hitmap.pdf)",folder_direction.Data()) );

    c4 -> Clear();

}

// note : the following functions are for making plots after the multi-track removal.
void post_multi_noise_distribution (vector<noise_event_str> input_noise_vec, TString folder_direction)
{

    TCanvas * c1 = new TCanvas("c1","c1",800,800);
    c1 -> cd();
    c1 -> SetLogy();

    TH1F * noise_hist_all = new TH1F ("","",20,0-3,20-3); // note : combine the noise of three layers
    noise_hist_all -> SetLineColor( TColor::GetColor("#1A3947") );
    noise_hist_all -> SetLineWidth(3);
    noise_hist_all -> SetTitle(Form("(mul-track removal) Run52, noise level, layer combined"));
    noise_hist_all -> GetXaxis() -> SetTitle("# of the noise hit");
    noise_hist_all -> GetYaxis() -> SetTitle("Entry");

    TH1F * noise_hist_each[3]; // note : all three layers
    for (int i = 0; i < 3; i++)
    {
        noise_hist_each[i] = new TH1F ("","",20,0-3,20-3);
        noise_hist_each[i] -> SetLineColor( TColor::GetColor("#1A3947") );
        noise_hist_each[i] -> SetLineWidth(3);
        noise_hist_each[i] -> SetTitle(Form("(mul-track removal) Run52, noise level, layer %i",i));
        noise_hist_each[i] -> GetXaxis() -> SetTitle("# of the noise hit");
        noise_hist_each[i] -> GetYaxis() -> SetTitle("Entry");
    }

    int noise_counting_l0 = 0;
    int noise_counting_l1 = 0;
    int noise_counting_l2 = 0;
    
    for (int i = 0; i < input_noise_vec.size(); i++)
    {
        noise_hist_all -> Fill(input_noise_vec[i].noise_event.size());

        for (int i1 = 0; i1 < input_noise_vec[i].noise_event.size(); i1++ )
        {
            if ( input_noise_vec[i].noise_event[i1].noise_layer == 0 )
            {
                noise_counting_l0 += 1;
            }
            else if ( input_noise_vec[i].noise_event[i1].noise_layer == 1 )
            {
                noise_counting_l1 += 1;
            }
            else if ( input_noise_vec[i].noise_event[i1].noise_layer == 2 )
            {
                noise_counting_l2 += 1;
            }
        }

        noise_hist_each[0] -> Fill(noise_counting_l0);
        noise_hist_each[1] -> Fill(noise_counting_l1);
        noise_hist_each[2] -> Fill(noise_counting_l2);

        noise_counting_l0 = 0;
        noise_counting_l1 = 0; 
        noise_counting_l2 = 0;
    }

    // note : print the bincontent 
    for (int i = 0; i < noise_hist_all -> GetNbinsX(); i++)
    {
        cout<<"(tracks removal) layer combined, low edge : " << noise_hist_all -> GetBinLowEdge(i+1) <<" entry : "<<noise_hist_all -> GetBinContent(i+1)<<endl;
    }
    
    for (int i = 0; i < 3; i++)
    {
        cout<<"======= ======= ======= ======= ======= ======="<<endl;
        for (int i1 = 0; i1 < noise_hist_each[i] -> GetNbinsX(); i1++)
        {
            cout<<"(tracks removal) layer "<< i <<", low edge : " << noise_hist_each[i] -> GetBinLowEdge(i1+1) <<" entry : "<<noise_hist_each[i] -> GetBinContent(i1+1)<<endl;
        }   
    }


    // note : Draw the plot
    c1 -> cd();
    noise_hist_all -> Draw("hist");
    c1 -> Print( Form("%s/PostTrack_noise_level_all.pdf(", folder_direction.Data()) );
    c1 -> Clear();

    for (int i = 0; i < 3; i++)
    {
        noise_hist_each[i] -> Draw("hist");
        c1 -> Print( Form("%s/PostTrack_noise_level_all.pdf", folder_direction.Data()) );
        c1 -> Clear();
    }
    c1 -> Print( Form("%s/PostTrack_noise_level_all.pdf)", folder_direction.Data()) );

    c1 -> Clear();

}

void post_multi_N_track_chip (multi_track_str input_vec, TString folder_direction)
{
    TCanvas * c1 = new TCanvas("c1","c1",800,800);
    c1 -> cd();
    // c1 -> SetLogy();

    int chip_id[13] = {1,2,3,4,5,6,7,8,9,10,11,12,13};

    TGraph * grr = new TGraph(13,chip_id,&input_vec.track_chip_count[0]);
    
    grr -> SetMarkerStyle(20);
    grr -> SetMarkerSize(2);
    grr -> SetMarkerColor(2);
    
    grr -> SetTitle("(multrack) Number of tracks, chip dependency");
    grr -> GetXaxis() -> SetTitle("Chip ID");
    grr -> GetYaxis() -> SetTitle("# of good track");

    grr -> Draw("apl");

    c1 -> Print( Form("%s/PostTrack_removal_N_track_chip.pdf",folder_direction.Data()) );
    c1 -> Clear();
}

void post_multi_noise_adc_dist (vector<noise_event_str> input_noise_vec, TString folder_direction)
{

    TCanvas * c1 = new TCanvas("c1","c1",800,800);
    c1 -> cd();
    // c1 -> SetLogy();

    TH1F * noise_adc_hist = new TH1F("","",50,0,500);

    noise_adc_hist -> SetLineColor( TColor::GetColor("#1A3947") );
    noise_adc_hist -> SetLineWidth(3);
    noise_adc_hist -> SetTitle("Noise adc distribution (#delta-ray electron + noise hit)");
    noise_adc_hist -> GetXaxis() -> SetTitle("adc value");
    noise_adc_hist -> GetYaxis() -> SetTitle("Entry");

    cout<<"noise_dat_dist : "<<input_noise_vec.size()<<endl;

    for (int i = 0; i < input_noise_vec.size(); i++)
    {
        for (int i1 = 0; i1 < input_noise_vec[i].noise_event.size(); i1++ )
        {
            noise_adc_hist -> Fill( input_noise_vec[i].noise_event[i1].noise_adc );
        }
        
    }

    noise_adc_hist -> Draw("hist");

    c1 -> Print( Form("%s/noise_hit_adc.pdf", folder_direction.Data()) );
    c1 -> Clear();

}


// note : the study of the correlation of double tracks
void di_track_correlation (vector<vector<pol1_info>> input_slope_info_vec, TString folder_direction)
{

    TCanvas * c2 = new TCanvas("c2","c2",800,800);
    c2 -> cd();
    // c2 -> SetLogy();

    TH1F * corrleation_hist = new TH1F("","",100,-0.4,0.4);

    corrleation_hist -> SetLineColor( TColor::GetColor("#1A3947") );
    corrleation_hist -> SetLineWidth(3);
    corrleation_hist -> SetTitle("Track A slpe - Track B slope");
    corrleation_hist -> GetXaxis() -> SetTitle("A.U.");
    corrleation_hist -> GetYaxis() -> SetTitle("Entry");

    cout<<"Slope info vec : "<<input_slope_info_vec.size()<<endl;

    for (int i = 0; i < input_slope_info_vec.size(); i++)
    {
        if ( input_slope_info_vec[i].size() == 2 ) // note : 2 good tracks
        {
            corrleation_hist -> Fill(input_slope_info_vec[i][0].slope - input_slope_info_vec[i][1].slope);
        }        
    }

    corrleation_hist -> Draw("hist");

    c2 -> Print( Form("%s/ditrack_slope_correlation.pdf", folder_direction.Data()) );
    c2 -> Clear();

}


void noise_distribution (vector<noise_event_str> input_noise_vec, TString folder_direction, int study_chip)
{

    TCanvas * c1 = new TCanvas("c1","c1",800,800);
    c1 -> cd();
    c1 -> SetLogy();

    TH1F * noise_hist_all = new TH1F ("","",20,0-3,20-3); // note : combine the noise of three layers
    noise_hist_all -> SetLineColor( TColor::GetColor("#1A3947") );
    noise_hist_all -> SetLineWidth(3);
    noise_hist_all -> SetTitle(Form("Run52, noise level, chip %i, layer combined",study_chip));
    noise_hist_all -> GetXaxis() -> SetTitle("# of the noise hit");
    noise_hist_all -> GetYaxis() -> SetTitle("Entry");

    TH1F * noise_hist_each[3]; // note : all three layers
    for (int i = 0; i < 3; i++)
    {
        noise_hist_each[i] = new TH1F ("","",20,0-3,20-3);
        noise_hist_each[i] -> SetLineColor( TColor::GetColor("#1A3947") );
        noise_hist_each[i] -> SetLineWidth(3);
        noise_hist_each[i] -> SetTitle(Form("Run52, noise level, chip %i, layer %i",study_chip,i));
        noise_hist_each[i] -> GetXaxis() -> SetTitle("# of the noise hit");
        noise_hist_each[i] -> GetYaxis() -> SetTitle("Entry");
    }

    int noise_counting_l0 = 0;
    int noise_counting_l1 = 0;
    int noise_counting_l2 = 0;
    
    for (int i = 0; i < input_noise_vec.size(); i++)
    {
        noise_hist_all -> Fill(input_noise_vec[i].noise_event.size());

        for (int i1 = 0; i1 < input_noise_vec[i].noise_event.size(); i1++ )
        {
            if ( input_noise_vec[i].noise_event[i1].noise_layer == 0 )
            {
                noise_counting_l0 += 1;
            }
            else if ( input_noise_vec[i].noise_event[i1].noise_layer == 1 )
            {
                noise_counting_l1 += 1;
            }
            else if ( input_noise_vec[i].noise_event[i1].noise_layer == 2 )
            {
                noise_counting_l2 += 1;
            }
        }

        noise_hist_each[0] -> Fill(noise_counting_l0);
        noise_hist_each[1] -> Fill(noise_counting_l1);
        noise_hist_each[2] -> Fill(noise_counting_l2);

        noise_counting_l0 = 0;
        noise_counting_l1 = 0; 
        noise_counting_l2 = 0;
    }

    // note : print the bincontent 
    for (int i = 0; i < noise_hist_all -> GetNbinsX(); i++)
    {
        cout<<"layer combined, low edge : " << noise_hist_all -> GetBinLowEdge(i+1) <<" entry : "<<noise_hist_all -> GetBinContent(i+1)<<endl;
    }
    
    for (int i = 0; i < 3; i++)
    {
        cout<<"======= ======= ======= ======= ======= ======="<<endl;
        for (int i1 = 0; i1 < noise_hist_each[i] -> GetNbinsX(); i1++)
        {
            cout<<"layer "<< i <<", low edge : " << noise_hist_each[i] -> GetBinLowEdge(i1+1) <<" entry : "<<noise_hist_each[i] -> GetBinContent(i1+1)<<endl;
        }   
    }


    // note : Draw the plot
    c1 -> cd();
    noise_hist_all -> Draw("hist");
    c1 -> Print( Form("%s/noise_level_U%i_all.pdf(", folder_direction.Data(),study_chip) );
    c1 -> Clear();

    for (int i = 0; i < 3; i++)
    {
        noise_hist_each[i] -> Draw("hist");
        c1 -> Print( Form("%s/noise_level_U%i_all.pdf", folder_direction.Data(),study_chip) );
        c1 -> Clear();
    }
    c1 -> Print( Form("%s/noise_level_U%i_all.pdf)", folder_direction.Data(),study_chip) );

    c1 -> Clear();

}

void noise_hitmap (vector<noise_event_str> input_noise_vec, TString folder_direction)
{

    TCanvas *c4 = new TCanvas("c4", "noise_hitmap", 0, 700, 1625, 250);
    TH2D* hitmap[3];
    for (int i = 0; i < 3; i++)
    {
        hitmap[i] = new TH2D("hitmap", "", 13, 0, 13, 200, -10.5, 10.5);
    }

    c4->cd();
    c4->Print( Form("%s/noise_hitmap.pdf(",folder_direction.Data()) );

    for (int i = 0; i < input_noise_vec.size(); i++)
    {
        for (int i1 = 0; i1 < input_noise_vec[i].noise_event.size(); i1++)
        {
            int chip_conversion = -input_noise_vec[i].noise_event[i1].noise_chip + (1 + int(input_noise_vec[i].noise_event[i1].noise_chip / 14)) * 13;

            hitmap[ input_noise_vec[i].noise_event[i1].noise_layer ] -> Fill( chip_conversion, input_noise_vec[i].noise_event[i1].noise_pos );
        }
        
    }

    for (int i = 0; i < 3; i++)
    {
        hitmap[i]->Draw("colz");
    
        hitmap[i]->GetXaxis()->SetNdivisions(15);
        hitmap[i]->GetXaxis()->SetLabelSize(0.08);
        hitmap[i]->GetXaxis()->SetTitleSize(0.08);
        hitmap[i]->GetYaxis()->SetLabelSize(0.08);
        // gStyle->SetOptStat(111111);
        hitmap[i]->SetStats(0);
        // draw chip_id on the hist
        for (int i1 = 0; i1 < 26; i1++) {
            TLatex* tex = new TLatex();
            double x = i1 < 13 ? 12.25 - i1 : 12.25 - i1 + 13;
            double y = i1 < 13 ? -5 : 5;
            tex->DrawLatex(x, y, Form("Chip%d", i1 + 1));
        }


        c4->Print( Form("%s/noise_hitmap.pdf",folder_direction.Data()) );
        
        c4->Clear();
    }

    c4->cd();
    c4->Print( Form("%s/noise_hitmap.pdf)",folder_direction.Data()) );

    c4 -> Clear();

}

void set_hist_par (TH1F * hist, TString color_code, TString title, TString X_name, TString Y_name)
{
    hist -> SetLineColor( TColor::GetColor(color_code) );
    hist -> SetLineWidth(3);
    hist -> SetTitle(title);
    hist -> GetXaxis() -> SetTitle(X_name);
    hist -> GetYaxis() -> SetTitle(Y_name);
} 

// note : GT stands for good track
void cluster_size_GT_dist ( vector<cluster_reformat_str> input_cluster_vec, vector<profile_str> input_profile_vec, int study_chip, TString folder_direction )
{
    TCanvas * c1 = new TCanvas("c1","c1",800,800);
    c1 -> cd();
    c1 -> SetLogy();

    TH1F * hist[3];

    for (int i = 0; i < 3; i++)
    {
        hist[i] = new TH1F ("","",20,0-3,20-3);
        hist[i] -> SetLineColor( TColor::GetColor("#1A3947") );
        hist[i] -> SetLineWidth(3);
        hist[i] -> SetTitle(Form("Run52, cluster size, U%i good track, layer : %i",study_chip,i));
        hist[i] -> GetXaxis() -> SetTitle("Cluster size");
        hist[i] -> GetYaxis() -> SetTitle("Entry");
    }

    int good_track_count = 0;

    for (int i = 0; i < input_profile_vec.size(); i++)
    {
        if (input_profile_vec[i].profile == 14)
        {
            if (input_profile_vec[i].profile_eID == input_cluster_vec[good_track_count].eID)
            {
                hist[0] -> Fill( input_cluster_vec[good_track_count].cluster_hit[study_chip - 1][0][input_profile_vec[i].clusterID_0] ); // note : layer 0
                hist[1] -> Fill( input_cluster_vec[good_track_count].cluster_hit[study_chip - 1][1][input_profile_vec[i].clusterID_1] ); // note : layer 1
                hist[2] -> Fill( input_cluster_vec[good_track_count].cluster_hit[study_chip - 1][2][input_profile_vec[i].clusterID_2] ); // note : layer 2
            }
            else 
            {
                cout<<" weird, the eID doesn't match, the eID of profile, in \"cluster_size_GT_dist\" : "<<input_profile_vec[i].profile_eID<<endl;
            }

            good_track_count += 1;
        }
    }

    c1 -> Print( Form("%s/cluster_size_dist_GT.pdf(",folder_direction.Data()) );
    for (int i = 0; i < 3; i++)
    {
        hist[i] -> Draw("hist");
        c1 -> Print( Form("%s/cluster_size_dist_GT.pdf",folder_direction.Data()) );
    }
    c1 -> Print( Form("%s/cluster_size_dist_GT.pdf)",folder_direction.Data()) );

    c1 -> Clear();

    
}


// note : to draw the cluster size distribution.
void cluster_size_all_dist (vector<cluster_str> input_cluster_vec, TString folder_direction)
{
    TCanvas * c1 = new TCanvas("c1","c1",800,800);
    c1 -> cd();
    c1 -> SetLogy();

    TH1F * hist[3];

    for (int i = 0; i < 3; i++)
    {
        hist[i] = new TH1F ("","",20,0-3,20-3);
        hist[i] -> SetLineColor( TColor::GetColor("#1A3947") );
        hist[i] -> SetLineWidth(3);
        hist[i] -> SetTitle(Form("Run52, cluster size, all clusters, layer : %i",i));
        hist[i] -> GetXaxis() -> SetTitle("Cluster size");
        hist[i] -> GetYaxis() -> SetTitle("Entry");
    }

    for (int i = 0; i < input_cluster_vec.size(); i++)
    {
        for (int i1 = 0; i1 < input_cluster_vec[i].cluster_layer.size(); i1++)
        {
            hist[ input_cluster_vec[i].cluster_layer[i1] ] -> Fill( input_cluster_vec[i].cluster_hit[i1] );
        }
    }

    c1 -> Print( Form("%s/cluster_size_dist_all.pdf(",folder_direction.Data()) );
    for (int i = 0; i < 3; i++)
    {
        hist[i] -> Draw("hist");
        c1 -> Print( Form("%s/cluster_size_dist_all.pdf",folder_direction.Data()) );
    }
    c1 -> Print( Form("%s/cluster_size_dist_all.pdf)",folder_direction.Data()) );

    c1 -> Clear();

}



// note : to draw the cluster size distribution. and compare
void cluster_size_all_dist_compare (vector<cluster_str> input_cluster_vec, vector<cluster_str> input_cluster_vec_compare, TString folder_direction, TString run_ID, bool linear_or_log, bool statsbox_bool)
{
    TCanvas * c1 = new TCanvas("c1","c1",800,800);
    c1 -> cd();

    // note : 0) is for SetGrid
    // note : 0,0) is for logY
    TPad *pad_obj = new TPad(Form("pad_obj"), "", 0.0, 0.30, 1.0, 1.0);
    Characterize_Pad(pad_obj, 0.15, 0.05, 0.1, 0.005, 0, 0);
    pad_obj -> Draw();

    TPad *pad_ratio = new TPad(Form("pad_ratio"), "", 0.0, 0.0, 1.0, 0.30);
    Characterize_Pad(pad_ratio, 0.15, 0.05, 0.02, 0.350, 0, 1);
    pad_ratio -> Draw();
    

    TH1F * hist_data[3];

    TH1F * hist_MC[3];

    TH1F *hist_ratio[3];



    TLegend *legend1 = new TLegend (0.5, 0.7, 0.75, 0.85);
	legend1 -> SetTextSize (0.050);
	// legend1 -> SetNColumns (4);

    TString rest_plot_name;
    if (linear_or_log == true) // note : linear
    {
        rest_plot_name = "cluster_size_dist_comp_all_linear";
    }
    else if (linear_or_log == false) // note : log
    {   
        pad_obj -> SetLogy();
        rest_plot_name = "cluster_size_dist_comp_all_log";
    }

    for (int i = 0; i < 3; i++)
    {
        hist_data[i] = new TH1F ("","",20,0-3,20-3);
        hist_data[i] -> SetLineColor( TColor::GetColor("#1A3947") );
        hist_data[i] -> SetLineWidth(3);
        hist_data[i] -> SetMarkerColor(TColor::GetColor("#1A3947"));
        hist_data[i] -> SetMarkerSize(1);
        hist_data[i] -> SetMarkerStyle(20);

        hist_data[i] -> GetXaxis() -> SetTitle("Cluster size");
        hist_data[i] -> GetYaxis() -> SetTitle("A.U.");
        // hist_data[i] -> GetYaxis() -> SetRangeUser(0,1);

        hist_MC[i] = new TH1F ("","",20,0-3,20-3);
        hist_MC[i] -> SetLineColor( TColor::GetColor("#A08144") );
        hist_MC[i] -> SetLineWidth(3);
        hist_MC[i] -> SetTitle(Form("%s, all clusters, layer-%i",run_ID.Data(),i));
        if (statsbox_bool == false) hist_MC[i] -> SetStats(0); // note : remove the box

        // hist_MC[i] -> SetTitle(Form("Run61, cluster size, all clusters, layer : %i",i));
        hist_MC[i] -> GetXaxis() -> SetTitle("Cluster size");
        hist_MC[i] -> GetYaxis() -> SetTitle("A.U.");
        hist_MC[i] -> GetYaxis() -> SetTitleSize   (0.06);
        hist_MC[i] -> GetYaxis() -> SetTitleOffset (0.70);
        hist_MC[i] -> GetYaxis() -> SetRangeUser(0,1);
    }

    legend1 -> AddEntry (hist_data[0], Form("%s, Data",run_ID.Data()),  "pl");
    legend1 -> AddEntry (hist_MC[0], Form("%s, MC",run_ID.Data()),  "f");

    for (int i = 0; i < input_cluster_vec.size(); i++)
    {
        for (int i1 = 0; i1 < input_cluster_vec[i].cluster_layer.size(); i1++)
        {
            hist_data[ input_cluster_vec[i].cluster_layer[i1] ] -> Fill( input_cluster_vec[i].cluster_hit[i1] );
        }
    }

    for (int i = 0; i < input_cluster_vec_compare.size(); i++)
    {
        for (int i1 = 0; i1 < input_cluster_vec_compare[i].cluster_layer.size(); i1++)
        {
            hist_MC[ input_cluster_vec_compare[i].cluster_layer[i1] ] -> Fill( input_cluster_vec_compare[i].cluster_hit[i1] );
        }
    }

    hist_data[0]->Scale(1./hist_data[0]->Integral(-1,-1));
    hist_data[1]->Scale(1./hist_data[1]->Integral(-1,-1));
    hist_data[2]->Scale(1./hist_data[2]->Integral(-1,-1));

    hist_MC[0]->Scale(1./hist_MC[0]->Integral(-1,-1));
    hist_MC[1]->Scale(1./hist_MC[1]->Integral(-1,-1));
    hist_MC[2]->Scale(1./hist_MC[2]->Integral(-1,-1));
    
    double Y_axis_max = (linear_or_log) ? 1.1 : 10;

    hist_MC[0] -> SetMaximum(Y_axis_max);
    hist_MC[1] -> SetMaximum(Y_axis_max);
    hist_MC[2] -> SetMaximum(Y_axis_max);

    c1 -> Print( Form("%s/%s_%s.pdf(",folder_direction.Data(),run_ID.Data(),rest_plot_name.Data()) );
    for (int i = 0; i < 3; i++)
    {
        hist_ratio[i] = (TH1F*)hist_data[i] -> Clone(); 
        hist_ratio[i] -> Divide (hist_MC[i]);

        Characterize_Rate1F(hist_ratio[i],1);
        
        pad_obj -> cd();
        hist_MC[i] -> Draw("hist");
        hist_data[i] -> Draw("ep same");
        legend1 -> Draw("same");

        pad_ratio -> cd();
        hist_ratio[i] -> Draw("ep");


        c1 -> Print( Form("%s/%s_%s.pdf",folder_direction.Data(),run_ID.Data(),rest_plot_name.Data()) );
    }
    c1 -> Print( Form("%s/%s_%s.pdf)",folder_direction.Data(),run_ID.Data(),rest_plot_name.Data()) );

    c1 -> Clear();

}





// // note : the v4 is based on the multi-track removal
// void noise_extraction_v4 ()
// {
//     // note : the setting pars
//     TString folder_direction = "/Users/chengweishi/Downloads/BeamTest_2021/noise_ana_data/multi_track";
//     TString file_name = "run52_no_clone_filter_all_int_1000";
//     TString cluster_file_name = "cluster_information_slope_adcinfo";
//     int study_chip = 8;

//     // vector<profile_str> profile_str_vec; profile_str_vec.clear();
//     // profile_str_vec = profile_read_and_build(folder_direction, file_name, cluster_file_name, study_chip);

//     vector<cluster_str> cluster_str_vec; cluster_str_vec.clear();
//     cluster_str_vec = cluster_read_and_build(folder_direction, file_name, cluster_file_name, study_chip);   

//     vector<cluster_reformat_str> cluster_reformat_str_vec; cluster_reformat_str_vec.clear();
//     cluster_reformat_str_vec = cluster_reformat(cluster_str_vec);

//     // note : multi-track removal 
//     multi_track_str noise_vec_post_multi;
//     noise_vec_post_multi = multiple_track_removal(cluster_reformat_str_vec);

//     // note : make the plots 
//     // post_multi_N_track(noise_vec_post_multi,folder_direction);
//     // post_multi_noise_hitmap(noise_vec_post_multi.noise_vec,folder_direction);
//     // post_multi_noise_distribution(noise_vec_post_multi.noise_vec,folder_direction);
//     // post_multi_N_track_chip(noise_vec_post_multi,folder_direction);
//     // post_multi_noise_adc_dist(noise_vec_post_multi.noise_vec,folder_direction);
//     // di_track_correlation(noise_vec_post_multi.track_fit_info_vec,folder_direction);

//     post_multi_N_track_extrapolation(noise_vec_post_multi,folder_direction);

//     // vector<cluster_reformat_str> cluster_match_vec; cluster_match_vec.clear();
//     // cluster_match_vec = cluster_eID_match(cluster_reformat_str_vec, profile_str_vec);



    
//     // // note : make the plot
//     // noise_distribution(noise_info_vec,folder_direction,study_chip);
//     // noise_hitmap(noise_info_vec,folder_direction);
//     // cluster_size_all_dist(cluster_str_vec,folder_direction);
//     // cluster_size_GT_dist(cluster_match_vec,profile_str_vec,study_chip,folder_direction);

//     // // note : multi-track removal 
//     // multi_track_str noise_vec_post_multi;
//     // noise_vec_post_multi = multiple_track_removal(cluster_match_vec,profile_str_vec,study_chip);
    
//     // // note : draw plot, post-track removal
//     // post_multi_N_track(noise_vec_post_multi,study_chip,folder_direction);
//     // post_multi_noise_distribution(noise_vec_post_multi.noise_vec,folder_direction,study_chip);
//     // post_multi_noise_hitmap(noise_vec_post_multi.noise_vec,folder_direction);


//     // // note : for the test 
//     // for (int i = 0; i < noise_vec_post_multi.noise_vec.size(); i++)
//     // {
//     //     if (noise_vec_post_multi.noise_vec[i].eID == 64018)
//     //     {
//     //         cout<<"eID : "<<noise_vec_post_multi.noise_vec[i].eID<<" "<<noise_vec_post_multi.noise_vec[i].noise_event.size()<<endl;
//     //     }
//     // }



// }