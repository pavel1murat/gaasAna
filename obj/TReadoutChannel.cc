///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "gaasAna/obj/TReadoutChannel.hh"

ClassImp(TReadoutChannel)

//-----------------------------------------------------------------------------
TReadoutChannel::TReadoutChannel() {
  fNSamples = -1;
}

//-----------------------------------------------------------------------------
TReadoutChannel::TReadoutChannel(int NSamples) {
  fNSamples = NSamples;
  fT.reserve(NSamples);
  fV0.reserve(NSamples);
  fV1.reserve(NSamples);
}

//-----------------------------------------------------------------------------
TReadoutChannel::~TReadoutChannel() {
}

//-----------------------------------------------------------------------------
void TReadoutChannel::Clear(Option_t* Opt) {
}
