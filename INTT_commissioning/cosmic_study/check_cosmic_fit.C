// #include "DAC_Scan_ladder.h"
//#include "InttConversion.h"
#include "../DAC_Scan/InttClustering.h"
// #include "../sigmaEff.h"

// todo : the number of number is given by the adc_setting_run !!!
// todo : also the range of the hist.
// todo : the adc follows the following convention
// todo : 1. the increment has to be 4
// todo : remember to check the "adc_conv"
// vector<vector<int>> adc_setting_run = {	
//     // {8  , 12 , 16 , 20 , 24 , 28 , 32 , 36 },
//     // {28 , 32 , 36 , 40 , 44 , 48 , 52 , 56 },
//     {48 , 52 , 56 , 60 , 64 , 68 , 72 , 76 }, // note : 3
//     {68 , 72 , 76 , 80 , 84 , 88 , 92 , 96 }, // note : 4
//     {88 , 92 , 96 , 100, 104, 108, 112, 116}, // note : 5
//     {108, 112, 116, 120, 124, 128, 132, 136}, // note : 6
//     {128, 132, 136, 140, 144, 148, 152, 156}, // note : 7
//     // {148, 152, 156, 160, 164, 168, 172, 176},
//     // {168, 172, 176, 180, 184, 188, 192, 196},
//     // {188, 192, 196, 200, 204, 208, 212, 216}
// };

vector<vector<int>> adc_setting_run = {	
    {15, 30, 60, 90, 120, 150, 180, 210, 240}
    // {15, 30, 50, 70, 90, 110, 130, 150,170}
    // {8  , 12 , 16 , 20 , 24 , 28 , 32 , 36 },
    // {28 , 32 , 36 , 40 , 44 , 48 , 52 , 56 },
    // {48 , 52 , 56 , 60 , 64 , 68 , 72 , 76 }, // note : 3
    // {68 , 72 , 76 , 80 , 84 , 88 , 92 , 96 }, // note : 4
    // {88 , 92 , 96 , 100, 104, 108, 112, 116}, // note : 5
    // {108, 112, 116, 120, 124, 128, 132, 136}, // note : 6
    // {128, 132, 136, 140, 144, 148, 152, 156}, // note : 7
    // {148, 152, 156, 160, 164, 168, 172, 176},
    // {168, 172, 176, 180, 184, 188, 192, 196},
    // {188, 192, 196, 200, 204, 208, 212, 216}
};

TString color_code_2[8] = {"#CC768D","#19768D","#DDA573","#009193","#6E9193","#941100","#A08144","#517E66"};

struct full_hit_info {
    int FC;
    int chip_id;
    int chan_id;
    int adc;
};


struct ladder_info {
    int FC;
    TString Port;
    int ROC;
    int Direction; // note : 0 : south, 1 : north 
};

double get_radius(double x, double y)
{
    return sqrt(pow(x,2)+pow(y,2));
}

double get_radius_sign(double x, double y)
{
    double phi = ((y) < 0) ? atan2((y),(x)) * (180./TMath::Pi()) + 360 : atan2((y),(x)) * (180./TMath::Pi());
    
    return (phi > 180) ? sqrt(pow(x,2)+pow(y,2)) * -1 : sqrt(pow(x,2)+pow(y,2)); 
}

void Characterize_Pad (TPad *pad, float left = 0.15, float right = 0.1, float top = 0.1, float bottom = 0.12, bool set_logY = false, int setgrid_bool = 0)
{
	if (setgrid_bool == true) {pad -> SetGrid (1, 1);}
	pad -> SetLeftMargin   (left);
	pad -> SetRightMargin  (right);
	pad -> SetTopMargin    (top);
	pad -> SetBottomMargin (bottom);
    pad -> SetTicks(1,1);
    if (set_logY == true)
    {
        pad -> SetLogy (1);
    }
	
}

std::vector<double> calculateDistanceAndClosestPoint(double x1, double y1, double x2, double y2, double target_x, double target_y) {
    
    if (x1 != x2)
    {
        // Calculate the slope and intercept of the line passing through (x1, y1) and (x2, y2)
        double a = (y2 - y1) / (x2 - x1);
        double b = y1 - a * x1;

        // cout<<"slope : y="<<a<<"x+"<<b<<endl;
        
        // Calculate the closest distance from (target_x, target_y) to the line y = ax + b
        double closest_distance = std::abs(a * target_x - target_y + b) / std::sqrt(a * a + 1);

        // Calculate the coordinates of the closest point (Xc, Yc) on the line y = ax + b
        double Xc = (target_x + a * target_y - a * b) / (a * a + 1);
        double Yc = a * Xc + b;

        return { closest_distance, Xc, Yc };
    }
    else 
    {
        double closest_distance = std::abs(x1 - target_x);
        double Xc = x1;
        double Yc = target_y;

        return { closest_distance, Xc, Yc };
    }
    
    
}

double get_z_vertex(clu_info inner_clu, clu_info outer_clu, double target_x, double target_y)
{
    // note : x = z, 
    // note : y = radius
    double inner_clu_r = sqrt(pow(inner_clu.x,2)+ pow(inner_clu.y,2));
    double outer_clu_r = sqrt(pow(outer_clu.x,2)+ pow(outer_clu.y,2));
    double target_r    = sqrt(pow(target_x,2)   + pow(target_y,2));

    // Use the slope equation (y = ax + b) to calculate the x-coordinate for the target y
    if ( fabs(outer_clu.z - inner_clu.z) < 0.00001 ){
        return outer_clu.z;
    }
    else {
        double slope = (outer_clu_r - inner_clu_r) / (outer_clu.z - inner_clu.z);
        double yIntercept = inner_clu_r - slope * inner_clu.z;
        double xCoordinate = (target_r - yIntercept) / slope;
        return xCoordinate;
    }
    
}

// Function to calculate the angle between two vectors in degrees using the cross product
double calculateAngleBetweenVectors(double x1, double y1, double x2, double y2, double targetX, double targetY) {
    // Calculate the vectors vector_1 (point_1 to point_2) and vector_2 (point_1 to target)
    double vector1X = x2 - x1;
    double vector1Y = y2 - y1;

    double vector2X = targetX - x1;
    double vector2Y = targetY - y1;

    // Calculate the cross product of vector_1 and vector_2 (z-component)
    double crossProduct = vector1X * vector2Y - vector1Y * vector2X;
    
    // cout<<" crossProduct : "<<crossProduct<<endl;

    // Calculate the magnitudes of vector_1 and vector_2
    double magnitude1 = std::sqrt(vector1X * vector1X + vector1Y * vector1Y);
    double magnitude2 = std::sqrt(vector2X * vector2X + vector2Y * vector2Y);

    // Calculate the angle in radians using the inverse tangent of the cross product and dot product
    double dotProduct = vector1X * vector2X + vector1Y * vector2Y;

    double angleInRadians = std::atan2(std::abs(crossProduct), dotProduct);
    // Convert the angle from radians to degrees and return it
    double angleInDegrees = angleInRadians * 180.0 / M_PI;
    
    double angleInRadians_new = std::asin( crossProduct/(magnitude1*magnitude2) );
    double angleInDegrees_new = angleInRadians_new * 180.0 / M_PI;
    
    // cout<<"angle : "<<angleInDegrees_new<<endl;

    double DCA_distance = sin(angleInRadians_new) * magnitude2;

    return DCA_distance;
}

void temp_bkg(TPad * c1, string conversion_mode, double peek, pair<double,double> beam_origin)
{
    c1 -> cd();

    int N_ladder[4] = {12, 12, 16, 16};
    string ladder_index_string[16] = {"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15"};

    vector<double> x_vec; x_vec.clear();
    vector<double> y_vec; y_vec.clear();

    vector<double> x_vec_2; x_vec_2.clear();
    vector<double> y_vec_2; y_vec_2.clear();

    TGraph * bkg = new TGraph();
    bkg -> SetTitle("INTT event display X-Y plane");
    bkg -> SetMarkerStyle(20);
    bkg -> SetMarkerSize(0.1);
    bkg -> SetPoint(0,0,0);
    bkg -> SetPoint(1,beam_origin.first,beam_origin.second);
    bkg -> GetXaxis() -> SetLimits(-150,150);
    bkg -> GetYaxis() -> SetRangeUser(-150,150);
    bkg -> GetXaxis() -> SetTitle("X [mm]");
    bkg -> GetYaxis() -> SetTitle("Y [mm]");
    
    bkg -> Draw("ap");

    TLine * ladder_line = new TLine();
    ladder_line -> SetLineWidth(1);

    for (int server_i = 0; server_i < 4; server_i++)
    {
        for (int FC_i = 0; FC_i < 14; FC_i++)
        {
            ladder_line -> DrawLine(
                InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,14,0,conversion_mode,peek).x, InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,14,0,conversion_mode,peek).y,
                InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,1,0,conversion_mode,peek).x, InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,1,0,conversion_mode,peek).y
            );
        }
    }
    
    ladder_line -> Draw("l same");

}

void generateSubsets(const std::vector<int>& nums, int m, int start, std::vector<int>& currentSubset, std::vector<std::vector<int>>& allSubsets) {
    if (m == 0) {
        allSubsets.push_back(currentSubset);
        return;
    }

    for (int i = start; i <= nums.size() - m; ++i) {
        currentSubset.push_back(nums[i]);
        generateSubsets(nums, m - 1, i + 1, currentSubset, allSubsets);
        currentSubset.pop_back();
    }
}

