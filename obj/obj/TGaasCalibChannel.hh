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
  float           fMinQ          ;
  int             fPulseIntWindow; 
  float           fGain          ;
  float           fMinWidth      ;     // min width of a type#2 pulse
  float           fSamplingTime  ;     // sampling time

  TGaasCalibChannel() {}
  ~TGaasCalibChannel() {}

  int ID      () { return fID;     }
  int Used    () { return fUsed;     }
  int NSamples() { return fNSamples; }
  int Polarity() { return fPolarity; }

  float MinQ        () const { return fMinQ;         }
  float MinWidth    () const { return fMinWidth;     }
  float SamplingTime() const { return fSamplingTime; }

  ClassDef(TGaasCalibChannel,1)
};

#endif
