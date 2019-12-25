//-----------------------------------------------------------------------------
// runs 161-164 : 
//-----------------------------------------------------------------------------
#include "TGraphErrors.h"
#include "TBox.h"

#include  "gaasAna/ana/TRunResults.hh"

//-----------------------------------------------------------------------------
// scan results - runs 161-164 :N1801-DEV9-D5-050 (no second peak (QD ?) - rates! 
//-----------------------------------------------------------------------------
					// local data
int   const nruns       = 12;

  // run number and the source offset (in mm)
  
float rn           [nruns] = {  161  ,   162 ,   163 ,   164 ,  165  ,   166 ,    167,  168  ,   169 ,  170 , 171, 172 };
// float dx           [nruns] = {  2.54 ,   2.54,  2.54 ,  2.54 ,  2.54 ,  2.54 ,   2.54,   0.  ,   0.  ,   0.  };

//   // running rime

float run_time     [nruns] = {  781.,    803.,  802. ,   814 ,   803.,  2481 ,    803,   863 ,  1751 ,  1674 , 2411, 2673 };

//   // integrated charges in mV*channels

// float qev_300      [nruns] = { 999   ,  983  ,  974  , 941   ,  953  ,  2902 ,  985  ,  819  ,  1548 , 1352  };

// float q1_300       [nruns] = { 14.504, 15.511, 15.954, 16.311, 13.113, 12.692, 11.882, 11.497, 12.331, 12.579};
// float q1_300_err   [nruns] = {  0.052,  0.055,  0.055,  0.063,  0.042,  0.023,  0.043,  0.057,  0.039,  0.117};
		     
// float v2max_300    [nruns] = { 0.608 ,  0.719,  0.748,  0.848,  0.587,  0.568,  0.493,  0.430,  0.503,  0.521};
// float v2max_300_err[nruns] = { 0.003 ,  0.003,  0.003,  0.003,  0.002,  0.001,  0.002,  0.002,  0.001,  0.002};

// float qev_400      [nruns] = {   0   ,    9  ,    8  ,  59   ,    43 ,    96 ,   6   ,   180 ,  423  ,   551 };

// float q1_400       [nruns] = {  -1.  , -1.   , -1.   , -1.   , 13.926, 13.962, 14.722, 11.661, 12.095, 11.048};
// float q1_400_err   [nruns] = {  -1.  , -1.   , -1.   , -1.   ,  4.014,  0.592, 20.953,  0.150,  0.093,  0.129};

//   // pulse heights

// float v2max_400    [nruns] = {  -1.  , -1.   , -1.   , -1.   , -0.555, -0.409,  0.776,  0.622,  0.693,  0.678};
// float v2max_400_err[nruns] = {  -1.  , -1.   , -1.   , -1.   , -1.   , -1.   , -1.   ,  0.007,  0.006,  0.007};
  
  // numbers of events in the fitted histograms
  
//-----------------------------------------------------------------------------
void plot_q1_vs_run_161_170() {

  TRunResults_t rr[1000];

  for (int i=0; i<nruns; i++) {
    int run_number = rn[i];
    make_run_results(run_number,&rr[run_number]);
  }

  float xerr[nruns], q1_300[nruns], q1_400[nruns], yerr[nruns];

  for (int i=0; i<nruns; i++) {
    int run_number = rn[i];
    TRunResults_t* res = &rr[run_number];
    xerr  [i] = 0;
    q1_300[i] = res->fQ1300.fMean;
    yerr  [i] = res->fQ1300.fMeanErr;
  }

  TCanvas* c = new TCanvas("c_q1_161_170","charge: runs 161-170",1400,700);

  TGraphErrors* gr = new TGraphErrors(nruns, rn,q1_300,xerr,yerr);

  gr->SetTitle("Bias scan of N1801-DEV9-D3-050, no _400");
  gr->SetMarkerStyle(20);
  gr->SetMaximum(20);
  gr->SetMinimum(10);
  gr->Draw("AP");

  for (int i=0; i<nruns; i++) {
    int run_number = rn[i];
    TRunResults_t* res = &rr[run_number];
    xerr  [i] = 0;
    q1_400[i] = res->fQ1400.fMean;
    yerr  [i] = res->fQ1400.fMeanErr;
  }

  TGraphErrors* gr2 = new TGraphErrors(nruns, rn,q1_400,xerr,yerr);

  gr2->SetMarkerStyle(24);
  gr2->SetMarkerColor(kRed+2);
  gr2->Draw("p,same");

  TBox* b = new TBox(160.9,11.5,167.1,12.5);
  b->SetFillStyle(3004);
  b->SetFillColor(kRed+2);
  b->Draw();

  TLatex* lat = new TLatex(162.9,11.9,"N1801-DEV9-D5-050");
  lat->SetTextSize(0.03);
  lat->Draw();

  TBox* b1 = new TBox(167.9,13.5,172.1,14.5);
  b1->SetFillStyle(3005);
  b1->SetFillColor(kRed+2);
  b1->Draw();

  TLatex* lat1 = new TLatex(168.1,13.9,"N1801-DEV9-D2-050");
  lat1->SetTextSize(0.03);
  lat1->Draw();

}

