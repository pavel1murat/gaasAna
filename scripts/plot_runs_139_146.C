//-----------------------------------------------------------------------------
// runs 139-146 : 
//-----------------------------------------------------------------------------
#include "TGraphErrors.h"
#include "TBox.h"
//-----------------------------------------------------------------------------
// scan results - runs 139-146 :N1801-DEV9-D3-0100
// in the end of the scan the diode died
//-----------------------------------------------------------------------------
namespace {
					// local data
  int   const nruns       = 8;

  // run number and the source offset (in mm)
  
  float rn           [nruns] = {  139 ,   140 ,   141 ,  142 ,  143 , 144 , 145  , 146 };
  float dx           [nruns] = {    0.,  0.635,  1.270, 1.905,  2.54,   0., 0.635, 1.270};
  // running rime

  float run_time     [nruns] = { 1007.,  1567.,  1546., 1126., 1229., 2752., 2690., 3174};

  // integrated charges in mV*channels

  float q1_300       [nruns] = { 10.7, 11.0, 11.0, 10.8, 10.7, 10.5, 10.7, 9.79};
  float q1_300_err   [nruns] = { 0.04, 0.04, 0.04, 0.05, 0.04, 0.05, 0.03, 0.20};
		     
  float q1_400       [nruns] = { 9.34, 9.49, 9.66, 9.32, 9.27, 8.95, 9.23, 7.96};
  float q1_400_err   [nruns] = { 0.16, 0.09, 0.10, 0.12, 0.14, 0.14, 0.15, 0.14};

  // pulse heights

  float v2max_300    [nruns] = { 0.19  , 0.20  , 0.20  , 0.19  , 0.20  , 0.19  , 0.20  , 0.19};
  float v2max_300_err[nruns] = { 0.0004, 0.0004, 0.0004, 0.0005, 0.0004, 0.0004, 0.0003, 0.003};

  float v2max_400    [nruns] = { 0.35 , 0.35 , 0.35 , 0.36 , 0.41 , 0.35 , 0.39 , 0.30 };
  float v2max_400_err[nruns] = { 0.005, 0.003, 0.003, 0.004, 0.005, 0.003, 0.006, 0.003};
  

  // numbers of events in the fitted histograms
  
  float qev_300      [nruns] = { 1499 , 1379 ,  1376  , 1087 ,  1648., 1129., 1742.,  95. };
  float qev_400      [nruns] = {  228 ,  455 ,   436  ,  311 ,   214.,  272.,  195.,  358.};
};

//-----------------------------------------------------------------------------
void plot_q1_vs_run_139_146() {

  float xerr[nruns];

  for (int i=0; i<nruns; i++) xerr[i] = 0;

  TGraphErrors* gr = new TGraphErrors(nruns, rn,q1_300,xerr,q1_300_err);

  gr->SetTitle("Horizontal scan of N1801-DEV9-D3-0100, the diode died in the end");
  gr->SetMarkerStyle(20);
  gr->SetMaximum(13);
  gr->SetMinimum(7);
  gr->Draw("AP");

  TGraphErrors* gr2 = new TGraphErrors(nruns, rn,q1_400,xerr,q1_400_err);

  gr2->SetMarkerStyle(24);
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

}

//-----------------------------------------------------------------------------
// plot v2max vs run
//-----------------------------------------------------------------------------
void plot_v2max_vs_run_139_146() {

  float xerr[nruns];

  for (int i=0; i<nruns; i++) xerr[i] = 0;

  TGraphErrors* gr = new TGraphErrors(nruns, rn,v2max_300, xerr, v2max_300_err);

  gr->SetTitle("Horizontal scan of N1801-DEV9-D3-0100 : V2Max:Run, the diode died in the end");
  gr->SetMarkerStyle(20);
  gr->SetMaximum(0.5);
  gr->SetMinimum(0);
  gr->Draw("AP");

  TGraphErrors* gr2 = new TGraphErrors(nruns, rn,v2max_400,xerr,v2max_400_err);

  gr2->SetMarkerStyle(24);
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

}


//-----------------------------------------------------------------------------
// plot v2max vs run
//-----------------------------------------------------------------------------
void plot_rate_vs_run_139_146() {
  // int   const nruns       = 8;
  // float rn           [nruns] = {  139 ,   140 ,   141 , 142  ,  143 , 144 ,  145 ,  146 };
  // float dx           [nruns] = {    0.,  0.635,  1.270, 1.905,  2.54,   0., 0.635, 1.270};
  
  // float time         [nruns] = { 1007.,  1567.,  1546., 1126., 1229., 2752., 2690., 3174};
  
  // float qev_300      [nruns] = { 1499 , 1379 ,  1376  , 1087 ,  1648., 1129., 1742.,  95. };
  // float qev_400      [nruns] = {  228 ,  455 ,   436  ,  311 ,   214.,  272.,  195.,  358.};

  float xerr[nruns], yerr_300[nruns], yerr_400[nruns], rate_300[nruns], rate_400[nruns];

  for (int i=0; i<nruns; i++) {
    xerr[i] = 0;
    rate_300[i] = qev_300[i]/run_time[i];
    yerr_300[i] = sqrt(qev_300[i])/run_time[i];

    rate_400[i] = qev_400[i]/run_time[i];
    yerr_400[i] = sqrt(qev_400[i])/run_time[i];
  }

  TCanvas* c_rr = new TCanvas("c_rates_139_146","rates: runs 139-146",1400,700);

  TGraphErrors* gr = new TGraphErrors(nruns, rn, rate_300, xerr, yerr_300);

  gr->SetTitle("N1801-DEV9-D3-0100 scans: Rate vs Run");
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
void plot_rate_ratio_vs_run_139_146() {

  float xerr[nruns], yerr[nruns], rr[nruns];

  for (int i=0; i<nruns; i++) {
    xerr[i] = 0;
    rr[i] = qev_300[i]/qev_400[i];
    yerr[i] = rr[i]*sqrt(1/qev_300[i] + 1./qev_400[i]);
  }

  TCanvas* c_rr = new TCanvas("c_rr_139_146","rate ratio",1400,700);
  
  TGraphErrors* gr = new TGraphErrors(nruns, rn, rr, xerr, yerr);

  gr->SetTitle("Scan of N1801-DEV9-D3-0100: Rate ratio vs Run (diode died in the end)");
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