std::vector<std::vector<int>> generateAllSubsets(const std::vector<int>& nums, int m) {
    std::vector<std::vector<int>> allSubsets;
    std::vector<int> currentSubset;

    generateSubsets(nums, m, 0, currentSubset, allSubsets);

    return allSubsets;
}

double grX_stddev (TGraph * input_grr)
{
    vector<double> input_vector; input_vector.clear();
    for (int i = 0; i < input_grr -> GetN(); i++)
    {  
        input_vector.push_back( input_grr -> GetPointX(i) );
    }

	double sum=0;
	double average;
	double sum_subt = 0;
	for (int i=0; i<input_vector.size(); i++)
		{
			sum+=input_vector[i];

		}
	average=sum/input_vector.size();
	//cout<<"average is : "<<average<<endl;

	for (int i=0; i<input_vector.size(); i++)
		{
			//cout<<input_vector[i]-average<<endl;
			sum_subt+=pow((input_vector[i]-average),2);

		}
	//cout<<"sum_subt : "<<sum_subt<<endl;
	double stddev;
	stddev=sqrt(sum_subt/(input_vector.size()-1));	
	return stddev;
}	

double grEY_stddev (TGraphErrors * input_grr)
{
    vector<double> input_vector; input_vector.clear();
    for (int i = 0; i < input_grr -> GetN(); i++)
    {  
        input_vector.push_back( input_grr -> GetPointY(i) );
    }

	double sum=0;
	double average;
	double sum_subt = 0;
	for (int i=0; i<input_vector.size(); i++)
		{
			sum+=input_vector[i];

		}
	average=sum/input_vector.size();
	//cout<<"average is : "<<average<<endl;

	for (int i=0; i<input_vector.size(); i++)
		{
			//cout<<input_vector[i]-average<<endl;
			sum_subt+=pow((input_vector[i]-average),2);

		}
	//cout<<"sum_subt : "<<sum_subt<<endl;
	double stddev;
	stddev=sqrt(sum_subt/(input_vector.size()-1));	
	return stddev;
}	

double grX_avg (TGraph * input_grr)
{
    vector<double> input_vector; input_vector.clear();
    for (int i = 0; i < input_grr -> GetN(); i++)
    {  
        input_vector.push_back( input_grr -> GetPointX(i) );
    }

	double sum=0;
	double average;
	double sum_subt = 0;
	for (int i=0; i<input_vector.size(); i++)
		{
			sum+=input_vector[i];

		}
	average=sum/input_vector.size();
	//cout<<"average is : "<<average<<endl;

    return average;
}

double grEY_avg (TGraphErrors * input_grr)
{
    vector<double> input_vector; input_vector.clear();
    for (int i = 0; i < input_grr -> GetN(); i++)
    {  
        input_vector.push_back( input_grr -> GetPointY(i) );
    }

	double sum=0;
	double average;
	double sum_subt = 0;
	for (int i=0; i<input_vector.size(); i++)
		{
			sum+=input_vector[i];

		}
	average=sum/input_vector.size();
	//cout<<"average is : "<<average<<endl;

    return average;
}

// note : the function check the distance of each two-point set, and find the smallest distance among all the sets
double grXY_deviation_small (TGraph * input_grr)
{
    vector<int> ele_index(input_grr -> GetN()); std::iota(ele_index.begin(), ele_index.end(), 0);
    vector<vector<int>> set_vec = generateAllSubsets(ele_index,2);

    double deviation;
    double distance;

    for (int i = 0; i < set_vec.size(); i++)
    {
        distance = sqrt(
            pow( input_grr -> GetPointX(set_vec[i][0]) - input_grr -> GetPointX(set_vec[i][1]), 2 ) + 
            pow( input_grr -> GetPointY(set_vec[i][0]) - input_grr -> GetPointY(set_vec[i][1]), 2 )
        );

        if (i == 0)
        {
            deviation = distance;
        }
        else if (deviation > distance)
        {
            deviation = distance;
        }
    }

    return deviation;
}

pair<double, double> mirrorPolynomial(double a, double b) {
    // Interchange 'x' and 'y'
    double mirroredA = 1.0 / a;
    double mirroredB = -b / a;

    return {mirroredA, mirroredB};
}

