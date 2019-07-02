///////////////////////////////////////////////////////////////////////////////
// different from TStnHeaderBlock only by initialization
///////////////////////////////////////////////////////////////////////////////
#ifndef TGaasHeaderBlock_hh
#define TGaasHeaderBlock_hh

#include "TString.h"
#include "Stntuple/obj/TStnHeaderBlock.hh"

class TGaasHeaderBlock: public TStnHeaderBlock {
  struct TGaasHeaderBlock_v2 {
    TString    fRunStartTime;
    TString    fRunEndTime;
  };
protected:
  TString    fRunStartTime;
  TString    fRunEndTime;
  int        fEpoch;        // event time in unix format [ added in V3 ]
  int        fUSec;	    // milliseconds*1000 and microseconds [ added in V3 ]
  int        fPSec;	    // nano-*1000 and pico-second parts of a timestamp [ added in V3 ]
  float      fDeltaT;       // time from the previous frame in the sequence
public:
  
  friend class InitGaasHeaderBlock;
//-----------------------------------------------------------------------------
//  functions
//-----------------------------------------------------------------------------
public:
//-----------------------------------------------------------------------------
// constructors and destructor
//-----------------------------------------------------------------------------
  TGaasHeaderBlock();
  virtual ~TGaasHeaderBlock();
//-----------------------------------------------------------------------------
// overloaded methods of TObject
//-----------------------------------------------------------------------------
  virtual void    Clear(Option_t* opt = "");
  virtual void    Print(Option_t* opt = "") const;
//-----------------------------------------------------------------------------
// schema evolution
//-----------------------------------------------------------------------------
  void ReadV1(TBuffer &R__b);
  void ReadV2(TBuffer &R__b);

  ClassDef(TGaasHeaderBlock,3)	// Event Header block for GaAs header
};

#endif
