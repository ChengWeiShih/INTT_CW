
// note : this code can be the simpler code for the charge sharing model
// note : the directory of this code : /home/cwshih/sPHENIX/INTT/INTT_GitHub/INTT_CW/INTT_BeamTest_AnaCode/Data_MC_dist_comp/charge_sharing_model

// note :  ~~~~~~~~~~~~~ ~~~~~~~~~~~~~ global variable ~~~~~~~~~~~~~ ~~~~~~~~~~~~~ 

// note : for the Z axis
double layer_classification[4] = {-20, 10, 30, 50}; // todo : may have to be adjust for the vertical rotation run
int module_id[3] = {1, 6, 5};

// note : for the X axis
// note : we move the whole box, instead of moving the beam position
// note : now we define : we move the positive side : positive value, follow the coordinate
// note : the number -74.16 is the default value
double Box_X_offset = -74.16; // todo : may have to add value 
// double chip_edge_A[9] = {-115.1, -99.1, -83.1, -67.1, -51.1, -35.1, -19.1, -3.1, 12.9}; // note : type A 
// double chip_edge_B[6] = {15.1, 35.1, 55.1, 75.1, 95.1, 115.1}; // note : type B
// note : we rotated the model in Geant4, in order to fit the experiment setup
double chip_edge_A[9] = {-12.9, 3.1, 19.1, 35.1, 51.1, 67.1, 83.1, 99.1, 115.1}; // note : type A 
double chip_edge_B[6] = {-115.1, -95.1, -75.1, -55.1, -35.1, -15.1}; // note : type B

// note : for the Y axis
// note : we define : move the thing downward : negative value
// note : 4.992 is the default value
double Box_Y_offset = -5. + 4.992; // todo : may have to add value 
double L0_ladder_offset = -0.29142500; // todo : may have to fill number 
double sensor_width = 0.078;
double sensor_lowest_center = -9.945;
double sensor_lowest_edge   = -9.984;

// note : pars for charge-sharing model
// note : the cloud width of each sub-step is different, determined by the gaussian distribution, the width of the distribution is determined by cloud_width
int nsegments = 2;
double cloud_width = 0.005; // note : 5 um
double used_sigma = 4; 

// note : pars for digitization
double ADC_gain = 100;
// note : based on the current knowledge, we need an offset term, and the offset value can be either 200 or 280.
// note : but the problem is, if we set it at 280, then the minimum ADC becomes 17.5, which means as long as there is a thing, it passes the threshold.
double ADC_offset = 200; 
double DAC_value[8] = {15, 30, 60, 90, 120, 150, 180, 210};

//note : other parameters
int double_size = 8; // note : the size of a double declaration
bool fix_rand_seed = false;
int random_seed = 300;
bool add_noise_bool = false;

//note : the model of the shape of the Charge cloud
TF1 * CC_model = new TF1("CC_model","TMath::Gaus(x,[0],[1],[2])",-50,50);


struct step_info{

    int    eID;
    double edep;

    double pre_X;
    double pre_Y;
    double pre_Z;

    double post_X;
    double post_Y;
    double post_Z;

    int layer_id; // note : layer_id, 0, 1, 2
    int column_id; // note : column ID, 1 ~ 13
    int chan_id;
};

struct segn_step_info{
    double edep;
    double CW; // note : cloud width
    double x;
    double y;
    double z;

    int layer_id; // note : layer_id, 0, 1, 2
    int column_id; // note : column ID, 1 ~ 13
    int chan_id;
    double chan_center;

    pair<int,int> involve_chan; // note : first upper, second, donw

    // int involve_chan_upper;
    // int involve_chan_bottom;
};

struct simple_pos{
    double x;
    double y;
    double z;
};

struct INTT_event_final_s
{
    vector<int> camac_adc_final;
    vector<int> camac_tdc_final;
    bool INTT_event_final;
    vector<int> adc_array_final;
    vector<int> ampl_array_final;
    vector<int> chip_id_array_final;
    vector<int> fpga_id_array_final;
    vector<int> module_array_final;
    vector<int> chan_id_array_final;
    vector<int> fem_id_array_final;
    vector<UInt_t> bco_array_final;
    vector<UInt_t> bco_full_array_final;
    vector<int> event_array_final;

    int MC_eID;

};

