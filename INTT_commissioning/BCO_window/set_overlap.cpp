#include "draw_style.h"

// TString color_code [8]={"#343434","#1A3947","#566575","#797983","#EFBD9D","#FCA26E","#F5751D","#F5321D"};
TString color_code_2 [8]={"#CC768D","#19768D","#DDA573","#009193","#941100","#1A3947","#A08144","#517E66"};

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

void set_overlap(TString mother_folder_directory, TString set_name, bool set_logY = true, bool nor_bool = true)
{

    // TString mother_folder_directory = "/home/inttdev/data/IR_DAQ_server/INTT_study_run/BCO_window";
    // TString set_name = "test_set";
    // bool set_logY = true;

    vector<string> file_list_vec; file_list_vec.clear();

    TString ana_directory       = mother_folder_directory + "/data_analysis/" + set_name;
    TString file_list_directory = ana_directory + "/file_list.txt";

    unsigned long len_unsigned_long = -1;

    fstream map_file;
    map_file.open(Form("%s", file_list_directory.Data()), ios::in);
    if (map_file.is_open()) 
    {
        string data;
        while (getline(map_file, data)) 
        {
            if ( TString(data[0]) == "#"  )  { continue; }
            if ( TString(data[0]) == " "  )  { continue; }
            if ( TString(data[0]) == ""   )  { continue; }
            if ( TString(data[0]) == "  " )  { continue; }
            if (TString(data).Length() < 2)  { continue; }
            
            TString file_name = TString(std::regex_replace(data, std::regex("^.*/"), "")).ReplaceAll(".root","");

            string file_name_str(file_name.Data());

            file_list_vec.push_back(file_name_str);
        }
        map_file.close();
    }

    // THStack * stack_hist = new THStack("stack_hist",Form("Stack, set : %s",set_name.Data()));

    vector<TH1F *> hist_vec(file_list_vec.size(),NULL);

    TLegend * legend = new TLegend (0.75, 0.6, 0.9, 0.90);
    legend -> SetTextSize (0.028);

    TCanvas * c1 = new TCanvas("c1","c1",850 ,800);
    TPad * pad_c1 = new TPad(Form("pad_c1"), "", 0.0, 0.0, 1.0, 1.0);
    Characterize_Pad(pad_c1, 0.15,  0.1,  0.1,  0.12, set_logY, 0);
    pad_c1 -> Draw();
    pad_c1 -> cd();

    //todo : easy assumption, all the servers are, used intt0 to intt7
    //todo : if you have less server to be ploted, modify here.
    for (int i = 0; i < file_list_vec.size(); i++)
    {
        TFile * file_in = new TFile(Form("%s/multiplicity_intt%i_%s.root",ana_directory.Data(),stoi(file_list_vec[i].substr(file_list_vec[i].find("intt")+4,1)),file_list_vec[i].c_str()));

        hist_vec[i] = (TH1F *) file_in->Get(Form("intt%i_hist",stoi(file_list_vec[i].substr(file_list_vec[i].find("intt")+4,1))));
        hist_vec[i] -> SetLineColor(TColor::GetColor(rainbowColors[i]));
        hist_vec[i] -> SetLineWidth(2);
        hist_vec[i] -> SetTitle(Form("Set : %s",set_name.Data()));
        hist_vec[i] -> GetXaxis() -> SetTitle("N_hit, single event");
        hist_vec[i] -> GetYaxis() -> SetTitle("Entry");
        hist_vec[i] -> SetStats(0);
        hist_vec[i] -> Scale(1. / hist_vec[i] -> Integral(-1,-1));

        hist_vec[i] -> SetMaximum(1.3);
        hist_vec[i] -> SetMinimum(pow(10,-6.5));

        TString draw_text = (i == 0) ? "hist" : "hist same";
        hist_vec[i] -> Draw(draw_text);

        legend -> AddEntry (hist_vec[i], Form("intt%i",stoi(file_list_vec[i].substr(file_list_vec[i].find("intt")+4,1))), "f");
    }
    
    // if (set_logY == true) {stack_hist -> Scale(1./stack_hist->Integral(-1,-1));}

    legend -> Draw("same");

    TString output_plot_name = Form("%s_overlap", set_name.Data());
    output_plot_name += (set_logY == true) ? "_log" : "_linear";   

    // output_plot_name += (nor_bool == true) ? "_nor" : void();

    c1 -> Print( Form("%s/%s.pdf",ana_directory.Data(), output_plot_name.Data()) );
	c1 -> Clear();




}