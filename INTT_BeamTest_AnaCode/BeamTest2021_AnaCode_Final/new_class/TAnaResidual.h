#ifndef tanaresidual_h
#define tanaresidual_h

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

class TAnaResidual
{
    public :
        TAnaResidual(TString class_name, int Initialize_mode, TString folder_direction, TString cluster_file_name, int study_chip, double l1_alignment_correction, double l0l2_slope_correction, double l0l1_slope_correction);
        
        TString GetClassName();

        void cluster_read_and_build();
        void cluster_reformat(); // note : L1 "single" column offset correction
        void cluster_reformat_after();
        // void cluster_reformat_all_column_correct (); // note : L1 "all columns" offset correction

        void   Alignment_DUT(int boundary_cut);
        double Get_l1_alignment(bool print_plots);
        double Get_l0l2_slope(bool print_plots, double slope_cut);
        void Gen_l0l2_slope_pos();

        void Pre_Get_l0l1_slope(double cluster_adc_value_requirement, double pos_cut, double slope_cut, int boundary_cut);
        double Get_l0l1_slope(bool print_plots, double slope_cut);
        void Gen_l0l1_slope_pos();
        TH1F * Gen_l0l1_scattering(int N_bin, double L_edge, double R_edge, bool print_plots);
        TH1F * Gen_l0l1_residual(int N_bin, double L_edge, double R_edge, bool print_plots);



        void Par_Set_l1_alignment(double input_value);
        void Par_Set_l0l2_slope(double input_value);
        void Par_Set_l0l1_slope(double input_value);
        double Par_Get_l1_alignment();
        double Par_Get_l0l2_slope();
        double Par_Get_l0l1_slope();
        TH1F * Get_l1_alignment_hist();
        TH1F * Get_l0l2_slope_hist();
        TH1F * Get_l0l1_slope_hist();

        // note : adjacent_requirement_mode = 0 : Only require the adjacent chips can not have cluster
        // note : adjacent_requirement_mode = 1 : check that only the selected chip can have the cluster, and N_cluster_l0 and N_cluster_l1 can only have 1 cluster for each
        void Get_l1_efficiency(double adjacent_requirement_mode, double slope_cut, double noise_hit_distance, int boundary_cut, double cluster_adc_value_requirement);
        // void Get_residual_dist(int N_bin, double L_edge, double R_edge, bool print_plots);
        // void Get_scattering_dist(int N_bin, double L_edge, double R_edge, bool print_plots);

    private : 
        // note : private function 
        void Characterize_Pad(TPad *pad, float left, float right, float top, float bottom, int logY, int setgrid_bool);
        void Characterize_Rate1F(TH1F *hist,  int hcolour);
        void Characterize_Hist1F(TH1F *hist,  int statsbox, TString color_ID);

        // note : private constant
        TString color_code[8]   = {"#343434","#1A3947","#566575","#797983","#EFBD9D","#FCA26E","#F5751D","#F5321D"};
        TString color_code_2[8] = {"#CC768D","#19768D","#DDA573","#009193","#6E9193","#941100","#A08144","#517E66"};

        double INTT_strip_width = 0.078;
        double lower_section_initial = -9.945;
        double upper_section_initial = 0.039;

        // note : the actual ladder position (unit : mm), the value is no longer to be 26.1, 
        // note : since 26.1 is the gap between ladders, without the consideration of ladder thickness
        double actual_xpos[3] = {0,29.552,59.104};

        vector<cluster_str> vec_cluster_read_and_build;
        vector<cluster_reformat_str> vec_cluster_reformat;
        vector<cluster_reformat_str> vec_cluster_reformat_after;
        DUT_str correction_data;
        DUT_str l0l1_correction_data; // note : for the l0l1 slope, and l0l1 mid. pos

        // note : the variables that initialized in the constructor
        TString class_name;
        int Initialize_mode;
        TString folder_direction;
        TString cluster_file_name;
        int study_chip;
        double l1_alignment_correction;
        double l0l2_slope_correction;
        double l0l1_slope_correction;
        int run_l0_alignment_tag;
        int run_l0l2_slope_tag;
        int run_l0l1_slope_tag;

        // note : for the scan comparison
        TH1F * l1_alignment_hist;
        TH1F * l0l2_slope_hist;
        TH1F * l0l1_slope_hist;

};

TAnaResidual::TAnaResidual( TString class_name, int Initialize_mode, TString folder_direction, TString cluster_file_name, int study_chip, double l1_alignment_correction = 0, double l0l2_slope_correction = 0, double l0l1_slope_correction = 0 )
:class_name( class_name ),
Initialize_mode(Initialize_mode),
folder_direction( folder_direction ),
cluster_file_name( cluster_file_name ),
study_chip( study_chip ),
l1_alignment_correction( l1_alignment_correction ),
l0l2_slope_correction( l0l2_slope_correction ),
l0l1_slope_correction( l0l1_slope_correction )
{

    vec_cluster_read_and_build.clear();
    vec_cluster_reformat.clear();
    vec_cluster_reformat_after.clear();

    run_l0_alignment_tag = 0;
    run_l0l2_slope_tag = 0;
    run_l0l1_slope_tag = 0;

    if ( Initialize_mode == 0 )
    {
        cout<<"Now you are using the initialize mode : 1"<<endl;
        cout<<"The misalignment correction will be applied to single column of L1"<<endl;

        // note : Now the amount of the alignment correction can be given here, don't need to reproduce a new file.
        TAnaResidual::cluster_read_and_build();   
        TAnaResidual::cluster_reformat();

        cout<<"the class : "<<class_name<<" has been initiated by using the mode 1 "<<endl;
    }

    cout<<" "<<endl;
    cout<<" "<<endl;
    // else if ( Initialize_mode == 2 )
    // {
    //     cout<<"Now you are using the initialize mode : 2"<<endl;
    //     cout<<"The misalignment correction is applied to all columns of L1"<<endl;
    //     // note : Now the amount of the alignment correction can be given here, don't need to reproduce a new file.
    //     TAnaResidual::cluster_read_and_build();   
    //     cluster_reformat_str_vec_predefined = TAnaResidual::cluster_reformat_all_column_correct(cluster_str_vec_predefined);

    //     cout<<"the class : "<<class_name<<" has been initiated by using the mode 2 "<<endl;
    // }
}

TString TAnaResidual::GetClassName()
{   
    return class_name;
}

void TAnaResidual::Par_Set_l1_alignment(double input_value)
{
    l1_alignment_correction = input_value;
}

double TAnaResidual::Par_Get_l1_alignment()
{
    return l1_alignment_correction;
}

void TAnaResidual::Par_Set_l0l2_slope(double input_value)
{
    l0l2_slope_correction = input_value;
}

double TAnaResidual::Par_Get_l0l2_slope()
{
    return l0l2_slope_correction;
}

void TAnaResidual::Par_Set_l0l1_slope(double input_value)
{
    l0l1_slope_correction = input_value;
}

double TAnaResidual::Par_Get_l0l1_slope()
{
    return l0l1_slope_correction;
}

TH1F * TAnaResidual::Get_l1_alignment_hist()
{
    return l1_alignment_hist;
}

