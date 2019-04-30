//-----------------------------------------------------------------------------
// calibrations follow H/W convention: strips=0 wire=1
//-----------------------------------------------------------------------------
#ifndef TGaasCalibChannel_hh
#define TGaasCalibChannel_hh

//-----------------------------------------------------------------------------
// Gaas reco module calib data
//-----------------------------------------------------------------------------
class TGaasCalibChannel: public TObject {
public:
  int             fID            ;
  int             fUsed          ;
  int             fNSamples      ;
  int             fPolarity      ;
  int             fMinSample[2]  ;
  int             fMaxSample[2]  ;
  float           fMaxP2P        ;	// to filter out the signal
  float           fMaxThr        ;
  int             fS1            ;
  int             fPulseIntWindow; 
  float           fGain          ;

  TGaasCalibChannel() {}
  ~TGaasCalibChannel() {}

  int ID      () { return fID;     }
  int Used    () { return fUsed;     }
  int NSamples() { return fNSamples; }
  int Polarity() { return fPolarity; }

  ClassDef(TGaasCalibChannel,1)
};

#endif
