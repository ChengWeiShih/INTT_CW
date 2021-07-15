void summary_plot( int number_of_file, TString folder_name, TString merge_file_name )
{
	
	TString list_buffer;
	vector<TString> list_array; list_array.clear();
	TString the_name;
	
	ifstream data_list;
	data_list.open(Form("%s/total_file.txt",folder_name.Data()));

	while ( 1 )
	{
	  data_list >> list_buffer  ;
	  list_array.push_back(list_buffer);
	  if ( !data_list.good() )
	    {
	      break;
		}
	}



	TFile* f1 = TFile::Open(Form("%s/%s.root",folder_name.Data(),merge_file_name.Data()));
	TTree* Gettree1    = (TTree*) f1->Get("Noise_check");
	TTree* Gettree2    = (TTree*) f1->Get("Entries_check");
	TTree* Gettree5    = (TTree*) f1->Get("Total_entries");

	int total_size1=Gettree1->GetEntriesFast();
	int total_size2=Gettree2->GetEntriesFast();
	int total_size5=Gettree5->GetEntriesFast();

	int chip_id_N, chan_id_N,source_id_N;
	double noise_level;

	int chip_id_E, chan_id_E,source_id_E;
	int entries_entries;	

	double entries_mean, entries_width;
	int source_id_entries;

	vector<double> entries_X; entries_X.clear();
	vector<double> entries_X_E; entries_X_E.clear();
	vector<double> entries_Y; entries_Y.clear();
	vector<double> entries_Y_E; entries_Y_E.clear();

	vector<int> noisy_channel[26][128];
	vector<int> entries_channel[26][128];

	vector<int> final_count_noise_chip; final_count_noise_chip.clear(); 
	vector<int> final_count_noise_chan; final_count_noise_chan.clear();
 
	vector<int> final_count_entry_chip; final_count_entry_chip.clear(); 
	vector<int> final_count_entry_chan; final_count_entry_chan.clear();
 
 	int double_count=0;

	for (int i=0; i< 26; i++)
	{
		for (int i1=0; i1<128; i1++)
		{
			noisy_channel[i][i1].clear();
			entries_channel[i][i1].clear();
		}
	}

	int file_entry [number_of_file][26][128];
	int file_noisy [number_of_file][26][128];
	int file_double_count[number_of_file];

	for(int i11=0; i11<number_of_file; i11++)
	{
		file_double_count[i11]=0;
		for(int i12=0; i12<26; i12++)
		{
			for(int i13=0; i13<128; i13++)
			{
				file_noisy[i11][i12][i13]=0;
				file_entry[i11][i12][i13]=0;
			}
		}
	}

	Gettree1->SetBranchAddress("chip_id",&chip_id_N);
	Gettree1->SetBranchAddress("chan_id",&chan_id_N);
	Gettree1->SetBranchAddress("noise_level",&noise_level);
	Gettree1->SetBranchAddress("source_id",&source_id_N);
	
	Gettree2->SetBranchAddress("chip_id",&chip_id_E);
	Gettree2->SetBranchAddress("chan_id",&chan_id_E);
	Gettree2->SetBranchAddress("entries",&entries_entries);
	Gettree2->SetBranchAddress("source_id",&source_id_E);

	Gettree5->SetBranchAddress("entries_mean",&entries_mean);
	Gettree5->SetBranchAddress("entries_width",&entries_width);
	Gettree5->SetBranchAddress("source_id",&source_id_entries);

	TCanvas * c2 = new TCanvas ("c2","c2",2600,1800);
	c2->Divide(2,2);	

	TCanvas * c1 = new TCanvas ("","",2200,1800);
	c1->cd();
	//gStyle->SetOptStat(111111);
	// TPad *pad = new TPad(Form("pad1"), "", 0.0, 0.0, 1.0, 1.0);
	// // pad->SetTopMargin(0.12);
	// // pad->SetBottomMargin(0.120);
	// // pad->SetLeftMargin(0.15);
	// // pad->SetRightMargin(0.05);
	// pad->SetGrid(0.1, 0.1);
	// pad->Draw("same");
	// pad->cd();


	TH2F * noise_plot_2D = new TH2F ("","channel noise check",26,1,27,128,0,128);
	noise_plot_2D->GetXaxis()->SetTitle("Chip ID");
	noise_plot_2D->GetYaxis()->SetTitle("Channel ID");
	noise_plot_2D->GetYaxis()->SetTitleOffset(1.5);
	noise_plot_2D->SetStats(0);
	noise_plot_2D->GetZaxis()->SetRangeUser(0,number_of_file);

	TH2F * entry_plot_2D = new TH2F ("","channel entries check",26,1,27,128,0,128);
	entry_plot_2D->GetXaxis()->SetTitle("Chip ID");
	entry_plot_2D->GetYaxis()->SetTitle("Channel ID");
	entry_plot_2D->GetYaxis()->SetTitleOffset(1.5);
	entry_plot_2D->SetStats(0);
	entry_plot_2D->GetZaxis()->SetRangeUser(0,number_of_file);

	TH1F * noise_plot_1D = new TH1F ("","Noise, # of bad channels",number_of_file+2,-1,number_of_file+1);
	noise_plot_1D->GetXaxis()->SetTitle("File ID (nth run)");
	noise_plot_1D->GetYaxis()->SetTitle("# of channels");

	TH1F * entry_plot_1D = new TH1F (""," Entry, # of bad channels",number_of_file+2,-1,number_of_file+1);
	entry_plot_1D->GetXaxis()->SetTitle("File ID (nth run)");
	entry_plot_1D->GetYaxis()->SetTitle("# of channels");

	TH1F * all_plot_1D = new TH1F (""," Both, # of bad channels",number_of_file+2,-1,number_of_file+1);
	all_plot_1D->GetXaxis()->SetTitle("File ID (nth run)");
	all_plot_1D->GetYaxis()->SetTitle("# of channels");


	for (int i=0; i< total_size1; i++)
	{
		Gettree1->GetEntry(i);
		noise_plot_2D->Fill(chip_id_N,chan_id_N);
		noisy_channel[chip_id_N-1][chan_id_N].push_back(1);
		noise_plot_1D->Fill(source_id_N);
		file_noisy[source_id_N][chip_id_N-1][chan_id_N]=1;
	}
	// file_noisy[0][3][3]=1;
	// file_noisy[0][3][4]=1;
	
	TLatex *tex1 = new TLatex();
	tex1->SetNDC();
	tex1->SetTextFont(42);
	tex1->SetTextSize(0.032);
	noise_plot_2D->Draw("COLZ0");
	tex1->DrawLatex(0.7, 0.91, Form(" Total entries : %.0f ", noise_plot_2D->GetEntries()));
	c1->Print(Form("%s/Summary_noise.pdf",folder_name.Data()));
	c1->Clear();
	tex1->Clear();


	for (int i=0; i< total_size2; i++)
	{
		Gettree2->GetEntry(i);
		entry_plot_2D->Fill(chip_id_E,chan_id_E);	
		entries_channel[chip_id_E-1][chan_id_E].push_back(1);
		entry_plot_1D->Fill(source_id_E);
		file_entry[source_id_E][chip_id_E-1][chan_id_E]=1;
	}

	// file_entry[0][3][3]=1;
	// file_entry[0][3][4]=1;

	c1->cd();
	// pad->Draw("same");
	// pad->cd();
	entry_plot_2D->Draw("COLZ0");
	tex1->DrawLatex(0.7, 0.91, Form(" Total entries : %.0f ", entry_plot_2D->GetEntries()));
	c1->Print(Form("%s/Summary_entry.pdf",folder_name.Data()));
	c1->Clear();
	
	cout<<"    "<<endl;

	for (int i=0; i< 26; i++)
	{
		for (int i1=0; i1<128; i1++)
		{
			if (noisy_channel[i][i1].size()>2)
			{
				cout<<"Noise channel found, chip : "<<i+1<<"	channel : "<<i1<<"	failed times : "<<noisy_channel[i][i1].size()<<Form("/%i",number_of_file)<<"	ratio : "<<double(noisy_channel[i][i1].size())/double(number_of_file)<<endl;
				final_count_noise_chip.push_back(i+1);
				final_count_noise_chan.push_back(i1);

			}
			
		}
	}

	for (int i=0; i< 26; i++)
	{
		for (int i1=0; i1<128; i1++)
		{
			if (noisy_channel[i][i1].size()>2 && noisy_channel[i][i1].size()<number_of_file)
			{
				cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
				cout<<"Noise channel, chip : "<<i+1<<" channel : "<<i1<<endl;
				for (int i2=0; i2<total_size1; i2++)
				{
					Gettree1->GetEntry(i2);
					if(chip_id_N == i+1 && chan_id_N == i1 )
					{
						cout<<" failed in file index :	"<<source_id_N<<"	file name : "<<list_array[source_id_N]<<", gaus width : "<<noise_level<<endl;
					}
										
				}
				cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
				cout<<" "<<endl;
			}
			
		}
	}


	cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;

	for (int i=0; i< 26; i++)
	{
		for (int i1=0; i1<128; i1++)
		{
			
			if (entries_channel[i][i1].size()>2)
			{
				cout<<"Weird entries found, chip : "<<i+1<<"	channel : "<<i1<<"	failed times : "<<entries_channel[i][i1].size()<<Form("/%i",number_of_file)<<"	ratio : "<<double(entries_channel[i][i1].size())/double(number_of_file)<<endl;
				final_count_entry_chip.push_back(i+1);
				final_count_entry_chan.push_back(i1);
			}
		}
	}

	for (int i=0; i< 26; i++)
	{
		for (int i1=0; i1<128; i1++)
		{
			if (entries_channel[i][i1].size()>2 && entries_channel[i][i1].size()<number_of_file)
			{
				cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
				cout<<"Bad entries channel, chip : "<<i+1<<" channel : "<<i1<<endl;
				for (int i2=0; i2<total_size2; i2++)
				{
					Gettree2->GetEntry(i2);
					if(chip_id_E == i+1 && chan_id_E == i1 )
					{
						cout<<" bad in file index : "<<source_id_E<<"	file name : "<<list_array[source_id_E]<<", entries : "<<entries_entries<<endl;
					}
										
				}
				cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
				cout<<" "<<endl;
			}
			
		}
	}

	for (int i3=0; i3<final_count_noise_chan.size(); i3++)
	{
		for (int i4=0; i4<final_count_entry_chan.size();i4++)
		{
			if (final_count_noise_chan[i3] == final_count_entry_chan[i4] && final_count_entry_chip[i4]== final_count_noise_chip[i3])
			{
				double_count+=1;
			}	
		}
	}

	int total_bad_channel = final_count_entry_chan.size()+final_count_noise_chan.size()-double_count;
	

	for(int i11=0; i11<number_of_file; i11++)
	{
		for(int i12=0; i12<26; i12++)
		{
			for(int i13=0; i13<128; i13++)
			{
				//cout<<"entry : "<< file_entry[i11][i12][i13][0] << " noisy : "<<file_noisy[i11][i12][i13][0]<<endl;
				if (file_entry[i11][i12][i13]==1 && file_noisy[i11][i12][i13]==1)
				{
					file_double_count[i11]+=1;
				}
			}
		}
	}
	
	cout<<"======================== Final summary ========================"<<endl;
	cout<<"# of noise channels : "<<final_count_noise_chan.size()<<endl;
	cout<<"# of bad entry chan : "<<final_count_entry_chan.size()<<endl;
	cout<<"# of double counting: "<<double_count<<endl;
	cout<<" "<<endl;
	cout<<"Total bad channels : "<<total_bad_channel<<endl;
	cout<<"Overall good channel ratio : "<<3328-total_bad_channel<<" / 3328 = "<<Form("%.3f",(double(3328-total_bad_channel)/3328.)*100.)<<"%"<<endl;
	cout<<"======================== Final summary ========================"<<endl;
	cout<<" "<<endl;

	for (int i5=0; i5<total_size5; i5++)
	{
		Gettree5->GetEntry(i5);

		entries_X.push_back(source_id_entries);
		entries_X_E.push_back(0);
		entries_Y.push_back(entries_mean);
		entries_Y_E.push_back(entries_width);
	}

	c2->cd(1);
	TGraph * entries_trial = new TGraphErrors (number_of_file,&entries_X[0],&entries_Y[0],&entries_X_E[0],&entries_Y_E[0]);
	entries_trial->SetMarkerStyle(20);
	entries_trial->SetMarkerColor(TColor::GetColor("#065381"));
	entries_trial->SetMarkerSize(4);
	entries_trial->SetTitle("Average # of channel entries vs nth runs");
	entries_trial->GetXaxis()->SetLimits(-1,number_of_file+1);
	entries_trial->GetXaxis()->SetTitle("File ID (nth run)");
	entries_trial->GetYaxis()->SetTitle("Average # of channel entries");
	entries_trial->Draw("apl");
	

	c2->cd(2);
	entry_plot_1D->Draw("hist");
	c2->cd(3);
	noise_plot_1D->Draw("hist");
	

	c2->cd(4);
	for (int i=0; i<number_of_file; i++)
	{
		all_plot_1D->SetBinContent(i+2,entry_plot_1D->GetBinContent(i+2)+noise_plot_1D->GetBinContent(i+2)-file_double_count[i]);
		//cout<<entry_plot_1D->GetBinContent(i+2)+noise_plot_1D->GetBinContent(i+2)-file_double_count[i]<<"	"<<entry_plot_1D->GetBinContent(i+2)<<"	"<<noise_plot_1D->GetBinContent(i+2)<<"	"<<file_double_count[i]<<endl;
	}

	all_plot_1D->Draw("hist");
	c2->Print(Form("%s/Summary_plots_time.pdf",folder_name.Data()));
}	