TH1F * TAnaResidual::Get_l0l2_slope_hist()
{
    return l0l2_slope_hist;
}

TH1F * TAnaResidual::Get_l0l1_slope_hist()
{
    return l0l1_slope_hist;
}


void TAnaResidual::Characterize_Pad (TPad *pad, float left = 0.2, float right = 0.05, float top = 0.1, float bottom = 0.15, int logY = 0, int setgrid_bool = 1)
{
	if (setgrid_bool == true) {pad -> SetGrid (1, 1);}
	pad -> SetLeftMargin   (left);
	pad -> SetRightMargin  (right);
	pad -> SetTopMargin    (top);
	pad -> SetBottomMargin (bottom);
	pad -> SetLogy (logY);
}

void TAnaResidual::Characterize_Rate1F (TH1F *hist,  int hcolour)
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

void TAnaResidual::Characterize_Hist1F (TH1F *hist,  int statsbox = 0, TString color_ID = "#1A3947")
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



void TAnaResidual::cluster_read_and_build()
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

    cluster_str fill_unit;

    fill_unit.cluster_layer.clear();
    fill_unit.cluster_chip.clear();
    fill_unit.cluster_hit.clear();
    fill_unit.cluster_position.clear();
    fill_unit.cluster_adc.clear();

    for (int i = 0; i < cluster_tree_size; i++)
    {
        cluster_tree -> GetEntry(i);

        vec_cluster_read_and_build.push_back(fill_unit);

        vec_cluster_read_and_build[vec_cluster_read_and_build.size()-1].cluster_eID = cluster_eID;
        for ( int i1 = 0; i1 < cluster_layer->size(); i1++ )
        {
            vec_cluster_read_and_build[vec_cluster_read_and_build.size()-1].cluster_layer.push_back(cluster_layer->at(i1));
            vec_cluster_read_and_build[vec_cluster_read_and_build.size()-1].cluster_chip.push_back(cluster_chip->at(i1));
            vec_cluster_read_and_build[vec_cluster_read_and_build.size()-1].cluster_hit.push_back(cluster_hit->at(i1));
            vec_cluster_read_and_build[vec_cluster_read_and_build.size()-1].cluster_position.push_back(cluster_position->at(i1));
            vec_cluster_read_and_build[vec_cluster_read_and_build.size()-1].cluster_adc.push_back(cluster_adc->at(i1));
        }


        // cout<<"-------------- -------------- -------------- -------------- --------------"<<endl;
        // if (1==1) {cout<<i<<" event check : "<<cluster_chip->size()<<endl;}
        // if (1==1) {cout<<i<<" event check : "<< vec_cluster_read_and_build[0].cluster_chip.size()<< " "<<vec_cluster_read_and_build[0].cluster_position[0] <<endl;}

    }

    
    // cout<<"event check out but in : "<< vec_cluster_read_and_build[0].cluster_chip.size() <<endl;
    cout<<"cluster size check : "<<vec_cluster_read_and_build.size()<<endl;

    cout<<" "<<endl;
    cout<<" "<<endl;
}

// note : l1_alignment is given here 
// note : change the structure of the data and apply the single column offset
void TAnaResidual::cluster_reformat()
{

    cluster_reformat_str fill_unit;

    for (int i = 0; i < 13; i++)
    {
        fill_unit.cluster_hit[0][i].clear(); fill_unit.cluster_hit[1][i].clear(); fill_unit.cluster_hit[2][i].clear();
        fill_unit.cluster_pos[0][i].clear(); fill_unit.cluster_pos[1][i].clear(); fill_unit.cluster_pos[2][i].clear();
        fill_unit.cluster_adc[0][i].clear(); fill_unit.cluster_adc[1][i].clear(); fill_unit.cluster_adc[2][i].clear();
    }

    for (int i = 0; i < vec_cluster_read_and_build.size(); i++ )
    {
        vec_cluster_reformat.push_back(fill_unit);
        
        // note : fill the eID
        vec_cluster_reformat[ vec_cluster_reformat.size() - 1 ].eID = vec_cluster_read_and_build[i].cluster_eID;
        
        for (int i1 = 0; i1 < vec_cluster_read_and_build[i].cluster_layer.size(); i1++)
        {
            vec_cluster_reformat[ vec_cluster_reformat.size() - 1 ].cluster_hit[ vec_cluster_read_and_build[i].cluster_chip[i1] - 1 ][ vec_cluster_read_and_build[i].cluster_layer[i1] ].push_back( vec_cluster_read_and_build[i].cluster_hit[i1] );
            vec_cluster_reformat[ vec_cluster_reformat.size() - 1 ].cluster_adc[ vec_cluster_read_and_build[i].cluster_chip[i1] - 1 ][ vec_cluster_read_and_build[i].cluster_layer[i1] ].push_back( vec_cluster_read_and_build[i].cluster_adc[i1] );
            
            if (vec_cluster_read_and_build[i].cluster_chip[i1] == study_chip && vec_cluster_read_and_build[i].cluster_layer[i1] == 1) // note : the study_chip of the middle layer is aligned.
            {
                vec_cluster_reformat[ vec_cluster_reformat.size() - 1 ].cluster_pos[ vec_cluster_read_and_build[i].cluster_chip[i1] - 1 ][ vec_cluster_read_and_build[i].cluster_layer[i1] ].push_back( vec_cluster_read_and_build[i].cluster_position[i1] + l1_alignment_correction);
            }
            else 
            {
                vec_cluster_reformat[ vec_cluster_reformat.size() - 1 ].cluster_pos[ vec_cluster_read_and_build[i].cluster_chip[i1] - 1 ][ vec_cluster_read_and_build[i].cluster_layer[i1] ].push_back( vec_cluster_read_and_build[i].cluster_position[i1] );
            }
            
        }
    }

    cout<<"reformat output check : "<<vec_cluster_reformat.size()<<endl;

    cout<<" "<<endl;
    cout<<" "<<endl;
    
}

