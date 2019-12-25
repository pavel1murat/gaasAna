//
#ifndef __GaasAna_TRunResults_hh__
#define __GaasAna_TRunResults_hh__

struct TGFitResults_t {
  int    fNEvents;
  double fMean;
  double fMeanErr;
  double fSigm;
  double fSigmErr;
};

struct TRunResults_t {
  int            fRunNumber;
  float          fDx;         // horisontal source offset vrt photodiode
  float          fDz;
  float          fTime;       // data aquisition time

  TGFitResults_t fQ1300;
  TGFitResults_t fV2300;
  TGFitResults_t fQ1400;
  TGFitResults_t fV2400;

  TGFitResults_t fPed300;
  TGFitResults_t fPed400;

  double         fFitSigm300[2];
  double         fFitSigm400[2];
};
#endif
