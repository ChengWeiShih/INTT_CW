/*!
	@file check_chip_prototypeMaximam5_faster.c
	@date ?
	@author ?
	@brief A TTree is made using data in .dat file and saved to a ROOT file. Some plots are made to have a look.
	@details Ver5 was made from ver3 by G. Nukazuka (RBRC, genki.nukazuka@riken.jp) on 2020/Sep/29.
			 It works almost the same as ver3 but faster.
*/

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdio>
#include "TNtuple.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TPad.h"

// returns the size of the file
int getFileSize(std::ifstream& s);

// Data in a .dat file is decode and filled to a TTree. A path to the ROOT file is returned. If error occured, "" is returned.
string MakeTree(const char* fname, int usemod = 3, int maxbuf = 0, int n_meas = 64, float maxscale = 200., bool decoded_output = false);

// Review plots are made using a TTree in the given ROOT file
void DrawPlots(string root_file, int usemod);

char* FindLatestFile();

int check_chip_prototypeMaximam_new_copy
(
	//const char* fname = "C:\root_5.34.36\macros", // a path to the data file
	int usemod = 3, // ID of the module
	int maxbuf = 0,
	int n_meas = 64,
	float maxscale = 200.
)
{

	//############################################################################################
	TString list_buffer;
	vector<TString> list_array; list_array.clear();
	TString fname;
	
	ifstream data_list;
	data_list.open(Form("dat_file.txt"));

	while ( 1 )
	{
	  data_list >> list_buffer  ;
	  list_array.push_back(list_buffer);
	  if ( !data_list.good() )
	    {
	      break;
		}
	}	

	cout<<"test list size : "<<list_array.size()<<endl;
	int run_file = data_index; // data index is not declared, it has to be changed by .sh file
	//int run_file = 0;
	fname = list_array[run_file];
	//############################################################################################



	// read the data file, decode the data, fill data to TTree, and save the TTree into a ROOT file
	bool decoded_out = false;
	//decoded_out = true;
	const string root_file = MakeTree(fname, usemod, maxbuf, n_meas, maxscale, decoded_out);

	// return 0;

	// If there was no error in MakeTree, draw some plots!
	if (root_file != "")
		DrawPlots(root_file, usemod);

	return 0;
}

