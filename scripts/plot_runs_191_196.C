//-----------------------------------------------------------------------------
// runs 139-146 : 
//-----------------------------------------------------------------------------
#include "TGraphErrors.h"
#include "TBox.h"
//-----------------------------------------------------------------------------
// scan results - runs 139-146 :N1801-DEV9-D3-0100
// in the end of the scan the diode died
//-----------------------------------------------------------------------------
void plot_q1_vs_run_191_196() {
  int   const nruns       = 5;
  float rn        [nruns] = {  191 ,  192 ,  193   , 195,  196 };
  float dx        [nruns] = {    0.,  3.175,  5.715,   0., 2.54};
  //                         trig.thr   -1.44 mV   |  -1.7mV  -2.0mV  
  float q1_300    [nruns] = { 11.70, 12.16, 17.04  , 10.6, 11.8};
  float q1_300_err[nruns] = {  0.03,  0.04,  0.11  , 0.02, 0.04};

  float q1_400    [nruns] = { 10.75, 12.16, 15.15 , 9.5, 10.0 };
  float q1_400_err[nruns] = {  0.07,  0.16,  1.43 , 0.1,  0.2 };

  float xerr[nruns];

  for (int i=0; i<nruns; i++) xerr[i] = 0;

  TGraphErrors* gr = new TGraphErrors(nruns, rn,q1_300,xerr,q1_300_err);

  gr->SetTitle("Horizontal scan of N1801-S3-D5-050: Q1 vs run number");
  gr->SetMarkerStyle(20);
  gr->SetMinimum( 8);
  gr->SetMaximum(18);
  gr->Draw("AP");

  TGraphErrors* gr2 = new TGraphErrors(nruns, rn,q1_400,xerr,q1_400_err);

  gr2->SetMarkerStyle(21);
  gr2->SetMarkerColor(kRed+2);
  gr2->Draw("p,same");

  TBox* b = new TBox(138.9,8.7,143.2,11.5);
  b->SetFillStyle(3004);
  b->SetFillColor(kRed+2);
  b->Draw();

  TBox* b1 = new TBox(143.9,7.4,146.2,11.);
  b1->SetFillStyle(3005);
  b1->SetFillColor(kRed+2);
  b1->Draw();

  TLatex* txt = new TLatex(191,15,"V_{thr} = -1.4 mV");
  txt->SetTextSize(0.035);
  txt->Draw();

  TLatex* txt1 = new TLatex(194.5,12,"V_{thr} = -1.7 mV");
  txt1->SetTextSize(0.035);
  txt1->Draw();

  TLatex* txt2 = new TLatex(195.5,14,"V_{thr} = -2.0 mV");
  txt2->SetTextSize(0.035);
  txt2->Draw();

}

