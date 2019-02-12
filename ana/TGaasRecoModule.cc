//////////////////////////////////////////////////////////////////////////////
//
// use of tmp:
//
// use of debug bits: bits 0-2 are reserved
//  0  : all events
//  1  : passed events
//  2  : rejected events
//
// call: "track_comp(28,4)
///////////////////////////////////////////////////////////////////////////////
#include "TF1.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TEnv.h"
#include "TSystem.h"

#include "Stntuple/loop/TStnAna.hh"
#include "Stntuple/loop/TStnInputModule.hh"
#include "Stntuple/base/TStnDataset.hh"
#include "Stntuple/obj/TStnHeaderBlock.hh"
#include "Stntuple/obj/TStnNode.hh"
#include "Stntuple/alg/TStntuple.hh"
#include "Stntuple/val/stntuple_val_functions.hh"
//------------------------------------------------------------------------------
// Mu2e offline includes
//-----------------------------------------------------------------------------
#include "gaasAna/ana/TGaasRecoModule.hh"

ClassImp(TGaasRecoModule)
//-----------------------------------------------------------------------------
TGaasRecoModule::TGaasRecoModule(const char* name, const char* title): TStnModule(name,title)
{
  fFillHistograms    = 1;
}

//-----------------------------------------------------------------------------
TGaasRecoModule::~TGaasRecoModule() {
}


//-----------------------------------------------------------------------------
int TGaasRecoModule::BeginJob() {
//-----------------------------------------------------------------------------
// register data blocks
//-----------------------------------------------------------------------------
  RegisterDataBlock("GaasDataBlock", "TGaasDataBlock"     ,&fGaasDataBlock);
//-----------------------------------------------------------------------------
// book histograms
//-----------------------------------------------------------------------------
  BookHistograms();

  return 0;
}


//_____________________________________________________________________________
int TGaasRecoModule::BeginRun() {
  //  int rn = GetHeaderBlock()->RunNumber();
  return 0;
}

//-----------------------------------------------------------------------------
void TGaasRecoModule::BookChannelHistograms(HistBase_t* HistR, const char* Folder) {
//   char name [200];
//   char title[200];

  ChannelHist_t* Hist =  (ChannelHist_t*) HistR;

  HBook1F(Hist->fNSamples,"ns"  ,Form("%s: N(samples)"  ,Folder), 100,  0  ,1000. ,Folder);
}

//-----------------------------------------------------------------------------
void TGaasRecoModule::BookEventHistograms(HistBase_t* HistR, const char* Folder) {
  //  char name [200];
  //  char title[200];

  EventHist_t* Hist =  (EventHist_t*) HistR;

  HBook1F(Hist->fNumber    ,"en"   ,Form("%s: Event Number"      ,Folder),500, 0,500,Folder);
  HBook1F(Hist->fNChannels ,"nch"  ,Form("%s: N(channels"        ,Folder),100, 0,100,Folder);
}

//_____________________________________________________________________________
void TGaasRecoModule::BookHistograms() {

  //  char name [200];
  //  char title[200];

  TFolder* fol;
  TFolder* hist_folder;
  char     folder_name[200];

  DeleteHistograms();

  TH1::SetDefaultSumw2(kTRUE);	

  hist_folder = (TFolder*) GetFolder()->FindObject("Hist");
//-----------------------------------------------------------------------------
// book event histograms
//-----------------------------------------------------------------------------
  int book_event_histset[kNEventHistSets];
  for (int i=0; i<kNEventHistSets; i++) book_event_histset[i] = 0;

  book_event_histset[ 0] = 1;		// all events

  for (int i=0; i<kNEventHistSets; i++) {
    if (book_event_histset[i] != 0) {
      sprintf(folder_name,"evt_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
      fHist.fEvent[i] = new EventHist_t;
      BookEventHistograms(fHist.fEvent[i],Form("Hist/%s",folder_name));
    }
  }

  //-----------------------------------------------------------------------------
// book track histograms
//-----------------------------------------------------------------------------
  TString*  channel_selection   [kNChannelHistSets];

  for (int i=0; i<kNChannelHistSets; i++) { channel_selection[i] = NULL; }

  channel_selection[  0] = new TString("all channels");

  const char* folder_title;
  for (int i=0; i<kNChannelHistSets; i++) {
    if (channel_selection[i] != nullptr) {
      sprintf(folder_name,"chn_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      folder_title = folder_name;
      folder_title = channel_selection[i]->Data();
      if (! fol) fol = hist_folder->AddFolder(folder_name,folder_title);
      fHist.fChannel[i] = new ChannelHist_t;
      BookChannelHistograms(fHist.fChannel[i],Form("Hist/%s",folder_name));
    }
  }
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void TGaasRecoModule::FillEventHistograms(HistBase_t* HistR) {

  EventHist_t* Hist = (EventHist_t*) HistR;

  Hist->fNumber->Fill(fEventNumber);
  //  Hist->fNChannels  ->Fill();
}

//-----------------------------------------------------------------------------
// for DIO : ultimately, one would need to renormalize the distribution
//-----------------------------------------------------------------------------
void TGaasRecoModule::FillChannelHistograms(HistBase_t* HistR, TReadoutChannel* Channel) {

  ChannelHist_t* Hist = (ChannelHist_t*) HistR;

  Hist->fNSamples->Fill(Channel->GetNSamples());
}



//_____________________________________________________________________________

void TGaasRecoModule::FillHistograms() {

//  TStnTrack*   trk;
//  TrackPar_t*  tp;
//  int          ihist, n_setc_tracks[2];
//-----------------------------------------------------------------------------
// event histograms
// EVT_0: all events
//-----------------------------------------------------------------------------
  FillEventHistograms(fHist.fEvent[0]);

  FillChannelHistograms(fHist.fChannel[0], &fChannel);
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int TGaasRecoModule::Event(int ientry) {

  fGaasDataBlock->GetEntry(ientry);

  fEventNumber = GetHeaderBlock()->EventNumber();

  if (fFillHistograms) FillHistograms();

  Debug();

  return 0;		       
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void TGaasRecoModule::Debug() {

  //  char        text[500];
//-----------------------------------------------------------------------------
// bit 0: All Events
//-----------------------------------------------------------------------------
  // if (GetDebugBit(0) == 1) {
  //   GetHeaderBlock()->Print(Form("TGaasRecoModule :bit000:"));
  //   printf("KPAR:\n");
  //   fTrackBlock[kPAR]->Print();

  //   for (int i=0; i<fNTracks[kPAR]; i++) { 
  //     PrintTrack(fTrackBlock[kPAR]->Track(i),&fTrackPar[kPAR][i],"data");
  //   }

  //   printf("KDAR:\n");
  //   fTrackBlock[kDAR]->Print();

  //   for (int i=0; i<fNTracks[kDAR]; i++) { 
  //     PrintTrack(fTrackBlock[kDAR]->Track(i),&fTrackPar[kDAR][i],"data");
  //   }
  // }
}

//-----------------------------------------------------------------------------
int TGaasRecoModule::EndJob() {
  printf("----- end job: ---- %s\n",GetName());

  return 0;
}
