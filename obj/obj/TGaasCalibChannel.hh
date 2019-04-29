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
  int             fChannelID     ;
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


  int GetNSamples() { return fNSamples; }
  int GetPolarity() { return fPolarity; }

  ClassDef(TGaasCalibChannel,1)
};

#endif
