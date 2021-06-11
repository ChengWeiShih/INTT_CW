//INTT calibration_ana_code
//Cheng-Wei Shih, NCU, Taiwan

#include <stdio.h>
#include <iostream>
#include "TRandom.h"
#include "TMath.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TProfile.h"
#include "TRandom.h"
#include "TTree.h"
#include <TFile.h>
#include <TH1.h>
#include <TH2D.h>
#include <TH3D.h>
#include <iostream>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TLatex.h>
#include "TGraphErrors.h"
#include <TTree.h>
#include <TLine.h>
#include <TAxis.h>
#include <TPad.h>
#include <TColor.h>
#include "Riostream.h"
#include <vector>
#include <fstream>
#include <string.h> //to add string with the tree name
#include <TSystem.h>
#include <TGraphAsymmErrors.h>
#include <TH1D.h>
#include <TF1.h>
#include <TLorentzVector.h>
#include <iomanip>
//#include "untuplizer.h"
//#include "sigmaEff.h"

double RC_eq1(double *t, double *par)
{
	return par[0] * t[0] + par[1];
}

//void name with "copy" is correct
void calibration_ana_code_multi_copy(TString folder_name, int module_number, bool run_option, bool assembly_check, int noise_level_check, bool new_check, bool unbound_check, bool noise_channel_check, bool multi_run)
{
	TString list_buffer;
	vector<TString> list_array;
	list_array.clear();
	TString the_name;

	ifstream data_list;
	data_list.open(Form("%s/total_file.txt", folder_name.Data()));

	while (1)
	{
		data_list >> list_buffer;
		list_array.push_back(list_buffer);
		if (!data_list.good())
		{
			break;
		}
	}

	cout << "test list size : " << list_array.size() << endl;
	int run_file = data_index; // data index is not declared, it has to be changed by .sh file
	the_name = list_array[run_file];

	cout << Form(" !!! The input data name : %s", the_name.Data()) << endl;
	system(Form("mkdir -p %s/folder_%s", folder_name.Data(), the_name.Data()));
	TFile *f1 = TFile::Open(Form("%s/%s", folder_name.Data(), the_name.Data()));
	TTree *Gettree = (TTree *)f1->Get("tree");

	TString data_buffer_a, data_buffer_b, data_buffer_c, data_buffer_d, data_buffer_e;
	vector<TString> data_a;
	data_a.clear();
	vector<TString> data_b;
	data_b.clear();
	vector<TString> data_c;
	data_c.clear();
	vector<TString> data_d;
	data_d.clear();
	vector<TString> data_e;
	data_e.clear();

	// TString DB_NB, DB_ND;
	// double DB_NA, DB_NC, DB_NE;
	// vector<double>D_NA; D_NA.clear();
	// vector<TString>D_NB; D_NB.clear();
	// vector<double>D_NC; D_NC.clear();
	// vector<TString>D_ND; D_ND.clear();
	// vector<double>D_NE; D_NE.clear();

	// TString DB_EB, DB_ED;
	// double DB_EA, DB_EC, DB_EE;
	// vector<double>D_EA; D_EA.clear();
	// vector<TString>D_EB; D_EB.clear();
	// vector<double>D_EC; D_EC.clear();
	// vector<TString>D_ED; D_ED.clear();
	// vector<double>D_EE; D_EE.clear();

	int chip_N, chan_N;
	int chip_E, chan_E, level_E;
	int chip_U, chan_U;
	int chip_W, chan_W;

	double level_N, level_U, level_W;
	//TString source_name;

	TFile *file_output = new TFile(Form("%s/folder_%s/%s_summary.root", folder_name.Data(), the_name.Data(), the_name.Data()), "RECREATE");
	//file_output->cd();
	TTree *tree_output1 = new TTree("Noise_check", "With bias 100V, noise level > 4");
	TTree *tree_output2 = new TTree("Entries_check", "With bias 100V, Entries > 400 or entries < 280");
	TTree *tree_output3 = new TTree("Unbonded_check", "Without bias, Noise level < 4");
	TTree *tree_output4 = new TTree("chan_gaus_width", "gaus width of each channel");

	tree_output1->Branch("source_id", &run_file);
	tree_output1->Branch("chip_id", &chip_N);
	tree_output1->Branch("chan_id", &chan_N);
	tree_output1->Branch("noise_level", &level_N);

	tree_output2->Branch("source_id", &run_file);
	tree_output2->Branch("chip_id", &chip_E);
	tree_output2->Branch("chan_id", &chan_E);
	tree_output2->Branch("entries", &level_E);

	tree_output3->Branch("source_id", &run_file);
	tree_output3->Branch("chip_id", &chip_U);
	tree_output3->Branch("chan_id", &chan_U);
	tree_output3->Branch("noise_level", &level_U);

	tree_output4->Branch("source_id", &run_file);
	tree_output4->Branch("chip_id", &chip_W);
	tree_output4->Branch("chan_id", &chan_W);
	tree_output4->Branch("Gaus_width", &level_W);

	if (multi_run == true)
	{
		ifstream input_data;
		input_data.open(Form("%s/multi_run_status.txt", folder_name.Data()));

		while (1)
		{
			input_data >> data_buffer_a >> data_buffer_b >> data_buffer_c >> data_buffer_d >> data_buffer_e;
			data_a.push_back(data_buffer_a);
			data_b.push_back(data_buffer_b);
			data_c.push_back(data_buffer_c);
			data_d.push_back(data_buffer_d);
			data_e.push_back(data_buffer_e);

			if (!input_data.good())
			{
				break;
			}
			//the_test_event.push_back(n+1);
		}

		// ifstream input_data_N;
		// input_data_N.open(Form("%s/multi_run_noise.txt"));

		// while ( 1 )
		// {
		//   input_data_N >> DB_NA >> DB_NB >> DB_NC >> DB_ND >> DB_NE ;
		//   D_NA.push_back(DB_NA);
		//   D_NB.push_back(DB_NB);
		//   D_NC.push_back(DB_NC);
		//   D_ND.push_back(DB_ND);
		//   D_NE.push_back(DB_NE);

		//   if ( !input_data_N.good() )
		//     {
		//       break;
		// 	}
		//   //the_test_event.push_back(n+1);
		// }

		// ifstream input_data_E;
		// input_data_E.open(Form("%s/multi_run_entries.txt"));

		// while ( 1 )
		// {
		//   input_data_E >> DB_EA >> DB_EB >> DB_EC >> DB_ED >> DB_EE ;
		//   D_EA.push_back(DB_EA);
		//   D_EB.push_back(DB_EB);
		//   D_EC.push_back(DB_EC);
		//   D_ED.push_back(DB_ED);
		//   D_EE.push_back(DB_EE);

		//   if ( !input_data_E.good() )
		//     {
		//       break;
		// 	}
		//   //the_test_event.push_back(n+1);
		// }
	}

	ofstream multi_txt;
	if (multi_run == true)
	{

		multi_txt.open(Form("%s/multi_run_status.txt", folder_name.Data()), ios::out); //¶}±ÒÀÉ®×
		if (!multi_txt)
		{ //¦pªG¶}±ÒÀÉ®×¥¢±Ñ¡Afp¬°0¡F¦¨¥\¡Afp¬°«D0
			cout << "Fail to open file: " << endl;
		}

		// fstream multi_txt_noise;
		// multi_txt_noise.open(Form("%s/multi_run_noise.txt"), ios::out);//¶}±ÒÀÉ®×
		// if(!multi_txt_noise)
		// { //¦pªG¶}±ÒÀÉ®×¥¢±Ñ¡Afp¬°0¡F¦¨¥\¡Afp¬°«D0
		// cout<<"Fail to open file: "<<endl;
		// }

		// fstream multi_txt_entries;
		// multi_txt_entries.open(Form("%s/multi_run_entries.txt"), ios::out);//¶}±ÒÀÉ®×
		// if(!multi_txt_entries)
		// { //¦pªG¶}±ÒÀÉ®×¥¢±Ñ¡Afp¬°0¡F¦¨¥\¡Afp¬°«D0
		// cout<<"Fail to open file: "<<endl;
		// }
	}

	// fstream fp;
	//   fp.open(Form("%s/%s/slope_offset_ChiNDF.txt",the_name.Data()  ), ios::out);//¶}±ÒÀÉ®×
	//   if(!fp)
	//   { //¦pªG¶}±ÒÀÉ®×¥¢±Ñ¡Afp¬°0¡F¦¨¥\¡Afp¬°«D0
	//     cout<<"Fail to open file: "<<endl;
	//   }

	// fstream fpall;
	//   fpall.open(Form("%s/%s/All_slope_offset_ChiNDF.txt",the_name.Data()  ), ios::out);//¶}±ÒÀÉ®×
	//   if(!fpall)
	//   { //¦pªG¶}±ÒÀÉ®×¥¢±Ñ¡Afp¬°0¡F¦¨¥\¡Afp¬°«D0
	//     cout<<"Fail to open file: "<<endl;
	//   }

	int adc = 0;
	int ampl = 0;
	int chip_id = 0;
	int fpga_id = 0;
	int module = 0;
	int chan_id = 0;
	int fem_id = 0;
	int bco_full = 0;
	int bco = 0;
	int event = 0;

	vector<int> adc_memory;
	adc_memory.clear();
	vector<int> ampl_memory;
	ampl_memory.clear();
	vector<int> chip_id_memory;
	chip_id_memory.clear();
	vector<int> fpga_id_memory;
	fpga_id_memory.clear();
	vector<int> module_memory;
	module_memory.clear();
	vector<int> chan_id_memory;
	chan_id_memory.clear();
	vector<int> fem_id_memory;
	fem_id_memory.clear();
	vector<int> bco_memory;
	bco_memory.clear();
	vector<int> bco_full_memory;
	bco_full_memory.clear();
	vector<int> event_memory;
	event_memory.clear();

	vector<int> chip_adc[26][128];	//chip_adc.clear();
	vector<int> chip_ampl[26][128]; //chip_ampl.clear();
	vector<double> ampl_adc_slope[26];
	vector<double> ampl_adc_offset[26];
	vector<double> ampl_adc_chiNDF[26];

	vector<int> unbound_chip;
	unbound_chip.clear();
	vector<int> unbound_channel;
	unbound_channel.clear();
	vector<double> unbound_width;
	unbound_width.clear();

	vector<int> noise_chip;
	noise_chip.clear();
	vector<int> noise_channel;
	noise_channel.clear();
	vector<double> noise_width;
	noise_width.clear();

	vector<int> entries_chip;
	entries_chip.clear();
	vector<int> entries_channel;
	entries_channel.clear();
	vector<int> entries_entries;
	entries_entries.clear();

	for (int i = 0; i < 26; i++)
	{
		ampl_adc_slope[i].clear();
		ampl_adc_offset[i].clear();
		ampl_adc_chiNDF[i].clear();
		for (int i1 = 0; i1 < 128; i1++)
		{
			chip_adc[i][i1].clear();
			chip_ampl[i][i1].clear();
		}
	}

	Gettree->SetBranchAddress("adc", &adc);
	Gettree->SetBranchAddress("ampl", &ampl);
	Gettree->SetBranchAddress("chip_id", &chip_id);
	Gettree->SetBranchAddress("fpga_id", &fpga_id);
	Gettree->SetBranchAddress("module", &module);
	Gettree->SetBranchAddress("chan_id", &chan_id);
	Gettree->SetBranchAddress("fem_id", &fem_id);
	Gettree->SetBranchAddress("bco", &bco);
	Gettree->SetBranchAddress("bco_full", &bco_full);
	Gettree->SetBranchAddress("event", &event);

	int total_size = Gettree->GetEntriesFast();
	cout << " total event : " << total_size << endl;

	cout << " the weird event :  event : ampl : adc : chip_id : chan_id " << endl;
	for (int i = 0; i < total_size; i++)
	{
		Gettree->GetEntry(i);
		//if (chan_id == 127)cout << event <<" : "<< ampl <<" : "<< adc <<" : "<< chip_id <<" : "<< chan_id<<endl;
		if (chip_id > 0 && chip_id < 27 && chan_id > -1 && chan_id < 128 && module == module_number)
		{
			chip_ampl[chip_id - 1][chan_id].push_back(ampl);
			chip_adc[chip_id - 1][chan_id].push_back(adc);
		}
		else
		{
			if (module == module_number)
			{
				cout << " the weird event : " << event << " : " << ampl << " : " << adc << " : " << chip_id << " : " << chan_id << endl;
			}
		}

		if (fpga_id != 0)
		{
			cout << " !! fpga_id event : " << event << " : " << ampl << " : " << adc << " : " << chip_id << " : " << chan_id << " fpga_id : " << fpga_id << " module : " << module << endl;
		}
		//if ( module != 8 ) {cout <<" !! module event : "<< event <<" : "<< ampl <<" : "<< adc <<" : "<< chip_id <<" : "<< chan_id<<" fpga_id : "<<fpga_id<<" module : "<<module<<endl;}
		// adc_memory.push_back(adc);
		// ampl_memory.push_back(ampl);
		// chip_id_memory.push_back(chip_id);
		// chan_id_memory.push_back(chan_id);
		// event_memory.push_back(event);

		// if (chip_id == )
		//   {
		//   	cout << event <<" : "<< ampl <<" : "<< adc <<" : "<< chip_id <<" : "<< chan_id<<endl;
		//   }
	}
	TCanvas *c1 = new TCanvas("", "", 1200, 1200);
	//TCanvas *c2=new TCanvas("","",1200,1200);
	TCanvas *c4 = new TCanvas("c4", "channel to ampl", 1625, 250);
	c4->Divide(13, 2);

	TCanvas *c5 = new TCanvas("c5", "channel to adc", 1625, 250);
	c5->Divide(13, 2);

	TF1 *slope = new TF1("slope", RC_eq1, 0, 70, 2);
	TH2F *ampl_adc[26];
	TLatex *tex111 = new TLatex();
	tex111->SetNDC();
	//tex11 -> SetTextFont (42);
	tex111->SetTextSize(0.028);
	TH1F *noise_level = new TH1F("", Form("noise_level : %d", noise_level_check), 70, 0, 70);
	TF1 *gaus_function = new TF1("gaus_function", "gaus", 0, 70);
	gaus_function->SetLineColor(2);

	TF1 *gaus_fit = new TF1("gaus_fit", "gaus", 0.0, 0.18);

	vector<double> chip_slope;
	chip_slope.clear();
	vector<double> chip_index;
	chip_index.clear();

	cout << "==============fitting data of 128_combine_ampladc ==========" << endl;
	for (int i4 = 0; i4 < 26; i4++)
	{
		c4->cd(i4 + 1);
		ampl_adc[i4] = new TH2F("", Form("chip_id=%d", i4 + 1), 70, 0, 70, 8, 0, 8);
		ampl_adc[i4]->SetStats(0);

		for (int i2 = 0; i2 < 128; i2++)
		{
			for (int i3 = 0; i3 < chip_ampl[i4][i2].size(); i3++)
			{
				if (chip_ampl[i4][i2][i3] > 0)
				{
					ampl_adc[i4]->Fill(chip_ampl[i4][i2][i3], chip_adc[i4][i2][i3]);
					if (chip_adc[i4][i2][i3] == noise_level_check)
					{
						noise_level->Fill(chip_ampl[i4][i2][i3]);
					}
				}
			}
		}
		ampl_adc[i4]->Draw("COLZ0");
		ampl_adc[i4]->Fit("slope", "NQ");

		//cout<<Form("chip_id=%d",i4+1)<<" slope : "<< slope->GetParameter(0)<<" offset : "<<  slope->GetParameter(1)<<" chi2/NDF "<<slope->GetChisquare()/slope->GetNDF()<<endl;
		//cout<<"TESTTESTTEST"<<endl;
		//fpall<<i4+1<<" "<< slope->GetParameter(0)<<" "<<  slope->GetParameter(1)<<" "<<slope->GetChisquare()/slope->GetNDF()<<"\r"<<endl;
		//cout<<"TESTTESTTEST"<<endl;
		slope->Draw("lsame");
		chip_slope.push_back(slope->GetParameter(0));
		chip_index.push_back(i4 + 1);
		//cout<<"TESTTESTTEST"<<endl;
		//c1 -> cd(); tex111 -> DrawLatex (0.12, 0.80-(i4*0.03), Form("chip_id=%d,  slope : %.4f,  offset : %.3f,  chi2/NDF : %.2f",i4+1,slope->GetParameter(0),slope->GetParameter(1),slope->GetChisquare()/slope->GetNDF()));
		//cout<<"TESTTESTTEST"<<endl;
		//ampl_adc->Reset("ICESM");
	}
	c4->Print(Form("%s/folder_%s/128_combine_ampladc.pdf", folder_name.Data(), the_name.Data()));
	//c1->Print(Form("%s/%s/128_combine_ampladc_detail.pdf",the_name.Data()));
	cout << "==============fitting data of 128_combine_ampladc ==========" << endl;
	c4->Clear();
	c1->Clear();
	//fpall.close();
	//cout<<"\r\n"<<"#the file fpall rewriting is finish#"<<"\r\n"<<endl;

	for (int i5 = 0; i5 < 26; i5++)
	{
		ampl_adc[i5]->Reset("ICESM");
	}

	cout << " " << endl;
	cout << " " << endl;
	cout << " " << endl;

	c1->cd();
	noise_level->Draw("hist");
	noise_level->Fit("gaus_function", "NQ");
	TLatex *texnoise_level = new TLatex();
	texnoise_level->SetNDC();
	//tex11 -> SetTextFont (42);
	texnoise_level->SetTextSize(0.04);
	texnoise_level->DrawLatex(0.12, 0.750, Form("Gaus width : %.4f", gaus_function->GetParameter(2)));

	gaus_function->Draw("lsame");
	c1->Print(Form("%s/folder_%s/128_combine_noise_level_check_%d.pdf", folder_name.Data(), the_name.Data(), noise_level_check));
	c1->Clear();

	TGraph *slop_chip_TG = new TGraph(26, &chip_index[0], &chip_slope[0]);
	slop_chip_TG->GetXaxis()->SetTitle("chip_id");
	slop_chip_TG->GetYaxis()->SetTitle("slope");
	//slop_chip_TG->GetYaxis()->SetRangeUser(-0.01,0.01);
	slop_chip_TG->GetYaxis()->SetTitleOffset(1.8);
	slop_chip_TG->SetTitle("");

	slop_chip_TG->SetMarkerStyle(20);
	slop_chip_TG->SetMarkerSize(1.5);
	slop_chip_TG->SetMarkerColor(2);
	TCanvas *c3 = new TCanvas("c3", "c3", 1200, 1200);
	//c1->SetRightMargin(0 );

	c3->SetLeftMargin(0.120);
	c3->SetRightMargin(0.075);
	c3->cd();
	slop_chip_TG->Draw("apl");
	c3->Print(Form("%s/folder_%s/chip_slope_TG.pdf", folder_name.Data(), the_name.Data()));

	cout << "==============information of 128_combine_chanampl ==========" << endl;
	TH2F *chan_ampl[26];
	TH2F *chan_adc[26];
	c4->Divide(13, 2);
	for (int i4 = 0; i4 < 26; i4++)
	{

		chan_ampl[i4] = new TH2F("", Form("chip_id=%d", i4 + 1), 140, 0, 140, 70, 0, 70);
		chan_ampl[i4]->SetStats(0);
		chan_ampl[i4]->SetMaximum(20);
		chan_ampl[i4]->SetMinimum(0);

		chan_adc[i4] = new TH2F("", Form("chip_id=%d", i4 + 1), 140, 0, 140, 8, 0, 8);
		chan_adc[i4]->SetStats(0);
		//chan_adc[i4]->SetMaximum(20);
		chan_adc[i4]->SetMinimum(0);

		for (int i2 = 0; i2 < 128; i2++)
		{
			if (chip_ampl[i4][i2].size() > 700 || chip_ampl[i4][i2].size() < 200)
				cout << " Need to check, the entries of each channel, chip : " << i4 + 1 << " channel : " << i2 << " " << chip_ampl[i4][i2].size() << endl;
			for (int i3 = 0; i3 < chip_ampl[i4][i2].size(); i3++)
			{
				if (chip_ampl[i4][i2][i3] > 0)
				{
					//if (i4+1==16 && i2==0) cout<<i2<<" "<<chip_ampl[i4][i2][i3]<<" "<<chip_adc[i4][i2][i3]<<endl;
					chan_ampl[i4]->Fill(i2, chip_ampl[i4][i2][i3]);
					chan_adc[i4]->Fill(i2, chip_adc[i4][i2][i3]);
				}
			}
		}
		c4->cd(i4 + 1);
		chan_ampl[i4]->Draw("COLZ0");

		c5->cd(i4 + 1);
		chan_adc[i4]->Draw("COLZ0");

		//ampl_adc->Reset("ICESM");
	}
	c4->Print(Form("%s/folder_%s/128_combine_chanampl.pdf", folder_name.Data(), the_name.Data()));
	cout << "==============information of 128_combine_chanampl ==========" << endl;

	c5->Print(Form("%s/folder_%s/128_combine_chanadc.pdf", folder_name.Data(), the_name.Data()));
	cout << "==============information of 128_combine_chanadc ==========" << endl;
	c4->Clear();
	c5->Clear();
	for (int i5 = 0; i5 < 26; i5++)
	{
		chan_ampl[i5]->Reset("ICESM");
	}
	
	cout << " " << endl;
	cout << " " << endl;
	cout << " " << endl;

	cout << "==============information of ampladc_detail ==========" << endl;

	TLatex *tex11 = new TLatex();
	tex11->SetNDC();
	//tex11 -> SetTextFont (42);
	tex11->SetTextSize(0.028);

	TH2F *ampladc_detail = new TH2F("", "", 70, 0, 70, 8, 0, 8);

	TH1F *check_new = new TH1F("", "", 70, 0, 70);
	check_new->Reset();
	check_new->GetXaxis()->SetTitle("offset_ampl");
	check_new->GetYaxis()->SetTitle("entries");

	TF1 *gaus_fit_new = new TF1("gaus_fit_new", "gaus", 0, 70);
	vector<double> response_width[26];

	TH1F *channel_entries_check = new TH1F("", "# of event each channel", 50, 280, 400);
	channel_entries_check->GetXaxis()->SetTitle("channel event");
	channel_entries_check->GetYaxis()->SetTitle("entries");

	TH2F *slope_TH2 = new TH2F("", "", 128, 0, 128, 128, 0.04, 0.15);
	slope_TH2->SetStats(0);
	TH2F *offset_TH2 = new TH2F("", "", 128, 0, 128, 128, 0.04, 0.15);
	offset_TH2->SetStats(0);
	TH2F *ChiNDF_TH2 = new TH2F("", "", 128, 0, 128, 128, 0.04, 0.15);
	ChiNDF_TH2->SetStats(0);

	TCanvas *c2 = new TCanvas("c2", "c2", 1200, 1200);
	TH1F *slop_detail_TH = new TH1F("", "", 100, 0.08, 0.18);
	slop_detail_TH->GetYaxis()->SetTitle("entries");
	slop_detail_TH->GetXaxis()->SetTitle("fit slope ");
	slop_detail_TH->GetYaxis()->SetTitleOffset(1.6);
	c2->SetLeftMargin(0.120);
	c2->SetRightMargin(0.075);
	gStyle->SetOptStat(111111);

	double average_adc[8];
	// double average_adc1;
	// double average_adc2;
	// double average_adc3;
	// double average_adc4;
	// double average_adc5;
	// double average_adc6;
	// double average_adc7;

	double sum_adc[8];
	// double sum_adc1;
	// double sum_adc2;
	// double sum_adc3;
	// double sum_adc4;
	// double sum_adc5;
	// double sum_adc6;
	// double sum_adc7;

	double count_adc[8];
	// double count_adc1;
	// double count_adc2;
	// double count_adc3;
	// double count_adc4;
	// double count_adc5;
	// double count_adc6;
	// double count_adc7;
	//ampladc_detail->SetStats(0);
	TCanvas *c6 = new TCanvas("c6", "c6", 1200, 1200);
	int channel_entries_outsider = 0;
	double amount_offset = 0;
	bool redraw = false;

	for (int i4 = 0; i4 < 26; i4++)
	{
		if (run_option == true)
		{
			c1->cd();
			c1->Print(Form("%s/folder_%s/chip%d_detail_ampladc.pdf(", folder_name.Data(), the_name.Data(), i4 + 1));
		}
		if (new_check == true)
		{
			c6->cd();
			c6->Print(Form("%s/folder_%s/chip%d_detail_amploffset.pdf(", folder_name.Data(), the_name.Data(), i4 + 1));
		}

		for (int i2 = 0; i2 < 128; i2++)
		{
			redraw=false;
			
			ampladc_detail->SetTitle(Form("chip_id=%d, chan_id = %d", i4 + 1, i2));
			check_new->SetTitle(Form("chip_id=%d, chan_id = %d", i4 + 1, i2));
			//if (chip_ampl[i4][i2].size()>700 || chip_ampl[i4][i2].size()<200) cout<<" Need to check, the entries of each channel, chip : "<<i4+1<<" channel : "<<i2<<" " <<chip_ampl[i4][i2].size()<<endl;
			for (int i3 = 0; i3 < chip_ampl[i4][i2].size(); i3++)
			{
				if (chip_ampl[i4][i2][i3] > 0 && chip_ampl[i4][i2][i3]<70 )
				{
					//if (i4+1==16 && i2==0) cout<<i2<<" "<<chip_ampl[i4][i2][i3]<<" "<<chip_adc[i4][i2][i3]<<endl;
					ampladc_detail->Fill(chip_ampl[i4][i2][i3], chip_adc[i4][i2][i3]);

					sum_adc[chip_adc[i4][i2][i3]] += chip_ampl[i4][i2][i3];
					count_adc[chip_adc[i4][i2][i3]] += 1;
				}
			}

			for (int i7 = 0; i7 < 8; i7++)
			{
				if (count_adc[i7] == 0)
				{
					average_adc[i7] = sum_adc[i7] / 1.;
				}
				else
				{
					average_adc[i7] = sum_adc[i7] / count_adc[i7];
				}
			}

			for (int i3 = 0; i3 < chip_ampl[i4][i2].size(); i3++)
			{
				if (chip_ampl[i4][i2][i3] > 0 && chip_ampl[i4][i2][i3]<70)
				{
					amount_offset = average_adc[chip_adc[i4][i2][i3]] - average_adc[0];
					if ((chip_ampl[i4][i2][i3] - amount_offset)>0 && (chip_ampl[i4][i2][i3] - amount_offset)<70 )
					{
						//cout<<"line : 427 check "<<endl;
						
						check_new->Fill(chip_ampl[i4][i2][i3] - amount_offset);	
					}
					
				}
			}

			if ((check_new->GetBinContent(71) + check_new->GetBinContent(0)) == 0)
			{
				redraw = false;
				c1->cd();
				ampladc_detail->Draw("COLZ0");
				ampladc_detail->Fit("slope", "NQ");
				c1->cd();
				slope->Draw("lsame");
				slope_TH2->Fill(i2, slope->GetParameter(0));
				ampl_adc_slope[i4].push_back(slope->GetParameter(0));
				ampl_adc_offset[i4].push_back(slope->GetParameter(1));
				//ampl_adc_chiNDF[i4].push_back(slope->GetChisquare()/slope->GetNDF());

				//fp<<slope->GetParameter(0)<<" "<<slope->GetParameter(1)<<" "<<slope->GetChisquare()/slope->GetNDF()<<"\r"<<endl;
				slop_detail_TH->Fill(slope->GetParameter(0));
				if (assembly_check == true) // default is false 
				{
					if (i4 == 0 && i2 == 0)
						cout << " =========== pad performance check =========" << endl;
					if (slope->GetParameter(0) > 0.115)
					{
						//cout<<" the error pads check, chip_id : "<<i4+1<<" channel : "<<128-(i2/2)<<" slope : "<< slope->GetParameter(0)<<endl;

						if (i4 < 13)
						{
							if (i2 % 2 == 0)
							{
								cout << "1_the error pads check, chip_id : " << i4 + 1 << " channel : " << 128 - (i2 / 2) << " slope : " << slope->GetParameter(0) << endl;
							}
							else
							{
								cout << "2_the error pads check, chip_id : " << i4 + 1 << " channel : " << 64 - ((i2 / 2) - 1) << " slope : " << slope->GetParameter(0) << endl;
							}
						}
						else if (i4 < 26)
						{
							if (i2 % 2 == 0)
							{
								cout << "4_the error pads check, chip_id : " << i4 + 1 << " channel : " << 65 + ((i2 / 2)) << " slope : " << slope->GetParameter(0) << endl;
							}
							else
							{
								cout << "3_the error pads check, chip_id : " << i4 + 1 << " channel : " << 1 + (i2 / 2) << " slope : " << slope->GetParameter(0) << endl;
							}
						}
					}
				}

				c1->cd();
				tex11->DrawLatex(0.12, 0.750, Form("slope : %.4f", slope->GetParameter(0)));
				c1->cd();
				tex11->DrawLatex(0.12, 0.720, Form("offset : %.2f", slope->GetParameter(1)));
				//c1->cd(); tex11 -> DrawLatex (0.12, 0.690, Form("chi2/NDF : %.2f", slope->GetChisquare()/slope->GetNDF()));

				if (run_option == true)
				{
					c1->cd();
					c1->Print(Form("%s/folder_%s/chip%d_detail_ampladc.pdf", folder_name.Data(), the_name.Data(), i4 + 1));
				}

				c6->cd();
				check_new->Draw("hist");
				check_new->Fit("gaus_fit_new", "NQ");
				c6->cd();
				gaus_fit_new->Draw("lsame");
				c6->cd();
				tex11->DrawLatex(0.12, 0.750, Form("gaus width : %.4f", gaus_fit_new->GetParameter(2)));
				c6->cd();
				tex11->DrawLatex(0.12, 0.720, Form("gaus size  : %.2f", gaus_fit_new->GetParameter(0)));
				c6->cd();
				tex11->DrawLatex(0.12, 0.690, Form("gaus mean  : %.2f", gaus_fit_new->GetParameter(1)));
				response_width[i4].push_back(gaus_fit_new->GetParameter(2));
				channel_entries_check->Fill(check_new->GetEntries());
				if (check_new->GetEntries() < 280 || check_new->GetEntries() > 400)
				{
					//cout<<"Outsider entries, please check : "<<Form("chip_id=%d, chan_id = %d",i4+1,i2)<<" Entries : "<<check_new->GetEntries()<<endl;
					entries_chip.push_back(i4 + 1);
					entries_channel.push_back(i2);
					entries_entries.push_back(check_new->GetEntries());
					channel_entries_outsider += 1;
				}

				if (new_check == true)
				{
					c6->cd();
					c6->Print(Form("%s/folder_%s/chip%d_detail_amploffset.pdf", folder_name.Data(), the_name.Data(), i4 + 1));
				}
			} //redraw
			else 
			{
				redraw=true;
				check_new->Fit("gaus_fit_new", "NQ");
				cout<<"Magic happen !!! restore Start !!!, chip :"<<i4+1<<" chan : "<<i2<<endl;
				cout<<"under : "<<check_new->GetBinContent(0)<<" over : "<<check_new->GetBinContent(71)<<endl;
				cout<<"Fit width : "<<gaus_fit_new->GetParameter(2)<<endl;
				cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
				i2-=1;
			}
			ampladc_detail->Reset("ICESM");
			check_new->Reset("ICESM");
			c1->Clear();
			c6->Clear();
			if (assembly_check == true && i4 == 25 && i2 == 127) // default is false 
			{
				cout << " =========== pad performance check done =========" << endl;
			}
			
			for (int i8 = 0; i8 < 8; i8++)
			{
				sum_adc[i8] = 0;
				count_adc[i8] = 0;
				average_adc[i8] = 0;
			}
		} //channel ends
		cout << " " << endl;

		if (run_option == true)
		{
			c1->cd();
			c1->Print(Form("%s/folder_%s/chip%d_detail_ampladc.pdf)", folder_name.Data(), the_name.Data(), i4 + 1));
		}
		if (new_check == true)
		{
			c6->cd();
			c6->Print(Form("%s/folder_%s/chip%d_detail_amploffset.pdf)", folder_name.Data(), the_name.Data(), i4 + 1));
		}
	}//chip ends

	cout << "==============information of ampladc_detail ==========" << endl;
	c1->Clear();
	c1->cd();
	slope_TH2->Draw("COLZ0");
	c1->Print(Form("%s/folder_%s/ampl_adc_slope_TH2.pdf", folder_name.Data(), the_name.Data()));
	c1->Clear();
	cout << " TH2 plot " << endl;
	cout << " " << endl;
	cout << " " << endl;
	slop_detail_TH->Fit("gaus_fit", "NQ");
	c2->cd();
	slop_detail_TH->Draw("hist");
	TLatex *tex2 = new TLatex();
	tex2->SetNDC();
	tex2->SetTextFont(42);
	tex2->SetTextSize(0.032);
	tex2->DrawLatex(0.2, 0.840, Form("Gaus Mean : %.4f", gaus_fit->GetParameter(1)));
	tex2->DrawLatex(0.2, 0.80, Form("Gaus Sigma : %.4f", gaus_fit->GetParameter(2)));
	gaus_fit->Draw("lsame");
	c2->Print(Form("%s/folder_%s/slop_detail_TH.pdf", folder_name.Data(), the_name.Data()));
	c2->Clear();

	cout << "==============information of rest plots ==========" << endl;

	//int xaxis_plot[128];

	vector<double> xaxis_plot;
	xaxis_plot.clear();
	for (int i6 = 0; i6 < 128; i6++)
	{
		xaxis_plot.push_back(i6);
	}
	
	c1->Print(Form("%s/folder_%s/ampl_adc_slope_detail.pdf(", folder_name.Data(), the_name.Data()));
	cout << " the size of file : " << ampl_adc_slope[0].size() << endl;

	TGraph *grr[26];

	c4->Clear();
	c4->Divide(13, 2);
	for (int i4 = 0; i4 < 26; i4++)
	{
		c4->cd(i4 + 1);

		grr[i4] = new TGraph(128, &xaxis_plot[0], &ampl_adc_slope[i4][0]);
		grr[i4]->GetYaxis()->SetRangeUser(0.04, 0.15);
		grr[i4]->SetTitle(Form("S_chip_id=%d", i4 + 1));
		grr[i4]->GetXaxis()->SetLimits(-5, 135);
		grr[i4]->SetMarkerStyle(20);
		grr[i4]->SetMarkerColor(2);
		grr[i4]->SetMarkerSize(1.5);
		grr[i4]->Draw("al");

		c1->cd();
		grr[i4]->Draw("apl");
		c1->Print(Form("%s/folder_%s/ampl_adc_slope_detail.pdf", folder_name.Data(), the_name.Data()));

		//ampl_adc->Reset("ICESM");
	}
	c4->Print(Form("%s/folder_%s/ampl_adc_slope.pdf", folder_name.Data(), the_name.Data()));
	c4->Clear();
	for (int i6 = 0; i6 < 26; i6++)
	{
		delete grr[i6];
	}
	
	c1->Print(Form("%s/folder_%s/ampl_adc_slope_detail.pdf)", folder_name.Data(), the_name.Data()));
	c1->Clear();

	c6->Clear();
	c6->Print(Form("%s/folder_%s/ampl_adc_width_detial.pdf(", folder_name.Data(), the_name.Data()));
	TGraph *grr_new[26];
	TH2F *width_TH2 = new TH2F("", "", 128, 0, 128, 128, 0.00, 13);
	width_TH2->SetStats(0);
	for (int i9 = 0; i9 < 26; i9++)
	{
		grr_new[i9] = new TGraph(128, &xaxis_plot[0], &response_width[i9][0]);
		grr_new[i9]->GetYaxis()->SetRangeUser(0.00, 13);
		grr_new[i9]->SetTitle(Form("S_chip_id=%d", i9 + 1));
		grr_new[i9]->GetXaxis()->SetLimits(-5, 135);
		grr_new[i9]->SetMarkerStyle(20);
		grr_new[i9]->SetMarkerColor(2);
		grr_new[i9]->SetMarkerSize(1.5);
		for (int i10 = 0; i10 < response_width[i9].size(); i10++)
		{
			width_TH2->Fill(xaxis_plot[i10], response_width[i9][i10]);
			//cout<<"TESTTEST : "<<response_width[i9].size()<<endl;
			if (true == true)
			{
				chip_W = i9 + 1;
				chan_W = i10;
				level_W = response_width[i9][i10];
				tree_output4->Fill();

				if (response_width[i9][i10] < 4)
				{
					unbound_chip.push_back(i9 + 1);
					unbound_channel.push_back(i10);
					unbound_width.push_back(response_width[i9][i10]);
				}
				else if (response_width[i9][i10] > 4)
				{
					noise_chip.push_back(i9 + 1);
					noise_channel.push_back(i10);
					noise_width.push_back(response_width[i9][i10]);
				}
			}
		}
		c6->cd();
		grr_new[i9]->Draw("apl");

		c6->Print(Form("%s/folder_%s/ampl_adc_width_detial.pdf", folder_name.Data(), the_name.Data()));
	}
	c6->Print(Form("%s/folder_%s/ampl_adc_width_detial.pdf)", folder_name.Data(), the_name.Data()));
	c6->Clear();
	c6->cd();
	width_TH2->Draw("COLZ0");
	c6->Print(Form("%s/folder_%s/ampl_adc_width_detial_TH2.pdf", folder_name.Data(), the_name.Data()));
	c6->Clear();

	c1->Print(Form("%s/folder_%s/ampl_adc_offset_detail.pdf(", folder_name.Data(), the_name.Data()));
	//TGraph * grr[26];

	c4->Divide(13, 2);
	for (int i4 = 0; i4 < 26; i4++)
	{
		c4->cd(i4 + 1);

		grr[i4] = new TGraph(128, &xaxis_plot[0], &ampl_adc_offset[i4][0]);
		grr[i4]->SetTitle(Form("O_chip_id=%d", i4 + 1));
		grr[i4]->GetXaxis()->SetLimits(-5, 135);
		grr[i4]->SetMarkerStyle(20);
		grr[i4]->SetMarkerColor(2);
		grr[i4]->SetMarkerSize(1.5);
		grr[i4]->Draw("al");

		c1->cd();
		grr[i4]->Draw("apl");
		c1->Print(Form("%s/folder_%s/ampl_adc_offset_detail.pdf", folder_name.Data(), the_name.Data()));

		//ampl_adc->Reset("ICESM");
	}
	c4->Print(Form("%s/folder_%s/ampl_adc_offset.pdf", folder_name.Data(), the_name.Data()));
	c4->Clear();
	c1->Print(Form("%s/folder_%s/ampl_adc_offset_detail.pdf)", folder_name.Data(), the_name.Data()));
	c1->Clear();
	for (int i6 = 0; i6 < 26; i6++)
	{	
		delete grr[i6];
	}
	// c1->Print(Form("%s/%s/ampl_adc_ChiNDF_detail.pdf(",the_name.Data()));
	// TGraph * grr[26];

	// c4->Divide(13,2);
	//   for (int i4=0; i4<26; i4++)
	//    {
	//    	 c4->cd(i4+1);

	//    	 grr[i4] = new TGraph (128,&xaxis_plot[0],&ampl_adc_chiNDF[i4][0]);

	//    	 grr[i4]->SetTitle(Form("C_chip_id=%d",i4+1));
	//    	 grr[i4]->GetXaxis()->SetLimits(-5,135);
	//    	 grr[i4]->SetMarkerStyle(20);
	//    	 grr[i4]->SetMarkerColor(2);
	//    	 grr[i4]->SetMarkerSize(1.5);
	//    	 grr[i4]->Draw("al");
	//    	 c1->cd (); grr[i4]->Draw("apl");
	//   	 c1->Print(Form("%s/%s/ampl_adc_ChiNDF_detail.pdf",the_name.Data()));

	//   	 //ampl_adc->Reset("ICESM");
	//    }
	// c4->Print(Form("%s/%s/ampl_adc_chiNDF.pdf",the_name.Data()));
	// c4 ->Clear();
	// for (int i6=0; i6<26; i6++) delete grr[i6];

	// cout<<"==============information of rest plots =========="<<endl;
	// c4 ->Clear();
	// c1->Print(Form("%s/%s/ampl_adc_ChiNDF_detail.pdf)",the_name.Data()));
	// c1 ->Clear();

	c4->Clear();
	c1->Clear();

	//fp.close();//Ãö³¬ÀÉ®×

	for (int i11 = 0; i11 < noise_channel.size(); i11++)
	{
		//source_name=Form("%s",the_name.Data());
		chip_N = noise_chip[i11];
		chan_N = noise_channel[i11];
		level_N = noise_width[i11];
		tree_output1->Fill();
	}
	for (int i11 = 0; i11 < entries_entries.size(); i11++)
	{
		//source_name=Form("%s",the_name.Data());
		chip_E = entries_chip[i11];
		chan_E = entries_channel[i11];
		level_E = entries_entries[i11];
		tree_output2->Fill();
	}

	for (int i11 = 0; i11 < unbound_chip.size(); i11++)
	{
		//source_name=Form("%s",the_name.Data());
		chip_U = unbound_chip[i11];
		chan_U = unbound_channel[i11];
		level_U = unbound_width[i11];
		tree_output3->Fill();
	}

	if (unbound_check == true)
	{
		cout << "======unbound_check======" << endl;
		cout << "number of unbounded channels : " << unbound_chip.size() << endl;
		cout << "chip : channel " << endl;
		for (int i11 = 0; i11 < unbound_chip.size(); i11++)
		{
			cout << unbound_chip[i11] << " : " << unbound_channel[i11] << endl;
		}
	}

	if (noise_channel_check == true)
	{
		cout << "======noise_check======" << endl;
		cout << "number of noise channels : " << noise_channel.size() << endl;
		cout << "chip : channel " << endl;
		for (int i11 = 0; i11 < noise_channel.size(); i11++)
		{
			cout << noise_chip[i11] << " : " << noise_channel[i11] << " width : " << noise_width[i11] << endl;
		}
	}

	c1->cd();
	channel_entries_check->Draw("hist");
	c1->Print(Form("%s/folder_%s/channel_entries.pdf", folder_name.Data(), the_name.Data()));
	c1->Clear();

	cout << "Number of channel entries outsider : " << channel_entries_outsider << endl;
	cout << "Entries outsider list : " << endl;
	for (int i11 = 0; i11 < entries_entries.size(); i11++)
	{
		cout << entries_chip[i11] << " : " << entries_channel[i11] << " entries : " << entries_entries[i11] << endl;
	}

	if (multi_run == true)
	{
		for (int i12 = 0; i12 < data_a.size(); i12++)
		{
			multi_txt << data_a[i12] << " " << data_b[i12] << " " << data_c[i12] << " " << data_d[i12] << " " << data_e[i12] << "\r" << endl;
		}
		multi_txt << Form("data name : %s.root .", the_name.Data()) << "\r" << endl;
		multi_txt << Form("noise channel number : ") << noise_channel.size() << "\r" << endl;
		for (int i11 = 0; i11 < noise_channel.size(); i11++)
		{
			multi_txt << noise_chip[i11] << " : " << noise_channel[i11] << " : " << noise_width[i11] << "\r" << endl;
		}
		multi_txt << Form("entries_outsider channel number : ") << channel_entries_outsider << "\r" << endl;
		for (int i11 = 0; i11 < entries_entries.size(); i11++)
		{
			multi_txt << entries_chip[i11] << " : " << entries_channel[i11] << " : " << entries_entries[i11] << endl;
		}
		multi_txt << "~~~~~~~~~~~~ ~~~~~~~~~~~~ ~~~~~~~~~~~~ ~~~~~~~~~~~~ ~~~~~~~~~~~~"<< "\r" << endl;
		multi_txt.close();

		// for (int i11=0; i11<DNA.size(); i11++)
		// {
		// 	multi_txt_noise<<D_NA[i11]<<" "<<D_NB[i11]<<" "<<D_NC[i11]<<" "<<D_ND[i11]<<" "<<D_NE[i11]<<"\r"<<endl;
		// }
	}

	tree_output1->Write("", TObject::kOverwrite);
	tree_output2->Write("", TObject::kOverwrite);
	tree_output3->Write("", TObject::kOverwrite);
	tree_output4->Write("", TObject::kOverwrite);

	// delete tree_output1;
	// delete tree_output2;

	//delete tree_output3;
	//delete file_output;
	file_output->Close();

	cout << "\r\n"<< "#the file rewriting is finish#"<< "\r\n"<< endl;
}
