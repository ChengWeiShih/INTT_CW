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
// note : the setting pars
TString folder_direction = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency/run89_data_NoCamac";
TString file_name = "not_used"; // note : not used
TString cluster_file_name = "cluster_information_offset-0.2850_adcinfo_NoCamac";
int study_chip = 10; 

// todo : changed
double slope_cut = 0.01; // note : the parameter for the DUT 

// todo : changed
double slope_correction = 0.005269; // note : to correct the slope distribution. 

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