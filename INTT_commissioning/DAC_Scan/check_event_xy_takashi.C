#include "InttConversion.h"

void temp_bkg(TCanvas * c1)
{
    c1 -> cd();

    int N_ladder[4] = {12, 12, 16, 16};
    string ladder_index_string[16] = {"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15"};

    vector<double> x_vec; x_vec.clear();
    vector<double> y_vec; y_vec.clear();

    vector<double> x_vec_2; x_vec_2.clear();
    vector<double> y_vec_2; y_vec_2.clear();

    for (int server_i = 0; server_i < 4; server_i++)
    {
        for (int FC_i = 0; FC_i < 14; FC_i++)
        {
            for (int chan_i = 0; chan_i < 128; chan_i++)
            {

                x_vec.push_back(InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,14,chan_i).x);
                y_vec.push_back(InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,14,chan_i).y);

                x_vec_2.push_back(InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,1,chan_i).x);
                y_vec_2.push_back(InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,1,chan_i).y);
                // cout<<"("<<InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,14,chan_i).first<<","<<InttConversion::Get_XY_all(Form("intt%i",server_i),FC_i,14,chan_i).second<<"),";

                // cout<<"("<<Get_XY_all(Form("%s%s",layer_text[i].c_str(),ladder_index_string[i1].c_str()),used_chip,127).first<<","<<Get_XY_all(Form("%s%s",layer_text[i].c_str(),ladder_index_string[i1].c_str()),used_chip,127).second<<")";
            }
        }
    }

    TGraph * aaa = new TGraph(x_vec.size(),&x_vec[0],&y_vec[0]);
    aaa -> SetMarkerStyle(20);
    aaa -> SetMarkerSize(0.1);

    aaa -> GetXaxis() -> SetTitle("X axis (mm)");
    aaa -> GetYaxis() -> SetTitle("Y axis (mm)");

    TGraph * bbb = new TGraph(x_vec_2.size(),&x_vec_2[0],&y_vec_2[0]);
    bbb -> SetMarkerStyle(20);
    bbb -> SetMarkerSize(0.1);
    bbb -> SetMarkerColor(4);

    vector<double> origin_x_vec={0,0};
    vector<double> origin_y_vec={0,5};
    TGraph * origin_g = new TGraph(origin_x_vec.size(),&origin_x_vec[0],&origin_y_vec[0]);
    origin_g -> SetMarkerStyle(20);
    origin_g -> SetMarkerSize(0.5);
    origin_g -> SetMarkerColor(4);

    
    
    aaa -> Draw("ap");
    bbb -> Draw("p same");
    origin_g -> Draw("psame");
}

void check_event_xy_takashi(float event_i)
{
    TFile * file_in = new TFile("/home/phnxrc/INTT/cwshih/DACscan_data/Takashi_file/AnaTutorial_run20869ZF_lo_BCyp5mm.root","read");
    TNtuple * Ntuple = (TNtuple*)file_in->Get("ntp_clus");

    float nclus;
    float nclus2;
    float bco_full;
    float evt;
    float size;
    float adc;
    float x;
    float y;
    float z;
    float lay;
    float lad;
    float sen;

    Ntuple -> SetBranchAddress( "nclus", &nclus );
    Ntuple -> SetBranchAddress( "nclus2", &nclus2 );
    Ntuple -> SetBranchAddress( "bco_full", &bco_full );
    Ntuple -> SetBranchAddress( "evt", &evt );
    Ntuple -> SetBranchAddress( "size", &size );
    Ntuple -> SetBranchAddress( "adc", &adc );
    Ntuple -> SetBranchAddress( "x", &x );
    Ntuple -> SetBranchAddress( "y", &y );
    Ntuple -> SetBranchAddress( "z", &z );
    Ntuple -> SetBranchAddress( "lay", &lay );
    Ntuple -> SetBranchAddress( "lad", &lad );
    Ntuple -> SetBranchAddress( "sen", &sen );

    TCanvas * c1 = new TCanvas("","",1000,800);

    vector<double> temp_x_nocolumn_vec; temp_x_nocolumn_vec.clear(); // note : 13 columns 
    vector<double> temp_y_nocolumn_vec; temp_y_nocolumn_vec.clear(); // note : 13 columns 


    for (int i = 0; i < 100000; i++)
    {
        Ntuple -> GetEntry(i);

        if (evt == event_i /*&& size < 5 && 40 < adc && adc < 200*/ && z > 0)
        {
            temp_x_nocolumn_vec.push_back(x * 10);
            temp_y_nocolumn_vec.push_back(y * 10);
        }
    }

    temp_bkg(c1);
    cout<<"N cluster : "<<temp_x_nocolumn_vec.size()<<endl;
    TGraph * event_display = new TGraph(temp_x_nocolumn_vec.size(),&temp_x_nocolumn_vec[0],&temp_y_nocolumn_vec[0]);
    event_display -> SetMarkerStyle(20);
    event_display -> SetMarkerColor(2);
    event_display -> SetMarkerSize(0.7);
    event_display -> Draw("p same");





}