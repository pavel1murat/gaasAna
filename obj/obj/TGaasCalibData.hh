//-----------------------------------------------------------------------------
// calibrations follow H/W convention: strips=0 wire=1
//-----------------------------------------------------------------------------
#ifndef TGaasCalibData_hh
#define TGaasCalibData_hh

#include "Stntuple/base/TCalibData.hh"
#include "gaasAna/obj/TGaasCalibChannel.hh"

class TCalibManager;
class TCalibRunRange;

class TGaasCalibData : public TCalibData {
  enum { kMaxNChannels = 4 } ;
  
public:
  int               fNChannels;              // number of active (used) channels
  TGaasCalibChannel fChannel[kMaxNChannels];
  int               fRunNumber;

  TCalibRunRange* fCalibRunRange;            //! non-persistent
//-----------------------------------------------------------------------------
// constructors and destructor
//-----------------------------------------------------------------------------
public:
  TGaasCalibData();
  virtual ~TGaasCalibData();
//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------
  int Init                (int RunNumber, TCalibManager* Manager);

  int InitReadoutMap      (int RunNumber, TCalibManager* Manager);
  //  int InitChannelGains    (int RunNumber, TCalibManager* Manager);

  int   GetNChannels() { return fNChannels; }
  TGaasCalibChannel*   GetChannel(int I) { return &fChannel[I]; }
  
  // float  GetPedestal   (int Channel) { return fPedestal   [Channel]; }
  // float  GetPedSigma   (int Channel) { return fPedSigma   [Channel]; }
  //  float  GetChannelGain(int Channel) { return fChannelGain[Channel]; }
  //  float  GetPulseIntegrationWindow(int Channel) { return fPulseIntegrationWindow[Channel]; }

//   int    GetChannelID(int Channel) { 
//     return (fModule[Channel]<<16) + (fIX[Channel]<<8) + fIY[Channel];
//   }

//  int    GetUsed  (int Channel) { return fUsed  [Channel]; }
//-----------------------------------------------------------------------------
// overloaded methods of TCalibData
//-----------------------------------------------------------------------------
  virtual int Init(TObject* Data) { return 0; }
//-----------------------------------------------------------------------------
// overloaded methods of TObject
//-----------------------------------------------------------------------------
  void Clear(Option_t* Option = "");
  void Print(Option_t* Option = "") const ;

  ClassDef (TGaasCalibData,1)
};

#endif
