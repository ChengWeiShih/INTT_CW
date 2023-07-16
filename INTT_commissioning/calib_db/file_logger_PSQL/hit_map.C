void hit_map(int module_in = 2)
{
    // TString folder_name = "/data4/chengwei/Geant4/INTT_simulation/G4/for_CW/Beamtest_data_Different_config/MC_run64_with_lead_plate_with_ZeroSlope_and_Run64U10offset";
    // TString file_name = "run64_X8_Y4_1M_WithSciAlFoil_10PercentHDIMetalMaterial_BeamSmear_EMZ_20umCut_Run64DataU10Offset_L1_2.73935e-01mm_ZeroSlope_WithLeadPlate_RoughTrigger"; 

    TString folder_name = "/home/inttdev/data/IR";
    TString file_name = "pedestal_packv5_042123_0208";

    TFile *f1 = new TFile (Form("%s/%s.root",folder_name.Data(),file_name.Data()), "read");
	TTree *tree_in = (TTree *)f1->Get("tree");

    TCanvas *c4 = new TCanvas("c4", "hit_map", 0, 700, 1625, 250);
    TH2D* hitmap = new TH2D("hitmap", "", 13, 0, 13, 257, 0, 257);

    // note : the additional cut
    int chan_id_upper = -1;
    int chan_id_down  = 128;
    TString additional_cut = ""; 

    int module_ID = module_in;
    
    hitmap->SetTitle(Form("Hit map, module ID : %i; chip_id; chan_id ",module_ID));
    stringstream cut_base; // useful class to generate string using non-string and char parameters
    cut_base << "ampl<70 " << "&& module ==" << module_ID << Form(" && %i < chan_id && chan_id < %i && 0 < chip_id && chip_id<27 %s ",chan_id_upper,chan_id_down,additional_cut.Data());

    string expression = string("int(chip_id / 14) * 256") // it's offsets for y-axis.
        + "+ pow(-1, int(chip_id / 14)) * chan_id"        // Y value increases(decreases) as chan_id decreases(increases) for chips1-13(14-26)
        + ": -chip_id"                                   // Basically, x value increases as chip_id decreases (chip13 at x=0, chip12 at x=-1, ...)
        + "+ (1 + int(chip_id / 14)) * 13"               // In addition to above, offset is needed. 
        + ">> hitmap";

    tree_in->Draw(expression.c_str(), cut_base.str().c_str(), "goff");
    hitmap->Draw("colz");

    hitmap->GetXaxis()->SetNdivisions(15);
    hitmap->GetXaxis()->SetLabelSize(0.08);
    hitmap->GetXaxis()->SetTitleSize(0.08);
    hitmap->GetYaxis()->SetLabelSize(0.08);
    hitmap->SetStats(0);
    // draw chip_id on the hist
    for (int i = 0; i < 26; i++) {
        TLatex* tex = new TLatex();
        double x = i < 13 ? 12.25 - i : 12.25 - i + 13;
        double y = i < 13 ? 64 : 192;
        tex->DrawLatex(x, y, Form("Chip%d", i + 1));
    }


    // c4->Print( Form("%s/folder_%s/hit_map.pdf",folder_direction.Data(),file_name.Data()) );
    
    // hitmap->Reset();
    // c4->Clear();
    
	
		

}
