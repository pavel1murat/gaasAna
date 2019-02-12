///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "gaasAna/obj/TReadoutChannel.hh"



ClassImp(TReadoutChannel)


//-----------------------------------------------------------------------------
TReadoutChannel::TReadoutChannel(int NSamples) {
  fNSamples = NSamples;
  if (fNSamples > 0) {
    fT.reserve(fNSamples);
    fV0.reserve(fNSamples);
    fV1.reserve(fNSamples);
  }
}


//-----------------------------------------------------------------------------
TReadoutChannel::~TReadoutChannel() {
}


//-----------------------------------------------------------------------------
void TReadoutChannel::Clear(Option_t* Opt) {
}

