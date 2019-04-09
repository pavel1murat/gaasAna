#ifndef __TScopeEvent_hh__
#define __TScopeEvent_hh__


#include "TString.h"


struct TScopeEvent {
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
  float    fSampleTime;
  float    fTriggerTime;
  
  float    fT[5000];			// this is a shortcut...
  float    fV[10][5000];		//

  float    fVOff[10];
  float    fVSlp[10];
  
};


#endif