//-----------------------------------------------------------------------------
// plot v2max vs run
//-----------------------------------------------------------------------------
void plot_v2max_vs_run_161_170() {

  TRunResults_t rr[1000];

  for (int i=0; i<nruns; i++) {
    int run_number = rn[i];
    make_run_results(run_number,&rr[run_number]);
  }

  float xerr[nruns], v2_300[nruns], v2_400[nruns], yerr[nruns];

  for (int i=0; i<nruns; i++) {
    int run_number = rn[i];
    TRunResults_t* res = &rr[run_number];
    xerr  [i] = 0;
    v2_300[i] = res->fV2300.fMean;
    yerr  [i] = res->fV2300.fMeanErr;
  }

  TCanvas* c = new TCanvas("c_v2max_161_170","pulse heigh: runs 161-170",1400,700);

  TGraphErrors* gr = new TGraphErrors(nruns, rn,v2_300, xerr, yerr);

  gr->SetTitle("Bias scans runs 161-170 : V2Max:Run");
  gr->SetMarkerStyle(20);
  gr->SetMaximum(1.);
  gr->SetMinimum(-0.2);
  gr->Draw("AP");

  for (int i=0; i<nruns; i++) {
    int run_number = rn[i];
    TRunResults_t* res = &rr[run_number];
    xerr  [i] = 0;
    v2_400[i] = res->fV2400.fMean;
    yerr  [i] = res->fV2400.fMeanErr;
  }

  TGraphErrors* gr2 = new TGraphErrors(nruns, rn,v2_400,xerr,yerr);

  gr2->SetMarkerStyle(24);
  gr2->SetMarkerColor(kRed+2);
  gr2->Draw("p,same");

  gr2->SetMarkerStyle(21);
  gr2->SetMarkerColor(kRed+2);
  gr2->Draw("p,same");

  // print legend
  TBox* b = new TBox(160.9,0.1,167.1,0.2);
  b->SetFillStyle(3004);
  b->SetFillColor(kRed+2);
  b->Draw();

  TLatex* lat = new TLatex(162.9,0.13,"N1801-DEV9-D5-050");
  lat->SetTextSize(0.03);
  lat->Draw();

  TBox* b1 = new TBox(167.9,0.1,172.1,0.2);
  b1->SetFillStyle(3005);
  b1->SetFillColor(kRed+2);
  b1->Draw();

  TLatex* lat1 = new TLatex(168.1,0.13,"N1801-DEV9-D2-050");
  lat1->SetTextSize(0.03);
  lat1->Draw();
}


//-----------------------------------------------------------------------------
// plot v2max vs run
//-----------------------------------------------------------------------------
void plot_rates_vs_run_161_170() {

  TRunResults_t rr[1000];

  for (int i=0; i<nruns; i++) {
    int run_number = rn[i];
    make_run_results(run_number,&rr[run_number]);
  }

  float xerr[nruns], yerr_300[nruns], yerr_400[nruns], rate_300[nruns], rate_400[nruns];

  for (int i=0; i<nruns; i++) {
    int run_number = rn[i];
    TRunResults_t* res = &rr[run_number];

    xerr[i]     = 0;
    rate_300[i] = res->fQ1300.fNEvents/run_time[i];
    yerr_300[i] = sqrt(res->fQ1300.fNEvents)/run_time[i];
    
    rate_400[i] = res->fQ1400.fNEvents/run_time[i];
    yerr_400[i] = sqrt(res->fQ1400.fNEvents)/run_time[i];
  }

  TCanvas* c_rr = new TCanvas("c_rates_161_170","rates: runs 161-170",1400,700);
  
  TGraphErrors* gr = new TGraphErrors(nruns, rn, rate_300, xerr, yerr_300);
  
  gr->SetTitle("Bias scans: Rate vs Run");
  gr->SetMarkerStyle(20);
  gr->SetMinimum(-0.5);
  gr->SetMaximum(2.5);
  gr->Draw("AP");
  
  TGraphErrors* gr2 = new TGraphErrors(nruns, rn,rate_400,xerr,yerr_400);

  gr2->SetMarkerStyle(21);
  gr2->SetMarkerColor(kRed+2);
  gr2->Draw("p,same");

  TBox* b = new TBox(160.9,-0.2,167.1,0.);
  b->SetFillStyle(3004);
  b->SetFillColor(kRed+2);
  b->Draw();

  TLatex* lat = new TLatex(162.9,-0.15,"N1801-DEV9-D5-050");
  lat->SetTextSize(0.03);
  lat->Draw();

  TBox* b1 = new TBox(167.9,-0.2,172.1,0.);
  b1->SetFillStyle(3005);
  b1->SetFillColor(kRed+2);
  b1->Draw();

  TLatex* lat1 = new TLatex(168.1,-0.15,"N1801-DEV9-D2-050");
  lat1->SetTextSize(0.03);
  lat1->Draw();
}

