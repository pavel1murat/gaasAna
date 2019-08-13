//
#include "Stntuple/val/stntuple_val_functions.hh"

int overlay(int RunNumber, int FirstBin, int LastBin, const char* Filename = 0) {

  TString name1 = Form("h_300_%05i",RunNumber);
  TString name2 = Form("h_400_%05i",RunNumber);

  char fn[200];

  const char* dir = "/mu2e/data/users/murat/gaas/hist/runs";

  if (Filename == 0) sprintf(fn,"%s/gaasqd_fnal.%06i_00000000.gaas_reco.hist",dir,RunNumber);
  else               strcpy(fn,Filename);

  TProfile* h300 = (TProfile*) gh2(fn,"GaasReco","chn_300/wf2")->ProfileX()->Clone(name1);
  TProfile* h400 = (TProfile*) gh2(fn,"GaasReco","chn_400/wf2")->ProfileX()->Clone(name2);

  h400->SetMarkerStyle(20);
  h400->SetMarkerSize (0.7);
  h400->SetMarkerColor(kRed+2);
  h400->GetYaxis()->SetTitle("V_{input}^{max}, mV");

  h300->SetMarkerStyle(20);
  h300->SetMarkerSize (0.7);
  h300->SetMarkerColor(kBlue+2);
//------------------------------------------------------------------------------
// subtract one from another one
// 1. determine the normalization factor
//-----------------------------------------------------------------------------
  TH1F* h1 = new TH1F("h1","h1",500,0,500);

  float anorm = h400->Integral(FirstBin,LastBin)/h300->Integral(FirstBin,LastBin);

  for (int i=0; i<500; i++) {
    float y = h400->GetBinContent(i+1)-h300->GetBinContent(i+1)*anorm;
    h1->SetBinContent(i,y);
  }


  TCanvas* cc = new TCanvas(Form("cc_run_%i",RunNumber),Form("Run %i",RunNumber),1400,1000);
  cc->Divide(2,2);

  cc->cd(1);

  h400->Draw();
  h300->Draw("sames");

  cc->cd(2);

  h400->Draw();
  h300->DrawNormalized("sames",h300->Integral()*anorm);

  cc->cd(3);

  gPad->SetGridy(1);
  h1->Draw();

  printf("h400 integral, h300 integral: %12.4f %12.4f\n",h400->Integral(),h300->Integral());

  printf("h1 int = %12.4f\n",h1->Integral(150,FirstBin-1));

  return 0;
}
