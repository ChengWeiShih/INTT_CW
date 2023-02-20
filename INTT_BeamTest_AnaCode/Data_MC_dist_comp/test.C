
#include "data_process.cpp"
#include "draw_style.h"

bool set_logY = false;
bool with_cut = false; // note : for cluster ADC and cluster size
bool X_label_less = true;
bool Y_label_far = true;

void initial_part()
{
    TCanvas * c1 = new TCanvas("c1","c1",850 ,800);
    TPad * pad_c1 = new TPad(Form("pad_c1"), "", 0.0, 0.0, 1.0, 1.0);
    Characterize_Pad(pad_c1, 0.15,  0.1,  0.1,  0.12, set_logY, 0);
    pad_c1 -> Draw();
    pad_c1 -> cd();
}


void test()
{
    cout<<X_label_less<<endl;

    initial_part();

    c1 -> Clear();
}