//-----------------------------------------------------------------------------
// plot v2max vs run
//-----------------------------------------------------------------------------
void plot_v2max_vs_run_191_196() {
  int   const nruns          = 5;
  float rn           [nruns] = { 191 , 192  , 193   , 195  , 196   };
  float dx           [nruns] = {   0., 3.175, 5.715 ,   0. ,  2.54};
  
  float v2max_300    [nruns] = { 0.34 , 0.44 , 0.70  , 0.37, 0.44};
  float v2max_300_err[nruns] = { 0.001, 0.001, 0.005 , 0.001, 0.002};

  float v2max_400    [nruns] = { 0.67 , 0.86 , 1.51  , 0.86,  1.06 };
  float v2max_400_err[nruns] = { 0.01 , 0.07 , 0.17  , 0.005, 0.015};

  float xerr[nruns];

  for (int i=0; i<nruns; i++) xerr[i] = 0;

  TGraphErrors* gr = new TGraphErrors(nruns, rn,v2max_300, xerr, v2max_300_err);

  gr->SetTitle("Horizontal scan of N1801-S3-D5-050 : V2Max:Run");
  gr->SetMarkerStyle(20);
  gr->SetMinimum(0);
  gr->SetMaximum(2.0);
  gr->Draw("AP");

  TGraphErrors* gr2 = new TGraphErrors(nruns, rn,v2max_400,xerr,v2max_400_err);

  gr2->SetMarkerStyle(21);
  gr2->SetMarkerColor(kRed+2);
  gr2->Draw("p,same");

  TBox* b = new TBox(138.9,0.15,143.1,0.45);
  b->SetFillStyle(3004);
  b->SetFillColor(kRed+2);
  b->Draw();

  TBox* b1 = new TBox(143.9,0.15,146.1,0.45);
  b1->SetFillStyle(3005);
  b1->SetFillColor(kRed+2);
  b1->Draw();

  TLatex* txt = new TLatex(191,1.5,"V_{thr} = -1.4 mV");
  txt->SetTextSize(0.035);
  txt->Draw();

  TLatex* txt1 = new TLatex(194.5,1.,"V_{thr} = -1.7 mV");
  txt1->SetTextSize(0.035);
  txt1->Draw();

  TLatex* txt2 = new TLatex(195.5,1.2,"V_{thr} = -2.0 mV");
  txt2->SetTextSize(0.035);
  txt2->Draw();
}
//-----------------------------------------------------------------------------
// plot v2max vs run
//-----------------------------------------------------------------------------
void plot_rate_vs_run_191_196() {
  int   const nruns          = 5;
  float rn           [nruns] = {  191 , 192  , 193   , 195  , 196   };
  float dx           [nruns] = {    0., 3.175, 5.715 ,   0. ,  2.54 };
  float time         [nruns] = { 2423., 1542 , 1402  , 2437 ,  1461 };
  
  float qev_300      [nruns] = { 4433 , 1792 ,  466  , 4190 ,  1603 };

  float qev_400      [nruns] = {  441 ,  206 ,   27  ,  495 ,   192 };

  float xerr[nruns], yerr_300[nruns], yerr_400[nruns], rate_300[nruns], rate_400[nruns];

  for (int i=0; i<nruns; i++) {
    xerr[i] = 0;
    rate_300[i] = qev_300[i]/time[i];
    yerr_300[i] = sqrt(qev_300[i])/time[i];

    rate_400[i] = qev_400[i]/time[i];
    yerr_400[i] = sqrt(qev_400[i])/time[i];
  }

  TGraphErrors* gr = new TGraphErrors(nruns, rn, rate_300, xerr, yerr_300);

  gr->SetTitle("Horizontal scan of N1801-S3-D5-050 : Rate vs Run");
  gr->SetMarkerStyle(20);
  gr->SetMinimum(-0.5);
  gr->SetMaximum(2.5);
  gr->Draw("AP");

  TGraphErrors* gr2 = new TGraphErrors(nruns, rn,rate_400,xerr,yerr_400);

  gr2->SetMarkerStyle(21);
  gr2->SetMarkerColor(kRed+2);
  gr2->Draw("p,same");

  TBox* b = new TBox(138.9,0.15,143.1,0.45);
  b->SetFillStyle(3004);
  b->SetFillColor(kRed+2);
  b->Draw();

  TBox* b1 = new TBox(143.9,0.15,146.1,0.45);
  b1->SetFillStyle(3005);
  b1->SetFillColor(kRed+2);
  b1->Draw();

  TLatex* txt = new TLatex(191,1.5,"V_{thr} = -1.4 mV");
  txt->SetTextSize(0.035);
  txt->Draw();

  TLatex* txt1 = new TLatex(194.5,1.,"V_{thr} = -1.7 mV");
  txt1->SetTextSize(0.035);
  txt1->Draw();

  TLatex* txt2 = new TLatex(195.5,1.2,"V_{thr} = -2.0 mV");
  txt2->SetTextSize(0.035);
  txt2->Draw();
}
//-----------------------------------------------------------------------------
// plot v2max vs run
//-----------------------------------------------------------------------------
void plot_rate_ratio_vs_run_191_196() {
  int   const nruns          = 5;
  float rn           [nruns] = {  191 , 192  , 193   , 195  , 196   };
  float dx           [nruns] = {    0., 3.175, 5.715 ,   0. ,  2.54 };
  float time         [nruns] = { 2423., 1542 , 1402  , 2437 ,  1461 };
  
  float qev_300      [nruns] = { 4433 , 1792 ,  466  , 4190 ,  1603 };

  float qev_400      [nruns] = {  441 ,  206 ,   27  ,  495 ,   192 };

  float xerr[nruns], yerr[nruns], rr[nruns];

  for (int i=0; i<nruns; i++) {
    xerr[i] = 0;
    rr[i] = qev_300[i]/qev_400[i];
    yerr[i] = rr[i]*sqrt(1/qev_300[i] + 1./qev_400[i]);
  }

  TCanvas* c_rr = new TCanvas("c_rr_191_196","rate ratio",1400,700);
  
  TGraphErrors* gr = new TGraphErrors(nruns, rn, rr, xerr, yerr);

  gr->SetTitle("Horizontal scan of N1801-S3-D5-050 : Rate vs Run");
  gr->SetMarkerStyle(20);
  // gr->SetMinimum(-0.5);
  // gr->SetMaximum(2.5);
  gr->Draw("AP");

  TLatex* txt = new TLatex(191,1.5,"V_{thr} = -1.4 mV");
  txt->SetTextSize(0.035);
  txt->Draw();

  TLatex* txt1 = new TLatex(194.5,1.,"V_{thr} = -1.7 mV");
  txt1->SetTextSize(0.035);
  txt1->Draw();

  TLatex* txt2 = new TLatex(195.5,1.2,"V_{thr} = -2.0 mV");
  txt2->SetTextSize(0.035);
  txt2->Draw();
}
