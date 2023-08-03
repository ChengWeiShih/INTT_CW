#include "../InttConversion.h"
void temp_bkg_test(TCanvas * c1, string conversion_mode, double peek)
{
    c1 -> cd();

    int N_ladder[4] = {12, 12, 16, 16};
    string ladder_index_string[16] = {"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15"};

    vector<double> x_vec; x_vec.clear();
    vector<double> y_vec; y_vec.clear();

    vector<double> x_vec_2; x_vec_2.clear();
    vector<double> y_vec_2; y_vec_2.clear();

    TGraph * bkg = new TGraph();
    bkg -> SetMarkerStyle(20);
    bkg -> SetMarkerSize(0.1);
    bkg -> SetPoint(0,0,0);
    bkg -> SetPoint(1,0,5);
    bkg -> GetXaxis() -> SetLimits(-150,150);
    bkg -> GetYaxis() -> SetRangeUser(-150,150);
    
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