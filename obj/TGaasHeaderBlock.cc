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

  TStnHeaderBlock::Streamer(R__b);
}

//------------------------------------------------------------------------------
TGaasHeaderBlock::TGaasHeaderBlock(): TStnHeaderBlock() {
}


//_____________________________________________________________________________
TGaasHeaderBlock::~TGaasHeaderBlock() {
}

