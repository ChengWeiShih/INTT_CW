// note : the features of the v4 version 
// note : the event doesn't have to have the good track in the first place. We do the tracking here
// note : all the good track can be removed
// note : in principle, the macro should work with the "cluster_info_slope_adc.root" (file made by the track_single_cluster_v2.C)

// note : we can have the adc information to have a better seapration on the delta-ray electron and the noise, 2022/11/05

// todo : find a way to improve the noise_hit_distance ? Right now what I am using now is from the second layer residual distribution of layer-1

TString color_code [8]={"#343434","#1A3947","#566575","#797983","#EFBD9D","#FCA26E","#F5751D","#F5321D"};
TString color_code_2 [8]={"#CC768D","#19768D","#DDA573","#009193","#6E9193","#941100","#A08144","#517E66"};
double noise_hit_distance = 0.1374; // note : 3 sigma of the l1 residual width
double actual_xpos [3] = {0,26.1,52.2}; // note : the actual ladder position (unit : mm)

double slope_cut = 0.4; // note : the parameter for the DUT

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



struct DUT_str{
    int track_111_count;
    int track_101_count;
    vector<double> middle_layer_residual; 
    vector<double> good_combination_slope; 
};

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
multi_track_str multiple_track_removal ( vector<cluster_reformat_str> input_cluster_vec )
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
                    cout<<"find the "<< multi_track_count <<"th track !! chip ID : "<< i1+1<<" eID : "<<input_cluster_vec[i].eID<<" position : "<<hit3_best_fit_picker_info[3]<<" "<<hit3_best_fit_picker_info[4]<<" "<<hit3_best_fit_picker_info[5]<<endl;

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

// note : this is the more advanced version of the N_track_distribution
// note : because some of the track may not from the beam
// note : what we can do is to check the extrapolation position, and to see whether the position is within the scintillator area
void post_multi_N_track_extrapolation (multi_track_str input_vec, TString folder_direction)
{
    TCanvas * c1 = new TCanvas("c1","c1",800,800);
    c1 -> cd();
    // c1 -> SetLogy();

    int good_tracking_counting = 0;

    double distance_to_sci = 183.5;

    TH1F * extra_pos_hist = new TH1F("","",100,-20,20);
    for (int i = 0; i < input_vec.track_fit_info_vec.size(); i++) // note : number of event which has at least one good track 
    {
        for ( int i1 = 0; i1 < input_vec.track_fit_info_vec[i].size(); i1++ ) // note : number of good track (criteria by fitting) in single event.
        {
            extra_pos_hist -> Fill( input_vec.track_fit_info_vec[i][i1].slope * distance_to_sci + input_vec.track_fit_info_vec[i][i1].offset );
        }

    }

    extra_pos_hist -> SetTitle("extrapolation position of the tracks");
    extra_pos_hist -> GetXaxis() -> SetTitle("position (mm)");
    extra_pos_hist -> GetYaxis() -> SetTitle("Entry");

    extra_pos_hist -> Draw("hist");

    c1 -> Print( Form("%s/PostTrack_track_extrapolation.pdf",folder_direction.Data()) );
    c1 -> Clear();
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
        hist[i] -> SetTitle(Form("Run61, cluster size, all clusters, layer : %i",i));
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
void cluster_size_all_dist_compare (vector<cluster_str> input_cluster_vec, vector<cluster_str> input_cluster_vec_compare, TString folder_direction)
{
    TCanvas * c1 = new TCanvas("c1","c1",800,800);
    c1 -> cd();

    TPad *pad = new TPad(Form("pad1"), "", 0.0, 0.0, 1.0, 1.0);
	//pad -> SetTopMargin(0.12);
	//pad -> SetBottomMargin(0.120);
	pad -> SetLeftMargin(0.15);
	pad -> SetRightMargin(0.05);
	//pad -> SetGrid(1, 1);
	pad -> Draw("same");
	pad -> cd();
    pad -> SetLogy();
    // c1 -> SetLogy();

    TH1F * hist[3];

    TH1F * hist_comp[3];

    TLegend *legend1 = new TLegend (0.5, 0.7, 0.75, 0.85);
	legend1 -> SetTextSize (0.030);
	// legend1 -> SetNColumns (4);

    for (int i = 0; i < 3; i++)
    {
        hist[i] = new TH1F ("","",20,0-3,20-3);
        hist[i] -> SetLineColor( TColor::GetColor("#A08144") );
        hist[i] -> SetLineWidth(3);
        hist[i] -> SetTitle(Form("Run61 comparison, all clusters, layer : %i",i));
        hist[i] -> GetXaxis() -> SetTitle("Cluster size");
        hist[i] -> GetYaxis() -> SetTitle("A.U.");

        hist_comp[i] = new TH1F ("","",20,0-3,20-3);
        hist_comp[i] -> SetLineColor( TColor::GetColor("#1A3947") );
        hist_comp[i] -> SetMarkerColor(TColor::GetColor("#1A3947"));
        hist_comp[i] -> SetMarkerSize(1);
        hist_comp[i] -> SetMarkerStyle(20);

        hist_comp[i] -> SetLineWidth(3);
        // hist_comp[i] -> SetTitle(Form("Run61, cluster size, all clusters, layer : %i",i));
        hist_comp[i] -> GetXaxis() -> SetTitle("Cluster size");
        hist_comp[i] -> GetYaxis() -> SetTitle("A.U.");
    }

    legend1 -> AddEntry (hist_comp[0], "Run61, Data",  "pl");
    legend1 -> AddEntry (hist[0], "Run61, MC",  "f");

    for (int i = 0; i < input_cluster_vec.size(); i++)
    {
        for (int i1 = 0; i1 < input_cluster_vec[i].cluster_layer.size(); i1++)
        {
            hist[ input_cluster_vec[i].cluster_layer[i1] ] -> Fill( input_cluster_vec[i].cluster_hit[i1] );
        }
    }

    for (int i = 0; i < input_cluster_vec_compare.size(); i++)
    {
        for (int i1 = 0; i1 < input_cluster_vec_compare[i].cluster_layer.size(); i1++)
        {
            hist_comp[ input_cluster_vec_compare[i].cluster_layer[i1] ] -> Fill( input_cluster_vec_compare[i].cluster_hit[i1] );
        }
    }

    hist[0]->Scale(1./hist[0]->Integral(-1,-1));
    hist[1]->Scale(1./hist[1]->Integral(-1,-1));
    hist[2]->Scale(1./hist[2]->Integral(-1,-1));

    hist_comp[0]->Scale(1./hist_comp[0]->Integral(-1,-1));
    hist_comp[1]->Scale(1./hist_comp[1]->Integral(-1,-1));
    hist_comp[2]->Scale(1./hist_comp[2]->Integral(-1,-1));
    

    c1 -> Print( Form("%s/cluster_size_dist_comp_all.pdf(",folder_direction.Data()) );
    for (int i = 0; i < 3; i++)
    {
        hist[i] -> Draw("hist");
        hist_comp[i] -> Draw("ep same");
        legend1 -> Draw("same");
        c1 -> Print( Form("%s/cluster_size_dist_comp_all.pdf",folder_direction.Data()) );
    }
    c1 -> Print( Form("%s/cluster_size_dist_comp_all.pdf)",folder_direction.Data()) );

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