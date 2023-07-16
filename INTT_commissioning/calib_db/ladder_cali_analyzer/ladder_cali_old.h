#ifndef ladder_cali_h
#define ladder_cali_h

// struct cali_hit {
//     int ampl;
//     int adc;
// };

// todo : if the cut increases, this part has to be modified as well
struct bad_ch_info {
    int chip;
    int chan;
    int ch_entry;
    int ch_entry_ampl_cut;
    double turn_on;
    double width;
    int profile;
};

class ladder_cali
{
    public :
        ladder_cali(TString ladder_name, int ROC_index, TString ladder_port, int Felix_ch, int threshold, int ampl_cut, double used_sigma, int classify_base, bool output_basic, bool output_dead_channel_tree);

        // note : some functions
        TString GetLadderName();
        TString GetLadderPort();
        int GetPortID();
        int GetFelixCh();
        int GetThreshold();
        int GetClassifyBase();
        void SetAmplCut(int input_ampl_cut);
        int GetAmplCut();
        void SetAdcPickL(int input_adc_pick_l);
        int GetAdcPickL();
        void SetAdcPickR(int input_adc_pick_r);
        int GetAdcPickR();
        void PrintClassifierPar();

        // note : core functions
        void Fill(int chip_id, int chan_id, int ampl, int adc); // note : the function should be in the for loop, the selection can also be here. I think
        void Fit();
        void Overall_info();
        void Classifier(TString outside_file_in);
        
        // note : draw_plot_range : U1 to U26
        // note : the "TString file_name" is the input root file name, to be shown on the plots.
        void Draw_ch_plot(vector<int>draw_plot_range, TString plot_folder_dire, TString file_name, TString draw_what );
        void Draw_overall_info(TString plot_folder_dire, TString file_name);
        void Draw_bad_ch_plot(TString plot_folder_dire, TString file_name);
        void Output_bad_ch_root(TString plot_folder_dire, TString file_name);
        void Output_ch_property_root(TString plot_folder_dire, TString file_name);
        // note : this is for the python, I know it's stupid.
        void Output_bad_ch_txt(TString plot_folder_dire); 

        
        
        // note : some parameters 
        TString ladder_name;
        int ROC_index;
        TString ladder_port;
        int Felix_ch;
        int threshold;
        int classify_base;
        bool output_basic; // note : the checked values of each channel
        bool output_dead_channel_tree; // note : output dead channel tree

        map<TString,int> port_map;

        // note : some parameters for hard selection
        int ampl_cut;
        int adc_pick_l; // note : used adc data points for the linearity and width consistency.
        int adc_pick_r; // note : used adc data points for the linearity and width consistency.

        // note : some space 
        TH1F * ampl_single[26][128][8]; 
        TF1 * ampl_single_fitter[26][128][8]; 
        
        TH1F * ampl_total[26][128]; 
        TF1 * ampl_total_fitter[26][128];

        TGraphErrors * linearity[26][128];
        TF1 * linearity_fitter[26][128];

        TGraphErrors * width_consistency[26][128];
        TF1 * width_consistency_fitter[26][128];

        int ampl_cut_count[26][128];

        vector<int>draw_plot_range;
        double used_sigma;
        int gold_profile;
        int N_check;
        int ch_profile[26][128];
        vector<bad_ch_info> bad_ch_list;
        
        // note : parameters for the fitting
        int ampl_max;
        int total_pulses;
        int default_true_threshold;
        double default_width;
        double max_turn_on_limit;
        double Overall_ch_entry_height;
        double Overall_ch_entry_mean;
        double Overall_ch_entry_width;
        double Overall_ch_entry_chi;


        // note : for the overall information
        TH1F * ch_entry;
        TH1F * ampl_total_turn_on;
        TH1F * ampl_total_width;
        TH1F * ch_entry_ampl_cut;

        TF1 * ch_entry_fitter;
        TF1 * ampl_total_turn_on_fitter;
        TF1 * ampl_total_width_fitter;

        //note : par for classifier
        pair <double,double> ch_entry_range;
        pair <double,double> ch_entry_ampl_cut_range;
        pair <double,double> ampl_total_turn_on_range;
        pair <double,double> ampl_total_width_range;


        // todo : need a function to print all the ch status (root file) --> done
        // todo : need a function to print the dead channel (root file) --> done
};  

#endif