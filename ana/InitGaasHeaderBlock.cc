///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

#include "gaasAna/ana/InitGaasHeaderBlock.hh"


//-----------------------------------------------------------------------------
// in this case AbsEvent is just not used
//-----------------------------------------------------------------------------
int InitGaasHeaderBlock::InitDataBlock(TStnDataBlock* Block, AbsEvent* Event, int Mode) {

  if (Block->Initialized(fScopeEvent->fEventNumber,fScopeEvent->fRunNumber)) return 0;
  TGaasHeaderBlock* block = (TGaasHeaderBlock*) Block;

  // don't have to clear - TStnEvent::Init() calls Clear() for each data block
  //  block->Clear();

  block->fVersion       = fScopeEvent->fVersion;
  block->fEventNumber   = fScopeEvent->fEventNumber;
  block->fRunNumber     = fScopeEvent->fRunNumber;
  block->fSectionNumber = fScopeEvent->fSubrunNumber;
  block->fMcFlag        = fScopeEvent->fMcFlag;
  block->fGoodRun       = fScopeEvent->fGoodRun;
  block->fBrCode        = fScopeEvent->fBrCode;
  block->fGoodTrig      = fScopeEvent->fGoodTrig;
  block->fTrigWord      = fScopeEvent->fTrigWord;
  block->fCpu           = fScopeEvent->fCpu;
  block->fStnVersion    = fScopeEvent->fStnVersion;

  block->f_EventNumber  = fScopeEvent->fEventNumber;
  block->f_RunNumber    = fScopeEvent->fRunNumber;
  block->f_SubrunNumber = fScopeEvent->fSubrunNumber;
  
  return 0;
}
