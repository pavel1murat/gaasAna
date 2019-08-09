//
#include "Stntuple/val/stntuple_val_functions.hh"

int overlay(const char* Filename, const char* Name1, const char* Name2) {
  TProfile* h300 = (TProfile*) gh2(Filename,"GaasReco","chn_300/wf2")->ProfileX()->Clone(Name1);
  TProfile* h400 = (TProfile*) gh2(Filename,"GaasReco","chn_400/wf2")->ProfileX()->Clone(Name2);


  h400->SetMarkerStyle(20);
  h400->SetMarkerSize (0.7);
  h400->SetMarkerColor(kRed+2);
  h400->Draw();

  h300->SetMarkerStyle(20);
  h300->SetMarkerSize (0.7);
  h300->SetMarkerColor(kBlue+2);
  h300->Draw("sames");

  return 0;
}
