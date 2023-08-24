#include "../DAC_Scan/InttClustering.h"
void print_INTT_geometry()
{
    // c1 -> cd();
    string mother_folder_directory = "/home/phnxrc/INTT/cwshih/DACscan_data/";
    TFile * out_file = new TFile(Form("%s/INTT_TLine_geo.root",mother_folder_directory.c_str()),"RECREATE");

    TTree * tree_out =  new TTree ("tree", "INTT Geometry XY plane");
    double x1, y1, x2, y2;
    tree_out -> Branch("x1",&x1);
    tree_out -> Branch("y1",&y1);
    tree_out -> Branch("x2",&x2);
    tree_out -> Branch("y2",&y2);


    int geo_mode_id = 1;
    string conversion_mode = (geo_mode_id == 0) ? "ideal" : "survey_1_XYAlpha_Peek";
    double peek = 3.32405;

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
    // bkg -> SetPoint(1,beam_origin.first,beam_origin.second);
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
            
            x1 = InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,14,0,conversion_mode,peek).x;
            y1 = InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,14,0,conversion_mode,peek).y;

            x2 = InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,1,0,conversion_mode,peek).x;
            y2 = InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,1,0,conversion_mode,peek).y;

            cout<<"("<<x1<<", "<<y1<<", "<<x2<<", "<<y2<<")"<<endl;

            tree_out -> Fill();

        }
    }
    
    ladder_line -> Draw("l same");

    tree_out->SetDirectory(out_file);
    tree_out -> Write("", TObject::kOverwrite);

    cout<<"conversion done, file : "<<endl;
	out_file -> Close();

}