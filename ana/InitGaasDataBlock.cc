///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

#include "gaasAna/ana/InitGaasDataBlock.hh"


//-----------------------------------------------------------------------------
// in this case AbsEvent is just not used
//-----------------------------------------------------------------------------
int InitGaasDataBlock::InitDataBlock(TStnDataBlock* Block, AbsEvent* Event, int Mode) {

  if (Block->Initialized(fScopeEvent->fEventNumber,fScopeEvent->fRunNumber)) return 0;
  
  TGaasDataBlock* block = (TGaasDataBlock*) Block;

  block->fNChannels     = fScopeEvent->fNChannels;
  block->fNSamples      = fScopeEvent->fNSamples;
  block->fTriggerSample = fScopeEvent->fTriggerSample;
  block->fSampleTime    = fScopeEvent->fSampleTime;
  block->fTriggerTime   = fScopeEvent->fTriggerTime;

  // fill times 

  block->fT.ResizeTo(block->fNSamples);
  block->fT.SetElements(fScopeEvent->fT);

  // fill voltages

  block->fV.ResizeTo(block->fNChannels,block->fNSamples);

  for (int i=0; i<block->fNChannels; i++) {
    for (int j=0; j<block->fNSamples; j++) {
      block->fV(i,j) = fScopeEvent->fV[i][j];
    }
  }

  // channel ID's

  block->fChannelID.Set(block->fNChannels);
 
  for (int i=0; i<block->fNChannels; i++) {
    block->fChannelID[i] = fScopeEvent->fChannelID[i];
  }

  block->f_EventNumber  = fScopeEvent->fEventNumber;
  block->f_RunNumber    = fScopeEvent->fRunNumber;
  block->f_SubrunNumber = fScopeEvent->fSubrunNumber;
  
  return 0;
}
