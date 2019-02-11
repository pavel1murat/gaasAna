///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#ifndef __InitGaasHeaderBlock_hh__
#define __InitGaasHeaderBlock_hh__

#include "Stntuple/obj/TStnInitDataBlock.hh"
#include "gaasAna/obj/TGaasHeaderBlock.hh"

#include "gaasAna/ana/TScopeEvent.hh"

class InitGaasHeaderBlock : public TStnInitDataBlock {
public:
  const TScopeEvent* fScopeEvent;
//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
  virtual int InitDataBlock(TStnDataBlock* Block, AbsEvent* Evt, int Mode);

  void    SetScopeEvent(const TScopeEvent* ScopeEvent) { fScopeEvent = ScopeEvent; }
};

#endif