// note : l1_alignment is given here 
// note : change the structure of the data and apply the single column offset
void TAnaResidual::cluster_reformat_after()
{

    if (run_l0_alignment_tag == 0)
    {
        vec_cluster_reformat_after = vec_cluster_reformat; 
        vec_cluster_reformat.clear();
        cout<<"vec_cluster_reformat_after size check : "<<vec_cluster_reformat_after.size()<<endl;
        cout<<"alignment seems to have been given already, the alignment tag is 0"<<endl;
        cout<<"the one with \"_after\" repalces the original one"<<endl;
        return 0;
    }
    
    cout<<"the alignment tag is 1, new alignment value is given, run the apply code"<<endl;

    cluster_reformat_str fill_unit;

    for (int i = 0; i < 13; i++)
    {
        fill_unit.cluster_hit[0][i].clear(); fill_unit.cluster_hit[1][i].clear(); fill_unit.cluster_hit[2][i].clear();
        fill_unit.cluster_pos[0][i].clear(); fill_unit.cluster_pos[1][i].clear(); fill_unit.cluster_pos[2][i].clear();
        fill_unit.cluster_adc[0][i].clear(); fill_unit.cluster_adc[1][i].clear(); fill_unit.cluster_adc[2][i].clear();
    }

    for (int i = 0; i < vec_cluster_read_and_build.size(); i++ )
    {
        vec_cluster_reformat_after.push_back(fill_unit);
        
        // note : fill the eID
        vec_cluster_reformat_after[ vec_cluster_reformat_after.size() - 1 ].eID = vec_cluster_read_and_build[i].cluster_eID;
        
        for (int i1 = 0; i1 < vec_cluster_read_and_build[i].cluster_layer.size(); i1++)
        {
            vec_cluster_reformat_after[ vec_cluster_reformat_after.size() - 1 ].cluster_hit[ vec_cluster_read_and_build[i].cluster_chip[i1] - 1 ][ vec_cluster_read_and_build[i].cluster_layer[i1] ].push_back( vec_cluster_read_and_build[i].cluster_hit[i1] );
            vec_cluster_reformat_after[ vec_cluster_reformat_after.size() - 1 ].cluster_adc[ vec_cluster_read_and_build[i].cluster_chip[i1] - 1 ][ vec_cluster_read_and_build[i].cluster_layer[i1] ].push_back( vec_cluster_read_and_build[i].cluster_adc[i1] );
            
            if (vec_cluster_read_and_build[i].cluster_chip[i1] == study_chip && vec_cluster_read_and_build[i].cluster_layer[i1] == 1) // note : the study_chip of the middle layer is aligned.
            {
                vec_cluster_reformat_after[ vec_cluster_reformat_after.size() - 1 ].cluster_pos[ vec_cluster_read_and_build[i].cluster_chip[i1] - 1 ][ vec_cluster_read_and_build[i].cluster_layer[i1] ].push_back( vec_cluster_read_and_build[i].cluster_position[i1] + l1_alignment_correction);
            }
            else 
            {
                vec_cluster_reformat_after[ vec_cluster_reformat_after.size() - 1 ].cluster_pos[ vec_cluster_read_and_build[i].cluster_chip[i1] - 1 ][ vec_cluster_read_and_build[i].cluster_layer[i1] ].push_back( vec_cluster_read_and_build[i].cluster_position[i1] );
            }
            
        }
    }

    vec_cluster_reformat.clear();
    cout<<"reformat output check : "<<vec_cluster_reformat_after.size()<<endl;

    cout<<" "<<endl;
    cout<<" "<<endl;
    
}

