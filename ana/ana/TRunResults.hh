//
#ifndef __GaasAna_TRunResults_hh__
#define __GaasAna_TRunResults_hh__

struct TRunResults_t {
  int   fRunNumber;
  int   fDx;                   // horisontal source offset vrt photodiode
  int   fDz;
  float fTime;                 // data aquisition time
  float fQ1_300;               
  float fQ1_300_Err;               
  float fV2Max_300;               
  float fV2Max_300_Err;               
  float fQ1_400;               
  float fQ1_400_Err;               
  float fV2Max_400;               
  float fV2Max_400_Err;
  float fPed;                  // <ped> - from the gaussian fit
  float fPedSigma;             // sigma of the distribution in ped, from the fit
  float fFitSigma ;            // mean of the distribution in the ped fit sigma
};
#endif
