// note : to study the systematic uncertainty

#include "vector_stddev.h"


struct SU_out_str{
    double average;
    double stddev;
    double max;
    double min;
};

SU_out_str cut_scan_calculate (TString file_name, TString folder_direction, int cut_id, double cut_lower, double cut_upper )
{
    TFile * file_in = new TFile (Form("%s/%s.root", folder_direction.Data(), file_name.Data()), "read");
	TTree * tree_in = (TTree *)file_in->Get("cut_scan");

    double edge_exclusion;
    double slope_cut;
    double residual_cut;
    double pass_event;
    double missing_101_2;
    double missing_101_3;

    cout<< " size of the file : " << tree_in -> GetEntries() <<endl;

    tree_in -> SetBranchAddress("edge_exclusion",&edge_exclusion);
    tree_in -> SetBranchAddress("slope_cut",&slope_cut);
    tree_in -> SetBranchAddress("residual_cut",&residual_cut);
    tree_in -> SetBranchAddress("pass_event",&pass_event);
    tree_in -> SetBranchAddress("missing_101_2",&missing_101_2);
    tree_in -> SetBranchAddress("missing_101_3",&missing_101_3);

    vector<double> effi_in_range; effi_in_range.clear();

    if (cut_id == 1)
    {
        cout<<"SU for residual : "<<endl;
    }
    else if (cut_id == 2)
    {
        cout<<"SU for slope : "<<endl;
    }
    else if (cut_id == 3)
    {
        cout<<"SU for edge_exclusion : "<<endl;
    }

    for (int i = 0; i < tree_in -> GetEntries(); i++)
    {
        tree_in -> GetEntry(i);

        if (cut_id == 1) // note : residual_cut
        {
            if (residual_cut > cut_lower && residual_cut < cut_upper)
            {
                cout<<"cut within range : "<<residual_cut<<" effi : "<<pass_event / (pass_event+missing_101_2+missing_101_3)<<endl;
                effi_in_range.push_back( pass_event / (pass_event+missing_101_2+missing_101_3) );
            }
        }
        else if (cut_id == 2) // note : slope_cut
        {
            if (slope_cut > cut_lower && slope_cut < cut_upper)
            {
                cout<<"cut within range : "<<slope_cut<<" effi : "<<pass_event / (pass_event+missing_101_2+missing_101_3)<<endl;
                effi_in_range.push_back( pass_event / (pass_event+missing_101_2+missing_101_3) );
            }
        }
        else if (cut_id == 3) // note : edge_exclusion
        {
            if (edge_exclusion > cut_lower && edge_exclusion < cut_upper)
            {
                cout<<"cut within range : "<<edge_exclusion<<" effi : "<<pass_event / (pass_event+missing_101_2+missing_101_3)<<endl;
                effi_in_range.push_back( pass_event / (pass_event+missing_101_2+missing_101_3) );
            }
        }

    }


    // const int N_bins = effi_in_range.size();
    // int sort_effi_index[N_bins];
    // TMath::Sort(N_bins, &effi_in_range[0], sort_effi_index);

    auto get_max = max_element(std::begin(effi_in_range), std::end(effi_in_range));
    auto get_min = min_element(std::begin(effi_in_range), std::end(effi_in_range));

    cout<<"cut_scan : "<<cut_id<<" avg : "<<vector_average(effi_in_range)<<" stddev : "<<vector_stddev(effi_in_range)<<endl;
    cout<<"cut_scan : "<<cut_id<<" max : "<<*get_max<<" min : "<<*get_min<<endl;

    SU_out_str output_str;
    output_str.average = vector_average(effi_in_range);
    output_str.stddev  = vector_stddev(effi_in_range);
    output_str.max     = *get_max;
    output_str.min     = *get_min;

    return output_str;

}

void print_each_SU( TString folder_direction, int study_chip, double golden_effi, double residual_scan[2], double slope_scan[2], double edge_scan[2] )
{
    cout<<"SU for residual : "<<endl;
    SU_out_str residual_str = cut_scan_calculate(Form("DUT_v2_cut_scan_1_U%i",study_chip),folder_direction,1,residual_scan[0],residual_scan[1]); // note : residual
    cout<<"avg - golden : "<<residual_str.average - golden_effi<<endl;
    cout<<" "<<endl;
    cout<<" "<<endl;

    cout<<"SU for slope : "<<endl;
    SU_out_str slope_str = cut_scan_calculate(Form("DUT_v2_cut_scan_2_U%i",study_chip),folder_direction,2,slope_scan[0],slope_scan[1]); // note : slope
    cout<<"avg - golden : "<<slope_str.average - golden_effi<<endl;
    cout<<" "<<endl;
    cout<<" "<<endl;


    cout<<"SU for edge_exclusion : "<<endl;
    SU_out_str edge_str = cut_scan_calculate(Form("DUT_v2_cut_scan_3_U%i",study_chip),folder_direction,3,edge_scan[0],edge_scan[1]); // note : edge_exclusion
    cout<<"avg - golden : "<<edge_str.average - golden_effi<<endl;
    cout<<" "<<endl;
    cout<<" "<<endl;


    cout<<"SU_1 (avg) : "<<sqrt(pow(residual_str.average - golden_effi,2)+pow(slope_str.average - golden_effi,2)+pow(edge_str.average - golden_effi,2))<<endl;

    cout<<"SU_2 up : "  <<sqrt(pow(residual_str.max - golden_effi,2)+pow(slope_str.max - golden_effi,2)+pow(edge_str.max - golden_effi,2))<<endl;
    cout<<"SU_2 down : "<<sqrt(pow(residual_str.min - golden_effi,2)+pow(slope_str.min - golden_effi,2)+pow(edge_str.min - golden_effi,2))<<endl;
}