/*
	@fn string MakeTree(const char* fname,int usemod,int maxbuf,int n_meas,float maxscale)
	@brief TTree fron data in the data file is saved in a ROOT file
	@param const char* fname
	@param int usemod
	@param int maxbuf
	@param int n_meas
	@param float maxscale
	@retval A path to the ROOT file or "" in the case of an error
	@details Some unused parameters and etc. are remained for the moment.
*/
string MakeTree(const char* fname, int usemod, int maxbuf, int n_meas, float maxscale, bool decoded_output)
{

	int bco, adc, ampl, col_raw, col, chip_id, fpga_id, module, mchip;
	int chan_id;
	int nhit_event, nchip_event, fem_id;
	int chiphit[8];
	int bco_full, old_bco_full, old_bco;
	int ievent = 0;
	bool check_sum_val = false;

	// constant parameters for the setup: a number of channels, chips
	const int NCHAN = 128;
	const int NCHIPS = 26;

	// make a file name to save
	string output = fname; // conversion fron const char* to string to use string::substr and string::find_last_of
	output = output.substr(0, output.find_last_of(".dat") - 3) + ".root"; // it means, for example, nwu_fphx_20200101_000000_0.dat -> nwu_fphx_20200101_000000_0 + .root
	cout << "filename   = " << output << endl;

	// save TTree into a root file
	const char* filename = output.c_str();
	TFile *file = new TFile(filename, "RECREATE");

	string output_decoded = fname; // conversion fron const char* to string to use string::substr and string::find_last_of
	output_decoded = output.substr(0, output.find_last_of(".dat") - 4) + "_decoded.dat";
	ofstream ofs;

	if (decoded_output) {
		cout << "decoded output file = " << output_decoded << endl;
		ofs = ofstream(output_decoded);
	}

	// return output; // quick solution to get the path to the output file without decoding, it means the ROOT file already exists 

	// made TTree to be filled with data
	TTree *tree = new TTree("tree", "chip info");

	// made branches for each column
	tree->Branch("adc", &adc, "adc/I");
	tree->Branch("ampl", &ampl, "ampl/I");
	tree->Branch("chip_id", &chip_id, "chip_id/I");
	tree->Branch("fpga_id", &fpga_id, "fpga_id/I");
	tree->Branch("module", &module, "module/I");
	tree->Branch("chan_id", &chan_id, "chan_id/I");
	tree->Branch("fem_id", &fem_id, "fem_id/I");
	tree->Branch("bco", &bco, "bco/I");
	tree->Branch("bco_full", &bco_full, "bco_full/I");
	tree->Branch("event", &ievent, "event/I");

	//--------------------------------------------------------------------------------------------------

	// open the data file
	std::ifstream f(fname, std::ifstream::binary);

	// quit this function if the data file cannot be opened
	if (!f.is_open()) {
		std::cerr << "Failed to open input file " << fname << std::endl;
		return "";
	}

	// get size of file
	int size = getFileSize(f);
	std::cout << "Input file " << fname << std::endl;
	std::cout << "Number of bytes in file = " << size << std::endl;

	if (size == 0) {
		cerr << "size is 0. return" << endl;
		return "";
	}

	// Brute force approach: allocate a single array to hold the entire
	// contents of the file
	UInt_t* data = new UInt_t[size / sizeof(UInt_t)];

	// read the data file and put all data into the array UInt_t* data
	f.read((char*)data, size);

	// if reading data failed, quit
	if (f.fail()) {
		std::cerr << "eof, fail, or bad condition during read" << std::endl;
		delete[] data;
		return "";
	}

	// close the data file since it's no longer needed
	f.close();

	// Counter to count the number of hits for each channel and amplitude, not used, why?
	int nhits[128][128] = { { 0 } };

	// Extract bco, channel, amplitude, and adc from each hit, and count the number of
	// hits at each amplitude for each channel:

	//  int ievent           = 0;
	int bufcnt = 0;
	int event_fem = -999, old_event_fem = -999;
	int inoise = 0, ihealthy = 0;

	int clock_data_num = 0;

	//--------------------------------------------------------------------------------------------------
	// loop over all data
	for (int index = 0; index < size / sizeof(int); bufcnt++) { // note: value of index is changed not only here

		if (maxbuf && bufcnt >= maxbuf)
			break;

#ifdef OLD
		// Get record size
		int start = index;
		int cnt = data[start];
		// Unpack the record
		int word1 = data[start + 1];
		int word2 = data[start + 2];
#else
		// here is used now
		int buflen = data[index];
		int bufid = data[index + 1];
		int cnt = buflen - 1;
		int start = index + 2;

		// Fake the old fmt logic
		int word1 = 0xFFFFFFFF;
		int word2 = 0xFFFFFFFF;

		if (bufid == 100) {
			word1 = 100;
		}
		else if (bufid == 101) {
			word1 = 0xFFFFFFFF;
			word2 = 0xFFFFFFFF;
		}
		else if (bufid == 102)
		{
			word1 = 102;
		}

		// if decoded output is required, write data
		if (decoded_output) {
			/*
			for (int j = 0; j < 3; j++) {
				ofs << setw(11) << data[index + j] << " ";
				cout << data[index + j] << " ";
			}*/
			ofs << endl; // break line 
			/*
			cout << data[index + 1] << " "
				<< data[index + buflen - 2] << " "
				<< data[index + buflen - 1] << " "
				<< endl;
				*/
		}

#endif

		if (word1 == 0xFFFFFFFF && word2 == 0xFFFFFFFF) {
			if (bufid == 101) {
				// This record is a time stamp
				// Format of timestamp record is
				//   data[0] = word count, excluding this word and checksum
				//   data[1] = 0xFFFFFFFF
				//   data[2] = 0xFFFFFFFF
				//   data[3] = time in clock ticks for cpu
				//   data[4] = CLOCKS_PER_SEC taken from the system value
				//   data[5] = checksum for buffer
				//std::cout << "Buffer " << bufcnt << ": Timestamp" << std::endl;

#ifdef OLD
				index = start + cnt + 2;
#else
				index = start + cnt;
#endif
				//index = start + cnt + 2;
				continue;

			}

		} // end of if(word1==0xFFFFFFFF && word2==0xFFFFFFFF){
		else if (word1 == 100) {
			// This buffer is a configuration record
			std::cout << "Buffer " << bufcnt << ": Configuration " << std::endl;

			index += 2;
			int runno = data[index++];

			// print the run number
			std::cout << "    Run " << runno << std::endl;

			unsigned short* p = (unsigned short*)&data[index];

			int n = 0;
			for (n = 0; n < 9; ) {
				unsigned short chipid = p[n++];
				unsigned short masks[8];

				for (int m = 0; m < 8; m++, n++)
					masks[m] = p[n];

				std::cout << "    Chip " << chipid << std::endl;
				std::cout << "      enable masks ";

				for (int m = 0; m < 8; m++)
					std::cout << "0x" << std::hex << masks[m] << " ";

				std::cout << std::dec << std::endl;
			}

			unsigned short* p2 = &p[n];
			for (n = 0; n < 16; n++) {
				std::cout << "      Reg " << n << ": " << (int)p2[n] << std::endl;
			}
#ifdef OLD
			index = start + cnt + 2;
#else
			index = start + cnt;
#endif
		} // end of if( word1 == 100 )
		else {

			// Format of record is
			//   data[0] = # of data words
			//   data[1..n] = data words
			//   data[n+1] = checksum for buffer
			if (bufcnt < 10)
				std::cout << "Buffer " << bufcnt << ": Data record, "
				<< "nwords = " << cnt << " checksum = "
				<< "0x" << std::hex << data[index + cnt + 1] << std::dec << std::endl;

			int checksum = 0;

			// loop over data in this event group until the next event group shows up
#ifdef OLD
			for (index++; index < start + cnt + 1; index++)
#else
			for (index += 2; index < start + cnt - 1; index++)
#endif
			{
				if ((index + 1) * sizeof(unsigned int) > size) {
					// Safety belt against a partially-written buffer (it will have the full length
					// field, but the whole buffer hasn't been read in).  This can happen, for instance,
					// if we are reading a file that is actively being written.
					std::cout << "Partial buffer detected, bailing" << std::endl;
					break;
				}

				// ^= means XOR
				checksum ^= data[index];

				int rawchip = -1;

				// Check whether this word holds an event#, a full 16-bit BCO# or hit data:
				if ((data[index] & 0xFFFF) == 1) {
					event_fem = (data[index] & 0xFFFF0000) >> 16;
					//cout  <<  "event_fem = "  <<  event_fem  << endl;
					continue;

				}
				else if ((data[index] & 0xFFFF) == 2) {
					bco_full = (data[index] & 0xFFFF0000) >> 16;
					//cout  <<  "bco_full = "  <<  bco_full  <<  endl;
					continue;
				}
				else {
					//check if this is a FEM_ID (bottom 12 bits = 0) or a data word:
					if ((data[index] & 0xfff) == 0) {
						fem_id = (data[index] & 0xf000) >> 12;
						continue;
					}
					else {
						rawchip = (data[index] >> 3) & 0x3F;

						if (rawchip < 27)
							chip_id = rawchip;
						else
							chip_id = rawchip - 26;

						fpga_id = 0;

						if (fem_id == 1) {
							if (rawchip < 27)
								module = 1;
							else
								module = 2;
						}
						else if (fem_id == 2) {
							if (rawchip < 27)
								module = 3;
							else
								module = 4;
						}
						else if (fem_id == 4) {
							if (rawchip < 27)
								module = 7;
							else
								module = 8;
						}
						else if (fem_id == 8) {
							if (rawchip < 27)
								module = 5;
							else
								module = 6;
						}

						// assign data to each variable to fill TTree
						mchip = 0;
						ampl = (data[index] >> 24) & 0x7F;
						bco = (data[index] >> 16) & 0x7F;
						chan_id = (data[index] >> 9) & 0x7F; //((data[index] & 0x200) >>3) | ((data[index] & 0xFC00)>>10); //data[index]>>9) & 0x7F; //
						adc = (data[index] & 0x07);

						/*
						if( chan_id<0 || chan_id>=128 || ampl<0 || ampl>=64 ){
						  cout << "---" << chan_id << " " << ampl << endl;
						}
						*/
						nhits[chan_id][ampl]++;
					}
				}

				int noise; //,inoise,ihealthy;

				if (ampl != 0)
					noise = 1;
				else if (chip_id == 0 || chip_id > 26)
					noise = 1;
				else if (chan_id == 0 || chan_id > 126)
					noise = 1;
				else if (fem_id != 4 || module != 7)
					noise = 1;
				else
					noise = 0;

				if (noise != 0)
					inoise++;
				else if (noise == 0)
					ihealthy++;

				if (event_fem != old_event_fem) {

					if (old_event_fem >= 0) {

						for (int ichip = 0; ichip < 8; ichip++) {
							if (chiphit[ichip] != 0)
								nchip_event++;

						}

					} // end of if( old_event_fem >= 0 )

					nhit_event = 1;
					nchip_event = 0;

					for (int ichip = 0; ichip < 8; ichip++) {
						chiphit[ichip] = 0;
					}

					if ((chip_id < 9) && (chip_id != 0)) {
						chiphit[chip_id - 1]++;
					}

					old_event_fem = event_fem;
					old_bco = bco;
					old_bco_full = bco_full;
				} // end of if( event_fem != old_event_fem )
				else {
					nhit_event++;
					if ((chip_id < 9) && (chip_id != 0))
						chiphit[chip_id - 1]++;
				}

				tree->Fill();

				//Note:  we seem to get some odd chip_ids out of the new DAQ VHDL code
				//after the event gets larger than some value.  Need to understand this:

				ievent++;
			}  //for loop on data[index]

			//if ( checksum != data[index] )
			//std::cout << "WARNING: bad checksum = "
			//      << std::hex << checksum << std::dec << std::endl;
#ifdef OLD
			index = start + cnt + 2;
#else
			index = start + cnt;
#endif

		}  // if block on record type
}  //Loop over data

	cout << "inoise   = " << inoise << endl;
	cout << "ihealthy = " << ihealthy << endl;

	if (decoded_output)
		ofs.close();

	cout << "Tree entries: " << tree->GetEntries() << endl;
	cout << "Clock data num: " << clock_data_num << endl;

	tree->Write();
	file->Close();

	return filename;
}


