#ifndef trunresidual_h
#define trunresidual_h

#include "TAnaResidual.h"


class TRunResidual : public TAnaResidual
{
    public : 
        TRunResidual(TString class_name, int Initialize_mode, TString folder_direction, TString cluster_file_name, int study_chip, double cluster_adc_value_requirement = 22.5, double l1_alignment_correction = 0, double l0l2_slope_correction = 0, double l0l1_slope_correction = 0)
        :TAnaResidual(class_name, Initialize_mode, folder_direction, cluster_file_name, study_chip, l1_alignment_correction, l0l2_slope_correction, l0l1_slope_correction)
        {
            TAnaResidual::Alignment_DUT(); // note : to get raw data for the alignment correction and l0l2 slope correction
            TAnaResidual::Par_Set_l1_alignment( TAnaResidual::Get_l1_alignment(true) );  // note : print_plots = true
            TAnaResidual::Par_Set_l0l2_slope( TAnaResidual::Get_l0l2_slope(true) ); // note : print_plots = true

            TAnaResidual::Alignment_DUT(); // note : this is for the "AFTER" the slope correction, the l0l2 slope plot
            TAnaResidual::Get_l0l2_slope(true);
            // note : re-generate the vector again, APPLY the l1 alignment
            // note : vec_cluster_reformat_after replaces vec_cluster_reformat
            TAnaResidual::cluster_reformat_after(); 
            TAnaResidual::Get_l1_efficiency();

            TAnaResidual::Pre_Get_l0l1_slope(cluster_adc_value_requirement);
            TAnaResidual::Par_Set_l0l1_slope( TAnaResidual::Get_l0l1_slope(true) );

            TAnaResidual::Pre_Get_l0l1_slope(cluster_adc_value_requirement);
            TAnaResidual::Get_l0l1_slope(true);
            TAnaResidual::Gen_l0l1_slope_pos();

            cout<<"Final, correction check, l0l2_slope "<< TAnaResidual::Par_Get_l0l2_slope()<<", l1_alignment "<< TAnaResidual::Par_Get_l1_alignment()<<", l0l1_slope :  "<< TAnaResidual::Par_Get_l0l1_slope()<<endl;
        }
    // private : 
    //     TH1F * Get_residual_dist();
    //     TH1F * Get_scattering_dist();
    //     TH1F * l0l1_residual_hist;
    //     TH1F * l0l1_scattering_hist;
};

#endif