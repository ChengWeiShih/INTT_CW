void Characterize_Pad (TPad *pad, float left = 0.15, float right = 0.1, float top = 0.1, float bottom = 0.12, int logY = 1, int setgrid_bool = 0)
{
	if (setgrid_bool == true) {pad -> SetGrid (1, 1);}
	pad -> SetLeftMargin   (left);
	pad -> SetRightMargin  (right);
	pad -> SetTopMargin    (top);
	pad -> SetBottomMargin (bottom);
    pad -> SetTicks(1,1);
	pad -> SetLogy (logY);
}

void alignment_dependence ()
{

    TString folder_name = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/BeamTest2021_data_MC_file_plot_final/data_file";

    // title : not use
    // // note : ========================== slot for run52 ==========================
    // TString run_name = "run52";
    // const int number_of_chip = 4;
    // double chip_id_array[number_of_chip] = {6,7,8,9};
    // double chip_id_array_error[number_of_chip] = {0,0,0,0};
    // double alignment [number_of_chip]       = {0.3620, 0.3178, 0.2908, 0.2629};
    // double alignment_width [number_of_chip] = {0.0613, 0.0337, 0.0324, 0.0316};
    // // note : ========================== slot for run52 ==========================

    // // note : ========================== slot for run89 ==========================
    // TString run_name = "run89";
    // const int number_of_chip = 4;
    // double chip_id_array[number_of_chip] = {8,9,10,11};
    // double chip_id_array_error[number_of_chip] = {0,0,0,0};
    // double alignment [number_of_chip]       = {0.3542,0.3125,0.2850,0.2608};
    // double alignment_width [number_of_chip] = {0.0669,0.0358,0.0328,0.0312};
    // // note : ========================== slot for run89 ==========================


    // // note : ========================== slot for run33 ==========================
    // TString run_name = "run33";
    // const int number_of_chip = 4;
    // double chip_id_array[number_of_chip] = {2,3,4,5};
    // double chip_id_array_error[number_of_chip] = {0,0,0,0};
    // double alignment [number_of_chip]       = {0.4661,0.4329,0.3885,0.3568};
    // double alignment_width [number_of_chip] = {0.0927,0.0516,0.0342,0.0341};
    // // note : ========================== slot for run33 ==========================


    // title : most recent, most accurate
    // // note : ========================== slot for run52 ========================== this is new, 2022/12/21, the error is fit gaussian width
    // TString run_name = "run52";
    // const int number_of_chip = 4;
    // double chip_id_array[number_of_chip] = {6,7,8,9};
    // double chip_id_array_error[number_of_chip] = {0,0,0,0};
    // double alignment [number_of_chip]       = {0.3646, 0.3190, 0.2975, 0.2651};
    // double alignment_width [number_of_chip] = {0.0882, 0.0298, 0.0283, 0.0362};
    // // note : ========================== slot for run52 ==========================

    // note : ========================== slot for run52 ========================== this is new, 2022/12/21, the error : fit mean error
    TString run_name = "run52";
    const int number_of_chip = 4;
    double chip_id_array[number_of_chip] = {6,7,8,9};
    double chip_id_array_error[number_of_chip] = {0,0,0,0};
    double alignment [number_of_chip]       = {0.3646, 0.3190, 0.2975, 0.2651};
    double alignment_width [number_of_chip] = {0.00194113, 0.000280809, 0.000141941, 0.00017819};
    // note : ========================== slot for run52 ==========================




    TCanvas * c1 = new TCanvas ("c1","c1",800,800);
    c1 -> cd();
    TPad *pad = new TPad(Form("pad1"), "", 0.0, 0.0, 1.0, 1.0);
    Characterize_Pad(pad, 0.15,  0.1,  0.1,  0.12, 0, 0);
    pad->Draw("same");
    pad->cd();

    TLatex * plot_note = new TLatex();
    plot_note->SetNDC();
    plot_note->SetTextSize(0.04);

    TF1 * pol1_fit = new TF1 ("pol1_fit","pol1",chip_id_array[0]-0.5,chip_id_array[number_of_chip-1]+0.5);
    pol1_fit -> SetLineWidth(3);

    TLegend *legend1 = new TLegend (0.6, 0.65, 0.85, 0.8);
	legend1 -> SetTextSize (0.050);
    legend1 -> SetBorderSize(0);

    TGraphErrors * align_plot = new TGraphErrors(number_of_chip,chip_id_array,alignment,chip_id_array_error,alignment_width);   
    align_plot -> SetMarkerStyle(20);
    align_plot -> SetMarkerSize(2);
    align_plot -> SetLineWidth(3);
    align_plot -> SetLineStyle(1);
    align_plot -> SetMarkerColor(TColor::GetColor("#1A3947"));

    // align_plot -> SetTitle(run_name+", chip dependency of alignment");
    align_plot -> SetTitle("");
    align_plot -> GetYaxis() -> SetTitle("Misalignment [mm] ");
    align_plot -> GetXaxis() -> SetTitle("Column ID");

    align_plot -> GetXaxis() -> SetNdivisions  (505);

    align_plot -> GetXaxis() -> SetTitleSize   (0.052);
	align_plot -> GetXaxis() -> SetTitleOffset (0.6);

	// align_plot -> GetYaxis() -> SetTitle       ("Data/MC");
	align_plot -> GetYaxis() -> SetTitleSize   (0.052);
	align_plot -> GetYaxis() -> SetTitleOffset (1.1);

    align_plot -> Draw("ap");
    // plot_note->DrawLatex(0.15, 0.912,"Error bar : fit gaussian width   Alignment on layer-1");
    // plot_note->DrawLatex(0.6, 0.912,"Alignment on layer-1");

    align_plot -> Fit("pol1_fit");
    pol1_fit -> Draw("lsame");

    // plot_note->DrawLatex(0.5, 0.85,Form("Fitting : Y = %.3fX + %.3f",pol1_fit->GetParameter(1),pol1_fit->GetParameter(0)));

    legend1 -> AddEntry(align_plot,"Data","ep");
    legend1 -> AddEntry(pol1_fit,"Linear fit","l");
    legend1 -> Draw("same");

    c1 -> Print(folder_name+"/"+run_name+"_alignment_dependency.pdf");
}