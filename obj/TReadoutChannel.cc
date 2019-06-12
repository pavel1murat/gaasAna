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
  fNptPed      = -1;
  fChi2Ped     = -999.;
  fV1Max       = -999.;
  fI1Max       = -1;
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
  fNptPed      = -1;
  fChi2Ped     = -999.;
  fV1Max       = -999.;
  fI1Max       = -1;
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
  printf(" ChID   NS Used     fQ      fQ1      Np1 fPed      sigP      fV1Max  fI1Max   fT0     fVMinP1   fVMaxP1   fVMinP2   fVMaxP2\n");
  printf("-------------------------------------------------------------------------------------------------------------\n");

  float sig = sqrt(fChi2Ped*(fNptPed-1)/fNptPed);
  
  printf(" %4i %4i    %1i %8.3f %8.3f %4i %9.6f %9.5f %9.5f  %5i  %7.3f %9.5f %9.5f %9.5f %9.5f\n",
	 fID, fNSamples,fUsed,fQ*1.e3,fQ1*1.e3,
    fNptPed,fPedestal*1e3,sig*1.e3,fV1Max*1.e3,fI1Max,fT0,fPar[0]*1e3,fPar[1]*1.e3,fPar[2]*1.e3,fPar[3]*1.e3);
}
