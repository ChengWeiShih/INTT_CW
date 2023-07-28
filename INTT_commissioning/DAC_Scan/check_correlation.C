// #include "DAC_Scan_ladder.h"
//#include "InttConversion.h"
#include "InttClustering.h"

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

// note : use "ls *.root > file_list.txt" to create the list of the file in the folder, full directory in the file_list.txt
// note : set_folder_name = "folder_xxxx"
// note : server_name = "inttx"
void check_correlation(/*pair<double,double>beam_origin*/)
{
    TCanvas * c1 = new TCanvas("","",1000,800);

    string mother_folder_directory = "/home/phnxrc/INTT/cwshih/DACscan_data/zero_magnet_Takashi_used";
    // string file_name = "beam_inttall-00020869-0000_event_base_ana_cluster_survey_1_XYAlpha_Peek_5.00mm_excludeR500";
    string file_name = "beam_inttall-00020869-0000_event_base_ana_cluster_100K_excludeR500";
    system(Form("mkdir %s/folder_%s",mother_folder_directory.c_str(),file_name.c_str()));
    pair<double,double> beam_origin = {-0,5};
    double temp_Y_align = 0.;
    double temp_X_align = -0.;

    TFile * file_in = new TFile(Form("%s/%s.root",mother_folder_directory.c_str(),file_name.c_str()),"read");
    TTree * tree = (TTree *)file_in->Get("tree_clu");
    
    long long N_event = tree -> GetEntries();
    cout<<Form("N_event in file %s : %lli",file_name.c_str(), N_event)<<endl;

    int N_hits;
    int N_cluster_inner;
    int N_cluster_outer;
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

    vector<clu_info> temp_sPH_inner_nocolumn_vec; temp_sPH_inner_nocolumn_vec.clear();
    vector<clu_info> temp_sPH_outer_nocolumn_vec; temp_sPH_outer_nocolumn_vec.clear();

    TH2F * angle_correlation = new TH2F("","angle_correlation",361,0,361,361,0,361);
    angle_correlation -> GetXaxis() -> SetTitle("Inner Phi (degree)");
    angle_correlation -> GetYaxis() -> SetTitle("Outer Phi (degree)");

    TH2F * inner_pos_xy = new TH2F("","inner_pos_xy",360,-100,100,360,-100,100);
    inner_pos_xy -> GetXaxis() -> SetTitle("X axis");
    inner_pos_xy -> GetYaxis() -> SetTitle("Y axis");

    TH2F * outer_pos_xy = new TH2F("","outer_pos_xy",360,-150,150,360,-150,150);
    outer_pos_xy -> GetXaxis() -> SetTitle("X axis");
    outer_pos_xy -> GetYaxis() -> SetTitle("Y axis");

    TH2F * inner_outer_pos_xy = new TH2F("","inner_outer_pos_xy",360,-150,150,360,-150,150);
    inner_outer_pos_xy -> GetXaxis() -> SetTitle("X axis");
    inner_outer_pos_xy -> GetYaxis() -> SetTitle("Y axis");

    TH1F * z_pos_diff = new TH1F("","z_pos_diff",360,-150,150);
    z_pos_diff -> GetXaxis() -> SetTitle("inner zpos - outer zpos");
    z_pos_diff -> GetYaxis() -> SetTitle("Eentry");

    TH2F * z_pos_diff_angle_diff = new TH2F("","z_pos_diff_angle_diff",100,-25,25,200,-11,11);
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
    z_pos_inner_outer -> GetXaxis() -> SetTitle("inner zpos");
    z_pos_inner_outer -> GetYaxis() -> SetTitle("outer pos");

    TH2F * DCA_point = new TH2F("","DCA_point",100,-10,10,100,-10,10);
    DCA_point -> GetXaxis() -> SetTitle("X pos (mm)");
    DCA_point -> GetYaxis() -> SetTitle("Y pos (mm)");

    TH2F * DCA_distance_inner_phi = new TH2F("","DCA_distance_inner_phi",100,0,360,100,-10,10);
    DCA_distance_inner_phi -> GetXaxis() -> SetTitle("inner phi");
    DCA_distance_inner_phi -> GetYaxis() -> SetTitle("DCA (mm)");

    TH2F * DCA_distance_outer_phi = new TH2F("","DCA_distance_outer_phi",100,0,360,100,-10,10);
    DCA_distance_outer_phi -> GetXaxis() -> SetTitle("outer phi");
    DCA_distance_outer_phi -> GetYaxis() -> SetTitle("DCA (mm)");

    TH1F * N_cluster_outer_pass = new TH1F("","N_cluster_outer_pass",100,0,100);
    N_cluster_outer_pass -> GetXaxis() -> SetTitle("N_cluster");
    N_cluster_outer_pass -> GetYaxis() -> SetTitle("Eentry");

    TH1F * N_cluster_inner_pass = new TH1F("","N_cluster_inner_pass",100,0,100);
    N_cluster_inner_pass -> GetXaxis() -> SetTitle("N_cluster");
    N_cluster_inner_pass -> GetYaxis() -> SetTitle("Eentry");

    TH2F * N_cluster_correlation = new TH2F("","N_cluster_correlation",100,0,100,100,0,100);
    N_cluster_correlation -> GetXaxis() -> SetTitle("inner N_cluster");
    N_cluster_correlation -> GetYaxis() -> SetTitle("Outer N_cluster");

    TH1F * temp_event_zvtx = new TH1F("","temp_event_zvtx",250,-500,500);
    temp_event_zvtx -> GetXaxis() -> SetTitle("Z vertex position (mm)");
    temp_event_zvtx -> GetYaxis() -> SetTitle("entry");

    TH1F * avg_event_zvtx = new TH1F("","avg_event_zvtx",250,-500,500);
    avg_event_zvtx -> GetXaxis() -> SetTitle("Z vertex position (mm)");
    avg_event_zvtx -> GetYaxis() -> SetTitle("entry");

    c1 -> Print(Form("%s/folder_%s/temp_event_zvtx.pdf(",mother_folder_directory.c_str(),file_name.c_str()));

    for (int event_i = 0; event_i < N_event; event_i++)
    {
        tree -> GetEntry(event_i);
        unsigned int length = column_vec -> size();

        if (N_hits > 450) continue;

        for (int hit_i = 0; hit_i < length; hit_i++)
        {
            if (size_vec -> at(hit_i) > 4) continue;
            // if (size_vec -> at(hit_i) < 2) continue;
            if (sum_adc_conv_vec -> at(hit_i) < 31) continue;
            // if (z_vec -> at(hit_i) < 0) continue;
            
            // note : inner
            if (layer_vec -> at(hit_i) == 0 && x_vec -> at(hit_i) < -75 && x_vec -> at(hit_i) > -80 && y_vec -> at(hit_i) > 7.5 && y_vec -> at(hit_i) < 12.5 ) continue;
            // if (layer_vec -> at(hit_i) == 0 && x_vec -> at(hit_i) > 35 && x_vec -> at(hit_i) < 40 && y_vec -> at(hit_i) > 65 && y_vec -> at(hit_i) < 70 ) continue;
            if (layer_vec -> at(hit_i) == 0 && phi_vec -> at(hit_i) > 295 && phi_vec -> at(hit_i) < 302) continue;
            if (layer_vec -> at(hit_i) == 0 && phi_vec -> at(hit_i) > 210 && phi_vec -> at(hit_i) < 213) continue;
            if (layer_vec -> at(hit_i) == 0 && phi_vec -> at(hit_i) > 55 && phi_vec -> at(hit_i) < 65) continue;
            if (layer_vec -> at(hit_i) == 0 && phi_vec -> at(hit_i) > 348 && phi_vec -> at(hit_i) < 353) continue;

            // note : outer
            if (layer_vec -> at(hit_i) == 1 && x_vec -> at(hit_i) < -70 && x_vec -> at(hit_i) > -75 && y_vec -> at(hit_i) > 70 && y_vec -> at(hit_i) < 80 ) continue;
            // if (layer_vec -> at(hit_i) == 1 && x_vec -> at(hit_i) > 70 && x_vec -> at(hit_i) < 83 && y_vec -> at(hit_i) > 50 && y_vec -> at(hit_i) < 65 ) continue;
            // if (layer_vec -> at(hit_i) == 1 && x_vec -> at(hit_i) > 70 && x_vec -> at(hit_i) < 83 && y_vec -> at(hit_i) > 63 && y_vec -> at(hit_i) < 75 ) continue;
            if (layer_vec -> at(hit_i) == 1 && x_vec -> at(hit_i) < -70 && x_vec -> at(hit_i) > -75 && y_vec -> at(hit_i) < -70 && y_vec -> at(hit_i) > -75 ) continue;
            if (layer_vec -> at(hit_i) == 1 && phi_vec -> at(hit_i) > 335 && phi_vec -> at(hit_i) < 340) continue;
            if (layer_vec -> at(hit_i) == 1 && phi_vec -> at(hit_i) > 105 && phi_vec -> at(hit_i) < 115) continue;
            if (layer_vec -> at(hit_i) == 1 && phi_vec -> at(hit_i) > 30 && phi_vec -> at(hit_i) < 47) continue;
            
            if (layer_vec -> at(hit_i) == 0) //note : inner
                temp_sPH_inner_nocolumn_vec.push_back({
                    column_vec -> at(hit_i), 
                    avg_chan_vec -> at(hit_i), 
                    sum_adc_vec -> at(hit_i), 
                    sum_adc_conv_vec -> at(hit_i), 
                    size_vec -> at(hit_i), 
                    (phi_vec -> at(hit_i) > 90 && phi_vec -> at(hit_i) < 270 ) ? x_vec -> at(hit_i) + temp_X_align : x_vec -> at(hit_i), 
                    (phi_vec -> at(hit_i) > 90 && phi_vec -> at(hit_i) < 270 ) ? y_vec -> at(hit_i) + temp_Y_align : y_vec -> at(hit_i), 
                    z_vec -> at(hit_i), 
                    layer_vec -> at(hit_i), 
                    phi_vec -> at(hit_i)
                });
            
            if (layer_vec -> at(hit_i) == 1) //note : inner
                temp_sPH_outer_nocolumn_vec.push_back({
                    column_vec -> at(hit_i), 
                    avg_chan_vec -> at(hit_i), 
                    sum_adc_vec -> at(hit_i), 
                    sum_adc_conv_vec -> at(hit_i), 
                    size_vec -> at(hit_i), 
                    (phi_vec -> at(hit_i) > 90 && phi_vec -> at(hit_i) < 270 ) ? x_vec -> at(hit_i) + temp_X_align : x_vec -> at(hit_i), 
                    (phi_vec -> at(hit_i) > 90 && phi_vec -> at(hit_i) < 270 ) ? y_vec -> at(hit_i) + temp_Y_align : y_vec -> at(hit_i), 
                    z_vec -> at(hit_i), 
                    layer_vec -> at(hit_i), 
                    phi_vec -> at(hit_i)
                });            
        }

        N_cluster_outer_pass -> Fill(temp_sPH_outer_nocolumn_vec.size());
        N_cluster_inner_pass -> Fill(temp_sPH_inner_nocolumn_vec.size());
        N_cluster_correlation -> Fill( temp_sPH_inner_nocolumn_vec.size(), temp_sPH_outer_nocolumn_vec.size() );

        for ( int inner_i = 0; inner_i < temp_sPH_inner_nocolumn_vec.size(); inner_i++ )
        {
            for ( int outer_i = 0; outer_i < temp_sPH_outer_nocolumn_vec.size(); outer_i++ )
            {
                
                vector<double> DCA_info_vec = calculateDistanceAndClosestPoint(
                    temp_sPH_inner_nocolumn_vec[inner_i].x, temp_sPH_inner_nocolumn_vec[inner_i].y,
                    temp_sPH_outer_nocolumn_vec[outer_i].x, temp_sPH_outer_nocolumn_vec[outer_i].y,
                    beam_origin.first, beam_origin.second
                );

                double DCA_sign = calculateAngleBetweenVectors(
                    temp_sPH_outer_nocolumn_vec[outer_i].x, temp_sPH_outer_nocolumn_vec[outer_i].y,
                    temp_sPH_inner_nocolumn_vec[inner_i].x, temp_sPH_inner_nocolumn_vec[inner_i].y,
                    beam_origin.first, beam_origin.second
                );

                if (DCA_info_vec[0] != fabs(DCA_sign) && fabs( DCA_info_vec[0] - fabs(DCA_sign) ) > 0.1 )
                    cout<<"different DCA : "<<DCA_info_vec[0]<<" "<<DCA_sign<<" diff : "<<DCA_info_vec[0] - fabs(DCA_sign)<<endl;

                // if ((temp_sPH_inner_nocolumn_vec[inner_i].z - fabs(temp_sPH_outer_nocolumn_vec[outer_i].z)) < 0.1 ) continue;

                if (fabs(temp_sPH_inner_nocolumn_vec[inner_i].phi - temp_sPH_outer_nocolumn_vec[outer_i].phi) < 3)
                {

                    temp_event_zvtx -> Fill( get_z_vertex(temp_sPH_inner_nocolumn_vec[inner_i],temp_sPH_outer_nocolumn_vec[outer_i],DCA_info_vec[1],DCA_info_vec[2]) );

                    DCA_point -> Fill( DCA_info_vec[1], DCA_info_vec[2] );

                    angle_correlation -> Fill(temp_sPH_inner_nocolumn_vec[inner_i].phi,temp_sPH_outer_nocolumn_vec[outer_i].phi);
                    z_pos_diff -> Fill( temp_sPH_inner_nocolumn_vec[inner_i].z - temp_sPH_outer_nocolumn_vec[outer_i].z );
                    z_pos_diff_angle_diff -> Fill( temp_sPH_inner_nocolumn_vec[inner_i].z - temp_sPH_outer_nocolumn_vec[outer_i].z, temp_sPH_inner_nocolumn_vec[inner_i].phi - temp_sPH_outer_nocolumn_vec[outer_i].phi );

                    Nhits_good -> Fill(N_hits);

                    z_pos_inner -> Fill(temp_sPH_inner_nocolumn_vec[inner_i].z);
                    z_pos_outer -> Fill(temp_sPH_outer_nocolumn_vec[outer_i].z);
                    z_pos_inner_outer -> Fill(temp_sPH_inner_nocolumn_vec[inner_i].z, temp_sPH_outer_nocolumn_vec[outer_i].z);

                    // DCA_distance_inner_phi -> Fill(temp_sPH_inner_nocolumn_vec[inner_i].phi, (temp_sPH_inner_nocolumn_vec[inner_i].phi > 90 && temp_sPH_inner_nocolumn_vec[inner_i].phi < 270) ? DCA_sign * -1 : DCA_sign );
                    // DCA_distance_outer_phi -> Fill(temp_sPH_outer_nocolumn_vec[outer_i].phi, (temp_sPH_outer_nocolumn_vec[outer_i].phi > 90 && temp_sPH_outer_nocolumn_vec[outer_i].phi < 270) ? DCA_sign * -1 : DCA_sign );

                    DCA_distance_inner_phi -> Fill(temp_sPH_inner_nocolumn_vec[inner_i].phi, DCA_sign );
                    DCA_distance_outer_phi -> Fill(temp_sPH_outer_nocolumn_vec[outer_i].phi, DCA_sign );

                    // cout<<"good comb : "<<fabs(temp_sPH_inner_nocolumn_vec[inner_i].phi - temp_sPH_outer_nocolumn_vec[outer_i].phi)<<" radius in : "<<get_radius(temp_sPH_inner_nocolumn_vec[inner_i].x, temp_sPH_inner_nocolumn_vec[inner_i].y)<<" radius out : "<<get_radius(temp_sPH_outer_nocolumn_vec[outer_i].x, temp_sPH_outer_nocolumn_vec[outer_i].y)<<endl;
                } // note : end of if 
                    

            } // note : end of outer loop
        } // note : end of inner loop

        if (temp_event_zvtx -> GetEntries() > 20)
            avg_event_zvtx -> Fill( temp_event_zvtx -> GetMean() );

        if (temp_event_zvtx -> GetEntries() > 20)
        {
            temp_event_zvtx -> Draw("hist");
            c1 -> Print(Form("%s/folder_%s/temp_event_zvtx.pdf(",mother_folder_directory.c_str(),file_name.c_str()));
        }

        for ( int inner_i = 0; inner_i < temp_sPH_inner_nocolumn_vec.size(); inner_i++ )
        {
            inner_pos_xy -> Fill(temp_sPH_inner_nocolumn_vec[inner_i].x,temp_sPH_inner_nocolumn_vec[inner_i].y);
            inner_outer_pos_xy -> Fill(temp_sPH_inner_nocolumn_vec[inner_i].x,temp_sPH_inner_nocolumn_vec[inner_i].y);
        }

        for ( int outer_i = 0; outer_i < temp_sPH_outer_nocolumn_vec.size(); outer_i++ )
        {
            outer_pos_xy -> Fill(temp_sPH_outer_nocolumn_vec[outer_i].x,temp_sPH_outer_nocolumn_vec[outer_i].y);
            inner_outer_pos_xy -> Fill(temp_sPH_outer_nocolumn_vec[outer_i].x,temp_sPH_outer_nocolumn_vec[outer_i].y);
        }

        
        temp_event_zvtx -> Reset("ICESM");
        temp_sPH_inner_nocolumn_vec.clear();
        temp_sPH_outer_nocolumn_vec.clear();
    } // note : end of event 

    c1 -> Print(Form("%s/folder_%s/temp_event_zvtx.pdf)",mother_folder_directory.c_str(),file_name.c_str()));

    c1 -> cd();

    avg_event_zvtx -> Draw("hist");
    c1 -> Print(Form("%s/folder_%s/avg_event_zvtx.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    c1 -> Clear();

    N_cluster_inner_pass -> Draw("hist"); 
    c1 -> Print(Form("%s/folder_%s/N_cluster_inner_pass.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    c1 -> Clear();

    N_cluster_outer_pass -> Draw("hist");
    c1 -> Print(Form("%s/folder_%s/N_cluster_outer_pass.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    c1 -> Clear();

    N_cluster_correlation -> Draw("colz0");
    c1 -> Print(Form("%s/folder_%s/N_cluster_correlation.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    c1 -> Clear();

    DCA_point -> Draw("colz0");
    c1 -> Print(Form("%s/folder_%s/DCA_point_X%.1fY%.1f_.pdf",mother_folder_directory.c_str(),file_name.c_str(),beam_origin.first,beam_origin.second));
    c1 -> Clear();

    DCA_distance_inner_phi -> Draw("colz0");
    c1 -> Print(Form("%s/folder_%s/DCA_distance_inner_phi_X%.1fY%.1f_.pdf",mother_folder_directory.c_str(),file_name.c_str(),beam_origin.first,beam_origin.second));
    c1 -> Clear();

    DCA_distance_outer_phi -> Draw("colz0");
    c1 -> Print(Form("%s/folder_%s/DCA_distance_outer_phi_X%.1fY%.1f_.pdf",mother_folder_directory.c_str(),file_name.c_str(),beam_origin.first,beam_origin.second));
    c1 -> Clear();

    z_pos_inner_outer -> Draw("colz0");
    c1 -> Print(Form("%s/folder_%s/z_pos_inner_outer.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    c1 -> Clear();

    z_pos_inner -> Draw("hist");
    c1 -> Print(Form("%s/folder_%s/z_pos_inner.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    c1 -> Clear();

    z_pos_outer -> Draw("hist");
    c1 -> Print(Form("%s/folder_%s/z_pos_outer.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    c1 -> Clear();

    Nhits_good -> Draw("hist");
    c1 -> Print(Form("%s/folder_%s/Nhits_good.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    c1 -> Clear();

    angle_correlation -> Draw("colz0");
    c1 -> Print(Form("%s/folder_%s/angle_correlation.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    c1 -> Clear();

    z_pos_diff -> Draw("hist");
    c1 -> Print(Form("%s/folder_%s/z_pos_diff.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    c1 -> Clear();

    z_pos_diff_angle_diff -> Draw("colz0");
    c1 -> Print(Form("%s/folder_%s/z_pos_diff_angle_diff.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    c1 -> Clear();

    inner_pos_xy -> Draw("colz0");
    c1 -> Print(Form("%s/folder_%s/inner_pos_xy.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    c1 -> Clear();

    outer_pos_xy -> Draw("colz0");
    c1 -> Print(Form("%s/folder_%s/outer_pos_xy.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    c1 -> Clear();

    inner_outer_pos_xy -> Draw("colz0");
    c1 -> Print(Form("%s/folder_%s/inner_outer_pos_xy.pdf",mother_folder_directory.c_str(),file_name.c_str()));
    c1 -> Clear();

    
}