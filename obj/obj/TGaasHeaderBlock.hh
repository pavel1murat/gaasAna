///////////////////////////////////////////////////////////////////////////////
// different from TStnHeaderBlock only by initialization
///////////////////////////////////////////////////////////////////////////////
#ifndef TGaasHeaderBlock_hh
#define TGaasHeaderBlock_hh

#include "TString.h"
#include "Stntuple/obj/TStnHeaderBlock.hh"

class TGaasHeaderBlock: public TStnHeaderBlock {
protected:
  TString    fRunStartTime;
  TString    fRunEndTime;
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

  ClassDef(TGaasHeaderBlock,2)	// Event Header block for GaAs header
};

#endif
