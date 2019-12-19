//
#ifndef __TGaasRunResults_hh__
#define __TGaasRunResults_hh__

struct TChannelFitResults {
  int   fNumber;                // channel number
  float fN;			// number of events in the histogram
  float fQ1   [4];              // fit results: <q1>   , <q1>    err, sigma, sigma err
  float fV2Max[4];              // fit results: <v2max>, <v2max> err, sigma, sigma err
};

struct TGaasRunResults {
  int   fRunNumber;
  float fDx;
  float fDz;
  float fChn_300;			// low-amplitude pulses
  float fChn_400;			// high-amplitude pulses
};

#endif
