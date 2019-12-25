#include "GaasAna/ana/TRunResults.hh"



TRunResults_t    data[1000];

//-----------------------------------------------------------------------------
void init_run_results() {
  
  TRunResults_t* x = new TRunResults_t();

  x->fRunNumber  = 139;
  x->fDx         = 0;
  x->fDz         = 0;
  x->fTime       = 1007.;
  x->fNev_300    = 1499 ;
  x->fNev_400    = 228  ;
  
  x->fQ1_300     = 10.7;
  x->fQ1_300_err = 0.04;
  x->fQ1_400     = 9.34;
  x->fQ1_400_err = 0.16;

  x->fV2Max_300  = 0.19;
  x->fV2Max_300_err  = 0.0004;

  x->fV2Max_400      = 0.35;
  x->fV2Max_400_err  = 0.005;

  x->fPed_100        = -0.289;    // mV
  x->fPedSigma_100   = 1.08;    // mV
  x->fFitSigma_100   = 2.57;     // mV

  x->fPed_300        = -0.31;
  x->fPedSigma_300   = 1.06;
  x->fFitSigMean_300 = 2.56;

  x->fPed_400        = -0.23;
  x->fPedSigma_400   = 1.16;
  x->fFitSigMean_400 = 2.64;
}
