///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "TH1.h"
#include "gaasAna/obj/TReadoutChannel.hh"

ClassImp(TReadoutChannel)

//-----------------------------------------------------------------------------
TReadoutChannel::TReadoutChannel() {
  fID          = -1;
  fNSamples    = -1;
  fUsed        =  1;
  fQ           = -999.;
  fQ1          = -999.;
  fPedestal    = -999.;
  fV1Min       = -999.;
  fI1Min       = -1;
  fT0          = -999.;

  fHist.fV0    = nullptr;
  fHist.fV1    = nullptr;
  fHist.fShape = nullptr;
}

//-----------------------------------------------------------------------------
// assuming the channel ID is an integer number
//-----------------------------------------------------------------------------
TReadoutChannel::TReadoutChannel(int ID, int NSamples) {
  fID          = ID;
  fUsed        = 1 ;
  SetNSamples(NSamples);

  fQ           = -999.;
  fQ1          = -999.;
  fPedestal    = -999.;
  fV1Min       = -999.;
  fI1Min       = -1;
  fT0          = -999.;

  fHist.fV0    = new TH1F(Form("v0_%02i",fID),"V0"   ,fNSamples,0,fNSamples);
  fHist.fV1    = new TH1F(Form("v1_%02i",fID),"V1"   ,fNSamples,0,fNSamples);
  fHist.fShape = new TH1F(Form("sh_%02i",fID),"Shape",fNSamples,0,fNSamples);
}

//-----------------------------------------------------------------------------
TReadoutChannel::~TReadoutChannel() {
  delete fHist.fV0;
  delete fHist.fV1;
  delete fHist.fShape;
}

//-----------------------------------------------------------------------------
void TReadoutChannel::SetNSamples(int N) {
  fNSamples = N;
  
  fT .reserve(N);
  fV0.reserve(N);
  fV1.reserve(N);
}

//-----------------------------------------------------------------------------
void TReadoutChannel::Clear(Option_t* Opt) {
}

//-----------------------------------------------------------------------------
void TReadoutChannel::Print(Option_t* Opt) const {
  printf("-------------------------------------------------------------------------------------------------------------\n");
  printf(" ChID   NS Used     fQ      fQ1      fPed      fV1Min  fI1Min   fT0     fVMinP1   fVMaxP1   fVMinP2   fVMaxP2\n");
  printf("-------------------------------------------------------------------------------------------------------------\n");

  printf(" %4i %4i    %1i %8.3f %8.3f %9.6f %9.5f  %5i  %7.3f %9.5f %9.5f %9.5f %9.5f\n",
	 fID, fNSamples,fUsed,fQ*1.e3,fQ1*1.e3,fPedestal*1e3,fV1Min*1.e3,fI1Min,fT0,fPar[0]*1e3,fPar[1]*1.e3,fPar[2]*1.e3,fPar[3]*1.e3);
}