//-----------------------------------------------------------------------------
// plot v2max vs run
//-----------------------------------------------------------------------------
void plot_rate_ratio_vs_run_161_170() {

  TRunResults_t rr[1000];

  for (int i=0; i<nruns; i++) {
    int run_number = rn[i];
    make_run_results(run_number,&rr[run_number]);
  }

  float xerr[nruns], yerr[nruns], rratio[nruns];

   for (int i=0; i<nruns; i++) {
    int run_number = rn[i];
    TRunResults_t* res = &rr[run_number];

    xerr[i] = 0;
     if (res->fQ1400.fMeanErr > 0) {
       rratio[i] = res->fQ1300.fNEvents/res->fQ1400.fNEvents;
       yerr   [i] = rratio[i]*sqrt(1/res->fQ1300.fNEvents + 1./res->fQ1300.fNEvents);
     }
     else {
   					// just to avoid crashes
       rratio[i] = -1.;
       yerr  [i] =  0;
     }
   }

   TCanvas* c_rr = new TCanvas("c_rr_161_170","rate ratio",1400,700);
  
   TGraphErrors* gr = new TGraphErrors(nruns, rn, rratio, xerr, yerr);

   gr->SetTitle("Bias Scan Rate ratio vs Run, 161-170");
   gr->SetMarkerStyle(20);
   gr->SetMinimum(-1.1);
   gr->SetMaximum(15);
   gr->Draw("AP");

  // print legend
  TBox* b = new TBox(160.9,-0.4,167.1,-0.);
  b->SetFillStyle(3004);
  b->SetFillColor(kRed+2);
  b->Draw();

  TLatex* lat = new TLatex(162.9,-0.20,"N1801-DEV9-D5-050");
  lat->SetTextSize(0.03);
  lat->Draw();

  TBox* b1 = new TBox(167.9,-0.4,172.1,-0.);
  b1->SetFillStyle(3005);
  b1->SetFillColor(kRed+2);
  b1->Draw();

  TLatex* lat1 = new TLatex(168.1,-0.2,"N1801-DEV9-D2-050");
  lat1->SetTextSize(0.03);
  lat1->Draw();
}


