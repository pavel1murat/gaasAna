///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "gaasAna/ana/TRunResults.hh"

//-----------------------------------------------------------------------------
int get_fit_gauss_results(TString* Fn, const char* HistName, int RebinFactor, TGFitResults_t* Res) {
  int rc(0);

  TString hn = Form("h_%s",HistName);
  hn.ReplaceAll("/","_");
  
  TH1F* h = (TH1F*) gh1(Fn->Data(),"GaasReco",HistName)->Clone(hn.Data());

  Res->fNEvents  = h->GetEntries();

  if (Res->fNEvents > 10) {
    if (RebinFactor != 1) h->Rebin(RebinFactor);
  
    h->Fit("gaus","Q");
    
    TF1* f = h->GetFunction("gaus");
    
    Res->fMean    = f->GetParameter(1);
    Res->fMeanErr = f->GetParError (1);
    Res->fSigm    = f->GetParameter(2);
    Res->fSigmErr = f->GetParError (2);
  }
  else {
    Res->fMean    = -0.1;
    Res->fMeanErr = -1.;
    Res->fSigm    = -0.1;
    Res->fSigmErr = -1.;
  }

  return rc;
}

//-----------------------------------------------------------------------------
int get_hist_mean(TString* Fn, const char* HistName, double* Mean) {
  int rc(0);

  TString hn = Form("h_%s",HistName);
  hn.ReplaceAll("/","_");
  
  TH1F* h = (TH1F*) gh1(Fn->Data(),"GaasReco",HistName)->Clone(hn.Data());

  int nev = h->GetEntries();

  if (nev > 0) {
    Mean[0] = h->GetMean();
    Mean[1] = h->GetStdDev()/(nev+1.e-12);
  }
  else {
    Mean[0] = -1.;
    Mean[1] = -1.;
  }

  return rc;
}


//-----------------------------------------------------------------------------
int make_run_results(int RunNumber, TRunResults_t* rr) {
  int rc(0);

  TString fn = Form("/projects/gaas/hist/runs/gaasqd_fnal.%06i_00000000.gaas_reco.hist",RunNumber);
//-----------------------------------------------------------------------------
// nq300 = na300, nq400 = na400
//-----------------------------------------------------------------------------
  rr->fRunNumber = RunNumber;
  rr->fDx        = -1.;
  rr->fDz        = -1.;
  rr->fTime      = -1.;
  
  int nq300, na300, nq400, na400, np300, np400;
  
  double q300[2], sq300[2], q400[2], sq400[2], a300[2], sa300[2], a400[2], sa400[2];
  double p300[2], sp300[2], p400[2], sp400[2], mp300[2], mp400[2] ;
  
  get_fit_gauss_results(&fn,"chn_300/q1_0" ,5,&rr->fQ1300);
  get_fit_gauss_results(&fn,"chn_300/v2max",5,&rr->fV2300);
  get_fit_gauss_results(&fn,"chn_400/q1_0" ,5,&rr->fQ1400);
  get_fit_gauss_results(&fn,"chn_400/v2max",5,&rr->fV2400);

  get_fit_gauss_results(&fn,"chn_300/ped_1",1,&rr->fPed300);
  get_fit_gauss_results(&fn,"chn_400/ped_1",1,&rr->fPed400);

  get_hist_mean(&fn,"chn_300/sigped_0",rr->fFitSigm300);
  get_hist_mean(&fn,"chn_400/sigped_0",rr->fFitSigm400);

  printf("%6i",RunNumber);
  printf("%6i %6.3f %6.3f %6.3f %6.3f %6i %6.3f %6.3f %6.3f %6.3f",
 	 rr->fQ1300.fNEvents, rr->fQ1300.fMean, rr->fQ1300.fMeanErr, rr->fQ1300.fSigm,rr->fQ1300.fSigmErr,  
 	 rr->fV2300.fNEvents, rr->fV2300.fMean, rr->fV2300.fMeanErr, rr->fV2300.fSigm,rr->fV2300.fSigmErr);
  
  printf("%6i %6.3f %6.3f %6.3f %6.3f %6i %6.3f %6.3f %6.3f %6.3f",
 	 rr->fQ1400.fNEvents, rr->fQ1400.fMean, rr->fQ1400.fMeanErr, rr->fQ1400.fSigm,rr->fQ1400.fSigmErr,  
 	 rr->fV2400.fNEvents, rr->fV2400.fMean, rr->fV2400.fMeanErr, rr->fV2400.fSigm,rr->fV2400.fSigmErr);
  
  printf("             %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f",
	 rr->fPed300.fMean, rr->fPed300.fMeanErr, rr->fPed300.fSigm, rr->fPed300.fSigmErr,
	 rr->fPed400.fMean, rr->fPed400.fMeanErr, rr->fPed400.fSigm, rr->fPed400.fSigmErr);

  printf("%6.3f %6.3f", rr->fFitSigm300[0], rr->fFitSigm400[0]);

  printf("\n");
  
  return rc;
}


void make_results(int RunNumber) {
  TRunResults_t  rr[1000];

  make_run_results(RunNumber,&rr[RunNumber]);
}
