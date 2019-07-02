#ifndef __TScopeEvent_hh__
#define __TScopeEvent_hh__


#include "TString.h"


struct TScopeEvent {

  enum { kMaxNChannels = 4 } ;
					// header
  int      fVersion;			// 0:Tektronix  1:Agilent  (may be)
  int      fEventNumber;
  int      fSubrunNumber;
  int      fRunNumber;
  int      fMcFlag;
  int      fGoodRun;
  int      fBrCode;
  int      fGoodTrig;
  int      fTrigWord;
  int      fCpu;
  TString  fStnVersion;

  int      fNChannels;
  int      fNSamples;
  int      fTriggerSample;
  int      fChannelID[kMaxNChannels];
  float    fSampleTime;
  float    fTriggerTime;
  TString  fRunStartTime;
  TString  fRunEndTime;
  int      fUSec;
  int      fEpoch;
  int      fPSec;
  float    fDeltaT;
  
  float    fT[5000];			// this is a shortcut...
  float    fV[kMaxNChannels][5000];		//

  float    fVSlp [kMaxNChannels];
  float    fVOff [kMaxNChannels];
  float    fVOff2[kMaxNChannels];
};


#endif