simple_pos Get_seg_pos (step_info input_step_info, int nsegments ,int index) // note : index starts from 0
{
    simple_pos pos_out;

    simple_pos total_dist;
    total_dist.x = (input_step_info.post_X - input_step_info.pre_X);
    total_dist.y = (input_step_info.post_Y - input_step_info.pre_Y);
    total_dist.z = (input_step_info.post_Z - input_step_info.pre_Z);

    
    
    pos_out.x = input_step_info.pre_X + (total_dist.x / (nsegments * 2.)) * (2 * index + 1);
    pos_out.y = input_step_info.pre_Y + (total_dist.y / (nsegments * 2.)) * (2 * index + 1);
    pos_out.z = input_step_info.pre_Z + (total_dist.z / (nsegments * 2.)) * (2 * index + 1);

    return pos_out;
}

int get_layer_id (double pre_Z, double post_Z )
{
    double center_pos_z = (post_Z + pre_Z) / 2.;
    int layer_index;
    int index_check[3] = {0,1,2};

    if ( center_pos_z > layer_classification[0] && center_pos_z < layer_classification[1] ) {layer_index = 0;}
    else if ( center_pos_z > layer_classification[1] && center_pos_z < layer_classification[2] ) {layer_index = 1;}
    else if ( center_pos_z > layer_classification[2] && center_pos_z < layer_classification[3] ) {layer_index = 2;}
    else 
    {
        cout<<"Layer calculation error !!! "<<endl;
        layer_index = -100;
    }
    
    return index_check[layer_index];
}

// note : column ID starts from 1 to 13.
int get_column_id (double pre_X, double post_X)
{
    double center_X = (post_X + pre_X) / 2.;
    
    int column_id = -10;
    
    if (center_X >= chip_edge_B[0] && center_X <= chip_edge_B[5]) // note : type B
    {
        for (int i = 0; i < 6 - 1; i++)
        {
            if ( center_X > chip_edge_B[i] && center_X < chip_edge_B[i+1] )
            {   
                column_id = i + 1;
                break;
            }
        }
    }
    else if (center_X >= chip_edge_A[0] && center_X <= chip_edge_A[8]) // note : type A
    {
        for (int i = 0; i < 9 - 1; i++)
        {
            if ( center_X > chip_edge_A[i] && center_X < chip_edge_A[i+1] )
            {   
                column_id = i + 1 + 5;
                break;
            }
        }
    }

    else 
    {
        cout<<pre_X<<" "<<post_X<<" Caution !!! The step hit with wrong X axis value"<<endl;
    }

    if (column_id == -10) cout<<"Caution !!! wrong in the step hit"<<endl;

    return column_id;

}

// note : first : the channel ID
// note : second : the center position of that channel
pair<int,double> get_chan_id (double pre_Y, double post_Y) 
{  

    double Ypos = (pre_Y + post_Y) / 2.;

    pair<int,double> pair_out;
    pair_out.first = int ((Ypos - sensor_lowest_edge) / sensor_width); // note : the channel ID
    pair_out.second = ( sensor_lowest_center + sensor_width * pair_out.first); // note : The center position of that channel 
    
    return pair_out;

}

// note : calculate number of channels is involved in this charge cloud
// note : first : upper, second : bottom 
pair<int,int> get_involve_chan (double y_pos, double CW, double chan_center)
{
    pair<int,int> pair_out;

    // note : upper
    pair_out.first  = TMath::CeilNint( ( (y_pos + CW *  used_sigma) - ( chan_center - (sensor_width / 2.) ) ) / sensor_width );
    // note : downward
    pair_out.second = TMath::CeilNint( ( ( chan_center + (sensor_width / 2.) ) - (y_pos - CW *  used_sigma) ) / sensor_width );

    return pair_out;
}

double Get_XYZ_dist (step_info input_step_info)
{
    return sqrt(pow(input_step_info.post_X - input_step_info.pre_X,2) + pow(input_step_info.post_Y - input_step_info.pre_Y,2) + pow(input_step_info.post_Z - input_step_info.pre_Z,2));
}

double get_portion_edep (double CW, double sub_step_center, double sub_step_edep, double CW_edge_low, double CW_edge_high)
{
    CC_model -> SetParameters(sub_step_center, CW, 1); // note ; the third parameters "1" is to turn on the normalize
    return sub_step_edep * fabs( CC_model -> Integral(CW_edge_low, CW_edge_high) );
}

double get_edep_convert (double input_edep)
{
    return (input_edep * 100 * 1.6 * ADC_gain / 3.6 + ADC_offset - 210.) / 4.;
}

