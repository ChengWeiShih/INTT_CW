#include "draw_style.h"

// TString color_code [8]={"#343434","#1A3947","#566575","#797983","#EFBD9D","#FCA26E","#F5751D","#F5321D"};
TString color_code_2[13]={"#941100","#CC768D","#EFBD9D","#19768D","#DDA573","#009193","#1A3947","#A08144","#F5751D","#517E66","#566575","#F5321D","#343434"};

TString rainbowColors[13] = {
        "#FF0000", // Red
        "#FFA500", // Orange
        "#FFFF00", // Yellow
        "#00FF00", // Green
        "#0000FF", // Blue
        "#4B0082", // Indigo
        "#EE82EE", // Violet
        "#FFC0CB", // Pink
        "#00FFFF", // Cyan
        "#FF00FF", // Magenta
        "#00CED1", // Dark Turquoise
        "#FF8C00", // Dark Orange
        "#9400D3"  // Dark Violet
    };

// std::string rainbowColors[] = {
//         "#800000",  // Dark Red
//         "#804000",  // Dark Orange
//         "#808000",  // Dark Yellow
//         "#008000",  // Dark Green
//         "#000080",  // Dark Blue
//         "#200040",  // Dark Indigo
//         "#400080",  // Dark Violet
//         "#800060",  // Dark Pink
//         "#008080",  // Dark Cyan
//         "#800080",  // Dark Magenta
//         "#20665D",  // Dark Turquoise
//         "#806000",  // Dark Gold
//         "#69609A"   // Dark Lavender
//     };

// 8020_Time_5min_L1Delay00_Ncollision127_Opentime120 

// vector<string> folder_name = {
//     // 8020_Time_5min_L1Delay00_Ncollision127_Opentime120
//     "8021_Time_5min_L1Delay20_Ncollision127_Opentime80",
//     "8023_Time_5min_L1Delay20_Ncollision127_Opentime75",
//     "8024_Time_5min_L1Delay20_Ncollision127_Opentime70",
//     "8025_Time_5min_L1Delay20_Ncollision127_Opentime65",
//     "8026_Time_5min_L1Delay20_Ncollision127_Opentime60",
//     "8027_Time_5min_L1Delay20_Ncollision127_Opentime50",
//     "8028_Time_5min_L1Delay20_Ncollision127_Opentime40",
//     "8030_Time_5min_L1Delay20_Ncollision127_Opentime30",
//     "8031_Time_5min_L1Delay20_Ncollision127_Opentime20",
//     "8035_Time_5min_L1Delay20_Ncollision127_Opentime19",
//     "8034_Time_5min_L1Delay20_Ncollision127_Opentime17",
//     "8033_Time_5min_L1Delay20_Ncollision127_Opentime15",
//     "8032_Time_5min_L1Delay20_Ncollision127_Opentime10"};

vector<string> folder_name = {
    "run_9548", // note : 80
    "run_9546", // note : 40
    "run_9553", // note : 40
    "run_9545", // note : 35
    "run_9547", // note : 30
    "run_9549", // note : 20
    "run_9550", // note : 10
    "run_9551", // note : 5
    "run_9552"  // note : 1
};

vector<int> open_time = {
    80,
    40,
    40,
    35,
    30,
    20,
    10,
    5,
    1
};


void dist_overlap (TString select_server)
{
    TString mother_directory = "/home/inttdev/data/IR_DAQ_server/INTT_study_run/BCO_window_06_09/data_analysis";
    // TString select_server = "intt0";
    bool set_logY = true;

    vector< TH1F * > hist_vec(folder_name.size(),NULL);

    TCanvas * c1 = new TCanvas("c1","c1",850 ,800);
    TPad * pad_c1 = new TPad(Form("pad_c1"), "", 0.0, 0.0, 1.0, 1.0);
    Characterize_Pad(pad_c1, 0.15,  0.1,  0.1,  0.12, set_logY, 0);
    pad_c1 -> Draw();
    pad_c1 -> cd();

    TLegend * legend = new TLegend (0.8, 0.55, 0.9, 0.90);
    legend -> SetTextSize (0.028);

    for (int i = 0; i < folder_name.size(); i++)
    {
        TFile * file_in = new TFile(Form("%s/%s/multiplicity_%s_beam_%s-0000%s-0000.root",mother_directory.Data(),folder_name[i].c_str(),select_server.Data(),select_server.Data(),folder_name[i].substr(4,4).c_str()),"read");
        
        hist_vec[i] = (TH1F *) file_in->Get(Form("%s_hist",select_server.Data()));
        // hist_vec[i] -> Scale(1. / hist_vec[i] -> GetBinContent(1));
        hist_vec[i] -> Scale(1. / hist_vec[i] -> Integral(-1,-1));

        // hist_vec[i] -> SetFillColorAlpha(TColor::GetColor(color_code_2[i]),0.6);
        // hist_vec[i] -> SetLineWidth(0);
        hist_vec[i] -> SetTitle(select_server);
        hist_vec[i] -> GetXaxis() -> SetTitle("N_hit, single event");
        hist_vec[i] -> GetYaxis() -> SetTitle("A.U.");
        hist_vec[i] -> SetLineColor(TColor::GetColor(rainbowColors[i]));
        hist_vec[i] -> SetStats(0);
        
        hist_vec[i] -> SetMinimum(pow(10,-9));

        // legend -> AddEntry(hist_vec[i], Form("%s",folder_name[i].substr(folder_name[i].find("Opentime") + 8,2).c_str()), "f");
        legend -> AddEntry(hist_vec[i], Form("%i",open_time[i]), "f");

        TString draw_text = ( i == 0 ) ? "hist" : "hist same"; 
        hist_vec[i] -> Draw(draw_text);

    }

    legend -> Draw("same");


    c1 -> Print(mother_directory + "/summary_plots" + "/" + select_server + "_overlap.pdf");

}