// note : slope is given here 
void TAnaResidual::Alignment_DUT(int boundary_cut = 5)
{ 
    if (l1_alignment_correction != 0. && l0l2_slope_correction != 0.)
    {
        cout<<"l1 alignment : "<<l1_alignment_correction<<endl;
        cout<<"l0l2 slope correction : "<<l0l2_slope_correction<<endl;
        cout<<"the \"l1 alignment \" and \" l0l2 slope correction \" are not zero, running this function"<<endl;
    }

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


    for (int i = 0; i < vec_cluster_reformat.size(); i++)
    {
        if (i % 10000 == 0){ cout<<"Doing the DUT test, eID : "<<vec_cluster_reformat[i].eID<<endl;}

        // note : abbreviation
        for (int i1 = 0; i1 < 13; i1++)
        {
            for (int i2 = 0; i2 < 3; i2++)
            {
                receiver_unit_clu_size[i1][i2] = vec_cluster_reformat[i].cluster_hit[ i1 ][i2];
                receiver_unit_clu_pos[i1][i2] = vec_cluster_reformat[i].cluster_pos[ i1 ][i2];
                receiver_unit_clu_adc[i1][i2] = vec_cluster_reformat[i].cluster_adc[ i1 ][i2];

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
        if (receiver_unit_clu_size[study_chip-1][0].size() != 1 || receiver_unit_clu_size[study_chip-1][2].size() != 1) continue;
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

        hit2_slope = (receiver_unit_clu_pos[study_chip-1][2][0] - receiver_unit_clu_pos[study_chip-1][0][0]) / actual_xpos[2] + l0l2_slope_correction;
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
            // cout<<"101 event, eID : "<<vec_cluster_reformat[i].eID<<" -> no cluster"<<endl;
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

            // if ( fabs(event_residual) < noise_hit_distance )
            // {
            //     event_counting_11 += 1;

            //     final_event_result.push_back(1); // note : in order to make the detection efficiency as function of pos.

            //     track_111_count += 1;
            // }
            // else 
            // {
            //     event_counting_12 += 1;

            //     final_event_result.push_back(2); // note : in order to make the detection efficiency as function of pos.

            //     track_101_count += 1;   
            //     // cout<<"101 event, eID : "<<vec_cluster_reformat[i].eID<<" -> middle has cluster"<<endl;
            // }

        }
        // note : start clean

    } // note : end of for loop, event


    correction_data.track_111_count = track_111_count;
    correction_data.track_101_count = track_101_count;
    correction_data.middle_layer_residual = middle_layer_residual;
    correction_data.good_combination_slope_hit3 = good_combination_slope_hit3; // note : empty, 2022/12/03
    correction_data.good_combination_slope_hit2 = good_combination_slope_hit2; // note : 3hits or 2hits cases are included
    correction_data.good_combination_l0_pos_hit3 = good_combination_l0_pos_hit3; 
    correction_data.good_combination_l1_pos_hit3 = good_combination_l1_pos_hit3;
    correction_data.good_combination_l2_pos_hit3 = good_combination_l2_pos_hit3;
    
    // note : in order to make the plot of the detection efficiency as a function of position. 
    correction_data.passed_middle_pos = passed_middle_pos;
    correction_data.final_event_result = final_event_result;

    cout<<" "<<endl;
    cout<<" "<<endl;
}

double TAnaResidual::Get_l1_alignment(bool print_plots)
{
    if (l1_alignment_correction != 0.)
    {   
        cout<<"the given alignment is "<<l1_alignment_correction<<endl;
        cout<<"the l1 alignment is given, not running the function"<<endl;
        return 0;
    }

    run_l0_alignment_tag  = 1;

    TCanvas * c1 = new TCanvas("c1","c1",850,800);
    c1 -> cd();

    TPad * pad_c1 = new TPad(Form("pad_c1"), "", 0.0, 0.0, 1.0, 1.0);
    TAnaResidual::Characterize_Pad(pad_c1, 0.15,  0.1,  0.1,  0.12, 0, 0);
    pad_c1 -> SetTicks(1,1);
    pad_c1 -> Draw();

    TF1 * gaus_fit = new TF1 ("gaus_fit","gaus",-1,1);
		
    // TF1 * D_gaus_fit = new TF1 ("D_gaus_fit",double_gaus,-10,10,5);
    // note : par[0] could be 10000 for data
    // note : par[0] could be 40000 for MC
    // D_gaus_fit -> SetParameters(10000,0.987,0,0.03,0.1);
    // D_gaus_fit -> SetParLimits  (1, 0.5,      1); // fraction
    // D_gaus_fit -> SetParLimits  (3, 0.025,      1); // note : ON for MC
    // D_gaus_fit -> SetParLimits  (1, 0.97,      1);  // note : ON for MC
    // D_gaus_fit -> SetLineColor(TColor::GetColor("#F5751D"));

    TH1F * l1_residual_hist = new TH1F("","",50,-0.4,1);
    l1_residual_hist -> SetStats(0);
    for (int i = 0; i < correction_data.middle_layer_residual.size(); i++)
    {
        l1_residual_hist -> Fill(correction_data.middle_layer_residual[i]);
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
    // l1_residual_hist -> Fit(D_gaus_fit,"N","",-1,1);
    // bool run_fit_effisig = double_gaus_getsigmarange (D_gaus_fit,the_portion,D_gaus_xmin,D_gaus_xmax);
    
    gaus_fit -> SetNpx(10000);
    // D_gaus_fit -> SetNpx(10000);

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
    
    l1_alignment_hist = l1_residual_hist;

    gaus_fit_latex -> Draw("same");

    if (print_plots == true)
    {
        c1 -> Print( Form("%s/DUT_align_narrow_U%i_pub.pdf",folder_direction.Data(),study_chip) );
        c1 -> Clear();
    }


    cout<<"=============================Alignment narrow information==============================="<<endl;
    cout<<"chip : "<<study_chip<<endl;
    cout<<"fit mean : "<<gaus_fit -> GetParameter(1)<<" width : "<<gaus_fit -> GetParameter(2)<<endl;
    cout<<"fit mean error : "<<gaus_fit->GetParError(1)<<endl;
    cout<<"CHi2 : "<<gaus_fit->GetChisquare()<<" NDF "<<gaus_fit->GetNDF()<<" = "<<gaus_fit->GetChisquare()/double(gaus_fit->GetNDF())<<endl;
    cout<<"stat mean : "<<l1_residual_hist -> GetMean()<<" stat width : "<<l1_residual_hist -> GetStdDev()<<endl;
    cout<<"stat entry : "<<l1_residual_hist -> GetEntries()<<endl;
    cout<<"=============================Alignment narrow information==============================="<<endl;

    cout<<"Final l1 alignment : "<<TString(Form("%.4f",gaus_fit -> GetParameter(1))).Data()<<endl;

    cout<<" "<<endl;
    cout<<" "<<endl;

    return -1 * stod(TString(Form("%.4f",gaus_fit -> GetParameter(1))).Data()); // note : the fit mean of the distribution

}
double TAnaResidual::Get_l0l2_slope(bool print_plots, double slope_cut = 0.01)
{
    cout<<" "<<endl;
    cout<<" "<<endl;
    cout<<"running function Get_l0l2_slope"<<endl;

    if (l0l2_slope_correction != 0.)
    {   
        cout<<"the given l0l2 slope correction is "<<l0l2_slope_correction<<endl;
        cout<<"the l0l2 slope correction is given"<<endl;
        // return 0;
    }

    run_l0l2_slope_tag  = 1;
    
    TCanvas * c1 = new TCanvas("c1","c1",850,800);
    c1 -> cd();

    TPad * pad_c1 = new TPad(Form("pad_c1"), "", 0.0, 0.0, 1.0, 1.0);
    TAnaResidual::Characterize_Pad(pad_c1, 0.15,  0.1,  0.1,  0.12, 0, 0);
    pad_c1 -> Draw();
    pad_c1 -> SetTicks(1,1);
    pad_c1 -> cd();

    cout<<"--------------------------------------------------------------------------------------------------------"<<endl;
    cout<<"----- Now we are doing the plot_angle "<<endl;
    cout<<"correction_data.good_combination_slope_hit2 size : "<<correction_data.good_combination_slope_hit2.size()<<endl;
    cout<<"--------------------------------------------------------------------------------------------------------"<<endl;

    TH1F * angle_hist = new TH1F("","",50,-0.05,0.05);

    angle_hist -> SetLineWidth(3);
    angle_hist -> SetLineColor(TColor::GetColor("#1A3947")); 

    angle_hist -> GetXaxis() -> SetNdivisions  (505);

    angle_hist -> GetXaxis() -> SetTitleSize   (0.05);
	angle_hist -> GetXaxis() -> SetTitleOffset (0.8);

	angle_hist -> GetYaxis() -> SetTitleSize   (0.05);
	angle_hist -> GetYaxis() -> SetTitleOffset (1.5);

    for (int i = 0; i < correction_data.good_combination_slope_hit2.size(); i++)
    {
        angle_hist -> Fill(correction_data.good_combination_slope_hit2[i]);
    }

    // angle_hist -> SetTitle(Form("DUT candidate track slope, U%i",study_chip));
    angle_hist -> SetStats(0);
    angle_hist -> GetXaxis() -> SetTitle("Track slope");
    angle_hist -> GetYaxis() -> SetTitle("Events");

    angle_hist -> Draw("hist");

    l0l2_slope_hist = angle_hist;

    TF1 * gaus_fit = new TF1 ("gaus_fit","gaus",-0.1,0.1);
    gaus_fit -> SetNpx(10000);

    angle_hist -> Fit("gaus_fit","NQ");


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


    TLegend *legend1 = new TLegend (0.62, 0.7, 0.85, 0.78);
	legend1 -> SetTextSize (0.03);
	// legend1 -> SetNColumns (4);
    legend1 -> SetBorderSize(0);
    legend1 -> AddEntry (slope_cut_positive, Form("Slope cut %.2f",slope_cut),  "l");


    slope_cut_negative -> Draw("l same");
    slope_cut_positive -> Draw("l same");

    legend1 -> Draw("l same");

    TString output_file_name = Form("DUT_candidate_track_slope_U%i",study_chip);
    output_file_name += ( l0l2_slope_correction == 0. ) ? "_before" : "_after";

    if (print_plots == true)
    {
        c1 -> Print( Form("%s/%s_nofitting.pdf",folder_direction.Data(),output_file_name.Data()) );
        
        
        gaus_fit_latex -> DrawLatex(0.12+0.05, 0.750, Form("Mean :  %.8f ", gaus_fit->GetParameter(1)));
        gaus_fit_latex -> DrawLatex(0.12+0.05, 0.710, Form("Width : %.4f", gaus_fit->GetParameter(2)));
        gaus_fit_latex -> DrawLatex(0.12+0.05, 0.670, Form("#chi^{2} : %.2f, NDF : %d", gaus_fit->GetChisquare(),gaus_fit->GetNDF()));
        gaus_fit_latex -> DrawLatex(0.12+0.05, 0.630, Form("#chi^{2}/NDF : %.4f",gaus_fit->GetChisquare()/double(gaus_fit->GetNDF())));

        gaus_fit->Draw("lsame");
        
        c1 -> Print( Form("%s/%s.pdf",folder_direction.Data(),output_file_name.Data()) );

        c1 -> Clear();
    }
    
    cout<<"Final l0l2 slope : "<<TString(Form("%.6f",gaus_fit -> GetParameter(1))).Data()<<endl;
    
    cout<<" "<<endl;
    cout<<" "<<endl;

    return -1 * stod(TString(Form("%.6f",gaus_fit -> GetParameter(1))).Data());
}

void TAnaResidual::Get_l1_efficiency(double adjacent_requirement_mode = 0, double slope_cut = 0.01, double noise_hit_distance = 0.234, int boundary_cut = 5, double cluster_adc_value_requirement = 22.5)
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


    for (int i = 0; i < vec_cluster_reformat_after.size(); i++)
    {
        if (i % 1000 == 0){ cout<<"Doing the DUT test, eID : "<<vec_cluster_reformat_after[i].eID<<endl;}

        // note : abbreviation
        for (int i1 = 0; i1 < 13; i1++)
        {
            for (int i2 = 0; i2 < 3; i2++)
            {
                receiver_unit_clu_size[i1][i2] = vec_cluster_reformat_after[i].cluster_hit[ i1 ][i2];
                receiver_unit_clu_pos[i1][i2] = vec_cluster_reformat_after[i].cluster_pos[ i1 ][i2];
                receiver_unit_clu_adc[i1][i2] = vec_cluster_reformat_after[i].cluster_adc[ i1 ][i2];

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
        if (receiver_unit_clu_size[study_chip-1][0].size() != 1 || receiver_unit_clu_size[study_chip-1][2].size() != 1) continue;
        event_counting_3 += 1;

        // // note : standalone cluster size cut of l0 and l2
        // if ( receiver_unit_clu_size[study_chip-1][0][0] <= cluster_size_requirement || receiver_unit_clu_size[study_chip-1][2][0] <= cluster_size_requirement ) continue;
        // event_counting_4 += 1;

        // note : standalone cluster adc cut
        if ( receiver_unit_clu_adc[study_chip-1][0][0] <= cluster_adc_value_requirement || receiver_unit_clu_adc[study_chip-1][2][0] <= cluster_adc_value_requirement ) continue;
        event_counting_5 += 1;

        // // note : combined cluster size cut
        // if ( receiver_unit_clu_size[study_chip-1][0][0]+receiver_unit_clu_size[study_chip-1][2][0] <= cluster_size_requirement_combine ) continue;
        // event_counting_6 += 1;


        // note : edge exclusion cut of the l0
        if ( receiver_unit_clu_pos[study_chip-1][0][0] <= edge_exclusion_bottom || receiver_unit_clu_pos[study_chip-1][0][0] >= edge_exclusion_upper ) continue;
        event_counting_7 += 1;

        // note : edge exclusion cut of the l2
        if ( receiver_unit_clu_pos[study_chip-1][2][0] <= edge_exclusion_bottom || receiver_unit_clu_pos[study_chip-1][2][0] >= edge_exclusion_upper ) continue;
        event_counting_8 += 1;

        hit2_slope = (receiver_unit_clu_pos[study_chip-1][2][0] - receiver_unit_clu_pos[study_chip-1][0][0]) / actual_xpos[2] + l0l2_slope_correction;
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
            cout<<"101 event, eID : "<<vec_cluster_reformat_after[i].eID<<" -> no cluster"<<endl;
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
                cout<<"101 event, eID : "<<vec_cluster_reformat_after[i].eID<<" -> middle has cluster"<<endl;
            }

        }
        // note : start clean

        chi2_register = 10000000;

    } // note : end of for loop, event


    // DUT_str output_space;
    // output_space.track_111_count = track_111_count;
    // output_space.track_101_count = track_101_count;
    // output_space.middle_layer_residual = middle_layer_residual;
    // output_space.good_combination_slope_hit3 = good_combination_slope_hit3; // note : empty, 2022/12/03
    // output_space.good_combination_slope_hit2 = good_combination_slope_hit2; // note : 3hits or 2hits cases are included
    // output_space.good_combination_l0_pos_hit3 = good_combination_l0_pos_hit3; 
    // output_space.good_combination_l1_pos_hit3 = good_combination_l1_pos_hit3;
    // output_space.good_combination_l2_pos_hit3 = good_combination_l2_pos_hit3;
    
    // // note : in order to make the plot of the detection efficiency as a function of position. 
    // output_space.passed_middle_pos = passed_middle_pos;
    // output_space.final_event_result = final_event_result;

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


    cout<<"=========================== Detection efficiency ==================================="<<endl;
    cout<<"# of 111 : "<<track_111_count<<endl;
    cout<<"# of 101 : "<<track_101_count<<endl;
    cout<<"Efficiency : "<<double(track_111_count) / double(track_111_count + track_101_count)<<endl;


    TH1F * total_hist = new TH1F ("","",1,0,1);
    TH1F * pass_hist = new TH1F ("","",1,0,1);
    
    // note : layer - 1 efficiency
    total_hist->SetBinContent(1,(track_111_count + track_101_count)); 
    pass_hist ->SetBinContent(1,track_111_count);

    TEfficiency * detection_effi = new TEfficiency (*pass_hist,*total_hist);
    printf("Efficiency by TEfficiency \n");
    printf("%.5f\t+%.5f\t-%.5f \n", detection_effi->GetEfficiency(1)*100.,detection_effi->GetEfficiencyErrorUp(1)*100.,detection_effi->GetEfficiencyErrorLow(1)*100.);

    cout<<"=========================== Detection efficiency ==================================="<<endl;

}


void TAnaResidual::Pre_Get_l0l1_slope(double cluster_adc_value_requirement = 22.5, double pos_cut = 5, double slope_cut = 0.01, int boundary_cut = 5)
{

    // cout<<"hi hi hi I am here : "<<cluster_adc_value_requirement<<endl;

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


    for (int i = 0; i < vec_cluster_reformat_after.size(); i++)
    {
        if (i % 10000 == 0){ cout<<"Doing the DUT test, eID : "<<vec_cluster_reformat_after[i].eID<<endl;}

        // note : abbreviation
        for (int i1 = 0; i1 < 13; i1++)
        {
            for (int i2 = 0; i2 < 3; i2++)
            {
                receiver_unit_clu_size[i1][i2] = vec_cluster_reformat_after[i].cluster_hit[ i1 ][i2];
                receiver_unit_clu_pos[i1][i2] = vec_cluster_reformat_after[i].cluster_pos[ i1 ][i2];
                receiver_unit_clu_adc[i1][i2] = vec_cluster_reformat_after[i].cluster_adc[ i1 ][i2];
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
        if (receiver_unit_clu_size[study_chip-1][0].size() != 1 || receiver_unit_clu_size[study_chip-1][1].size() != 1 || receiver_unit_clu_size[study_chip-1][2].size() != 1) continue;
        event_counting_3 += 1;

        // note : standalone cluster adc cut
        if ( receiver_unit_clu_adc[study_chip-1][0][0] <= cluster_adc_value_requirement || receiver_unit_clu_adc[study_chip-1][1][0] <= cluster_adc_value_requirement || receiver_unit_clu_adc[study_chip-1][2][0] <= cluster_adc_value_requirement ) 
        {
            // cout<<"low adc : "<<receiver_unit_clu_adc[study_chip-1][0][0] <<" "<< receiver_unit_clu_adc[study_chip-1][1][0] <<" "<< receiver_unit_clu_adc[study_chip-1][2][0] <<endl;
            continue;
        }
        
        event_counting_4 += 1;


        l0_l1_slope = (receiver_unit_clu_pos[study_chip-1][1][0] - receiver_unit_clu_pos[study_chip-1][0][0]) / actual_xpos[1] + l0l1_slope_correction;
        l0_l1_slope_vec.push_back( l0_l1_slope );

        l0_l1_pos = ( receiver_unit_clu_pos[study_chip-1][0][0] + receiver_unit_clu_pos[study_chip-1][1][0] ) / 2.;
        l0_l1_pos_vec.push_back( l0_l1_pos );

        
        
        // note : the slope cut
        if ( fabs(l0_l1_slope) >= slope_cut ) continue;
        event_counting_5 += 1;

        // note : the position cut
        if ( fabs(l0_l1_pos) >= pos_cut ) continue;
        event_counting_6 += 1;

        

        // note : to check the N clusters of the middle layer
        // if ( receiver_unit_clu_pos[study_chip-1][1].size() == 0 ) // note : no hits in the middle
        // {
        //     event_counting_10 += 1;

        //     final_event_result.push_back(0); // note : in order to make the detection efficiency as function of pos.

        //     track_101_count += 1;
        //     cout<<"101 event, eID : "<<vec_cluster_reformat_after[i].eID<<" -> no cluster"<<endl;
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
        //         cout<<"101 event, eID : "<<vec_cluster_reformat_after[i].eID<<" -> middle has cluster"<<endl;
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


    // l0l1_correction_data.track_111_count = track_111_count;
    // l0l1_correction_data.track_101_count = track_101_count;
    l0l1_correction_data.middle_layer_residual = middle_layer_residual;
    // l0l1_correction_data.good_combination_slope_hit3 = good_combination_slope_hit3; // note : empty, 2022/12/03
    // l0l1_correction_data.good_combination_slope_hit2 = good_combination_slope_hit2; // note : 3hits or 2hits cases are included
    // l0l1_correction_data.good_combination_l0_pos_hit3 = good_combination_l0_pos_hit3; 
    // l0l1_correction_data.good_combination_l1_pos_hit3 = good_combination_l1_pos_hit3;
    // l0l1_correction_data.good_combination_l2_pos_hit3 = good_combination_l2_pos_hit3;
    
    // note : in order to make the plot of the detection efficiency as a function of position. 
    // l0l1_correction_data.passed_middle_pos = passed_middle_pos;
    // l0l1_correction_data.final_event_result = final_event_result;
    
    // note : for the scattering study, new attempt 
    l0l1_correction_data.l0_l1_pos_vec = l0_l1_pos_vec;
    l0l1_correction_data.l0_l1_slope_vec = l0_l1_slope_vec;
    l0l1_correction_data.scattering_vec = scattering_vec;

    cout<<"event counting 1 \t : "<<event_counting_1<<" all "<<endl;

    cout<<"event counting 2 \t : "<<event_counting_2<<" single cluster each layer "<<endl;    
    cout<<"event counting 3 \t : "<<event_counting_3<<" selected chip 1 cluster, all layers  "<<endl;
    cout<<"event counting 4 \t : "<<event_counting_4<<" cluster adc "<<endl;
    cout<<"event counting 5 \t : "<<event_counting_5<<" l0_l1_slope cut "<<endl;
    cout<<"event counting 6 \t : "<<event_counting_6<<" l0_l1_pos cut "<<endl;
    
}

double TAnaResidual::Get_l0l1_slope(bool print_plots, double slope_cut = 0.01)
{
    cout<<" "<<endl;
    cout<<" "<<endl;
    cout<<"running function Get_l0l1_slope"<<endl;

    if (l0l1_slope_correction != 0.)
    {   
        cout<<"the given l0l1 slope correction is "<<l0l1_slope_correction<<endl;
        cout<<"the l0l1 slope correction is given"<<endl;
        // return 0;
    }

    run_l0l1_slope_tag  = 1;
    
    TCanvas * c1 = new TCanvas("c1","c1",850,800);
    c1 -> cd();

    TPad * pad_c1 = new TPad(Form("pad_c1"), "", 0.0, 0.0, 1.0, 1.0);
    TAnaResidual::Characterize_Pad(pad_c1, 0.15,  0.1,  0.1,  0.12, 0, 0);
    pad_c1 -> Draw();
    pad_c1 -> SetTicks(1,1);
    pad_c1 -> cd();

    cout<<"--------------------------------------------------------------------------------------------------------"<<endl;
    cout<<"----- Now we are doing the plot_angle l0l1 "<<endl;
    cout<<"l0l1_correction_data.l0_l1_slope_vec size : "<<l0l1_correction_data.l0_l1_slope_vec.size()<<endl;
    cout<<"--------------------------------------------------------------------------------------------------------"<<endl;

    TH1F * angle_hist = new TH1F("","",50,-0.05,0.05);

    angle_hist -> SetLineWidth(3);
    angle_hist -> SetLineColor(TColor::GetColor("#1A3947")); 

    angle_hist -> GetXaxis() -> SetNdivisions  (505);

    angle_hist -> GetXaxis() -> SetTitleSize   (0.05);
	angle_hist -> GetXaxis() -> SetTitleOffset (0.8);

	angle_hist -> GetYaxis() -> SetTitleSize   (0.05);
	angle_hist -> GetYaxis() -> SetTitleOffset (1.5);

    for (int i = 0; i < l0l1_correction_data.l0_l1_slope_vec.size(); i++)
    {
        angle_hist -> Fill(l0l1_correction_data.l0_l1_slope_vec[i]);
    }

    // angle_hist -> SetTitle(Form("DUT candidate track slope, U%i",study_chip));
    angle_hist -> SetStats(0);
    angle_hist -> GetXaxis() -> SetTitle("Track slope");
    angle_hist -> GetYaxis() -> SetTitle("Events");

    angle_hist -> Draw("hist");

    l0l1_slope_hist = angle_hist;

    TF1 * gaus_fit = new TF1 ("gaus_fit","gaus",-0.1,0.1);
    gaus_fit -> SetNpx(10000);

    angle_hist -> Fit("gaus_fit","NQ");


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


    TLegend *legend1 = new TLegend (0.62, 0.7, 0.85, 0.78);
	legend1 -> SetTextSize (0.03);
	// legend1 -> SetNColumns (4);
    legend1 -> SetBorderSize(0);
    legend1 -> AddEntry (slope_cut_positive, Form("Slope cut %.2f",slope_cut),  "l");


    slope_cut_negative -> Draw("l same");
    slope_cut_positive -> Draw("l same");

    legend1 -> Draw("l same");

    TString output_file_name = Form("DUT_candidate_l0l1_track_slope_U%i",study_chip);
    output_file_name += ( l0l1_slope_correction == 0. ) ? "_before" : "_after";

    if (print_plots == true)
    {
        c1 -> Print( Form("%s/%s_nofitting.pdf",folder_direction.Data(),output_file_name.Data()) );
        
        
        gaus_fit_latex -> DrawLatex(0.12+0.05, 0.750, Form("Mean :  %.8f ", gaus_fit->GetParameter(1)));
        gaus_fit_latex -> DrawLatex(0.12+0.05, 0.710, Form("Width : %.4f", gaus_fit->GetParameter(2)));
        gaus_fit_latex -> DrawLatex(0.12+0.05, 0.670, Form("#chi^{2} : %.2f, NDF : %d", gaus_fit->GetChisquare(),gaus_fit->GetNDF()));
        gaus_fit_latex -> DrawLatex(0.12+0.05, 0.630, Form("#chi^{2}/NDF : %.4f",gaus_fit->GetChisquare()/double(gaus_fit->GetNDF())));

        gaus_fit->Draw("lsame");
        
        c1 -> Print( Form("%s/%s.pdf",folder_direction.Data(),output_file_name.Data()) );

        c1 -> Clear();
    }
    
    cout<<"Final l0l1 slope : "<<TString(Form("%.6f",gaus_fit -> GetParameter(1))).Data()<<endl;
    
    cout<<" "<<endl;
    cout<<" "<<endl;

    return -1 * stod(TString(Form("%.6f",gaus_fit -> GetParameter(1))).Data());
}

void TAnaResidual::Gen_l0l1_slope_pos()
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
    cout<<"input_slope size : "<<l0l1_correction_data.l0_l1_slope_vec.size()<<endl;
    cout<<"input_pos size   : "<<l0l1_correction_data.l0_l1_pos_vec.size()<<endl;
    cout<<"--------------------------------------------------------------------------------------------------------"<<endl;

    TH2F * angle_pos_hist = new TH2F("","",50,-0.05,0.05,50,-10,10);

    // angle_pos_hist -> SetLineWidth(3);
    // angle_pos_hist -> SetLineColor(TColor::GetColor("#1A3947")); 

    angle_pos_hist -> GetXaxis() -> SetNdivisions  (505);

    angle_pos_hist -> GetXaxis() -> SetTitleSize   (0.05);
	angle_pos_hist -> GetXaxis() -> SetTitleOffset (0.8);

	angle_pos_hist -> GetYaxis() -> SetTitleSize   (0.05);
	angle_pos_hist -> GetYaxis() -> SetTitleOffset (1.5);

    for (int i = 0; i < l0l1_correction_data.l0_l1_slope_vec.size(); i++)
    {
        angle_pos_hist -> Fill(l0l1_correction_data.l0_l1_slope_vec[i],l0l1_correction_data.l0_l1_pos_vec[i]);
    }

    // angle_pos_hist -> SetTitle(Form("DUT candidate track slope, U%i",study_chip));
    angle_pos_hist -> SetStats(0);
    angle_pos_hist -> GetXaxis() -> SetTitle("Track slope");
    angle_pos_hist -> GetYaxis() -> SetTitle("Track mid pos [mm]");

    angle_pos_hist -> Draw("colz0");

    TString output_file_name = Form("DUT_candidate_track_slope_Pos_U%i",study_chip);
    output_file_name += ( l0l1_slope_correction == 0. ) ? "_before" : "_after";

    c1 -> Print( Form("%s/%s.pdf",folder_direction.Data(),output_file_name.Data()) );
    c1 -> Clear();
}

TH1F * TAnaResidual::Gen_l0l1_scattering(int N_bin = 50, double L_edge = -0.05, double R_edge = 0.05, bool print_plots = true)
{
    cout<<" "<<endl;
    cout<<" "<<endl;
    cout<<"running function Get_l0l1 method, scattering : "<<endl;

    if (l0l1_slope_correction != 0.)
    {   
        cout<<"the given l0l1 slope correction is "<<l0l1_slope_correction<<endl;
        cout<<"the l0l1 slope correction is given"<<endl;
        // return 0;
    }
    
    TCanvas * c1 = new TCanvas("c1","c1",850,800);
    c1 -> cd();

    TPad * pad_c1 = new TPad(Form("pad_c1"), "", 0.0, 0.0, 1.0, 1.0);
    TAnaResidual::Characterize_Pad(pad_c1, 0.15,  0.1,  0.1,  0.12, 0, 0);
    pad_c1 -> Draw();
    pad_c1 -> SetTicks(1,1);
    pad_c1 -> SetLogy(1);
    pad_c1 -> cd();

    cout<<"--------------------------------------------------------------------------------------------------------"<<endl;
    cout<<"----- Now we are doing the scattering dist. by using l0l1 method "<<endl;
    cout<<"l0l1_correction_data.scattering_vec size : "<<l0l1_correction_data.scattering_vec.size()<<endl;
    cout<<"--------------------------------------------------------------------------------------------------------"<<endl;

    TH1F * angle_hist = new TH1F("","",N_bin,L_edge,R_edge);

    angle_hist -> SetLineWidth(3);
    angle_hist -> SetLineColor(TColor::GetColor("#1A3947")); 

    angle_hist -> GetXaxis() -> SetNdivisions  (505);

    angle_hist -> GetXaxis() -> SetTitleSize   (0.05);
	angle_hist -> GetXaxis() -> SetTitleOffset (0.8);

	angle_hist -> GetYaxis() -> SetTitleSize   (0.05);
	angle_hist -> GetYaxis() -> SetTitleOffset (1.5);

    for (int i = 0; i < l0l1_correction_data.scattering_vec.size(); i++)
    {
        angle_hist -> Fill(l0l1_correction_data.scattering_vec[i]);
    }

    // angle_hist -> SetTitle(Form("DUT candidate track slope, U%i",study_chip));
    angle_hist -> SetStats(0);
    angle_hist -> GetXaxis() -> SetTitle("Scattering [slope_{l2l1} - slope_{l1l0}]");
    angle_hist -> GetYaxis() -> SetTitle("Events");

    angle_hist -> Draw("hist");

    TF1 * gaus_fit = new TF1 ("gaus_fit","gaus",-0.1,0.1);
    gaus_fit -> SetNpx(10000);

    // angle_hist -> Fit("gaus_fit","NQ");


    // TLine * slope_cut_positive = new TLine(slope_cut,angle_hist -> GetMinimum(),slope_cut,angle_hist -> GetMaximum()*1.05);
    // slope_cut_positive -> SetLineWidth(3);
    // slope_cut_positive -> SetLineColor(TColor::GetColor("#A08144"));
    // slope_cut_positive -> SetLineStyle(2);

    // TLine * slope_cut_negative = new TLine(-1 * slope_cut,angle_hist -> GetMinimum(),-1 * slope_cut,angle_hist -> GetMaximum()*1.05);
    // slope_cut_negative -> SetLineWidth(3);
    // slope_cut_negative -> SetLineColor(TColor::GetColor("#A08144"));
    // slope_cut_negative -> SetLineStyle(2);

    TLatex *gaus_fit_latex = new TLatex();
    gaus_fit_latex -> SetNDC();
    gaus_fit_latex -> SetTextSize(0.028);

    // cout<<"Fit slope mean : "<<gaus_fit->GetParameter(1)<<endl;
    // cout<<"Fit slope width : "<<gaus_fit->GetParameter(2)<<endl;
    // cout<<"stat slope mean : "<<angle_hist -> GetMean()<<endl;
    // cout<<"stat slope width : "<<angle_hist -> GetStdDev()<<endl;
    // cout<<"stat entry : "<<angle_hist -> GetEntries()<<endl;


    TLegend *legend1 = new TLegend (0.62, 0.7, 0.85, 0.78);
	legend1 -> SetTextSize (0.03);
	// legend1 -> SetNColumns (4);
    legend1 -> SetBorderSize(0);
    // legend1 -> AddEntry (slope_cut_positive, Form("Slope cut %.2f",slope_cut),  "l");


    // slope_cut_negative -> Draw("l same");
    // slope_cut_positive -> Draw("l same");

    // legend1 -> Draw("l same");

    TString output_file_name = Form("DUT_candidate_l0l1_scattering_U%i",study_chip);
    output_file_name += ( l0l1_slope_correction == 0. ) ? "_before" : "_after";

    if (print_plots == true)
    {
        c1 -> Print( Form("%s/%s_nofitting.pdf",folder_direction.Data(),output_file_name.Data()) );
        
        
        // gaus_fit_latex -> DrawLatex(0.12+0.05, 0.750, Form("Mean :  %.8f ", gaus_fit->GetParameter(1)));
        // gaus_fit_latex -> DrawLatex(0.12+0.05, 0.710, Form("Width : %.4f", gaus_fit->GetParameter(2)));
        // gaus_fit_latex -> DrawLatex(0.12+0.05, 0.670, Form("#chi^{2} : %.2f, NDF : %d", gaus_fit->GetChisquare(),gaus_fit->GetNDF()));
        // gaus_fit_latex -> DrawLatex(0.12+0.05, 0.630, Form("#chi^{2}/NDF : %.4f",gaus_fit->GetChisquare()/double(gaus_fit->GetNDF())));

        // gaus_fit->Draw("lsame");
        
        // c1 -> Print( Form("%s/%s.pdf",folder_direction.Data(),output_file_name.Data()) );

        c1 -> Clear();
    }

    return angle_hist;
}


TH1F * TAnaResidual::Gen_l0l1_residual(int N_bin = 50, double L_edge = -1.5, double R_edge = 1.5, bool print_plots = true)
{
    cout<<" "<<endl;
    cout<<" "<<endl;
    cout<<"running function Get_l0l1 method, residual : "<<endl;

    if (l0l1_slope_correction != 0.)
    {   
        cout<<"the given l0l1 slope correction is "<<l0l1_slope_correction<<endl;
        cout<<"the l0l1 slope correction is given"<<endl;
        // return 0;
    }
    
    TCanvas * c1 = new TCanvas("c1","c1",850,800);
    c1 -> cd();

    TPad * pad_c1 = new TPad(Form("pad_c1"), "", 0.0, 0.0, 1.0, 1.0);
    TAnaResidual::Characterize_Pad(pad_c1, 0.15,  0.1,  0.1,  0.12, 0, 0);
    pad_c1 -> Draw();
    pad_c1 -> SetTicks(1,1);
    pad_c1 -> SetLogy(1);
    pad_c1 -> cd();

    cout<<"--------------------------------------------------------------------------------------------------------"<<endl;
    cout<<"----- Now we are doing the residual dist. by using l0l1 method "<<endl;
    cout<<"l0l1_correction_data.middle_layer_residual size : "<<l0l1_correction_data.middle_layer_residual.size()<<endl;
    cout<<"--------------------------------------------------------------------------------------------------------"<<endl;

    TH1F * angle_hist = new TH1F("","",N_bin,L_edge,R_edge);

    angle_hist -> SetLineWidth(3);
    angle_hist -> SetLineColor(TColor::GetColor("#1A3947")); 

    // angle_hist -> GetXaxis() -> SetNdivisions  (505);

    angle_hist -> GetXaxis() -> SetTitleSize   (0.05);
	angle_hist -> GetXaxis() -> SetTitleOffset (0.8);

	angle_hist -> GetYaxis() -> SetTitleSize   (0.05);
	angle_hist -> GetYaxis() -> SetTitleOffset (1.5);

    for (int i = 0; i < l0l1_correction_data.middle_layer_residual.size(); i++)
    {
        angle_hist -> Fill(l0l1_correction_data.middle_layer_residual[i]);
    }

    // angle_hist -> SetTitle(Form("DUT candidate track slope, U%i",study_chip));
    angle_hist -> SetStats(0);
    angle_hist -> GetXaxis() -> SetTitle("residual [mm]");
    angle_hist -> GetYaxis() -> SetTitle("Events");

    angle_hist -> Draw("hist");

    TF1 * gaus_fit = new TF1 ("gaus_fit","gaus",-0.1,0.1);
    gaus_fit -> SetNpx(10000);

    // angle_hist -> Fit("gaus_fit","NQ");


    // TLine * slope_cut_positive = new TLine(slope_cut,angle_hist -> GetMinimum(),slope_cut,angle_hist -> GetMaximum()*1.05);
    // slope_cut_positive -> SetLineWidth(3);
    // slope_cut_positive -> SetLineColor(TColor::GetColor("#A08144"));
    // slope_cut_positive -> SetLineStyle(2);

    // TLine * slope_cut_negative = new TLine(-1 * slope_cut,angle_hist -> GetMinimum(),-1 * slope_cut,angle_hist -> GetMaximum()*1.05);
    // slope_cut_negative -> SetLineWidth(3);
    // slope_cut_negative -> SetLineColor(TColor::GetColor("#A08144"));
    // slope_cut_negative -> SetLineStyle(2);

    TLatex *gaus_fit_latex = new TLatex();
    gaus_fit_latex -> SetNDC();
    gaus_fit_latex -> SetTextSize(0.028);

    // cout<<"Fit slope mean : "<<gaus_fit->GetParameter(1)<<endl;
    // cout<<"Fit slope width : "<<gaus_fit->GetParameter(2)<<endl;
    // cout<<"stat slope mean : "<<angle_hist -> GetMean()<<endl;
    // cout<<"stat slope width : "<<angle_hist -> GetStdDev()<<endl;
    // cout<<"stat entry : "<<angle_hist -> GetEntries()<<endl;


    TLegend *legend1 = new TLegend (0.62, 0.7, 0.85, 0.78);
	legend1 -> SetTextSize (0.03);
	// legend1 -> SetNColumns (4);
    legend1 -> SetBorderSize(0);
    // legend1 -> AddEntry (slope_cut_positive, Form("Slope cut %.2f",slope_cut),  "l");


    // slope_cut_negative -> Draw("l same");
    // slope_cut_positive -> Draw("l same");

    // legend1 -> Draw("l same");

    TString output_file_name = Form("DUT_candidate_l0l1_residual_U%i",study_chip);
    output_file_name += ( l0l1_slope_correction == 0. ) ? "_before" : "_after";

    if (print_plots == true)
    {
        c1 -> Print( Form("%s/%s_nofitting.pdf",folder_direction.Data(),output_file_name.Data()) );
        
        
        // gaus_fit_latex -> DrawLatex(0.12+0.05, 0.750, Form("Mean :  %.8f ", gaus_fit->GetParameter(1)));
        // gaus_fit_latex -> DrawLatex(0.12+0.05, 0.710, Form("Width : %.4f", gaus_fit->GetParameter(2)));
        // gaus_fit_latex -> DrawLatex(0.12+0.05, 0.670, Form("#chi^{2} : %.2f, NDF : %d", gaus_fit->GetChisquare(),gaus_fit->GetNDF()));
        // gaus_fit_latex -> DrawLatex(0.12+0.05, 0.630, Form("#chi^{2}/NDF : %.4f",gaus_fit->GetChisquare()/double(gaus_fit->GetNDF())));

        // gaus_fit->Draw("lsame");
        
        // c1 -> Print( Form("%s/%s.pdf",folder_direction.Data(),output_file_name.Data()) );

        c1 -> Clear();
    }

    return angle_hist;
}


#endif