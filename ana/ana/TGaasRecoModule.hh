///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#ifndef gaasAna_ana_TGaasRecoModule_hh
#define gaasAna_ana_TGaasRecoModule_hh

#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

#include "Stntuple/base/HistBase_t.h"
#include "Stntuple/loop/TStnModule.hh"

#include "gaasAna/obj/TGaasDataBlock.hh"
#include "gaasAna/obj/TReadoutChannel.hh"

class TGaasRecoModule: public TStnModule {
public:
//-----------------------------------------------------------------------------
// play a trick: make sure HistBase_t is an internal class !
//-----------------------------------------------------------------------------
#include "Stntuple/base/HistBase_t.h"

//-----------------------------------------------------------------------------
// number of channels being read out in a given run
//-----------------------------------------------------------------------------
  enum { kNChannels = 1 };
    
//-----------------------------------------------------------------------------
//  histograms
//-----------------------------------------------------------------------------
  struct EventHist_t : public HistBase_t {
    TH1F*   fNumber;
    TH1F*   fNChannels;
  };

  struct ChannelHist_t : public HistBase_t {
    TH1F*    fNSamples;			// 
					// so far, assume one channel
    TH2F*   fWaveform     ;
    TH1F*   fLastWaveform ;
    TH1F*   fQ;				// charge
    TH1F*   fQ1;			// charge, corrected
    TH1F*   fT0;			// T0
    TH1F*   fPedestal;
    TH1F*   fChi2Ped;
    TH1F*   fPedP2P;
  };

  enum { kNEventHistSets   =  100 };
  enum { kNChannelHistSets =  500 };

  struct Hist_t {
    EventHist_t*   fEvent  [kNEventHistSets];
    ChannelHist_t* fChannel[kNChannelHistSets];
  };
//-----------------------------------------------------------------------------
//  data members
//-----------------------------------------------------------------------------
public:
					// pointers to the data blocks used
  TGaasDataBlock*    fGaasDataBlock;	//

  Hist_t             fHist;

  int                fFillHistograms;
  int                fEventNumber;
  int                fNSamples;

  TReadoutChannel*   fChannel[kNChannels];
//-----------------------------------------------------------------------------
//  functions
//-----------------------------------------------------------------------------
public:
  TGaasRecoModule(const char* name="GaasReco", const char* title="GaasReco");
  ~TGaasRecoModule();
//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------
  Hist_t*            GetHist      () { return &fHist;      }
  //  TStnTrackID*       GetTrackID   () { return fTrackID; }
//-----------------------------------------------------------------------------
// modifiers
//-----------------------------------------------------------------------------
  void    SetFillHistograms(int Fill) { fFillHistograms = Fill; }
//-----------------------------------------------------------------------------
// overloaded methods of TStnModule
//-----------------------------------------------------------------------------
  int     BeginJob();
  int     BeginRun();
  int     Event   (int ientry);
  int     EndJob  ();
//-----------------------------------------------------------------------------
// other methods
//-----------------------------------------------------------------------------
  void    BookEventHistograms   (HistBase_t*   Hist, const char* Folder);
  void    BookChannelHistograms (HistBase_t*   Hist, const char* Folder);

  void    FillEventHistograms    (HistBase_t*  Hist);
  void    FillChannelHistograms  (HistBase_t*  Hist, TReadoutChannel* Channel);

  void    BookHistograms();
  void    FillHistograms();

  int     ProcessChannels();
  int     ReconstructChannel(TReadoutChannel* Channel);

  void    Debug();

  ClassDef(TGaasRecoModule,0)
};

#endif
