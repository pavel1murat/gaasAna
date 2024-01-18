///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#ifndef __gaasAna_ana_TGaasSpmcAnaModule_hh__
#define __gaasAna_ana_TGaasSpmcAnaModule_hh__

#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TDatabasePDG.h"
#include "TParticlePDG.h"

#include "Stntuple/loop/TStnModule.hh"

#include "Stntuple/obj/TGenpBlock.hh"
#include "Stntuple/obj/TStepPointMCBlock.hh"

#include "Stntuple/base/TStnArrayI.hh"

class TGaasSpmcAnaModule: public TStnModule {
public:
//-----------------------------------------------------------------------------
// play a trick: pretend HistBase_t is an internal class !
//-----------------------------------------------------------------------------
  struct HistBase_t {
    int     fHistBaseID;			// name to avoid confusions
  };
  // #include "Stntuple/base/HistBase_t.h"
//-----------------------------------------------------------------------------
//  histograms
//-----------------------------------------------------------------------------
  struct EventHist_t : public HistBase_t {
    TH1F*      fRunNumber;
    TH1F*      fEventNumber;
    TH1F*      fN1010;
  };

  struct GenpHist_t : public HistBase_t {
    TH1F*    fPdgCode;		       //
    TH1F*    fGenCode;		       // generator code
    TH1F*    fMomentum;
    TH1F*    fR;
    TH1F*    fZ;
    TH1F*    fPz;
  };

  struct StepPointMCHist_t : public HistBase_t {
    TH1F*      fVolumeID[2];		       //
    TH1F*      fGenIndex;		       //
    TH1F*      fSimID;
    TH1F*      fPDGCode[2];  // just different ranges
    TH1F*      fCreationCode;
    TH1F*      fParentSimID;
    TH1F*      fParentPDGCode;
    TH1F*      fEndProcessCode;

    TH1F*      fEDepTot;
    TH1F*      fEDepNio;
    TH1F*      fTime;
    TH1F*      fStepLength;

    TH1F*      fMomentum;
    TH1F*      fEKin;

    TH2F*      fYVsZ;
    TH2F*      fYVsX;
  };

  struct SimpData_t {
    int           fIndex;		// so far, not used
    TSimParticle* fParent;              // muon parent
  };

  struct SpmcData_t {
    TParticlePDG*  fParticle;		// so far, not used
  };

//-----------------------------------------------------------------------------
  enum { kNEventHistSets        =   100 };
  enum { kNStepPointMCHistSets  = 10000 };
  enum { kNGenpHistSets         =  1000 };

  struct Hist_t {
    EventHist_t*        fEvent      [kNEventHistSets      ];
    StepPointMCHist_t*  fStepPointMC[kNStepPointMCHistSets];
    GenpHist_t*         fGenp       [kNGenpHistSets       ];
  };
//-----------------------------------------------------------------------------
//  data members
//-----------------------------------------------------------------------------
public:
					// pointers to the data blocks used
  TStepPointMCBlock*    fSpmcBlock;
  TGenpBlock*           fGenpBlock;
					// histograms filled
  Hist_t                fHist;

  TString               fSpmcBlockName;

  TSimParticle*         fMuon;		// pointer to stopped muon (pend=0)
  TSimParticle*         fParent;
  TSimParticle*         fProton;

  TParticlePDG*         fParticleCache[5000];

  TDatabasePDG*         fPdgDb;

  int                   fN1010;
  int                   fNGenp;
//-----------------------------------------------------------------------------
//  functions
//-----------------------------------------------------------------------------
public:
  TGaasSpmcAnaModule(const char* name="StepPointMCAna", const char* title="StepPointMCAna");
  ~TGaasSpmcAnaModule();
//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------
  Hist_t*            GetHist        () { return &fHist;        }
//-----------------------------------------------------------------------------
// setters
//-----------------------------------------------------------------------------
  void SetSpmcBlockName(const char* Name) { fSpmcBlockName = Name; }

  void          SetParticleCache(int PdgCode, TParticlePDG* P) { fParticleCache[2500+PdgCode] = P; }
  TParticlePDG* GetParticleCache(int PdgCode) { return fParticleCache[2500+PdgCode]; }
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
  void    BookEventHistograms (HistBase_t* Hist, const char* Folder);
  void    BookGenpHistograms  (HistBase_t* Hist, const char* Folder);
  void    BookSpmcHistograms  (HistBase_t* Hist, const char* Folder);

  void    FillEventHistograms (HistBase_t* Hist);
  void    FillGenpHistograms  (HistBase_t* Hist, TGenParticle* Part);
  void    FillSpmcHistograms  (HistBase_t* Hist, TStepPointMC* Step, SpmcData_t* SpmcData);

  void    BookHistograms();
  void    FillHistograms();


  void    Debug();
//-----------------------------------------------------------------------------
// test
//-----------------------------------------------------------------------------
  void    Test001();

  ClassDef(TGaasSpmcAnaModule,0)
};

#endif
