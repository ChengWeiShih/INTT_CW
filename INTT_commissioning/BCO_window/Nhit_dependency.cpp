#include "vector_stddev.h"
#include "draw_style.h"

vector<string> set_vec = {
    // "8020_Time_5min_L1Delay00_Ncollision127_Opentime120",
    "8021_Time_5min_L1Delay20_Ncollision127_Opentime80",
    "8023_Time_5min_L1Delay20_Ncollision127_Opentime75",
    "8024_Time_5min_L1Delay20_Ncollision127_Opentime70",
    "8025_Time_5min_L1Delay20_Ncollision127_Opentime65",
    "8026_Time_5min_L1Delay20_Ncollision127_Opentime60",
    "8027_Time_5min_L1Delay20_Ncollision127_Opentime50",
    "8028_Time_5min_L1Delay20_Ncollision127_Opentime40",
    "8030_Time_5min_L1Delay20_Ncollision127_Opentime30",
    "8031_Time_5min_L1Delay20_Ncollision127_Opentime20",
    "8035_Time_5min_L1Delay20_Ncollision127_Opentime19",
    "8034_Time_5min_L1Delay20_Ncollision127_Opentime17",
    "8033_Time_5min_L1Delay20_Ncollision127_Opentime15",
    "8032_Time_5min_L1Delay20_Ncollision127_Opentime10"
    
};

vector<string> server_vec = {
    "intt0",
    "intt1", // note : new added, big file, was not converted into the ROOT file in the beginning.
    "intt2",
    "intt3",
    "intt4",
    "intt5",
    "intt6",
    "intt7"
};

 TString mother_directory = "/home/inttdev/data/IR_DAQ_server/INTT_study_run/BCO_window/data_analysis";

void Nhit_dependency()
{

    bool set_logY = false;
    int bin_cut = 2;

    vector< TH1F * > hist_vec(server_vec.size(),NULL);

    vector<double> y_vec; y_vec.clear();
    vector<double> y_e_vec; y_e_vec.clear();

    vector<double> x_vec; x_vec.clear();
    vector<double> x_e_vec; x_e_vec.clear();

    vector<double> set_calculate_vec; set_calculate_vec.clear();

    for (int set = 0; set < set_vec.size(); set++) // note : set
    {
        for (int server = 0; server < server_vec.size(); server++) // note : server
        {
            TFile * file_in = new TFile(Form("%s/%s/multiplicity_%s_calib_%s-0000%s-0000.root",mother_directory.Data(),set_vec[set].c_str(),server_vec[server].c_str(),server_vec[server].c_str(),set_vec[set].substr(0,4).c_str()),"read");
        
            hist_vec[server] = (TH1F *) file_in->Get(Form("%s_hist",server_vec[server].c_str()));

            set_calculate_vec.push_back(hist_vec[server] -> Integral(bin_cut, -1 ) / hist_vec[server]->GetEntries());
        }

        y_vec.push_back( vector_average(set_calculate_vec) );
        y_e_vec.push_back( vector_stddev(set_calculate_vec) );

        x_vec.push_back( stoi(set_vec[set].substr(set_vec[set].find("Opentime") + 8,2)) );
        x_e_vec.push_back(0);

        cout<<"x test : "<<x_vec[set]<<endl;

        set_calculate_vec.clear();
        if (set != set_vec.size() - 1) hist_vec.clear();
    }

    TCanvas * c1 = new TCanvas("c1","c1",850 ,800);
    TPad * pad_c1 = new TPad(Form("pad_c1"), "", 0.0, 0.0, 1.0, 1.0);
    Characterize_Pad(pad_c1, 0.15,  0.1,  0.1,  0.12, set_logY, 0);
    pad_c1 -> Draw();
    pad_c1 -> cd();

    TF1 * fitting = new TF1("","[2] * 0.5* (1.0 + TMath::Erf((x - [0]) / [1] / TMath::Sqrt2())) + [3]",0, 100);
    fitting -> SetParameter(0,17); // note :turn on
    fitting -> SetParameter(1,1); // note :slope
    fitting -> SetParameter(2,0.15);// note :height
    fitting -> SetParameter(3,0.1);// note :offset

    TGraphErrors * plot = new TGraphErrors(x_vec.size(), &x_vec[0], &y_vec[0], &x_e_vec[0], &y_e_vec[0]);
    plot -> SetMarkerStyle(20);
    plot -> SetMarkerSize(2);
    plot -> SetTitle("Entry ratio in certain range vs BCO window");
    plot -> GetXaxis() -> SetTitle("BCO window");
    plot -> GetYaxis() -> SetTitle(Form("(Entry in bin%i to bin%i) / All entry ",bin_cut,hist_vec[0]->GetNbinsX()));

    plot -> Fit(fitting,"N","",16,45);

    plot -> Draw("apl");

    fitting -> Draw("lsame");

    c1 -> Print(Form(mother_directory + "/" "ratio_plot_startbin_%i.pdf",bin_cut));
}