//-----------------------------------------------------------------------------
void plot_baseline_vs_run_161_170() {

  TRunResults_t rr[1000];

  for (int i=0; i<nruns; i++) {
    int run_number = rn[i];
    make_run_results(run_number,&rr[run_number]);
  }

  float xerr[nruns], yerr[nruns], ped[nruns];

  for (int i=0; i<nruns; i++) {
    int run_number     = rn[i];
    TRunResults_t* res = &rr[run_number];
    
    xerr[i] = 0;
    if (res->fPed300.fMeanErr > 0) {
      ped [i] = res->fPed300.fMean;
      yerr[i] = res->fPed300.fMeanErr;
    }
    else {
					// just to avoid crashes
      ped [i] = -1.;
      yerr[i] =  0;
    }
  }

  TCanvas* c_rr = new TCanvas("c_ped_161_170","baseline (pedestal)",1400,700);
  
  TGraphErrors* gr = new TGraphErrors(nruns, rn, ped, xerr, yerr);

  gr->SetTitle("Baseline position vs Run, 161-170");
  gr->SetMarkerStyle(20);
  gr->Draw("AP");

  for (int i=0; i<nruns; i++) {
    int run_number     = rn[i];
    TRunResults_t* res = &rr[run_number];
    
    xerr[i] = 0;
    if (res->fPed400.fMeanErr > 0) {
      ped [i] = res->fPed400.fMean;
      yerr[i] = res->fPed400.fMeanErr;
    }
    else {
					// just to avoid crashes
      ped [i] = -1.;
      yerr[i] =  0;
    }
  }

  TGraphErrors* gr2 = new TGraphErrors(nruns, rn, ped, xerr, yerr);

  gr2->SetMarkerStyle(24);
  gr2->SetMarkerColor(kRed+2);
  gr2->Draw("same,P");
  
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
void plot_ped_width_vs_run_161_170() {

  TRunResults_t rr[1000];

  for (int i=0; i<nruns; i++) {
    int run_number = rn[i];
    make_run_results(run_number,&rr[run_number]);
  }

  float xerr[nruns], yerr[nruns], ped[nruns];

  for (int i=0; i<nruns; i++) {
    int run_number     = rn[i];
    TRunResults_t* res = &rr[run_number];
    
    xerr[i] = 0;
    if (res->fPed300.fSigmErr > 0) {
      ped [i] = res->fPed300.fSigm;
      yerr[i] = res->fPed300.fSigmErr;
    }
    else {
					// just to avoid crashes
      ped [i] = -1.;
      yerr[i] =  0;
    }
  }

  TCanvas* c_rr = new TCanvas("c_pw_161_170","pedestal distribution width",1400,700);
  
  TGraphErrors* gr = new TGraphErrors(nruns, rn, ped, xerr, yerr);

  gr->SetTitle("Pedestal Width vs Run, 161-170");
  gr->SetMarkerStyle(20);
  gr->Draw("AP");

  for (int i=0; i<nruns; i++) {
    int run_number     = rn[i];
    TRunResults_t* res = &rr[run_number];
    
    xerr[i] = 0;
    if (res->fPed400.fSigmErr > 0) {
      ped [i] = res->fPed400.fSigm;
      yerr[i] = res->fPed400.fSigmErr;
    }
    else {
					// just to avoid crashes
      ped [i] = -1.;
      yerr[i] =  0;
    }
  }

  TGraphErrors* gr2 = new TGraphErrors(nruns, rn, ped, xerr, yerr);

  gr2->SetMarkerStyle(24);
  gr2->SetMarkerSize (2);
  gr2->SetMarkerColor(kRed+2);
  gr2->Draw("same,P");
  
  // TLatex* txt = new TLatex(191,1.5,"V_{thr} = -1.4 mV");
  // txt->SetTextSize(0.035);
  // txt->Draw();

  // TLatex* txt1 = new TLatex(194.5,1.,"V_{thr} = -1.7 mV");
  // txt1->SetTextSize(0.035);
  // txt1->Draw();

  // TLatex* txt2 = new TLatex(195.5,1.2,"V_{thr} = -2.0 mV");
  // txt2->SetTextSize(0.035);
  // txt2->Draw();
}

//-----------------------------------------------------------------------------
void plot_noise_vs_run_161_170() {

  TRunResults_t rr[1000];

  for (int i=0; i<nruns; i++) {
    int run_number = rn[i];
    make_run_results(run_number,&rr[run_number]);
  }

  float xerr[nruns], yerr[nruns], ped[nruns];

  for (int i=0; i<nruns; i++) {
    int run_number     = rn[i];
    TRunResults_t* res = &rr[run_number];
    
    xerr[i] = 0;
    ped [i] = res->fFitSigm300[0];
    yerr[i] = res->fFitSigm300[0]/sqrt(res->fPed300.fNEvents+1.e-12);
  }

  TCanvas* c_rr = new TCanvas("c_ped_161_170","noise",1400,700);
  
  TGraphErrors* gr = new TGraphErrors(nruns, rn, ped, xerr, yerr);

  gr->SetTitle("Noise vs Run, 161-170");
  gr->SetMarkerStyle(20);
  gr->SetMinimum(-1.1);
  gr->Draw("AP");

  for (int i=0; i<nruns; i++) {
    int run_number     = rn[i];
    TRunResults_t* res = &rr[run_number];
    
    xerr[i] = 0;
    ped [i] = res->fFitSigm400[0];
    yerr[i] = res->fFitSigm400[0]/sqrt(res->fPed400.fNEvents+1.e-12);
  }

  TGraphErrors* gr2 = new TGraphErrors(nruns, rn, ped, xerr, yerr);

  gr2->SetMarkerStyle(24);
  gr2->SetMarkerSize (2);
  gr2->SetMarkerColor(kRed+2);
  gr2->Draw("same,P");
  
  // TLatex* txt = new TLatex(191,1.5,"V_{thr} = -1.4 mV");
  // txt->SetTextSize(0.035);
  // txt->Draw();

  // TLatex* txt1 = new TLatex(194.5,1.,"V_{thr} = -1.7 mV");
  // txt1->SetTextSize(0.035);
  // txt1->Draw();

  // TLatex* txt2 = new TLatex(195.5,1.2,"V_{thr} = -2.0 mV");
  // txt2->SetTextSize(0.035);
  // txt2->Draw();
}
