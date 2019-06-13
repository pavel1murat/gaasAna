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

//______________________________________________________________________________
void TGaasHeaderBlock::Streamer(TBuffer &R__b)
{
   // Stream an object of class TGaasHeaderBlock: should never be called

  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion();
    TStnHeaderBlock::Streamer(R__b);
    if (R__v == 1) {
      fRunStartTime = "default";
      fRunEndTime   = "default";
    }
    else if (R__v == 2) {
      R__b >> fRunStartTime;
      R__b >> fRunEndTime;
    }
  }
  else {
    R__b.WriteVersion(TGaasHeaderBlock::IsA());
    TStnHeaderBlock::Streamer(R__b);
    R__b << fRunStartTime;
    R__b << fRunEndTime;
  }
}

//------------------------------------------------------------------------------
TGaasHeaderBlock::TGaasHeaderBlock(): TStnHeaderBlock() {
}


//_____________________________________________________________________________
TGaasHeaderBlock::~TGaasHeaderBlock() {
}

