#include "analyzer.cpp"

void analyzer_calib_packv5_040523_0434(int single_moduleid)
{   
    TString DataIn_directory = "/home/inttdev/data/IR"; 
    TString DataOut_directory = "/home/inttdev/work/cwshih/calib_db/ladder_cali_analyzer/test_folder";
    TString file_name = "calib_packv5_040523_0434.root"; 
    int ROC_index = 1;
    int threshold = 15; 
    int ampl_cut = 50; 
    double used_sigma = 4.0; 
    int classify_base = 0; 
    bool output_basic = 0; 
    bool output_dead_channel_tree = false;

    analyzer * file_1 = new analyzer(DataIn_directory, DataOut_directory, ROC_index, file_name, threshold, ampl_cut, used_sigma, classify_base, output_basic, output_dead_channel_tree);
    
    // todo : this is for the multi process.
    vector<int> input_module_id = {single_moduleid};

    file_1 -> check_ladder(input_module_id);
    
}
