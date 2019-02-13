///////////////////////////////////////////////////////////////////////////////
// different from TStnHeaderBlock only by initialization
///////////////////////////////////////////////////////////////////////////////
#ifndef TGaasHeaderBlock_hh
#define TGaasHeaderBlock_hh

#include "Stntuple/obj/TStnHeaderBlock.hh"

class TGaasHeaderBlock: public TStnHeaderBlock {
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

  ClassDef(TGaasHeaderBlock,1)	// Event Header block for GaAs header
};

#endif
