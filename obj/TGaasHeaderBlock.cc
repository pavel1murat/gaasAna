///////////////////////////////////////////////////////////////////////////////
//  always written out in split mode, so Streamer should never be called
///////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation
#endif

#include <iostream>

#include "Stntuple/obj/TStnEvent.hh"
#include "gaasAna/obj/TGaasHeaderBlock.hh"

ClassImp(TGaasHeaderBlock)


//______________________________________________________________________________
void TGaasHeaderBlock::Streamer(TBuffer &R__b)
{
   // Stream an object of class TGaasHeaderBlock: should never be called

  int nwi     = ((Int_t*  )fFloat      )-&fVersion;
  int nwf     = ((Float_t*)&fStnVersion)-fFloat;

  if (R__b.IsReading()) {
    //    Version_t R__v = R__b.ReadVersion(); 
    R__b.ReadVersion(); 
//-----------------------------------------------------------------------------
//  current version : 1
//-----------------------------------------------------------------------------
    R__b.ReadFastArray(&fVersion,nwi);
    R__b.ReadFastArray(fFloat   ,nwf);
    R__b >> fStnVersion;
  } 
  else {
    R__b.WriteVersion(TGaasHeaderBlock::IsA());
    R__b.WriteFastArray(&fVersion,nwi);
    R__b.WriteFastArray(fFloat   ,nwf);
    R__b << fStnVersion;
  }
}

//------------------------------------------------------------------------------
TGaasHeaderBlock::TGaasHeaderBlock(): TStnDataBlock(), 
  fEventNumber     (-1),
  fRunNumber       (-1),
  fSubrunNumber    (-1),
  fStnVersion      ("")
{
  fVersion      = -1;
  fMcFlag       =  0;
  fGoodRun      =  0;
  fBrCode       =  0;
  fGoodTrig     =  0;
  fTrigWord     =  0;
  fCpu          =  0;

  for (int i=0; i<kNFreeInts  ; i++) fInt  [i] = 0;
  for (int i=0; i<kNFreeFloats; i++) fFloat[i] = 0;
}


//_____________________________________________________________________________
TGaasHeaderBlock::~TGaasHeaderBlock() {
}


//_____________________________________________________________________________
void TGaasHeaderBlock::Clear(Option_t* opt) {
  fLinksInitialized = 0;
}


//_____________________________________________________________________________
void TGaasHeaderBlock::Print(Option_t* opt) const {
  // don't print header for the same event 2 times in a raw
  static char f_last_opt[1000] = {0};

  TGaasHeaderBlock* block = (TGaasHeaderBlock*) this;

  if ((f_EventNumber       == fEventNumber ) && 
      (f_RunNumber         == fRunNumber   ) &&
      (f_SubrunNumber      == fSubrunNumber) &&
      (strcmp(opt,f_last_opt) == 0            )    ) return;

  block->f_EventNumber  = fEventNumber;
  block->f_SubrunNumber = fSubrunNumber;
  block->f_RunNumber    = fRunNumber;
  strncpy(f_last_opt,opt,1000);

  printf(" *** Run,Subrun,Event: %6i,%-9i,%7i : %s\n", 
	 fRunNumber,fEventNumber,fSubrunNumber,opt);
}
