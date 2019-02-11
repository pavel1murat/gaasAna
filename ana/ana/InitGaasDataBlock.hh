///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#ifndef __InitGaasDataBlock
#define __InitGaasDataBlock

#include "Stntuple/obj/TStnInitDataBlock.hh"
#include "gaasAna/obj/TGaasDataBlock.hh"
#include "gaasAna/ana/TScopeEvent.hh"

class InitGaasDataBlock : public TStnInitDataBlock {
public:
  const TScopeEvent* fScopeEvent;
//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
public:
  
  virtual int InitDataBlock(TStnDataBlock* Block, AbsEvent* Evt, int Mode);

  void    SetScopeEvent(const TScopeEvent* ScopeEvent) { fScopeEvent = ScopeEvent; }
};

#endif
