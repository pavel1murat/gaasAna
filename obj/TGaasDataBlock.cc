///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "gaasAna/obj/TGaasDataBlock.hh"

ClassImp(TGaasDataBlock)

//-----------------------------------------------------------------------------
void TGaasDataBlock::Streamer(TBuffer &R__b) {

  // Stream an object of class TGaasDataBlock
  // current version: V1

  int nwi     = ((Int_t*  )&fSampleTime)-&fNSamples;
  int nwf     = ((Float_t*)&fT         )-&fSampleTime;

  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(); if (R__v) { }
    
    R__b.ReadFastArray(&fNSamples,nwi);
    R__b.ReadFastArray(&fSampleTime,nwf);
    fT.Streamer(R__b);
    fV.Streamer(R__b);
  } 
  else {
    R__b.WriteVersion(TGaasDataBlock::IsA());

    R__b.WriteFastArray(&fNSamples,nwi);
    R__b.WriteFastArray(&fSampleTime,nwf);
    fT.Streamer(R__b);
    fV.Streamer(R__b);
  }
}


//_____________________________________________________________________________
TGaasDataBlock::TGaasDataBlock(): TStnDataBlock() {
  fNSamples      = -1;
  fNChannels     = -1;
  fTriggerSample = -1;

  fSampleTime    = -1.;
  fTriggerTime   = -1.;
  
  for (int i=0; i<kNFreeInts  ; i++) fInt  [i] = 0;
  for (int i=0; i<kNFreeFloats; i++) fFloat[i] = 0;
}

//_____________________________________________________________________________
TGaasDataBlock::~TGaasDataBlock() {
}

//_____________________________________________________________________________
void  TGaasDataBlock::Clear(Option_t* opt)  {

  int nwi     = ((Int_t*  )&fSampleTime )-&fNSamples;
  int nwf     = ((Float_t*)&fT          )-&fSampleTime;

  memset(&fNSamples         ,0,nwi*sizeof(int)  );
  memset(&fSampleTime       ,0,nwf*sizeof(float));

  fT.Clear();
  fV.Clear();
}


//_____________________________________________________________________________
void  TGaasDataBlock::Print(Option_t* opt) const {
  printf(" TGaasDataBlock::Print ERROR: not implemented yet! \n");
}