int get_adc (double intput_edep) // note : after the conversion to DAC value
{
    double adc;

    if      (intput_edep > DAC_value[0] && intput_edep <= DAC_value[1]) { adc = 0; }
    else if (intput_edep > DAC_value[1] && intput_edep <= DAC_value[2]) { adc = 1; }
    else if (intput_edep > DAC_value[2] && intput_edep <= DAC_value[3]) { adc = 2; }
    else if (intput_edep > DAC_value[3] && intput_edep <= DAC_value[4]) { adc = 3; }
    else if (intput_edep > DAC_value[4] && intput_edep <= DAC_value[5]) { adc = 4; }
    else if (intput_edep > DAC_value[5] && intput_edep <= DAC_value[6]) { adc = 5; }
    else if (intput_edep > DAC_value[6] && intput_edep <= DAC_value[7]) { adc = 6; }
    else{adc=7;}

    return adc;
}

void CSM_test_1 ()
{

    // note : unit, mm for all the code 

    TString file_directory = "/home/cwshih/sPHENIX/INTT/Testbeam/Testbeam2021/INTT_BeamTest_2021_G4/INTT_G4_v5_2023_01_04/session7_solution_build";
    TString file_name      = "e+_beam-para_4";

    //note : for the output
    TString output_file_name = file_name + Form("_CW_%.3f",cloud_width) + Form("_nSubStep_%i",nsegments) + Form("_UsedSigma_%.3f",used_sigma) + Form("_DACOffset_%i",ADC_offset);
    if (add_noise_bool == true) {output_file_name += "_WithNoise_";}

    TFile* G4_step_file_in = TFile::Open (Form("%s/%s.root", file_directory.Data(),file_name.Data()), "read");
    
    TTree *step_tree = (TTree *)G4_step_file_in -> Get("event_particle");
    int N_step_tree= step_tree -> GetEntriesFast();
    cout<<"Number of event in step tree : "<<N_step_tree<<endl;

    int eID;
    double step_edep;
    int pre_process;
    double pre_X, pre_Y, pre_Z;
    int post_process;
    double post_X, post_Y, post_Z;


    step_tree -> SetBranchAddress("Event_ID",&eID);

    step_tree -> SetBranchAddress("step_edep",&step_edep);

    step_tree -> SetBranchAddress("pre_X",&pre_X);
    step_tree -> SetBranchAddress("pre_Y",&pre_Y);
    step_tree -> SetBranchAddress("pre_Z",&pre_Z);
    step_tree -> SetBranchAddress("pre_process",&pre_process);

    step_tree -> SetBranchAddress("post_X",&post_X);
    step_tree -> SetBranchAddress("post_Y",&post_Y);
    step_tree -> SetBranchAddress("post_Z",&post_Z);
    step_tree -> SetBranchAddress("post_process",&post_process);

    step_tree -> GetEntry(0);
    int first_eID = eID;

    vector<vector<step_info>> event_array; event_array.clear();
    vector<step_info> single_event_array; single_event_array.clear();
    step_info step_unit;
    double sum_Y_offset;

    for (int i = 0; i < N_step_tree; i++)
    {
        step_tree -> GetEntry(i);

        if (i % 10000 == 0)
        {
            cout<<"step info, eID : "<<eID<<" edep : "<<step_edep<<" \\ pre : "<<pre_X<<" "<<pre_Y<<" "<<pre_Z<<" "<<pre_process<<" \\ Post : "<<post_X<<" "<<post_Y<<" "<<post_Z<<" "<<post_process<<endl;
        }


        if (eID == first_eID)
        {
            step_unit.eID   = eID;
            step_unit.edep  = step_edep;

            step_unit.pre_Z = pre_Z;
            step_unit.post_Z = post_Z;
            step_unit.layer_id  = get_layer_id(step_unit.pre_Z, step_unit.post_Z);

            step_unit.pre_X = pre_X - Box_X_offset;
            step_unit.post_X = post_X - Box_X_offset;
            step_unit.column_id = get_column_id(step_unit.pre_X, step_unit.post_X);

            sum_Y_offset = (step_unit.layer_id == 1) ? (Box_Y_offset + L0_ladder_offset) : Box_Y_offset;
            step_unit.pre_Y = pre_Y - sum_Y_offset;
            step_unit.post_Y = post_Y - sum_Y_offset;
            step_unit.chan_id = get_chan_id(step_unit.pre_Y, step_unit.post_Y).first;

            single_event_array.push_back(step_unit);
        }
        else 
        {
            first_eID = eID;

            event_array.push_back(single_event_array);

            single_event_array.clear();
            

            // note : ========== part for saving the step ===========
            step_unit.eID   = eID;
            step_unit.edep  = step_edep;

            step_unit.pre_Z = pre_Z;
            step_unit.post_Z = post_Z;
            step_unit.layer_id  = get_layer_id(step_unit.pre_Z, step_unit.post_Z);

            step_unit.pre_X = pre_X - Box_X_offset;
            step_unit.post_X = post_X - Box_X_offset;
            step_unit.column_id = get_column_id(step_unit.pre_X, step_unit.post_X);

            sum_Y_offset = (step_unit.layer_id == 1) ? (Box_Y_offset + L0_ladder_offset) : Box_Y_offset;
            step_unit.pre_Y = pre_Y - sum_Y_offset;
            step_unit.post_Y = post_Y - sum_Y_offset;
            step_unit.chan_id = get_chan_id(step_unit.pre_Y, step_unit.post_Y).first;
            // note : ========== part for saving the step ===========
            
            single_event_array.push_back(step_unit);
        }
    } // note : end of for loop, number of step

    event_array.push_back(single_event_array);
    single_event_array.clear();

    cout<<"===================== event structure done ====================="<<endl;

    // note : ===================== ===================== event structure done ===================== ===================== 

    segn_step_info sub_step_unit;
    // vector<segn_step_info> sub_step_array; sub_step_array.clear();
    
    TRandom * rand_CW = new TRandom3();
    vector<double> sub_stephit_CW; sub_stephit_CW.clear(); // title : not used
    TH1F * CW_dist = new TH1F("","",100,-0.05,0.05);
    double stephit_CW_unit;
    
    // note : the container to carry the edep
    double chan_edep[3][13][256];
	memset (chan_edep, 0, sizeof(chan_edep));
    double CW_edge_low, CW_edge_high;

    //note : things for the output file
    double edep_convert;

    TRandom *rand_bcofull = new TRandom3 ( 0 ); if (fix_rand_seed == true) { rand_bcofull -> SetSeed(random_seed); }
	UInt_t Frand_bcofull;
	UInt_t Frand_bco;

    TFile *file_output;
    file_output = new TFile( Form("%s/%s.root", file_directory.Data(), output_file_name.Data()), "RECREATE");
    
    vector<int> camac_adc = {100,100,100,100};
	vector<int> camac_tdc = {20,20,20,20,20,20};
	bool INTT_event = 1;

    vector<int> adc_array;      adc_array.clear();
	vector<int> ampl_array;     ampl_array.clear();
	vector<int> chip_id_array;  chip_id_array.clear();
	vector<int> fpga_id_array;  fpga_id_array.clear();
	vector<int> module_array;   module_array.clear();
	vector<int> chan_id_array;  chan_id_array.clear();
	vector<int> fem_id_array;   fem_id_array.clear();
	vector<UInt_t>	bco_array;      bco_array.clear();
	vector<UInt_t> bco_full_array; bco_full_array.clear();
	vector<int> INTT_event_array;    INTT_event_array.clear();
    int MC_eID;

    TTree *tree_output3 = new TTree("tree_both", "tree_both");
    tree_output3->Branch("camac_adc" ,&camac_adc);
	tree_output3->Branch("camac_tdc" ,&camac_tdc);
	tree_output3->Branch("INTT_event",&INTT_event);
	tree_output3->Branch("adc"       ,&adc_array);
	tree_output3->Branch("ampl"      ,&ampl_array);
	tree_output3->Branch("chip_id"   ,&chip_id_array);
	tree_output3->Branch("fpga_id"   ,&fpga_id_array);
	tree_output3->Branch("module"    ,&module_array);
	tree_output3->Branch("chan_id"   ,&chan_id_array);
	tree_output3->Branch("fem_id"    ,&fem_id_array);
	tree_output3->Branch("bco"       ,&bco_array);
	tree_output3->Branch("bco_full"  ,&bco_full_array);
	tree_output3->Branch("event"     ,&INTT_event_array); //-1 for always
    tree_output3->Branch("MC_eID"     ,&MC_eID);
    

    for (int i = 0; i < 1 /*event_array.size()*/; i++) // note : the for loop for the number of event
    {
        if(i % 10000 == 0) cout<<"===================== ===================== ===================== runnting CC model : " << i << " ===================== ===================== ====================="<<endl; 
        for (int i1 = 0; i1 < event_array[i].size(); i1++) // note : the for loop for the step hit in the event
        {
            if(i % 10000 == 0) cout<<"--------------------- event step, eID : "<<event_array[i][i1].eID<<" "<<event_array[i][i1].edep<<" \\ pre pos : "<<event_array[i][i1].pre_X<<" "<<event_array[i][i1].pre_Y<<" "<<event_array[i][i1].pre_Z<<"\\ post pos : "<<event_array[i][i1].post_X<<" "<<event_array[i][i1].post_Y<<" "<<event_array[i][i1].post_Z<<endl;
            if(i % 10000 == 0) cout<<"step edep :  "<<event_array[i][i1].edep<<endl;
            if(i % 10000 == 0) cout<<"total distance : "<<Get_XYZ_dist(event_array[i][i1])<<endl;
            if(i % 10000 == 0) cout<<"Dist, X : "<<fabs(event_array[i][i1].post_X - event_array[i][i1].pre_X)<<" Y : "<<fabs(event_array[i][i1].post_Y - event_array[i][i1].pre_Y)<<" Z : "<<fabs(event_array[i][i1].post_Z - event_array[i][i1].pre_Z)<<endl;
            if(i % 10000 == 0) cout<<"layer : "<<event_array[i][i1].layer_id<<" column : "<<event_array[i][i1].column_id<<" chan : "<<event_array[i][i1].chan_id<<" chan_center : "<<get_chan_id(event_array[i][i1].pre_Y,event_array[i][i1].post_Y).second<<endl;
            if(i % 10000 == 0) cout<<"~~~~~~~~~~~~~~~ n segments start ~~~~~~~~~~~~~~~"<<endl;
            for (int i2 = 0; i2 < nsegments; i2++) // note the for loop for the sub step hit
            {   
                // note : sub step_hit position
                simple_pos centroid = Get_seg_pos(event_array[i][i1], nsegments, i2);
                sub_step_unit.x = centroid.x; sub_step_unit.y = centroid.y; sub_step_unit.z = centroid.z; 
                
                // note : sub step_hit edep, layer_id, column_id, chan_id
                sub_step_unit.edep = (event_array[i][i1].edep) / double(nsegments); 
                sub_step_unit.layer_id = event_array[i][i1].layer_id; sub_step_unit.column_id = event_array[i][i1].column_id; sub_step_unit.chan_id = event_array[i][i1].chan_id;  
                sub_step_unit.chan_center = get_chan_id(sub_step_unit.y, sub_step_unit.y).second; // note : (A + A) / 2. = A

                // note : for the cloud width determination
                //todo, maybe we tune the cloud_width based on the edep
                //todo, may need a function to check the width of the charge cloud
                stephit_CW_unit = rand_CW -> Gaus(0,cloud_width); 
                // sub_stephit_CW.push_back(stephit_CW_unit); // title : not used
                CW_dist -> Fill(stephit_CW_unit);
                sub_step_unit.CW = fabs(stephit_CW_unit);

                // note : get the involved channel
                sub_step_unit.involve_chan = get_involve_chan(sub_step_unit.y, sub_step_unit.CW, sub_step_unit.chan_center);


                // note : start filling the edep to the channel
                for (int ct = 0; ct < sub_step_unit.involve_chan.first; ct++) // note : for loop for upper
                {
                    CW_edge_low  = (ct == 0) ? sub_step_unit.y : CW_edge_high;
                    CW_edge_high = ( (sub_step_unit.y + sub_step_unit.CW * used_sigma) > ((sub_step_unit.chan_center + sensor_width/2.) + sensor_width * ct) ) ? ((sub_step_unit.chan_center + sensor_width/2.) + sensor_width * ct) : (sub_step_unit.y + sub_step_unit.CW * used_sigma);

                    chan_edep[sub_step_unit.layer_id][sub_step_unit.column_id][sub_step_unit.chan_id+ct] += get_portion_edep(sub_step_unit.CW, sub_step_unit.y, sub_step_unit.edep, CW_edge_low, CW_edge_high);

                    if(i % 10000 == 0) cout<<"index : "<<i2<<" upper channel add adc : "<<ct<<" edge low : "<<CW_edge_low<<" high : "<<CW_edge_high<<" edep portion : "<<get_portion_edep(sub_step_unit.CW, sub_step_unit.y, sub_step_unit.edep, CW_edge_low, CW_edge_high)<<endl;
                }
                for (int cb = 0; cb < sub_step_unit.involve_chan.second; cb++) // note : for loop for the bottom
                {
                    CW_edge_high = (cb == 0) ? sub_step_unit.y : CW_edge_low;
                    CW_edge_low  = ( (sub_step_unit.y - sub_step_unit.CW * used_sigma) < ((sub_step_unit.chan_center - sensor_width/2.) - sensor_width * cb) ) ? ((sub_step_unit.chan_center - sensor_width/2.) - sensor_width * cb) : (sub_step_unit.y - sub_step_unit.CW * used_sigma);

                    chan_edep[sub_step_unit.layer_id][sub_step_unit.column_id][sub_step_unit.chan_id-cb] += get_portion_edep(sub_step_unit.CW, sub_step_unit.y, sub_step_unit.edep, CW_edge_low, CW_edge_high);

                    if(i % 10000 == 0) cout<<"index : "<<i2<<" bottom channel add adc : "<<cb<<" edge low : "<<CW_edge_low<<" high : "<<CW_edge_high<<" edep portion : "<<get_portion_edep(sub_step_unit.CW, sub_step_unit.y, sub_step_unit.edep, CW_edge_low, CW_edge_high)<<endl;
                }

                if(i % 10000 == 0) cout<<"index : "<<i2<<" "<<sub_step_unit.x<<"\t"<<sub_step_unit.y<<"\t"<<sub_step_unit.z<<endl;
                if(i % 10000 == 0) cout<<"cloud width : "<<sub_step_unit.CW<<" involved channel : "<<sub_step_unit.involve_chan.first<<" "<<sub_step_unit.involve_chan.second<<endl;
                if(i % 10000 == 0) cout<<"chan_id : "<<sub_step_unit.chan_id<<" chan center : "<<sub_step_unit.chan_center<<endl;
            } // note : end of loop, segmantation of each step
            

        } // note : end of loop, N step in event
        
        // note : ========== ========== ========== ========== Do the data format conversion thing ========== ========== ========== ========== 
        
        MC_eID = event_array[i][0].eID;

        Frand_bcofull = int(rand_bcofull->Uniform(0.,65536.)); // note : 0 ~ 65535
        Frand_bco = Frand_bcofull & UInt_t( 255 ); // note : take the lowest 7 bits from the BCO full

        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 13; k++)
            {
                for (int l = 0; l < 256; l++)
                {
                    if (chan_edep[j][k][l] > 0)
                    {

                        edep_convert = get_edep_convert(chan_edep[j][k][l]);
                        if(i % 10000 == 0) cout<<"test in chan_edep selection : "<<edep_convert<<" "<<chan_edep[j][k][l]<<endl;

                        if (edep_convert <= DAC_value[0]) continue;

                        // cout<<"test pass the DAC0 "<<endl;

                        adc_array.push_back( get_adc(edep_convert) );
                        module_array.push_back( module_id[j] );
                        
                        // note : for chip ID and channel ID
                        if ( l > 127 ) // note : upper row, 128 ~ 255
                        {
                            chan_id_array.push_back(255 - l);
                            chip_id_array.push_back(k + 1 + 13);
                        }
                        else
                        {
                            chan_id_array.push_back(l);
                            chip_id_array.push_back(k + 1);
                        }
                        
                        bco_full_array.push_back( Frand_bcofull );
                        bco_array.push_back( Frand_bco );
                        
                        // note : the rest, nu-used values
                        ampl_array.push_back( 1 );
                        fpga_id_array.push_back( 1 );
                        fem_id_array.push_back( 1 );
                        INTT_event_array.push_back( 1 );

                        
                    } 
                }
            }
        } // note : end of fill INTT hit

        tree_output3 -> Fill();


        // note : start clean
        adc_array.clear();
        ampl_array.clear();
        chip_id_array.clear();
        fpga_id_array.clear();
        module_array.clear();
        chan_id_array.clear();
        fem_id_array.clear();
        bco_array.clear();
        bco_full_array.clear();
        INTT_event_array.clear();

        memset (chan_edep, 0, sizeof(chan_edep));

    } // note : end of event loop

    file_output->cd();
	tree_output3->Write("", TObject::kOverwrite);
	cout<<" "<<endl;
    cout<<"Charge-cloud model considered, conversion done"<<endl;
	file_output->Close();

} // note : end of world

