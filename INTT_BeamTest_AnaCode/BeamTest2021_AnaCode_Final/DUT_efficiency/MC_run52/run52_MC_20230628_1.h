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
//note : 2023_06_24 the file is still here, but trying to organize all the files and the plots in one place
// TString folder_direction = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/DUT_efficiency/run52_new_alignment"; 
// TString folder_direction = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/BeamTest2021_data_MC_file_plot_final/MC_file/MC_run52/folder_run52_MC_run_20230628_1";
TString folder_direction = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/BeamTest2021_data_MC_file_plot_final/MC_file/MC_run52/folder_run52_MC_run_20230628_1/scattering_l0l1_attempt";

TString file_name = "not_used"; // note : not used
TString cluster_file_name = "cluster_information_offset-0.0000_adcinfo_SingleTrigger";
 

// note : official, Column 11
int study_chip = 11;
double amount_of_alignment = -0.3042;
// double slope_correction = 0.00157887; // note : to correct the slope distribution, easy to apply the cut
double slope_correction = 0.00235037; // note : slope correction for l0 and l1


// note : for test
// double amount_of_alignment = -0.;
// double slope_correction = 0.;

// todo : changed
double slope_cut = 0.01; // note : the parameter for the DUT 

double cluster_adc_value_requirement = 22.5; // note : " > the_value"

//todo : 
double noise_hit_distance = 0.234; // note : currently, the width of three channels

// todo : changed
int boundary_cut = 5; 

// note : the cuts for the scattering study
double slope_cut_scattering = 0.01;
double pos_cut_scattering   = 4.5; // note : mm
double cluster_adc_value_requirement_scattering = 0;
double slope_correction_l0_l1 = slope_correction;

// todo : changed
// note : the scan range for the systematic uncertainty
double residual_scan[2]= {0.1632,0.305};
double slope_scan[2]   = {0.0087,0.0113};
double edge_scan[2]    = {-1,11};

// note : this is for the effi-pos plot, to mention the beam spot region
double beam_spot_range[2] = {-7., 6.};
double effi_pos_legend_offset = 0.;