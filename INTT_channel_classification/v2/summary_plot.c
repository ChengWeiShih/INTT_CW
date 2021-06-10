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

	int total_size1=Gettree1->GetEntriesFast();
	int total_size2=Gettree2->GetEntriesFast();

	int chip_id_N, chan_id_N,source_id_N;
	double noise_level;

	int chip_id_E, chan_id_E,source_id_E;
	int entries_entries;	

	vector<int> noisy_channel[26][128];
	vector<int> entries_channel[26][128];

	for (int i=0; i< 26; i++)
	{
		for (int i1=0; i1<128; i1++)
		{
			noisy_channel[i][i1].clear();
			entries_channel[i][i1].clear();
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

	for (int i=0; i< total_size1; i++)
	{
		Gettree1->GetEntry(i);
		noise_plot_2D->Fill(chip_id_N,chan_id_N);
		noisy_channel[chip_id_N-1][chan_id_N].push_back(1);
	}
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
	}

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

}	