// note : use "ls *.root > file_list.txt" to create the list of the file in the folder, full directory in the file_list.txt
// note : set_folder_name = "folder_xxxx"
// note : server_name = "inttx"
void check_cosmic_fit(/*pair<double,double>beam_origin*/)
{
    TCanvas * c2 = new TCanvas("","",2500,800);    
    c2 -> cd();
    TPad *pad_xy = new TPad(Form("pad_xy"), "", 0.0, 0.0, 0.33, 1.0);
    Characterize_Pad(pad_xy, 0.15, 0.1, 0.1, 0.1 , 0, 0);
    pad_xy -> Draw();

    TPad *pad_rz = new TPad(Form("pad_rz"), "", 0.33, 0.0, 0.66, 1.0);
    Characterize_Pad(pad_rz, 0.15, 0.1, 0.1, 0.1 , 0, 0);
    pad_rz -> Draw();

    TPad *pad_z = new TPad(Form("pad_z"), "", 0.66, 0.0, 1.0, 1.0);
    Characterize_Pad(pad_z, 0.15, 0.1, 0.1, 0.1 , 0, 0);
    pad_z -> Draw();

    TCanvas * c1 = new TCanvas("","",1000,800);
    c1 -> cd();
    
    string mother_folder_directory = "/sphenix/user/ChengWei/INTT/INTT_commissioning/cosmic/25952";
    // string file_name = "beam_inttall-00020869-0000_event_base_ana_cluster_ideal_excludeR1500_100kEvent";
    string file_name = "cosmics_inttall-00025952-0000_event_base_ana_cluster_survey_1_XYAlpha_Peek_3.32mm_excludeR500_1000kEvent_0HotCut";
    gErrorIgnoreLevel = kWarning;

    // string mother_folder_directory = "/home/phnxrc/INTT/cwshih/DACscan_data/2023_08_01/24767";
    // string file_name = "beam_inttall-00024767-0000_event_base_ana_cluster_ideal_excludeR2000_100kEvent";

    // string mother_folder_directory = "/home/phnxrc/INTT/cwshih/DACscan_data/new_DAC_Scan_0722/AllServer/DAC2";
    // string file_name = "beam_inttall-00023058-0000_event_base_ana_cluster_ideal_excludeR2000_100kEvent";

    system(Form("mkdir %s/folder_%s_cosmic",mother_folder_directory.c_str(),file_name.c_str()));
    pair<double,double> beam_origin = {-0,2};
    double temp_Y_align = 0.;
    double temp_X_align = -0.;

    double zvtx_hist_l = -500;
    double zvtx_hist_r = 500;

    int Nhit_cut = 500;           // note : if (> Nhit_cut)          -> continue
    int clu_size_cut = 4;         // note : if (> clu_size_cut)      -> continue
    double clu_sum_adc_cut = 23;  // note : if (< clu_sum_adc_cut)   -> continue
    int N_clu_cut = 10;          // note : if (> N_clu_cut)         -> continue  unit number
    double XY_chi_cut = 5;
    double rZ_chi_cut = 5;
    
    double phi_diff_cut = 5.72;   // note : if (< phi_diff_cut)      -> pass      unit degree
    double DCA_cut = 4;           // note : if (< DCA_cut)           -> pass      unit mm
    int zvtx_cal_require = 15;    // note : if (> zvtx_cal_require)  -> pass
    int zvtx_draw_requireL = 15;       
    int zvtx_draw_requireR = 100;   // note : if ( zvtx_draw_requireL < event && event < zvtx_draw_requireR) -> pass
    double Integrate_portion = 0.6826;
    bool print_event_display = true; // todo : print the plots here
    
    //todo : change the mode for drawing
    int geo_mode_id = 1;
    string conversion_mode = (geo_mode_id == 0) ? "ideal" : "survey_1_XYAlpha_Peek";
    double peek = 3.32405;

    TFile * file_in = new TFile(Form("%s/%s.root",mother_folder_directory.c_str(),file_name.c_str()),"read");
    TTree * tree = (TTree *)file_in->Get("tree_clu");
    
    long long N_event = tree -> GetEntries();
    cout<<Form("N_event in file %s : %lli",file_name.c_str(), N_event)<<endl;

    int N_hits;
    int N_cluster_inner;
    int N_cluster_outer;
    Long64_t bco_full;
    vector<int>* column_vec = new vector<int>();
    vector<double>* avg_chan_vec = new vector<double>();
    vector<int>* sum_adc_vec = new vector<int>();
    vector<int>* sum_adc_conv_vec = new vector<int>();
    vector<int>* size_vec = new vector<int>();
    vector<double>* x_vec = new vector<double>();
    vector<double>* y_vec = new vector<double>();
    vector<double>* z_vec = new vector<double>();
    vector<int>* layer_vec = new vector<int>();
    vector<double>* phi_vec = new vector<double>();

    tree -> SetBranchAddress("nhits",&N_hits);
    tree -> SetBranchAddress("nclu_inner",&N_cluster_inner);
    tree -> SetBranchAddress("nclu_outer",&N_cluster_outer);
    tree -> SetBranchAddress("bco_full",&bco_full);

    tree -> SetBranchAddress("column", &column_vec);
    tree -> SetBranchAddress("avg_chan", &avg_chan_vec);
    tree -> SetBranchAddress("sum_adc", &sum_adc_vec);
    tree -> SetBranchAddress("sum_adc_conv", &sum_adc_conv_vec);
    tree -> SetBranchAddress("size", &size_vec);
    tree -> SetBranchAddress("x", &x_vec);
    tree -> SetBranchAddress("y", &y_vec);
    tree -> SetBranchAddress("z", &z_vec);
    tree -> SetBranchAddress("layer", &layer_vec);
    tree -> SetBranchAddress("phi", &phi_vec);

    TLatex *draw_text = new TLatex();
    draw_text -> SetNDC();
    draw_text -> SetTextSize(0.02);

    vector<clu_info> temp_sPH_inner_nocolumn_vec; temp_sPH_inner_nocolumn_vec.clear();
    vector<clu_info> temp_sPH_outer_nocolumn_vec; temp_sPH_outer_nocolumn_vec.clear();
    vector<clu_info> temp_sPH_all_nocolumn_vec; temp_sPH_all_nocolumn_vec.clear();
    vector<vector<double>> temp_sPH_nocolumn_vec(4); // note : cluster x, y, Nclu, sum_adc_conv
    vector<vector<double>> temp_sPH_nocolumn_rz_vec(2);
    vector<vector<double>> temp_sPH_nocolumn_rz_error_vec(2);

    gStyle->SetOptStat(10);

    TH2F * angle_correlation = new TH2F("","angle_correlation",361,0,361,361,0,361);
    angle_correlation -> SetStats(0);
    angle_correlation -> GetXaxis() -> SetTitle("Inner Phi (degree)");
    angle_correlation -> GetYaxis() -> SetTitle("Outer Phi (degree)");

    TH1F * angle_inner = new TH1F("","angle_inner",361,0,361);
    angle_inner -> SetStats(0);
    angle_inner -> GetXaxis() -> SetTitle("Inner Phi (degree)");
    angle_inner -> GetYaxis() -> SetTitle("Entry");

    TH1F * angle_outer = new TH1F("","angle_outer",361,0,361);
    angle_outer -> SetStats(0);
    angle_outer -> GetXaxis() -> SetTitle("Outer Phi (degree)");
    angle_outer -> GetYaxis() -> SetTitle("Entry");

    TH2F * inner_pos_xy = new TH2F("","inner_pos_xy",360,-100,100,360,-100,100);
    inner_pos_xy -> SetStats(0);
    inner_pos_xy -> GetXaxis() -> SetTitle("X axis");
    inner_pos_xy -> GetYaxis() -> SetTitle("Y axis");

    

    // TH1F * angle_inner_post_track = new TH1F("","angle_inner_post_track",361,0,361);
    // angle_inner_post_track -> SetStats(0);
    // angle_inner_post_track -> GetXaxis() -> SetTitle("Inner Phi (degree)");
    // angle_inner_post_track -> GetYaxis() -> SetTitle("Entry");

    TH1F * angle_inner_outer_post_track = new TH1F("","angle_inner_outer_post_track",361,0,361);
    // angle_inner_outer_post_track -> SetStats(0);
    angle_inner_outer_post_track -> GetXaxis() -> SetTitle("Outer Phi (degree)");
    angle_inner_outer_post_track -> GetYaxis() -> SetTitle("Entry");

    TH2F * inner_outer_pos_xy_post_track_pZ = new TH2F("","inner_outer_pos_xy_post_track_pZ",300,-150,150,300,-150,150);
    // inner_outer_pos_xy_post_track_pZ -> SetStats(0);
    inner_outer_pos_xy_post_track_pZ -> GetXaxis() -> SetTitle("X axis");
    inner_outer_pos_xy_post_track_pZ -> GetYaxis() -> SetTitle("Y axis");

    TH2F * inner_outer_pos_xy_post_track_nZ = new TH2F("","inner_outer_pos_xy_post_track_nZ",300,-150,150,300,-150,150);
    // inner_outer_pos_xy_post_track_nZ -> SetStats(0);
    inner_outer_pos_xy_post_track_nZ -> GetXaxis() -> SetTitle("X axis");
    inner_outer_pos_xy_post_track_nZ -> GetYaxis() -> SetTitle("Y axis");

    TH2F * inner_outer_pos_rz_post_track = new TH2F("","inner_outer_pos_rz_post_track",300,-250,250,300,-130,130);
    // inner_outer_pos_rz_post_track -> SetStats(0);
    inner_outer_pos_rz_post_track -> GetXaxis() -> SetTitle("Z axis");
    inner_outer_pos_rz_post_track -> GetYaxis() -> SetTitle("Radius");

    vector<TH2F *> Rchi2_xy_phi;
    for (int i = 0; i < 10; i++){
        Rchi2_xy_phi.push_back(new TH2F("",Form("Rchi2_xy_phi, z range : %i",i),181,0,181,200,0,10));
        // Rchi2_xy_phi[i] -> SetStats(0);
        Rchi2_xy_phi[i] -> GetXaxis() -> SetTitle("Fitting Phi [degree]");
        Rchi2_xy_phi[i] -> GetYaxis() -> SetTitle("#chi^{2} / NDF");
    }



    TH2F * outer_pos_xy = new TH2F("","outer_pos_xy",360,-150,150,360,-150,150);
    outer_pos_xy -> SetStats(0);
    outer_pos_xy -> GetXaxis() -> SetTitle("X axis");
    outer_pos_xy -> GetYaxis() -> SetTitle("Y axis");

    TH2F * inner_outer_pos_xy = new TH2F("","inner_outer_pos_xy",360,-150,150,360,-150,150);
    inner_outer_pos_xy -> SetStats(0);
    inner_outer_pos_xy -> GetXaxis() -> SetTitle("X axis");
    inner_outer_pos_xy -> GetYaxis() -> SetTitle("Y axis");

    TH1F * z_pos_diff = new TH1F("","z_pos_diff",360,-150,150);
    z_pos_diff -> GetXaxis() -> SetTitle("inner zpos - outer zpos");
    z_pos_diff -> GetYaxis() -> SetTitle("Eentry");

    TH2F * z_pos_diff_angle_diff = new TH2F("","z_pos_diff_angle_diff",100,-25,25,200,-11,11);
    z_pos_diff_angle_diff -> SetStats(0);
    z_pos_diff_angle_diff -> GetXaxis() -> SetTitle("inner zpos - outer zpos");
    z_pos_diff_angle_diff -> GetYaxis() -> SetTitle("Inner phi - outer phi");

    TH1F * Nhits_good = new TH1F("","Nhits_good",360,0,1000);
    Nhits_good -> GetXaxis() -> SetTitle("N hits in one event");
    Nhits_good -> GetYaxis() -> SetTitle("Eentry");

    TH1F * z_pos_inner = new TH1F("","z_pos_inner",200,-150,150);
    z_pos_inner -> GetXaxis() -> SetTitle("inner zpos");
    z_pos_inner -> GetYaxis() -> SetTitle("Eentry");

    TH1F * z_pos_outer = new TH1F("","z_pos_outer",200,-150,150);
    z_pos_outer -> GetXaxis() -> SetTitle("outer zpos");
    z_pos_outer -> GetYaxis() -> SetTitle("Eentry");

    TH2F * z_pos_inner_outer = new TH2F("","z_pos_inner_outer",100,-150,150, 100,-150,150);
    z_pos_inner_outer -> SetStats(0);
    z_pos_inner_outer -> GetXaxis() -> SetTitle("inner zpos");
    z_pos_inner_outer -> GetYaxis() -> SetTitle("outer pos");

    TH2F * DCA_point = new TH2F("","DCA_point",100,-10,10,100,-10,10);
    DCA_point -> SetStats(0);
    DCA_point -> GetXaxis() -> SetTitle("X pos (mm)");
    DCA_point -> GetYaxis() -> SetTitle("Y pos (mm)");

    TH2F * DCA_distance_inner_phi = new TH2F("","DCA_distance_inner_phi",100,0,360,100,-10,10);
    DCA_distance_inner_phi -> SetStats(0);
    DCA_distance_inner_phi -> GetXaxis() -> SetTitle("inner phi");
    DCA_distance_inner_phi -> GetYaxis() -> SetTitle("DCA (mm)");

    TH2F * DCA_distance_outer_phi = new TH2F("","DCA_distance_outer_phi",100,0,360,100,-10,10);
    DCA_distance_outer_phi -> SetStats(0);
    DCA_distance_outer_phi -> GetXaxis() -> SetTitle("outer phi");
    DCA_distance_outer_phi -> GetYaxis() -> SetTitle("DCA (mm)");

    TH1F * N_cluster_outer_pass = new TH1F("","N_cluster_outer_pass",100,0,100);
    N_cluster_outer_pass -> GetXaxis() -> SetTitle("N_cluster");
    N_cluster_outer_pass -> GetYaxis() -> SetTitle("Eentry");

    TH1F * N_cluster_inner_pass = new TH1F("","N_cluster_inner_pass",100,0,100);
    N_cluster_inner_pass -> GetXaxis() -> SetTitle("N_cluster");
    N_cluster_inner_pass -> GetYaxis() -> SetTitle("Eentry");

    TH1F * Rchi2_xy_hist = new TH1F("","Rchi2_xy_hist",100,0,10);
    Rchi2_xy_hist -> GetXaxis() -> SetTitle("N_cluster");
    Rchi2_xy_hist -> GetYaxis() -> SetTitle("Eentry");

    TH1F * Rchi2_rZ_hist = new TH1F("","Rchi2_rZ_hist",100,0,10);
    Rchi2_rZ_hist -> GetXaxis() -> SetTitle("N_cluster");
    Rchi2_rZ_hist -> GetYaxis() -> SetTitle("Eentry");

    TH2F * N_cluster_correlation = new TH2F("","N_cluster_correlation",100,0,500,100,0,500);
    N_cluster_correlation -> SetStats(0);
    N_cluster_correlation -> GetXaxis() -> SetTitle("inner N_cluster");
    N_cluster_correlation -> GetYaxis() -> SetTitle("Outer N_cluster");

    TH1F * temp_event_zvtx = new TH1F("","Z vertex dist",125,zvtx_hist_l,zvtx_hist_r);
    temp_event_zvtx -> GetXaxis() -> SetTitle("Z vertex position (mm)");
    temp_event_zvtx -> GetYaxis() -> SetTitle("Entry");
    vector<float> temp_event_zvtx_vec; temp_event_zvtx_vec.clear();
    vector<float> temp_event_zvtx_info; temp_event_zvtx_info.clear();
    TLine * effi_sig_range_line = new TLine();
    effi_sig_range_line -> SetLineWidth(3);
    effi_sig_range_line -> SetLineColor(TColor::GetColor("#A08144"));
    effi_sig_range_line -> SetLineStyle(2);
    TF1 * zvtx_fitting = new TF1("","gaus",-500,500);
    // zvtx_fitting -> SetLi

    TH1F * cosmic_phi_hist = new TH1F ("","cosmic_phi_hist",180,0,180);
    cosmic_phi_hist -> SetStats(0);
    cosmic_phi_hist -> GetXaxis() -> SetTitle("cosmic angle [phi]");
    cosmic_phi_hist -> GetYaxis() -> SetTitle("Entry"); 

    TH1F * cosmic_z_hist = new TH1F ("","cosmic_z_hist",20,-250,250);
    cosmic_z_hist -> SetStats(0);
    cosmic_z_hist -> GetXaxis() -> SetTitle("cosmic z [mm]");
    cosmic_z_hist -> GetYaxis() -> SetTitle("Entry"); 


    
    vector<vector<double>> good_track_xy_vec; good_track_xy_vec.clear();
    vector<vector<double>> good_track_rz_vec; good_track_rz_vec.clear();
    TLine * track_line = new TLine();
    track_line -> SetLineWidth(1);
    track_line -> SetLineColor(38);

    TLine * coord_line = new TLine();
    coord_line -> SetLineWidth(1);
    coord_line -> SetLineColor(16);
    coord_line -> SetLineStyle(2);


    vector<float> avg_event_zvtx_vec; avg_event_zvtx_vec.clear();
    TH1F * avg_event_zvtx = new TH1F("","avg_event_zvtx",125,zvtx_hist_l,zvtx_hist_r);
    avg_event_zvtx -> GetXaxis() -> SetTitle("Z vertex position (mm)");
    avg_event_zvtx -> GetYaxis() -> SetTitle("entry");
    
    double mini_DCAXY;
    int mini_inner_i;
    int mini_outer_i;

    vector<double> out_clu_x; out_clu_x.clear();
    vector<double> out_clu_y; out_clu_y.clear();
    vector<double> out_clu_z; out_clu_z.clear();
    vector<double> out_clu_r_sign; out_clu_r_sign.clear();
    vector<double> out_clu_size; out_clu_size.clear();
    vector<double> out_clu_sum_adc_conv; out_clu_sum_adc_conv.clear();
    Long64_t bco_full_out;
    int N_clu;
    int eID_out;

    TFile * out_file = new TFile(Form("%s/folder_%s_cosmic/INTT_eventdisplay_cluster_fit.root",mother_folder_directory.c_str(),file_name.c_str()),"RECREATE");
    TTree * tree_out =  new TTree ("tree_clu", "clustering info. for event display");
    tree_out -> Branch("eID",&eID_out);
    tree_out -> Branch("bco_full",&bco_full_out);
    tree_out -> Branch("nclu",&N_clu);
    tree_out -> Branch("clu_x",&out_clu_x);
    tree_out -> Branch("clu_y",&out_clu_y);
    tree_out -> Branch("clu_z",&out_clu_z);
    tree_out -> Branch("clu_r",&out_clu_r_sign);
    tree_out -> Branch("clu_size",&out_clu_size);
    tree_out -> Branch("clu_conv_adc",&out_clu_sum_adc_conv);


    TF1 * fit_xy = new TF1("fit_xy","pol1",-150,150);
    fit_xy -> SetLineColor(TColor::GetColor("#13b4fe"));
    fit_xy -> SetLineStyle(2);
    TF1 * fit_rz = new TF1("fit_rz","pol1",-500,500);
    fit_rz -> SetLineColor(TColor::GetColor("#13b4fe"));
    fit_rz -> SetLineStyle(2);
    TLine * fake_track = new TLine();
    fake_track -> SetLineColor(TColor::GetColor("#13b4fe"));
    fake_track -> SetLineStyle(2);
    
    double event_final_chi2;
    vector<int> event_final_set; event_final_set.clear();
    double event_comb_chi2;
    vector<int> event_comb_set; event_comb_set.clear();
    int used_clu;
    
    if (print_event_display) {c2 -> Print(Form("%s/folder_%s_cosmic/temp_event_display_fit.pdf(",mother_folder_directory.c_str(),file_name.c_str()));}

    for (int event_i = 0; event_i < N_event; event_i++)
    {
        tree -> GetEntry(event_i);
        unsigned int length = column_vec -> size();

        if (event_i % 1000 == 0)
            cout<<"--------- running event : "<<event_i<<endl;

        bco_full_out = bco_full;
        eID_out = event_i;

        // note : here is kinda pre-cut, the N_cluster_inner and N_cluster_outer are without consideration of the size and adc cut
        if (N_hits > Nhit_cut) continue;
        if (N_cluster_inner == 0 || N_cluster_outer == 0) continue;
        if (N_cluster_inner == -1 || N_cluster_outer == -1) continue;
        // // if ((N_cluster_inner + N_cluster_outer) < zvtx_cal_require) continue;
        // if (N_cluster_inner > 5) continue;
        // if (N_cluster_outer > 5) continue;

        

        // note : apply some selection to remove the hot channels
        // note : and make the inner_clu_vec and outer_clu_vec
        for (int clu_i = 0; clu_i < length; clu_i++)
        {
            if (size_vec -> at(clu_i) > clu_size_cut) continue;
            // if (size_vec -> at(clu_i) < 2) continue;
            if (sum_adc_conv_vec -> at(clu_i) < clu_sum_adc_cut) continue;
            // if (z_vec -> at(clu_i) < 0) continue;
            
            // note : inner
            // if (layer_vec -> at(clu_i) == 0 && x_vec -> at(clu_i) < -75 && x_vec -> at(clu_i) > -80 && y_vec -> at(clu_i) > 7.5 && y_vec -> at(clu_i) < 12.5 ) continue;
            // // if (layer_vec -> at(clu_i) == 0 && x_vec -> at(clu_i) > 35 && x_vec -> at(clu_i) < 40 && y_vec -> at(clu_i) > 65 && y_vec -> at(clu_i) < 70 ) continue;
            // if (layer_vec -> at(clu_i) == 0 && phi_vec -> at(clu_i) > 295 && phi_vec -> at(clu_i) < 302) continue;
            // if (layer_vec -> at(clu_i) == 0 && phi_vec -> at(clu_i) > 210 && phi_vec -> at(clu_i) < 213) continue;
            // if (layer_vec -> at(clu_i) == 0 && phi_vec -> at(clu_i) > 55 && phi_vec -> at(clu_i) < 65) continue;
            // if (layer_vec -> at(clu_i) == 0 && phi_vec -> at(clu_i) > 348 && phi_vec -> at(clu_i) < 353) continue;
            // if (layer_vec -> at(clu_i) == 0 && phi_vec -> at(clu_i) > 265 && phi_vec -> at(clu_i) < 270) continue; // todo : for the 2023_08_01/24767
            // if (layer_vec -> at(clu_i) == 0 && phi_vec -> at(clu_i) > 237.5 && phi_vec -> at(clu_i) < 242.5) continue;
            // if (layer_vec -> at(clu_i) == 0 && phi_vec -> at(clu_i) > 297.5 && phi_vec -> at(clu_i) < 302.5) continue;
            // if (layer_vec -> at(clu_i) == 0 && phi_vec -> at(clu_i) > 258 && phi_vec -> at(clu_i) < 262) continue; 
            // if (layer_vec -> at(clu_i) == 0 && phi_vec -> at(clu_i) > 268 && phi_vec -> at(clu_i) < 272) continue; 
            
            // if (layer_vec -> at(clu_i) == 0 && phi_vec -> at(clu_i) > 210 && phi_vec -> at(clu_i) < 214) continue; 
            // if (layer_vec -> at(clu_i) == 0 && phi_vec -> at(clu_i) > 260 && phi_vec -> at(clu_i) < 270) continue; 

            // if (layer_vec -> at(clu_i) == 0 && phi_vec -> at(clu_i) > 349 ) continue; 

            // if (layer_vec -> at(clu_i) == 0 && phi_vec -> at(clu_i) >= 349 && phi_vec -> at(clu_i) < 350 ) continue; 

            // // note : outer
            // if (layer_vec -> at(clu_i) == 1 && x_vec -> at(clu_i) < -70 && x_vec -> at(clu_i) > -75 && y_vec -> at(clu_i) > 70 && y_vec -> at(clu_i) < 80 ) continue;
            // // if (layer_vec -> at(clu_i) == 1 && x_vec -> at(clu_i) > 70 && x_vec -> at(clu_i) < 83 && y_vec -> at(clu_i) > 50 && y_vec -> at(clu_i) < 65 ) continue;
            // // if (layer_vec -> at(clu_i) == 1 && x_vec -> at(clu_i) > 70 && x_vec -> at(clu_i) < 83 && y_vec -> at(clu_i) > 63 && y_vec -> at(clu_i) < 75 ) continue;
            // if (layer_vec -> at(clu_i) == 1 && x_vec -> at(clu_i) < -70 && x_vec -> at(clu_i) > -75 && y_vec -> at(clu_i) < -70 && y_vec -> at(clu_i) > -75 ) continue;
            // if (layer_vec -> at(clu_i) == 1 && phi_vec -> at(clu_i) > 335 && phi_vec -> at(clu_i) < 340) continue;
            // if (layer_vec -> at(clu_i) == 1 && phi_vec -> at(clu_i) > 105 && phi_vec -> at(clu_i) < 115) continue;
            // if (layer_vec -> at(clu_i) == 1 && phi_vec -> at(clu_i) > 25 && phi_vec -> at(clu_i) < 47) continue; // todo : for the "new_DAC_Scan_0722/AllServer/DAC2"
            // if (layer_vec -> at(clu_i) == 1 && phi_vec -> at(clu_i) > 37.5 && phi_vec -> at(clu_i) < 43) continue;
            // if (layer_vec -> at(clu_i) == 1 && phi_vec -> at(clu_i) > 105 && phi_vec -> at(clu_i) < 120) continue;  
            // if (layer_vec -> at(clu_i) == 1 && phi_vec -> at(clu_i) > 135 && phi_vec -> at(clu_i) < 145) continue;
            // if (layer_vec -> at(clu_i) == 1 && phi_vec -> at(clu_i) > 167.5 && phi_vec -> at(clu_i) < 172.5) continue; 
            // if (layer_vec -> at(clu_i) == 1 && phi_vec -> at(clu_i) > 220 && phi_vec -> at(clu_i) < 230) continue; 

            // if (layer_vec -> at(clu_i) == 1 && phi_vec -> at(clu_i) > 23 && phi_vec -> at(clu_i) < 28) continue; 
            // if (layer_vec -> at(clu_i) == 1 && phi_vec -> at(clu_i) > 121 && phi_vec -> at(clu_i) < 125) continue; 
            // if (layer_vec -> at(clu_i) == 1 && phi_vec -> at(clu_i) > 275.5 && phi_vec -> at(clu_i) < 277) continue;

            // if (layer_vec -> at(clu_i) == 1 && phi_vec -> at(clu_i) >= 25 && phi_vec -> at(clu_i) < 26) continue;

            temp_sPH_nocolumn_vec[0].push_back( (phi_vec -> at(clu_i) > 90 && phi_vec -> at(clu_i) < 270 ) ? x_vec -> at(clu_i) + temp_X_align : x_vec -> at(clu_i) );
            temp_sPH_nocolumn_vec[1].push_back( (phi_vec -> at(clu_i) > 90 && phi_vec -> at(clu_i) < 270 ) ? y_vec -> at(clu_i) + temp_Y_align : y_vec -> at(clu_i) );
            temp_sPH_nocolumn_vec[2].push_back( size_vec -> at(clu_i) );
            temp_sPH_nocolumn_vec[3].push_back( sum_adc_conv_vec -> at(clu_i));

            double clu_radius = get_radius(
                (phi_vec -> at(clu_i) > 90 && phi_vec -> at(clu_i) < 270 ) ? x_vec -> at(clu_i) + temp_X_align : x_vec -> at(clu_i), 
                (phi_vec -> at(clu_i) > 90 && phi_vec -> at(clu_i) < 270 ) ? y_vec -> at(clu_i) + temp_Y_align : y_vec -> at(clu_i)
            );
            temp_sPH_nocolumn_rz_vec[0].push_back(z_vec -> at(clu_i));
            temp_sPH_nocolumn_rz_vec[1].push_back( ( phi_vec -> at(clu_i) > 180 ) ? clu_radius * -1 : clu_radius );
            temp_sPH_nocolumn_rz_error_vec[0].push_back( ( fabs( z_vec -> at(clu_i) ) < 130 ) ? 8 : 10 ); // note :  X error of the rZ plot
            temp_sPH_nocolumn_rz_error_vec[1].push_back(0.16); // note : Y error of the rZ plot

            temp_sPH_all_nocolumn_vec.push_back({
                column_vec -> at(clu_i), 
                avg_chan_vec -> at(clu_i), 
                sum_adc_vec -> at(clu_i), 
                sum_adc_conv_vec -> at(clu_i), 
                size_vec -> at(clu_i), 
                (phi_vec -> at(clu_i) > 90 && phi_vec -> at(clu_i) < 270 ) ? x_vec -> at(clu_i) + temp_X_align : x_vec -> at(clu_i), 
                (phi_vec -> at(clu_i) > 90 && phi_vec -> at(clu_i) < 270 ) ? y_vec -> at(clu_i) + temp_Y_align : y_vec -> at(clu_i), 
                z_vec -> at(clu_i), 
                layer_vec -> at(clu_i), 
                phi_vec -> at(clu_i)
            });            

            if (layer_vec -> at(clu_i) == 0) //note : inner
                temp_sPH_inner_nocolumn_vec.push_back({
                    column_vec -> at(clu_i), 
                    avg_chan_vec -> at(clu_i), 
                    sum_adc_vec -> at(clu_i), 
                    sum_adc_conv_vec -> at(clu_i), 
                    size_vec -> at(clu_i), 
                    (phi_vec -> at(clu_i) > 90 && phi_vec -> at(clu_i) < 270 ) ? x_vec -> at(clu_i) + temp_X_align : x_vec -> at(clu_i), 
                    (phi_vec -> at(clu_i) > 90 && phi_vec -> at(clu_i) < 270 ) ? y_vec -> at(clu_i) + temp_Y_align : y_vec -> at(clu_i), 
                    z_vec -> at(clu_i), 
                    layer_vec -> at(clu_i), 
                    phi_vec -> at(clu_i)
                });
            
            if (layer_vec -> at(clu_i) == 1) //note : outer
                temp_sPH_outer_nocolumn_vec.push_back({
                    column_vec -> at(clu_i), 
                    avg_chan_vec -> at(clu_i), 
                    sum_adc_vec -> at(clu_i), 
                    sum_adc_conv_vec -> at(clu_i), 
                    size_vec -> at(clu_i), 
                    (phi_vec -> at(clu_i) > 90 && phi_vec -> at(clu_i) < 270 ) ? x_vec -> at(clu_i) + temp_X_align : x_vec -> at(clu_i), 
                    (phi_vec -> at(clu_i) > 90 && phi_vec -> at(clu_i) < 270 ) ? y_vec -> at(clu_i) + temp_Y_align : y_vec -> at(clu_i), 
                    z_vec -> at(clu_i), 
                    layer_vec -> at(clu_i), 
                    phi_vec -> at(clu_i)
                });            
        }

        int original_outer_vec_size = temp_sPH_outer_nocolumn_vec.size(); 
        N_cluster_outer_pass -> Fill(temp_sPH_outer_nocolumn_vec.size());
        N_cluster_inner_pass -> Fill(temp_sPH_inner_nocolumn_vec.size());
        N_cluster_correlation -> Fill( temp_sPH_inner_nocolumn_vec.size(), temp_sPH_outer_nocolumn_vec.size() );

        if ((temp_sPH_inner_nocolumn_vec.size() + temp_sPH_outer_nocolumn_vec.size()) > N_clu_cut)
        {
            temp_event_zvtx_info = {-1000,-1000,-1000};
            temp_event_zvtx_vec.clear();
            temp_event_zvtx -> Reset("ICESM");
            good_track_xy_vec.clear();
            good_track_rz_vec.clear();
            temp_sPH_nocolumn_rz_vec.clear(); temp_sPH_nocolumn_rz_vec = vector<vector<double>>(2);
            temp_sPH_nocolumn_rz_error_vec.clear(); temp_sPH_nocolumn_rz_error_vec = vector<vector<double>>(2);
            temp_sPH_nocolumn_vec.clear(); temp_sPH_nocolumn_vec = vector<vector<double>>(4);
            temp_sPH_inner_nocolumn_vec.clear();
            temp_sPH_outer_nocolumn_vec.clear();
            continue;
        }

        if ( temp_sPH_inner_nocolumn_vec.size() > 1 && temp_sPH_outer_nocolumn_vec.size() > 1 && 3 < temp_sPH_nocolumn_vec[0].size() && temp_sPH_nocolumn_vec[0].size() < 8  ) // note : at least 3 points, 4 to 7 (allow some noise hits)
        // if ( temp_sPH_inner_nocolumn_vec.size() == 2 && temp_sPH_outer_nocolumn_vec.size() == 2)
        {
            TGraph * temp_event_xy = new TGraph(temp_sPH_nocolumn_vec[0].size(),&temp_sPH_nocolumn_vec[0][0],&temp_sPH_nocolumn_vec[1][0]);
            temp_event_xy -> SetTitle("INTT event display X-Y plane");
            temp_event_xy -> GetXaxis() -> SetLimits(-150,150);
            temp_event_xy -> GetYaxis() -> SetRangeUser(-150,150);
            temp_event_xy -> GetXaxis() -> SetTitle("X [mm]");
            temp_event_xy -> GetYaxis() -> SetTitle("Y [mm]");
            temp_event_xy -> SetMarkerStyle(20);
            temp_event_xy -> SetMarkerColor(2);
            temp_event_xy -> SetMarkerSize(1);
            TGraphErrors * temp_event_rz = new TGraphErrors(temp_sPH_nocolumn_rz_vec[0].size(),&temp_sPH_nocolumn_rz_vec[0][0],&temp_sPH_nocolumn_rz_vec[1][0],&temp_sPH_nocolumn_rz_error_vec[0][0],&temp_sPH_nocolumn_rz_error_vec[1][0]);
            temp_event_rz -> SetTitle("INTT event display r-Z plane");
            temp_event_rz -> GetXaxis() -> SetLimits(-250,250);
            temp_event_rz -> GetYaxis() -> SetRangeUser(-150,150);
            temp_event_rz -> GetXaxis() -> SetTitle("Z [mm]");
            temp_event_rz -> GetYaxis() -> SetTitle("Radius [mm]");
            temp_event_rz -> SetMarkerStyle(20);
            temp_event_rz -> SetMarkerColor(2);
            temp_event_rz -> SetMarkerSize(1);

            double valid_valid_count = 0;

            int select_max = (temp_sPH_nocolumn_vec[0].size() > 7) ? 8 : temp_sPH_nocolumn_vec[0].size() + 1;
            vector<int> ele_index(temp_sPH_nocolumn_vec[0].size()); std::iota(ele_index.begin(), ele_index.end(), 0);
            for (int clu_i = 4; clu_i < select_max; clu_i++ ) // note use "clu_i" element to do the fitting at least 4 elements, along with maximum 8
            {                
                vector<vector<int>> subsets_index = generateAllSubsets(ele_index, clu_i);
                // cout<<"------------------------------------- ------------------------------------- -------------------------------------"<<endl;
                // cout<<"event : "<<event_i<<" N element in the event : "<<ele_index.size()<<" select : "<<clu_i<<" total combination "<<subsets.size()<<endl;
                
                // note : the reason for this valid_count is because we "continue" the combinations which have the clusters so close to each other.
                double valid_count = 0;

                for (int set_i = 0; set_i < subsets_index.size(); set_i++) // note : N combination, can be {0,1,2,3,5}, {0,1,3,4,6}...
                {
                    TGraph * xy_gr = new TGraph();
                    // TGraph * rz_gr = new TGraph();
                    vector<vector<double>> subsets_rz_point(4); // note : x, y, xe, ye

                    for (int ele_i = 0; ele_i < subsets_index[set_i].size(); ele_i++) // note : n clu in that combination
                    {
                        xy_gr -> SetPoint(ele_i, temp_sPH_nocolumn_vec[0][ subsets_index[set_i][ele_i] ],    temp_sPH_nocolumn_vec[1][ subsets_index[set_i][ele_i] ]);
                        subsets_rz_point[0].push_back( temp_sPH_nocolumn_rz_vec[0][ subsets_index[set_i][ele_i] ] );
                        subsets_rz_point[1].push_back( temp_sPH_nocolumn_rz_vec[1][ subsets_index[set_i][ele_i] ] );
                        subsets_rz_point[2].push_back( temp_sPH_nocolumn_rz_error_vec[0][ subsets_index[set_i][ele_i] ] );
                        subsets_rz_point[3].push_back( temp_sPH_nocolumn_rz_error_vec[1][ subsets_index[set_i][ele_i] ] );
                        // rz_gr -> SetPoint(ele_i, temp_sPH_nocolumn_rz_vec[0][ subsets_index[set_i][ele_i] ], temp_sPH_nocolumn_rz_vec[1][ subsets_index[set_i][ele_i] ]);
                        // cout<<"used point, xy : "<<temp_sPH_nocolumn_vec[0][ subsets_index[set_i][ele_i] ]<<", "<<temp_sPH_nocolumn_vec[1][ subsets_index[set_i][ele_i] ]<<" rz : "<<temp_sPH_nocolumn_rz_vec[0][ subsets_index[set_i][ele_i] ] <<" "<< temp_sPH_nocolumn_rz_vec[1][ subsets_index[set_i][ele_i] ] <<" "<< temp_sPH_nocolumn_rz_error_vec[0][ subsets_index[set_i][ele_i] ] <<" "<< temp_sPH_nocolumn_rz_error_vec[1][ subsets_index[set_i][ele_i] ]<<endl;
                    }

                    // Title : it's a test, because that the rz plane can not be fit well, so, rotate the axis, just for fitting
                    TGraphErrors * rz_gr = new TGraphErrors(subsets_rz_point[0].size(),&subsets_rz_point[1][0],&subsets_rz_point[0][0],&subsets_rz_point[3][0],&subsets_rz_point[2][0]);

                    if (bco_full == 128389189792) {cout<<"test : "<<subsets_rz_point[0].size()<<endl;}

                    // note : if the clusters are too close to each other, continue this combination
                    // note : unit : mm (1 mm for example)
                    if ( grXY_deviation_small(xy_gr) < 1 ) continue; 
                    // cout<<event_i<<" xy fitting"<<endl;
                    double chi2_xy = ( fabs( grX_stddev(xy_gr) ) < 0.00001 ) ? 0 : 1; // note : check whether it's vertical line or not
                    if (chi2_xy == 1){ // note : not vertical, can fit
                        xy_gr -> Fit(fit_xy,"QN");
                        chi2_xy = fit_xy -> GetChisquare() / double(fit_xy -> GetNDF());
                    }
                    // cout<<event_i<<" rz fitting"<<endl;
                    double chi2_rz = ( fabs( grEY_stddev(rz_gr) ) < 0.00001 ) ? 0 : 1;
                    if (chi2_rz == 1){ // note : not vertical, can fit
                        fit_rz -> SetParameters(0,0);
                        rz_gr -> Fit(fit_rz,"QN");
                        chi2_rz = fit_rz -> GetChisquare() / double(fit_rz -> GetNDF());
                    }

                    if (bco_full == 128389189792) {cout<<"test rz deviation : "<<grEY_stddev(rz_gr)<<endl;}
                    
                    if (event_i == 4090)
                        cout<<"test test : "<<clu_i<<" "<<set_i<<" "<<chi2_xy<<" "<<chi2_rz<<endl;

                    if (valid_count == 0){
                        event_comb_chi2 = sqrt(pow(chi2_xy,2) + pow(chi2_rz,2));
                        event_comb_set  = subsets_index[set_i];
                        valid_count = 1;
                    }
                    else if ( event_comb_chi2 > sqrt(pow(chi2_xy,2) + pow(chi2_rz,2)) )
                    {
                        event_comb_chi2 = sqrt(pow(chi2_xy,2) + pow(chi2_rz,2));
                        event_comb_set  = subsets_index[set_i];
                    }

                }
                if(event_i == 4090)
                {
                    cout<<"total clu : "<<temp_sPH_nocolumn_vec[0].size()<<endl;
                    cout<<"used clu : "<<clu_i<<" best chi2 : "<<event_comb_chi2<<endl;
                }

                if (valid_count == 1) valid_valid_count += 1;

                if (valid_valid_count == 1 && valid_count == 1){
                    used_clu = clu_i;
                    event_final_chi2 = event_comb_chi2;
                    event_final_set = event_comb_set;
                }
                else if (valid_count == 1 && event_final_chi2 > event_comb_chi2) {
                    used_clu = clu_i;
                    event_final_chi2 = event_comb_chi2;
                    event_final_set = event_comb_set;
                }
            }
            
            // cout<<"~~~~~ "<<event_i<<"finish the loop test"<<endl;

            if (valid_valid_count != 0)
            {
                TGraph * xy_gr = new TGraph();
                // TGraph * rz_gr = new TGraph();
                vector<vector<double>> subsets_rz_point(4); // note : x, y, xe, ye

                for (int ele_i = 0; ele_i < event_final_set.size(); ele_i++) // note : n clu in that combination
                {
                    xy_gr -> SetPoint(ele_i, temp_sPH_nocolumn_vec[0][ event_final_set[ele_i] ],    temp_sPH_nocolumn_vec[1][ event_final_set[ele_i] ]);
                    subsets_rz_point[0].push_back( temp_sPH_nocolumn_rz_vec[0][ event_final_set[ele_i] ] );
                    subsets_rz_point[1].push_back( temp_sPH_nocolumn_rz_vec[1][ event_final_set[ele_i] ] );
                    subsets_rz_point[2].push_back( temp_sPH_nocolumn_rz_error_vec[0][ event_final_set[ele_i] ] );
                    subsets_rz_point[3].push_back( temp_sPH_nocolumn_rz_error_vec[1][ event_final_set[ele_i] ] );
                    // rz_gr -> SetPoint(ele_i, temp_sPH_nocolumn_rz_vec[0][ event_final_set[ele_i] ], temp_sPH_nocolumn_rz_vec[1][ event_final_set[ele_i] ]);
                }
                
                // note : again, the X and Y are swapped for better fitting
                TGraphErrors * rz_gr = new TGraphErrors(subsets_rz_point[0].size(), &subsets_rz_point[1][0], &subsets_rz_point[0][0], &subsets_rz_point[3][0], &subsets_rz_point[2][0]);
                if (bco_full == 128389189792) {cout<<"test after : "<<rz_gr -> GetN()<<endl;}

                double chi2_xy = ( fabs( grX_stddev(xy_gr) ) < 0.00001 ) ? 0 : 1;
                if (chi2_xy == 1){ // note : not vertical, can fit
                    xy_gr -> Fit(fit_xy,"NQ");
                    chi2_xy = (fit_xy -> GetChisquare() / double(fit_xy -> GetNDF()));
                }
                double chi2_rz = ( fabs( grEY_stddev(rz_gr) ) < 0.00001 ) ? 0 : 1;
                if (bco_full == 128389189792) {cout<<"test after, rz deviation : "<<grEY_stddev(rz_gr)<<endl;}
                if (chi2_rz == 1){ // note : not vertical, can fit
                    // fit_rz = new TF1("fit_rz","pol1",-500,500);
                    // fit_rz -> SetParameters(5.3,224);
                    rz_gr -> Fit(fit_rz,"NQ");
                    
                    if( bco_full == 128389189792 ){
                        for (int gr_i = 0; gr_i < rz_gr->GetN(); gr_i++)
                        {
                            cout<<"In fitting, check value : "<<rz_gr -> GetPointX(gr_i)<<" "<<rz_gr -> GetPointY(gr_i)<<endl;
                        }
                    }
                    chi2_rz = (fit_rz -> GetChisquare() / double(fit_rz -> GetNDF()));

                    if (bco_full == 128389189792) {cout<<"In fitting : "<<fit_rz->GetParameter(1)<<" "<<fit_rz->GetParameter(0)<<endl;}     
                }

                if (bco_full == 128389189792) {cout<<"test after, fianl chi2_rz : "<<chi2_rz<<endl;}

                pad_xy -> cd();
                temp_bkg(pad_xy, conversion_mode, peek, beam_origin);
                if (chi2_xy != 0){ // note : not vertical, can fit  
                    double cosmic_phi = ( atan(fit_xy -> GetParameter(1)) * (180/M_PI) < 0 ) ? atan(fit_xy -> GetParameter(1)) * (180/M_PI) + 180 : atan(fit_xy -> GetParameter(1)) * (180/M_PI);
                    draw_text -> DrawLatex(0.2, 0.82, Form("Used cluster : %i, chi2/NDF : %.3f",used_clu, (fit_xy -> GetChisquare() / double(fit_xy -> GetNDF())) ));
                    draw_text -> DrawLatex(0.2, 0.79, Form("cosmic phi : %.2f degree", cosmic_phi ));
                    draw_text -> DrawLatex(0.2, 0.76, Form("bco_full : %lli",bco_full));
                    
                    if ( chi2_xy < XY_chi_cut && chi2_rz < rZ_chi_cut ){
                        cosmic_phi_hist -> Fill( cosmic_phi );
                        fit_xy -> Draw("lsame");
                    }
                    else{
                        TF1 * false_fit_xy = (TF1*)fit_xy -> Clone("");
                        false_fit_xy -> SetLineColor(42);
                        false_fit_xy -> SetLineStyle(2);
                        false_fit_xy -> Draw("lsame");
                    }
                }
                else {            
                    draw_text -> DrawLatex(0.2, 0.82, Form("Used cluster : %i, Vertical !",used_clu ));
                    draw_text -> DrawLatex(0.2, 0.79, Form("cosmic phi : 90 degree" ));

                    if ( chi2_xy < XY_chi_cut && chi2_rz < rZ_chi_cut ){
                        cosmic_phi_hist -> Fill( 90 );
                        fake_track -> DrawLine(xy_gr -> GetPointX(0), -150, xy_gr -> GetPointX(0), 150);
                    }
                    else {
                        TLine * false_track = new TLine(xy_gr -> GetPointX(0), -150, xy_gr -> GetPointX(0), 150);
                        false_track -> SetLineColor(42);
                        false_track -> SetLineStyle(2);
                        false_track -> Draw("lsame");
                    }
                }
                temp_event_xy -> Draw("p same");
                draw_text -> DrawLatex(0.2, 0.85, Form("eID : %i, Total event hit : %i, innter Ncluster : %i, outer Ncluster : %i",event_i,N_hits,temp_sPH_inner_nocolumn_vec.size(),original_outer_vec_size));
                

                pad_rz -> cd();
                temp_event_rz -> Draw("ap");   
                if (chi2_rz != 0){ // note : not vertical, can fit
                    if ( chi2_xy < XY_chi_cut && chi2_rz < rZ_chi_cut ){
                        double par_0 = fit_rz -> GetParameter(0);
                        double par_1 = fit_rz -> GetParameter(1);
                        fit_rz -> SetParameters( mirrorPolynomial(par_1,par_0).second, mirrorPolynomial(par_1,par_0).first);
                        fit_rz -> Draw("lsame");
                        
                        if (bco_full == 128389189792)
                        {
                            cout<<"are you here ? "<<setprecision(12)<<" "<<fit_rz -> GetParameter(0)<<" "<<fit_rz -> GetParameter(1)<<" chi2 : "<< (fit_rz -> GetChisquare() / double(fit_rz -> GetNDF()))<<endl;
                        } 
                    }
                    else {
                        TF1 * false_fit_rz = (TF1*) fit_rz -> Clone("");
                        
                        double par_0 = false_fit_rz -> GetParameter(0);
                        double par_1 = false_fit_rz -> GetParameter(1);
                        false_fit_rz -> SetParameters( mirrorPolynomial(par_1,par_0).second, mirrorPolynomial(par_1,par_0).first);

                        false_fit_rz -> SetLineColor(42);
                        false_fit_rz -> SetLineStyle(2);
                        false_fit_rz -> Draw("lsame");
                    }
                    draw_text -> DrawLatex(0.2, 0.82, Form("Used cluster : %i, chi2/NDF : %.3f",used_clu, (fit_rz -> GetChisquare() / double(fit_rz -> GetNDF())) ));
                }
                else{
                    if ( chi2_xy < XY_chi_cut && chi2_rz < rZ_chi_cut ){
                        // fake_track -> DrawLine(rz_gr -> GetPointX(0), -150, rz_gr -> GetPointX(0), 150);
                        fake_track -> DrawLine(rz_gr -> GetPointY(0), -150, rz_gr -> GetPointY(0), 150); // note : because the rZ plane is opposite, for the better fitting
                    }
                    else {
                        // TLine * false_track = new TLine(rz_gr -> GetPointX(0), -150, rz_gr -> GetPointX(0), 150);
                        TLine * false_track = new TLine(rz_gr -> GetPointY(0), -150, rz_gr -> GetPointY(0), 150); // note : because the rZ plane is opposite, for the better fitting
                        false_track -> SetLineColor(42);
                        false_track -> SetLineStyle(2);
                        false_track -> Draw("lsame");
                    }
                    draw_text -> DrawLatex(0.2, 0.82, Form("Used cluster : %i, Vertical !",used_clu ));
                }
                double avg_z = grEY_avg(rz_gr);
                draw_text -> DrawLatex(0.2, 0.79, Form("Average z : %.2f mm",avg_z ));
                temp_event_rz -> Draw("p same");   

                Rchi2_rZ_hist -> Fill(chi2_rz);
                Rchi2_xy_hist -> Fill(chi2_xy);
                double fit_phi = (chi2_xy == 0) ? 90 : ( atan2( fabs(fit_xy -> GetParameter(1)), (fit_xy -> GetParameter(1) < 0) ? -1 : 1 ) * (180./TMath::Pi()) );
                Rchi2_xy_phi[ int((avg_z+230)/46) ] -> Fill( fit_phi, chi2_xy );

                if ( chi2_xy < XY_chi_cut && chi2_rz < rZ_chi_cut ){
                    for (int ele_i = 0; ele_i < xy_gr->GetN(); ele_i++) { // note : n clu in that combination
                        if (rz_gr -> GetPointY(ele_i) > 0){
                            inner_outer_pos_xy_post_track_pZ -> Fill( xy_gr -> GetPointX(ele_i), xy_gr -> GetPointY(ele_i) );    
                        }
                        else {
                            inner_outer_pos_xy_post_track_nZ -> Fill( xy_gr -> GetPointX(ele_i), xy_gr -> GetPointY(ele_i) );
                        }                        
                        angle_inner_outer_post_track -> Fill( (xy_gr -> GetPointY(ele_i) < 0) ? atan2(xy_gr -> GetPointY(ele_i),xy_gr -> GetPointX(ele_i)) * (180./TMath::Pi()) + 360 : atan2(xy_gr -> GetPointY(ele_i),xy_gr -> GetPointX(ele_i)) * (180./TMath::Pi()) );
                        inner_outer_pos_rz_post_track -> Fill( rz_gr -> GetPointY(ele_i), rz_gr -> GetPointX(ele_i) );
                    }

                    cosmic_z_hist -> Fill(avg_z);

                    if (print_event_display) {
                        c2 -> Print(Form("%s/folder_%s_cosmic/temp_event_display_fit.pdf",mother_folder_directory.c_str(),file_name.c_str()));
                    }

                    out_clu_x = temp_sPH_nocolumn_vec[0]; // note : for the output root tree.
                    out_clu_y = temp_sPH_nocolumn_vec[1];
                    out_clu_z = temp_sPH_nocolumn_rz_vec[0];
                    out_clu_r_sign = temp_sPH_nocolumn_rz_vec[1];
                    out_clu_size = temp_sPH_nocolumn_vec[2];
                    out_clu_sum_adc_conv = temp_sPH_nocolumn_vec[3];
                    N_clu = temp_sPH_nocolumn_vec[0].size();
                    tree_out -> Fill();
                }

                
            }
            
            pad_xy -> Clear();
            pad_rz -> Clear();
            pad_z  -> Clear(); 
            delete temp_event_xy;
            delete temp_event_rz;
        }

        for ( int inner_i = 0; inner_i < temp_sPH_inner_nocolumn_vec.size(); inner_i++ )
        {
            inner_pos_xy -> Fill(temp_sPH_inner_nocolumn_vec[inner_i].x,temp_sPH_inner_nocolumn_vec[inner_i].y);
            inner_outer_pos_xy -> Fill(temp_sPH_inner_nocolumn_vec[inner_i].x,temp_sPH_inner_nocolumn_vec[inner_i].y);
            angle_inner -> Fill(temp_sPH_inner_nocolumn_vec[inner_i].phi);
        }

        for ( int outer_i = 0; outer_i < temp_sPH_outer_nocolumn_vec.size(); outer_i++ )
        {
            outer_pos_xy -> Fill(temp_sPH_outer_nocolumn_vec[outer_i].x,temp_sPH_outer_nocolumn_vec[outer_i].y);
            inner_outer_pos_xy -> Fill(temp_sPH_outer_nocolumn_vec[outer_i].x,temp_sPH_outer_nocolumn_vec[outer_i].y);
            angle_outer -> Fill(temp_sPH_outer_nocolumn_vec[outer_i].phi);
        }

        temp_event_zvtx_info = {-1000,-1000,-1000};
        temp_event_zvtx_vec.clear();
        temp_event_zvtx -> Reset("ICESM");
        good_track_xy_vec.clear();
        good_track_rz_vec.clear();
        temp_sPH_nocolumn_rz_vec.clear(); temp_sPH_nocolumn_rz_vec = vector<vector<double>>(2);
        temp_sPH_nocolumn_rz_error_vec.clear(); temp_sPH_nocolumn_rz_error_vec = vector<vector<double>>(2);
        temp_sPH_nocolumn_vec.clear(); temp_sPH_nocolumn_vec = vector<vector<double>>(4);
        temp_sPH_inner_nocolumn_vec.clear();
        temp_sPH_outer_nocolumn_vec.clear();
    } // note : end of event 

    if (print_event_display) {c2 -> Print(Form("%s/folder_%s_cosmic/temp_event_display_fit.pdf)",mother_folder_directory.c_str(),file_name.c_str()));}
    c2 -> Clear();
    c1 -> Clear();
    

    // c1 -> cd();
    // // vector<float> avg_event_zvtx_info = sigmaEff_avg(avg_event_zvtx_vec,Integrate_portion);
    // avg_event_zvtx -> Draw("hist");
    // effi_sig_range_line -> DrawLine(avg_event_zvtx_info[1],0,avg_event_zvtx_info[1],avg_event_zvtx -> GetMaximum()*1.05);
    // effi_sig_range_line -> DrawLine(avg_event_zvtx_info[2],0,avg_event_zvtx_info[2],avg_event_zvtx -> GetMaximum()*1.05);    
    // draw_text -> DrawLatex(0.15, 0.84, Form("EffiSig min : %.2f mm, max : %.2f mm",avg_event_zvtx_info[1],avg_event_zvtx_info[2]));
    // draw_text -> DrawLatex(0.15, 0.81, Form("EffiSig avg : %.2f mm",avg_event_zvtx_info[0]));
    // c1 -> Print(Form("%s/folder_%s_cosmic/avg_event_zvtx.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    // c1 -> Clear();

    // N_cluster_inner_pass -> Draw("hist"); 
    // c1 -> Print(Form("%s/folder_%s_cosmic/N_cluster_inner_pass.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    // c1 -> Clear();

    // N_cluster_outer_pass -> Draw("hist");
    // c1 -> Print(Form("%s/folder_%s_cosmic/N_cluster_outer_pass.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    // c1 -> Clear();

    // N_cluster_correlation -> Draw("colz0");
    // c1 -> Print(Form("%s/folder_%s_cosmic/N_cluster_correlation.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    // c1 -> Clear();

    // DCA_point -> Draw("colz0");
    // c1 -> Print(Form("%s/folder_%s_cosmic/DCA_point_X%.1fY%.1f_.pdf",mother_folder_directory.c_str(),file_name.c_str(),beam_origin.first,beam_origin.second));
    // c1 -> Clear();

    // DCA_distance_inner_phi -> Draw("colz0");
    // c1 -> Print(Form("%s/folder_%s_cosmic/DCA_distance_inner_phi_X%.1fY%.1f_.pdf",mother_folder_directory.c_str(),file_name.c_str(),beam_origin.first,beam_origin.second));
    // c1 -> Clear();

    // DCA_distance_outer_phi -> Draw("colz0");
    // c1 -> Print(Form("%s/folder_%s_cosmic/DCA_distance_outer_phi_X%.1fY%.1f_.pdf",mother_folder_directory.c_str(),file_name.c_str(),beam_origin.first,beam_origin.second));
    // c1 -> Clear();

    // z_pos_inner_outer -> Draw("colz0");
    // c1 -> Print(Form("%s/folder_%s_cosmic/z_pos_inner_outer.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    // c1 -> Clear();

    // z_pos_inner -> Draw("hist");
    // c1 -> Print(Form("%s/folder_%s_cosmic/z_pos_inner.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    // c1 -> Clear();

    // z_pos_outer -> Draw("hist");
    // c1 -> Print(Form("%s/folder_%s_cosmic/z_pos_outer.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    // c1 -> Clear();

    // Nhits_good -> Draw("hist");
    // c1 -> Print(Form("%s/folder_%s_cosmic/Nhits_good.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    // c1 -> Clear();

    // angle_correlation -> Draw("colz0");
    // c1 -> Print(Form("%s/folder_%s_cosmic/angle_correlation.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    // c1 -> Clear();

    // z_pos_diff -> Draw("hist");
    // c1 -> Print(Form("%s/folder_%s_cosmic/z_pos_diff.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    // c1 -> Clear();

    // z_pos_diff_angle_diff -> Draw("colz0");
    // c1 -> Print(Form("%s/folder_%s_cosmic/z_pos_diff_angle_diff.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    // c1 -> Clear();

    // inner_pos_xy -> Draw("colz0");
    // c1 -> Print(Form("%s/folder_%s_cosmic/inner_pos_xy.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    // c1 -> Clear();

    // outer_pos_xy -> Draw("colz0");
    // c1 -> Print(Form("%s/folder_%s_cosmic/outer_pos_xy.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    // c1 -> Clear();

    
    
    inner_outer_pos_xy_post_track_pZ -> Draw("colz0");
    c1 -> Print(Form("%s/folder_%s_cosmic/inner_outer_pos_xy_post_track_pZ.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    c1 -> Clear();

    inner_outer_pos_xy_post_track_nZ -> Draw("colz0");
    c1 -> Print(Form("%s/folder_%s_cosmic/inner_outer_pos_xy_post_track_nZ.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    c1 -> Clear();

    angle_inner_outer_post_track -> Draw("hist");
    c1 -> Print(Form("%s/folder_%s_cosmic/angle_inner_outer_post_track.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    c1 -> Clear();
    
    inner_outer_pos_rz_post_track -> Draw("colz0");
    c1 -> Print(Form("%s/folder_%s_cosmic/inner_outer_pos_rz_post_track.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    c1 -> Clear();

    Rchi2_rZ_hist -> Draw("hist");
    c1 -> Print(Form("%s/folder_%s_cosmic/Rchi2_rZ_hist.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    c1 -> Clear();

    Rchi2_xy_hist -> Draw("hist");
    c1 -> Print(Form("%s/folder_%s_cosmic/Rchi2_xy_hist.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    c1 -> Clear();
    
    angle_inner -> Draw("hist");
    c1 -> Print(Form("%s/folder_%s_cosmic/angle_inner.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    c1 -> Clear();

    angle_outer -> Draw("hist");
    c1 -> Print(Form("%s/folder_%s_cosmic/angle_outer.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    c1 -> Clear();

    inner_outer_pos_xy -> Draw("colz0");
    c1 -> Print(Form("%s/folder_%s_cosmic/inner_outer_pos_xy.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    c1 -> Clear();

    cosmic_phi_hist -> Draw("hist");
    c1 -> Print(Form("%s/folder_%s_cosmic/cosmic_phi_hist.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    c1 -> Clear();

    cosmic_z_hist -> Draw("hist");
    c1 -> Print(Form("%s/folder_%s_cosmic/cosmic_z_hist.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    c1 -> Clear();

    c1 -> Print(Form("%s/folder_%s_cosmic/Rchi2_xy_phi.pdf(",mother_folder_directory.c_str(),file_name.c_str()));
    for (int i = 0; i < Rchi2_xy_phi.size(); i++)
    {
        Rchi2_xy_phi[i] -> Draw("colz0");
        c1 -> Print(Form("%s/folder_%s_cosmic/Rchi2_xy_phi.pdf",mother_folder_directory.c_str(),file_name.c_str()));
        c1 -> Clear();
    }
    c1 -> Print(Form("%s/folder_%s_cosmic/Rchi2_xy_phi.pdf)",mother_folder_directory.c_str(),file_name.c_str()));

    
    tree_out->SetDirectory(out_file);
    tree_out -> Write("", TObject::kOverwrite);
    cout<<"output file generated"<<endl;

    out_file -> Close();
}