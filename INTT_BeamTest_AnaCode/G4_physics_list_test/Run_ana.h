#ifndef Run_ana_h
#define Run_ana_h

struct hit_info {
    int PID_order;
    int PID;
    double PID_energy;
    double particle_X;
    double particle_Y;
    double particle_Z;
    int eID;
    int pre_process_ID;
    int post_process_ID;
};

class data_process
{
    public : 
        data_process(TString class_name, TString folder_direction, TString file_name);

        vector<vector<hit_info>> file_read_and_build ();
        vector<vector<hit_info>> get_file_read_and_build ();
        TH1F * get_scattering_distribution (int hit_cut, int scattering_option, int SD_Nbins, double SD_Ledge, double SD_Redge, vector<vector<hit_info>> event_info_predefine);
        TString GetClassName();



        // note : declare the parameters
        TString class_name;
        TString folder_direction;
        TString file_name;
        vector<vector<hit_info>> event_info_predefine;
        int SD_Nbins;
        double SD_Ledge;
        double SD_Redge;

        // note : for the scattering distribution
        int hit_cut;
        int scattering_option;
};


#endif