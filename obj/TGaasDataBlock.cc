///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "gaasAna/obj/TGaasDataBlock.hh"

ClassImp(TGaasDataBlock)

//-----------------------------------------------------------------------------
void TGaasDataBlock::Streamer(TBuffer &R__b) {

  // Stream an object of class TGaasDataBlock
  // current version: V1

  int nwi     = ((Int_t*  )&fSampleTime)-&fNChannels;
  int nwf     = ((Float_t*)&fT         )-&fSampleTime;

  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(); if (R__v) { }
    
    R__b.ReadFastArray(&fNChannels,nwi);
    R__b.ReadFastArray(&fSampleTime,nwf);
    fT.Streamer(R__b);
    fV.Streamer(R__b);
  } 
  else {
    R__b.WriteVersion(TGaasDataBlock::IsA());

    R__b.WriteFastArray(&fNChannels ,nwi);
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
  printf("   ns      stns    nCh    trigS  ttime \n");
  printf(" ---------------------------- \n");
  float ttns  = fTriggerTime*1e9;
  float stns  = fSampleTime*1e9;
  
  printf(" %4i %12.5f %3i %4i %12.5f\n",fNSamples,stns,fNChannels,fTriggerSample,ttns);
//-----------------------------------------------------------------------------
// print times
//-----------------------------------------------------------------------------
  printf("------ times: \n");
  
  int nw = 0;
  for (int i=0; i<fNSamples; i++) {
    printf(" %7.3f",fT(i)*1.e9);
    nw++;
    if (nw >= 20) {
      printf("\n");
      nw = 0;
    }
  }

  if (nw > 0) printf("\n");
//-----------------------------------------------------------------------------
// print waveforms - so far - just one, the first one
//-----------------------------------------------------------------------------
  for (int ich=0; ich<fNChannels; ich++) {
    printf("------ waveform in CH%i: \n",ich);
    nw = 0;
    for (int i=0; i<fNSamples; i++) {
      printf(" %7.3f",fV(ich,i)*1.e3);
      nw++;
      if (nw >= 20) {
	printf("\n");
	nw = 0;
      }
    }

    if (nw > 0) printf("\n");
  }
}
