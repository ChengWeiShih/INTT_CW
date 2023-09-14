void fitting_test()
{
  TGraph * aaa = new TGraph();
  aaa -> SetPoint(0,0,0);
  aaa -> SetPoint(1,1,1);
  aaa -> SetPoint(2,2.3,2);
  aaa -> SetPoint(3,3,3.3);
  aaa -> SetPoint(4,4,4);

  aaa -> SetMarkerStyle(20);
  aaa -> SetMarkerSize(1);

  TF1 * bbb = new TF1("","pol1",-10,10);
  aaa -> Fit(bbb,"N");

  aaa -> Draw("ap");
  bbb -> Draw("lsame");
}
