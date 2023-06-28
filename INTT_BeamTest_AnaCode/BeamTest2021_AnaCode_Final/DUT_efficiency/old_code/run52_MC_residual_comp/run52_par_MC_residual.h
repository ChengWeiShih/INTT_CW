TString color_code [8]={"#343434","#1A3947","#566575","#797983","#EFBD9D","#FCA26E","#F5751D","#F5321D"};
TString color_code_2 [8]={"#CC768D","#19768D","#DDA573","#009193","#6E9193","#941100","#A08144","#517E66"};

double INTT_strip_width = 0.078;
double lower_section_initial = -9.945;
double upper_section_initial = 0.039;

// note : decided not to change these two cuts.
int cluster_size_requirement_combine = 1; // note : " > the_value" "to allow the combination to be 2+1"
int cluster_size_requirement = 0; // note : the cluster size of single cluster, // note : " > the_value" 
int hit_allowed_in_adjacent_layers = 1; // note : " == the_value "

// note : the actual ladder position (unit : mm), the value is no longer to be 26.1, 
// note : since 26.1 is the gap between ladders, without the consideration of ladder thickness
double actual_xpos [3] = {0,29.552,59.104};

bool file_generated = true; // note : this is the protection key, to avoid re-generate the scan file

// note ============== ============== ============== we may change the following ============== ============== ==============

// todo : changed
TString folder_direction = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/MC_data_residual_comp/folder_e+_934MeV_L1offset_1M_filter_all_int_1000_chip_9";
TString file_name = "not_used"; // note : not used
TString cluster_file_name = "cluster_information_offset-0.0000_adcinfo_NoCamac";
 

// note : official, Column 8
int study_chip = 11;
double amount_of_alignment = -0.3062;
double slope_correction = 0.; // note : to correct the slope distribution, easy to apply the cut

// note : for test
// double amount_of_alignment = -0.;
// double slope_correction = 0.001712;

// todo : changed
double slope_cut = 0.01; // note : the parameter for the DUT 

double cluster_adc_value_requirement = 22.5; // note : " > the_value"

//todo : 
double noise_hit_distance = 0.234; // note : currently, the width of three channels

// todo : changed
int boundary_cut = 5; 

// todo : changed
// note : the scan range for the systematic uncertainty
double residual_scan[2]= {0.1632,0.305};
double slope_scan[2]   = {0.0087,0.0113};
double edge_scan[2]    = {-1,11};

// note : this is for the effi-pos plot, to mention the beam spot region
double beam_spot_range[2] = {-7., 6.};
double effi_pos_legend_offset = 0.;