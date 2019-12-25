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
int   const nruns       = 4;

  // run number and the source offset (in mm)
  
float rn           [nruns] = {  173  ,   174 ,   175 ,   176 };


// float dx           [nruns] = {  2.54 ,   2.54,  2.54 ,  2.54 ,  2.54 ,  2.54 ,   2.54,   0.  ,   0.  ,   0.  };

//   // running rime

float run_time     [nruns] = { 3311.,    3324.,  733. ,  1378};

//-----------------------------------------------------------------------------
void plot_q1_vs_run() {

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

  TCanvas* c = new TCanvas("c_q1",Form("charge: runs %4.0f -%4.0f",rn[0],rn[nruns-1]),1400,700);

  TGraphErrors* gr = new TGraphErrors(nruns, rn,q1_300,xerr,yerr);

  gr->SetTitle("Bias scan of N1801-DEV9-D3-050, no _400");
  gr->SetMarkerStyle(20);
  gr->SetMaximum(15.);
  gr->SetMinimum( 5.);
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
void plot_v2max_vs_run() {

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

  TCanvas* c = new TCanvas("c_v2max",Form("pulse height v2: runs %4.0f-%4.0f",rn[0],rn[nruns-1]),1400,700);

  TGraphErrors* gr = new TGraphErrors(nruns, rn,v2_300, xerr, yerr);

  gr->SetTitle(Form("Threshold scan,V2max vs run, runs %4.0f-%4.0f",rn[0],rn[nruns-1]));
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
void plot_rates_vs_run() {

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

  TCanvas* c = new TCanvas("c_rates",Form("rates: runs %4.0f-%4.0f",rn[0],rn[nruns-1]),1400,700);
  
  TGraphErrors* gr = new TGraphErrors(nruns, rn, rate_300, xerr, yerr_300);
  
  gr->SetTitle("Threshold scans: Rate vs Run");
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
void plot_rate_ratio_vs_run() {

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
    if ((res->fQ1400.fMeanErr > 0) && (res->fQ1300.fMeanErr > 0) ) {
      rratio[i] = res->fQ1300.fNEvents/res->fQ1400.fNEvents;
      yerr   [i] = rratio[i]*sqrt(1/res->fQ1300.fNEvents + 1./res->fQ1300.fNEvents);
    }
    else {
   					// just to avoid crashes
      rratio[i] = -1.;
      yerr  [i] =  0;
    }
  }
  
  TCanvas* c = new TCanvas("c_rr",Form("rate ratio: runs %4.0f-%4.0f",rn[0],rn[nruns-1]),1400,700);
  
  TGraphErrors* gr = new TGraphErrors(nruns, rn, rratio, xerr, yerr);
  
  gr->SetTitle(Form("Threshold Scan Rate ratio vs Run, runs %4.0f-%4.0f",rn[0],rn[nruns-1]));
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
void plot_baseline_vs_run() {

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

  TCanvas* c = new TCanvas("c_ped",Form("charge: runs %4.0f-%4.0f",rn[0],rn[nruns-1]),1400,700);
  
  TGraphErrors* gr = new TGraphErrors(nruns, rn, ped, xerr, yerr);

  gr->SetTitle(Form("Baseline position vs Run, %4.0f - %4.0f",rn[0],rn[nruns-1]));
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
}


//-----------------------------------------------------------------------------
void plot_ped_width_vs_run() {

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

  TCanvas* c = new TCanvas("c_pw",Form("ped width: runs %4.0f-%4.0f",rn[0],rn[nruns-1]),1400,700);
  
  TGraphErrors* gr = new TGraphErrors(nruns, rn, ped, xerr, yerr);

  gr->SetTitle(Form("Pedestal Width vs Run, %4.0f-%4.0f",rn[0],rn[nruns-1]));
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
void plot_noise_vs_run() {
  
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
  
  TCanvas* c_rr = new TCanvas("c_noise","noise",1400,700);
  
  TGraphErrors* gr = new TGraphErrors(nruns, rn, ped, xerr, yerr);

  gr->SetTitle(Form("Noise vs Run, runs %4.0f - %4.0f",rn[0],rn[nruns-1]));
	       
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
  }