/*!
	@fn void DrawPlots( string root_file, int usemod ){
	@brief Some plots are drawn using root_file.
	@param string root_file A path to a ROOT file to be used. Return value of MakeTree is OK.
	@param int usemod ID of the module
	@details Following plots are drawn:
			 - Amplitude vs ADC for each chip
			 - Amplitude vs Channel for each chip
			 - Channel distribution for each chip
			 - Hit map (chip vs channel)
			 Common cuts for all plots are in cut_base.

			 TTree::Draw was performed many times to draw a plot of a chip in ver3 so the macro took time.
			 To save the time, histograms with additional dimension of chip_id are made and projected on x-y dimensions.
			 For example, a histogram of amplitude vs ADC vs chips is made and projected on amplitude vs ADC dimension with selection of a chip.
			 Old methods are remained as they are for the moment.

*/
void DrawPlots(string root_file, int usemod) {

	// general setting for ROOT
	gStyle->SetPalette(1);
	gStyle->SetOptFit(0);
	gStyle->SetOptStat(0);
	//gStyle->SetOptTitle(0);
	gStyle->SetFrameBorderMode(0);
	gStyle->SetCanvasColor(0);
	gStyle->SetCanvasBorderMode(0);
	gStyle->SetPadColor(0);
	gStyle->SetPadBorderMode(0);

	// open the ROOT file
	TFile* tf = new TFile(root_file.c_str(), "READ"); // c_str() converts string type to const char* type

	// get TTree in the ROOT file
	auto tree = (TTree*)tf->Get("tree");

	// definition of the order of chips
	int chip_order[26] = { 26,25,24,23,22,21,20,19,18,17,16,15,14,
						13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

	// basic cuts to be applied in any case
	stringstream cut_base; // useful class to generate string using non-string and char parameters
	cut_base << "ampl<70 " << "&& module==" << usemod << " && chan_id<127 && chip_id<27";

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Amplitude vs ADC for each chip                                                               //
	//////////////////////////////////////////////////////////////////////////////////////////////////
	TCanvas *c1 = new TCanvas("c1", "amplitude vs ADC", 0, 0, 1625, 250);


	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Idea to draw histogram of all chips faster:                                                  //
	//     Make a histogram as a function of chip,                                                  //
	//     and then make histograms for each chip by projecting on the axis of chip                 //
	//////////////////////////////////////////////////////////////////////////////////////////////////

	// hist of amplitude vs ADC vs chip
	TH3D* hist_ampl_adc_chip = new TH3D("ampl_adc_chip", "Amplitude vs ADC vs Chip ID;Amplitude;ADC;Chip ID",
		70, 0, 70, 8, 0, 8, 26, 0, 26);

	// expression for Draw
	string expression_ampl_adc_chip = string("chip_id:adc:ampl>>") + hist_ampl_adc_chip->GetName();

	// draw the distribution and fill the hist with the result
	tree->Draw(expression_ampl_adc_chip.c_str(), cut_base.str().c_str(), "goff");

	// 2D hists to contain distribution of amplitude vs ADC for each chip
	TH2D* hist_ampl_adc[26];

	// divide the current canvas into 13x2 pads
	c1->Divide(13, 2);

	// loop over all chips to draw each hist
	for (int i = 0; i < 26; i++) {
		// move to the next pad
		c1->cd(i + 1);
		gPad->SetTopMargin(0.10);
		gPad->SetBottomMargin(0.08);
		gPad->SetRightMargin(0.03);
		gPad->SetLeftMargin(0.08);

		// to specify the range of an axis for TH2::Project3D, SetRange or SetRangeUser is needed. Otherwize you can't select a certain chip
		//hist_ampl_adc_chip->GetZaxis()->SetRangeUser(chip_order[i], chip_order[i] + 1);
		//hist_ampl_adc_chip->GetZaxis()->SetRangeUser(chip_order[i], chip_order[i]);
		hist_ampl_adc_chip->GetZaxis()->SetRange(chip_order[i]+1, chip_order[i]+1);

		// make projection to get amplitude vs ADC for the chip
		//hist_ampl_adc[i] = (TH2D*)hist_ampl_adc_chip->Project3D(Form("yx_chip%d", chip_order[i]));
		hist_ampl_adc[i] = (TH2D*)hist_ampl_adc_chip->Project3D("YX");
		hist_ampl_adc[i]->SetName(Form("yx_chip%d", chip_order[i]));
		hist_ampl_adc[i]->SetTitle(Form("chip_id==%d", chip_order[i]));
		hist_ampl_adc[i]->Draw("colz");

		// configuration of titles and labels for axes
		hist_ampl_adc[i]->GetXaxis()->SetTitleSize(0.1);
		hist_ampl_adc[i]->GetXaxis()->SetLabelSize(0.06);
		hist_ampl_adc[i]->GetYaxis()->SetTitleSize(0.1);
		hist_ampl_adc[i]->GetYaxis()->SetLabelSize(0.08);

		gPad->Update();

		// draw the title of the histo manually to change the size
		auto pave_title = (TPaveText*)gPad->GetPrimitive("title"); // "auto" means the program guesses what kind of type should be used by itself from right side
		pave_title->SetTextSize(0.1);
		pave_title->SetTextAlign(23);
		pave_title->Draw();

	}

	// save the current canvas into a pdf file
	string output_adc_vs_ampl = root_file.substr(0, root_file.find_last_of(".root") - 4)
		+ "_adcvsampl.pdf";
	c1->Print(output_adc_vs_ampl.c_str());

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Amplitude vs Channel for each chip                                                           //
	//////////////////////////////////////////////////////////////////////////////////////////////////
	TCanvas *c2 = new TCanvas("c2", "channel_id vs amplitude", 0, 300, 1625, 250);
	c2->Divide(13, 2);

	// 3D hist ( amplitude vs channel vs chip )  --> 2D hist of a chip ( amplitude vs channel) by making a profile on z-axis
	TH3D* hist_ampl_ch_chip = new TH3D("ampl_ch_chip", "Amplitude vs Channel vs Chip;Amplitude;Channel;Chip",
		70, 0, 70, 128, 0, 128, 26, 0, 26); // channel bins are reduced by a factor of 4
	hist_ampl_ch_chip -> SetMaximum(25);
	//written by M.Morita 2020/Oct./26th

	string expression_ampl_ch_chip = string("chip_id:chan_id:ampl>>") + hist_ampl_ch_chip->GetName();
	tree->Draw(expression_ampl_ch_chip.c_str(), cut_base.str().c_str(), "goff");

	for (int i = 0; i < 26; i++) { // loop over all chips
		c2->cd(i + 1);
		gPad->SetTopMargin(0.10);
		gPad->SetBottomMargin(0.08);
		gPad->SetRightMargin(0.03);
		gPad->SetLeftMargin(0.08);

		// change the range of z-axis (chip) to select only this chip
		//hist_ampl_ch_chip->GetZaxis()->SetRange(chip_order[i], chip_order[i] + 1);
		hist_ampl_ch_chip->GetZaxis()->SetRange(chip_order[i]+1, chip_order[i] + 1);

		// make projection hist (2D)
		auto hist_ampl_ch = (TH2D*)hist_ampl_ch_chip->Project3D(Form("xy%d", chip_order[i]));
		hist_ampl_ch->SetTitle(Form("chip_id==%d", chip_order[i]));

		// draw more than 15 on the chip which data number is over 15 at each channel
		// set range 15 number
		// you have to know if more than 15 data comes
		
		bool is_warned = false;

		for(int j = 0; j< 128; j++){
			for ( int k = 0; k < 64; k++){
				int bin_entry = hist_ampl_ch->GetBinContent(j, k);
				
				if(bin_entry > 15)
					is_warned = true;
			}
			if (is_warned == true)
					break;
		}

		// zoom the range of y-axis for better view
		for (int j = 127; j > 0; j--) { // loop over all bins of y-axis
			int entries = hist_ampl_ch->ProjectionX(Form("ampl_of_ch%d", j), j, j + 1, "goff")->GetEntries(); // get a number of events in j <= y <= y+1
			//cout << i << "\t" << chip_order[i] << "\t" << j << "\t" << entries << endl;
			if (entries != 0) { // if non-zero event was found, that's the maximum value
				hist_ampl_ch->GetYaxis()->SetRangeUser(0, j + 1);
				break;
			}

			//hist_ampl_ch->GetXaxis()->SetTitleSize( 0.08 );
			hist_ampl_ch->GetXaxis()->SetLabelSize(0.08); // zoom the labal for x-axis
			//hist_ampl_ch->GetYaxis()->SetTitleSize( 0.08 );
			hist_ampl_ch->GetYaxis()->SetLabelSize(0.08); // zoom the labal for y-axis
		}

		hist_ampl_ch->Draw("colz");

		hist_ampl_ch->GetZaxis()->SetRangeUser(0., 15.);

		if(is_warned == true){
			TLatex *warning = new TLatex();
			warning -> SetTextSize(0.25);
			warning -> DrawLatex(5.0, 40.0, "over 15");
		}

		gPad->Update();
		auto pave_title = (TPaveText*)gPad->GetPrimitive("title");
		pave_title->SetTextSize(0.1);
		pave_title->SetTextAlign(23);
		pave_title->Draw();
	}
	
	string output_ampl_vs_chan = root_file.substr(0, root_file.find_last_of(".root") - 4)
		+ "_amplvschan.pdf";
	c2->Print(output_ampl_vs_chan.c_str());

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Channel distribution for each chip                                                           //
	//////////////////////////////////////////////////////////////////////////////////////////////////

	TCanvas *c3 = new TCanvas("c3", "entry vs channel_id", 0, 560, 1625, 250);
	TH2D* hist_ch_chip = new TH2D("ch_chip", "Channel vs Chip;Channel;Chip", 130, -1, 129, 26, 0, 26);

	string expression_ch_chip = string("chip_id:chan_id>>") + hist_ch_chip->GetName();
	tree->Draw(expression_ch_chip.c_str(), cut_base.str().c_str(), "goff");

	c3->Divide(13, 2);
	for (int i = 0; i < 26; i++) {
		c3->cd(i + 1);

		auto hist_ch = hist_ch_chip->ProjectionX(Form("ch_chip%d", chip_order[i]), chip_order[i] + 1, chip_order[i] + 1, "goff");
		hist_ch->SetTitle(Form("chip_id==%d", chip_order[i]));

		hist_ch->Draw();
		hist_ch->GetYaxis()->SetRangeUser(0.0, 450.0);
		hist_ch->GetXaxis()->SetLabelSize(0.08);
		hist_ch->GetYaxis()->SetLabelSize(0.1);
		//gPad->SetLogy(true);

		gPad->Update();
		auto pave_title = (TPaveText*)gPad->GetPrimitive("title");
		pave_title->SetTextSize(0.1);
		pave_title->SetTextAlign(23);
		pave_title->Draw();
	}	

	string output_entry_vs_chan_pdf = root_file.substr(0, root_file.find_last_of(".root") - 4)
		+ "_entry_vs_channel_ID.pdf";
	string output_entry_vs_chan_jpg = root_file.substr(0, root_file.find_last_of(".root") - 4)
		+ "_entry_vs_channel_ID.jpg";
	c3->Print(output_entry_vs_chan_pdf.c_str());
	//c3->Print(output_entry_vs_chan_jpg.c_str());

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Hit map ( chip vs channel )                                                                  //
	//////////////////////////////////////////////////////////////////////////////////////////////////
	TCanvas *c4 = new TCanvas("c4", "hit_map", 0, 700, 1625, 250);

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//                                                                                              //
	//                  26  25  24  23  22  21  20  19  28  17  16  15  14   chip_id                //
	//                +---+---+---+---+---+---+---+---+---+---+---+---+---+                         //
	//   256    c   1 |   |   |   |   |   |   |   |   |   |   |   |   |   |                         //
	//   255    h   2 |   |   |   |   |   |   |   |   |   |   |   |   |   |                         //
	//   ...    a ... |   |   |   |   |   |   |   |   |   |   |   |   |   |                         //
	//   130    n 126 |   |   |   |   |   |   |   |   |   |   |   |   |   |                         //
	//   128      127 |   |   |   |   |   |   |   |   |   |   |   |   |   |                         //
	//          i --- +---+---+---+---+---+---+---+---+---+---+---+---+---+                         //
	//   127    d 127 |   |   |   |   |   |   |   |   |   |   |   |   |   |                         //
	//   126      126 |   |   |   |   |   |   |   |   |   |   |   |   |   |                         //
	//   ...      ... |   |   |   |   |   |   |   |   |   |   |   |   |   |                         //
	//     2        2 |   |   |   |   |   |   |   |   |   |   |   |   |   |                         //
	//     1        1 |   |   |   |   |   |   |   |   |   |   |   |   |   |                         //
	//   y-axis       +---+---+---+---+---+---+---+---+---+---+---+---+---+                         //
	//                  13  12  11  10   9   8   7   6   5   4   3   2   1  chip_id                 //
	//           x-axis  1   2   3   4   5   6   7   8   9  10  11  12  13                          //
	//                                                                                              //
	//                       Schematic figure of what will be drawn                                 //
	//////////////////////////////////////////////////////////////////////////////////////////////////

	TH2D* hitmap = new TH2D("hitmap", "Hit map; chip_id; chan_id", 13, 0, 13, 257, 0, 257);

	// Super long expression to draw a hit map in one execution:
	//   int(chip_id / 14) * 256+ pow(-1, int(chip_id / 14)) * chan_id: -chip_id+ (1 + int(chip_id / 14)) * 13"
	// here
	//    int(chip_id / 14) : = 0 (chip=0, 1, ..., 13) or = 1 (chip=14, ..., 27)
	//                x-axis: -chip_id+ (1 + int(chip_id / 14)) * 13  
	//                         X value should start from 13 (for chip1) and value decreases as chip number increases basically.
	//                         For chips #14 or later, another offset of 13 (26 in total) is needed.
	//                         The first term -chip_id is the depending term on chip ID.
	//                         The second term is an offset to draw hist from right to left.
	//
	//                y-axis: int(chip_id / 14) * 256+ pow(-1, int(chip_id / 14)) * chan_id
	//                        Y value is bit complicated...
	//                        For chip from 1 to 13, a channel ID is simply the same as Y coordinate (ch_{i} = y_{i}).
	//                        For chip from 14 to 26, relation between a channel ID ch_{i}  and Y coordinate y_{i} is ch_{i} = 256 - y_{i} (i=0, 1, ...).
	//                        The first term is an offset, 0 for chip 0-13 and 256 for chip 14-27.
	//                        The second term is the depending term on the channel ID. Sign is + for chip 0-13 and - for chip 14-27.

	string expression = string("int(chip_id / 14) * 256") // it's offsets for y-axis.
		+ "+ pow(-1, int(chip_id / 14)) * chan_id"        // Y value increases(decreases) as chan_id decreases(increases) for chips1-13(14-26)
		+ ": -chip_id"                                   // Basically, x value increases as chip_id decreases (chip13 at x=0, chip12 at x=-1, ...)
		+ "+ (1 + int(chip_id / 14)) * 13"               // In addition to above, offset is needed. 
		+ ">> hitmap";

	tree->Draw(expression.c_str(), cut_base.str().c_str(), "goff");
	hitmap->Draw("colz");

	hitmap->GetXaxis()->SetNdivisions(15);
	hitmap->GetXaxis()->SetLabelSize(0.08);
	hitmap->GetXaxis()->SetTitleSize(0.08);
	hitmap->GetYaxis()->SetLabelSize(0.08);

	// draw chip_id on the hist
	for (int i = 0; i < 26; i++) {
		TLatex* tex = new TLatex();
		double x = i < 13 ? 12.25 - i : 12.25 - i + 13;
		double y = i < 13 ? 64 : 192;
		tex->DrawLatex(x, y, Form("Chip%d", i + 1));
	}

	string output_hitmap = root_file.substr(0, root_file.find_last_of(".root") - 4)
		+ "_hitmap.pdf";
	c4->Print(output_hitmap.c_str());

	//tf->Close();
	return;
}

char* FindLatestFile() { // ongoing...
	string search_dir = "./";

	DIR* dp = opendir(search_dir.c_str());
	if (dp == NULL) {
		cerr << "The directory \"" << search_dir << "\" is not found." << endl;
		return "";
	}



	return "";
}

// returns the size of file
int getFileSize(std::ifstream& f) {
	f.seekg(0, std::ifstream::end);
	if (f.fail()) {
		std::ifstream::iostate state = f.rdstate();
		std::cout << "error seeking to end, read state = " << state << std::endl;
		if (state & std::ios::eofbit)  std::cout << " eof bit set" << std::endl;
		if (state & std::ios::failbit) std::cout << " fail bit set" << std::endl;
		if (state & std::ios::badbit)  std::cout << " bad bit set" << std::endl;
		return 0;
	}
	int size = f.tellg();
	if (size < 0) {
		std::ifstream::iostate state = f.rdstate();
		std::cout << "error in tellg, read state = " << state << std::endl;
		if (state & std::ios::eofbit)  std::cout << " eof bit set" << std::endl;
		if (state & std::ios::failbit) std::cout << " fail bit set" << std::endl;
		if (state & std::ios::badbit)  std::cout << " bad bit set" << std::endl;
		return 0;
	}

	// Return the file stream to the start of the file
	f.seekg(0, std::ifstream::beg);

	return size;
}
