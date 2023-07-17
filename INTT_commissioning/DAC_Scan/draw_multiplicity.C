vector<string> read_file_list (TString file_list_directory)
{

    // TString file_list_directory = mother_folder_directory + "/data_analysis/" + set_folder_name + "/" + server_name + "/" + "file_list.txt";
    // TString file_list_directory = mother_folder_directory + "/file_list.txt";
    vector<string> file_list_vec; file_list_vec.clear();

    unsigned long len_unsigned_long = -1;

    fstream list_file;
    list_file.open(Form("%s", file_list_directory.Data()), ios::in);
    if (list_file.is_open()) 
    {
        string data;
        while (getline(list_file, data)) 
        {
            if ( TString(data[0]) == "#"  )  { continue; }
            if ( TString(data[0]) == " "  )  { continue; }
            if ( TString(data[0]) == ""   )  { continue; }
            if ( TString(data[0]) == "  " )  { continue; }
            if (TString(data).Length() < 2)  { continue; }
            if ( data.find(".root")   == len_unsigned_long)     { continue; }

            TString data_TString(data);

            file_list_vec.push_back(data);
        }
        list_file.close();
    }

    
    return file_list_vec;
}

void draw_multiplicity()
{   
    TCanvas * c1 = new TCanvas("","",800,800);

    vector<string> file_name_vec = read_file_list("/home/phnxrc/INTT/cwshih/DACscan_data/test_intt4/file_list.txt");

    for (int i = 0; i < file_name_vec.size(); i++)
    {
        c1 -> cd();
        c1 ->SetLogy();

        TFile * file_in = new TFile(file_name_vec[i].c_str(),"read");
        TTree * tree = (TTree *)file_in->Get("tree");

        tree -> Draw("fNhits");

        c1 -> Print(Form("/home/phnxrc/INTT/cwshih/DACscan_data/test_intt4/plots/hit_multiplicity/%s_multiplicity.pdf", file_name_vec[i].substr(file_name_vec[i].find("beam_intt4-"),file_name_vec[i].find(".root")-file_name_vec[i].find("beam_intt4-")).c_str()));
        c1 -> Clear();
        file_in -> Close();
    }
}