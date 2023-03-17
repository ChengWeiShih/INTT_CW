#include "Run_ana.h"

data_process::data_process(TString class_name, TString folder_direction, TString file_name)
:   class_name(class_name),
    folder_direction(folder_direction),
    file_name(file_name)
{
    cout<<"============================================================================="<<endl;
    cout<<"the class : "<<class_name<<" has been initiated by using the mode 1 "<<endl;
    cout<<"============================================================================="<<endl;

    event_info_predefine = data_process::file_read_and_build();
}

vector<vector<hit_info>> data_process::file_read_and_build()
{
    TFile* hit_file = TFile::Open (Form("%s/%s.root", folder_direction.Data(),file_name.Data()), "read");
    TTree *event_particle_tree = (TTree *)hit_file->Get("event_particle");
    int event_particle_size = event_particle_tree->GetEntriesFast();
    cout<<"Tree event_particle size : "<<event_particle_size<<endl;

    int PID_order;
    int PID;
    double PID_energy;
    double particle_X;
    double particle_Y;
    double particle_Z;
    int eID;
    int pre_process_ID;
    int post_process_ID;
    
    event_particle_tree -> SetBranchAddress ("PID_order",&PID_order);
    event_particle_tree -> SetBranchAddress ("PID",&PID);
    event_particle_tree -> SetBranchAddress ("PID_energy",&PID_energy);
    event_particle_tree -> SetBranchAddress ("particle_X",&particle_X);
    event_particle_tree -> SetBranchAddress ("particle_Y",&particle_Y);
    event_particle_tree -> SetBranchAddress ("particle_Z",&particle_Z);
    event_particle_tree -> SetBranchAddress ("eID",&eID);
    event_particle_tree -> SetBranchAddress ("pre_process",&pre_process_ID);
    event_particle_tree -> SetBranchAddress ("post_process",&post_process_ID);

    hit_info single_hit_info;
    vector<hit_info> hit_info_in_event; hit_info_in_event.clear();
    vector<vector<hit_info>> hit_info_all; hit_info_all.clear();

    event_particle_tree -> GetEntry(0);
    int on_time_eID = eID;

    for (int i = 0; i < event_particle_size; i++)
    {

        if(i%100000 == 0)
		{
			cout<<"processing : "<<i<<endl;
		}

        event_particle_tree -> GetEntry(i);

        if (on_time_eID == eID)
        {
            single_hit_info.PID_order = PID_order;
            single_hit_info.PID = PID;
            single_hit_info.PID_energy = PID_energy;
            single_hit_info.particle_X = particle_X;
            single_hit_info.particle_Y = particle_Y;
            single_hit_info.particle_Z = particle_Z;
            single_hit_info.eID = eID;
            single_hit_info.pre_process_ID = pre_process_ID;
            single_hit_info.post_process_ID = post_process_ID;      

            hit_info_in_event.push_back(single_hit_info);
        }
        else
        {

            on_time_eID = eID;

            hit_info_all.push_back(hit_info_in_event);
            hit_info_in_event.clear();

            single_hit_info.PID_order = PID_order;
            single_hit_info.PID = PID;
            single_hit_info.PID_energy = PID_energy;
            single_hit_info.particle_X = particle_X;
            single_hit_info.particle_Y = particle_Y;
            single_hit_info.particle_Z = particle_Z;
            single_hit_info.eID = eID;
            single_hit_info.pre_process_ID = pre_process_ID;
            single_hit_info.post_process_ID = post_process_ID;   

            hit_info_in_event.push_back(single_hit_info);
        }

    }

    hit_info_all.push_back(hit_info_in_event);
    hit_info_in_event.clear();

    return hit_info_all;
}

vector<vector<hit_info>> data_process::get_file_read_and_build()
{
    return event_info_predefine;
}

TString data_process::GetClassName()
{   
    return class_name;
}

TH1F * data_process::get_scattering_distribution (int hit_cut, int scattering_option, int SD_Nbins, double SD_Ledge, double SD_Redge, vector<vector<hit_info>> event_info_predefine)
{

    TH1F * scattering_dist = new TH1F("","",SD_Nbins,SD_Ledge,SD_Redge);
    cout<<"============================================================================="<<endl;
    if      (scattering_option == 0) {cout<<"now is scattering in X axis "<<endl;}
    else if (scattering_option == 1) {cout<<"now is scattering in Y axis "<<endl;}
    else if (scattering_option == 2) {cout<<"now is scattering in R  "<<endl;}
    cout<<"============================================================================="<<endl;

    for (int i = 0; i < event_info_predefine.size(); i++) // note : event
    {
        // cout<<"===================================================="<<endl;
        if (event_info_predefine[i].size() < hit_cut)
        {
            for (int i1 = 0; i1 < event_info_predefine[i].size(); i1++) // note : hit in event
            {
                if (scattering_option == 0) // note : X
                {
                    scattering_dist -> Fill(event_info_predefine[i][i1].particle_X);
                       
                }
                else if (scattering_option == 1) 
                {
                    scattering_dist -> Fill(event_info_predefine[i][i1].particle_Y);
                }
                else if ( scattering_option ==2 )
                {
                    scattering_dist -> Fill( sqrt( pow(event_info_predefine[i][i1].particle_X,2) + pow(event_info_predefine[i][i1].particle_Y,2) ) );
                }
                else if ( scattering_option == 3 )
                {
                    scattering_dist -> Fill(event_info_predefine[i][i1].particle_Y / 20.);
                }
                // cout<<"PID order : "<<event_info_predefine[i][i1].PID_order<<" PID : "<<event_info_predefine[i][i1].PID<<" energy : "<<event_info_predefine[i][i1].PID_energy<<" X : "<<event_info_predefine[i][i1].particle_X<<" Y : "<<event_info_predefine[i][i1].particle_Y<<" Z : "<<event_info_predefine[i][i1].particle_Z<<" eID : "<<event_info_predefine[i][i1].eID<<" pre proc : "<<event_info_predefine[i][i1].pre_process_ID<<" post proc : "<<event_info_predefine[i][i1].post_process_ID<<endl;

            }   
        }

    }

    return scattering_dist;
}