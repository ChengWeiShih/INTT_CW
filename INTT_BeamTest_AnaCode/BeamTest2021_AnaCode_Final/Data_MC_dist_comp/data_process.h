#ifndef data_process_h
#define data_process_h

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

    vector<double> passed_middle_pos;
    vector<int> final_event_result;
};

// note : ======================================== for the N track study ======================================== 
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
// note : ======================================== for the N track study ========================================



TString color_code [8]={"#343434","#1A3947","#566575","#797983","#EFBD9D","#FCA26E","#F5751D","#F5321D"};
TString color_code_2 [8]={"#CC768D","#19768D","#DDA573","#009193","#6E9193","#941100","#A08144","#517E66"};

double INTT_strip_width = 0.078;
double lower_section_initial = -9.945;
double upper_section_initial = 0.039;

// note : the actual ladder position (unit : mm), the value is no longer to be 26.1, 
// note : since 26.1 is the gap between ladders, without the consideration of ladder thickness
double actual_xpos [3] = {0,29.552,59.104};
double distance_to_sci = 183.5; // note : the scintillator position, calculation based on MC 2023/02/08.


class data_process
{
    public :
        
        data_process(TString class_name, TString folder_direction, TString cluster_file_name, int study_chip, double amount_of_alignment, double slope_correction);
        
        // note : merge case
        data_process(int Initialize_mode, TString class_name, TString folder_direction, TString cluster_file_name, int study_chip, double amount_of_alignment, double slope_correction);

        // note : for MC
        data_process(TString class_name,TString folder_direction, TString MC_raw_file_name, TString cluster_file_name, int study_chip, double amount_of_alignment, double slope_correction);

        void MC_cluster_file_generator ();

        vector<cluster_str> cluster_read_and_build ();
        vector<cluster_reformat_str> cluster_reformat ( vector<cluster_str> input_vec ); // note : L1 "single" column offset correction
        vector<cluster_reformat_str> cluster_reformat_all_column_correct ( vector<cluster_str> input_vec ); // note : L1 "all columns" offset correction
        TH1F * Get_scattering_dist (int SD_Nbins, double SD_Ledge, double SD_Redge, vector<cluster_reformat_str> input_vec);
        TH1F * Get_residual_dist (int SD_Nbins, double SD_Ledge, double SD_Redge, vector<cluster_reformat_str> input_vec);
        TH1F * Get_alignment_dist (int SD_Nbins, double SD_Ledge, double SD_Redge, vector<cluster_reformat_str> input_vec);
        TH1F * Get_track_candidate_slope (int SD_Nbins, double SD_Ledge, double SD_Redge, vector<cluster_reformat_str> input_vec);
        vector <TH1F *> Get_cluster_size (bool with_cut, int SD_Nbins, double SD_Ledge, double SD_Redge, vector<cluster_reformat_str> input_vec);
        vector <TH1F *> Get_cluster_adc (bool with_cut, int SD_Nbins, double SD_Ledge, double SD_Redge, vector<cluster_reformat_str> input_vec);

        multi_track_str Get_Ntrack (vector<cluster_reformat_str> input_vec, bool print_all_track);
        TH1F * Get_Ntrack_extrapolation_dist (int SD_Nbins, double SD_Ledge, double SD_Redge, multi_track_str input_vec);
        TH1F * Get_Ntrack_dist(int SD_Nbins, double SD_Ledge, double SD_Redge, double track_extrapol_low, double track_extrapol_high, multi_track_str input_vec);

        // TH1F * residual_dist (vector<cluster_reformat_str> input_vec);
        
        void para_check();
        TString GetClassName();

        vector<cluster_str> get_cluster_read_and_build ();
        vector<cluster_reformat_str> get_cluster_reformat ();
        // TH1F * get_scattering_dist (int SD_Nbins, double SD_Ledge, double SD_Redge);



        // note : declare the parameters
        int Initialize_mode;
        TString class_name;

        // note : for the scattering_dist setting 
        int SD_Nbins;
        double SD_Ledge;
        double SD_Redge;
        bool with_cut;

        // note : for normal operation
        TString folder_direction;
        TString file_name;
        TString cluster_file_name;

        // note : directly from G4
        TString MC_raw_file_name;
        
        vector<cluster_str> input_vec;

        int study_chip;
        double amount_of_alignment;
        double slope_correction;
        
        double cluster_adc_value_requirement; // note : " > the_value"
        double noise_hit_distance; // note : currently, the width of three channels
        int boundary_cut; 
        double slope_cut;

        // note : decided not to change these two cuts.
        int cluster_size_requirement_combine; // note : " > the_value" "to allow the combination to be 2+1"
        int cluster_size_requirement; // note : the cluster size of single cluster, // note : " > the_value" 
        int hit_allowed_in_adjacent_layers; // note : " == the_value "

        // note : for the merge case
        vector<cluster_str> cluster_str_vec_predefined;
        vector<cluster_reformat_str> cluster_reformat_str_vec_predefined;

        // note : for the N track stuy
        bool print_all_track;
        double track_extrapol_low; 
        double track_extrapol_high;
};

// /data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency/run52_new_alignment


#endif