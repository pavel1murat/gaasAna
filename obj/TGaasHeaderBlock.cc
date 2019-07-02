///////////////////////////////////////////////////////////////////////////////
//  always written out in split mode, so Streamer should never be called
///////////////////////////////////////////////////////////////////////////////
// #ifdef __GNUG__
// #pragma implementation
// #endif

// #include <iostream>

#include "Stntuple/obj/TStnEvent.hh"
#include "gaasAna/obj/TGaasHeaderBlock.hh"

ClassImp(TGaasHeaderBlock)

//-----------------------------------------------------------------------------
void TGaasHeaderBlock::ReadV1(TBuffer &R__b) {
  TStnHeaderBlock::Streamer(R__b);

  fRunStartTime = "default";
  fRunEndTime   = "default";
}

//-----------------------------------------------------------------------------
void TGaasHeaderBlock::ReadV2(TBuffer &R__b) {
  TStnHeaderBlock::Streamer(R__b);
  R__b >> fRunStartTime;
  R__b >> fRunEndTime;
}

//______________________________________________________________________________
void TGaasHeaderBlock::Streamer(TBuffer &R__b)
{
   // Stream an object of class TGaasHeaderBlock: should never be called

  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion();
    if      (R__v == 1) ReadV1(R__b);
    else if (R__v == 2) ReadV2(R__b);
    else {
      TStnHeaderBlock::Streamer(R__b);
      R__b >> fRunStartTime;
      R__b >> fRunEndTime;
      R__b >> fEpoch;
      R__b >> fUSec;
      R__b >> fPSec;
      R__b >> fDeltaT;
    }
  }
  else {
    R__b.WriteVersion(TGaasHeaderBlock::IsA());
    TStnHeaderBlock::Streamer(R__b);
    R__b << fRunStartTime;
    R__b << fRunEndTime;
    R__b << fEpoch;
    R__b << fUSec;
    R__b << fPSec;
    R__b << fDeltaT;
  }
}

//------------------------------------------------------------------------------
TGaasHeaderBlock::TGaasHeaderBlock(): TStnHeaderBlock() {
}


//_____________________________________________________________________________
TGaasHeaderBlock::~TGaasHeaderBlock() {
}

//_____________________________________________________________________________
void TGaasHeaderBlock::Clear(Option_t* opt) {
}

//_____________________________________________________________________________
void TGaasHeaderBlock::Print(Option_t* opt) const {
  TStnHeaderBlock::Print(opt);
  printf(" Run start time: %s\n",fRunStartTime.Data());
  printf(" Run end   time: %s\n",fRunEndTime.Data());
}

