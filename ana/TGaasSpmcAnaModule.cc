///////////////////////////////////////////////////////////////////////////////
// use of tmp:
//
// Tmp(0) : nax seg
// Tmp(1) : nst seg
// 
// use of debug bits: bits 0-2 are reserved
// 0  : all events
// 1  : passed events
// 2  : rejected events
// 
// 3  : events with SPMC T<100
// 4  : events with NHitsTF > 1
// 5  : UNUSED
///////////////////////////////////////////////////////////////////////////////
#include "TF1.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TEnv.h"
#include "TSystem.h"

#include "Stntuple/loop/TStnAna.hh"
#include "Stntuple/obj/TStnHeaderBlock.hh"
#include "Stntuple/alg/TStntuple.hh"
#include "Stntuple/val/stntuple_val_functions.hh"
//------------------------------------------------------------------------------
// Mu2e offline includes
//-----------------------------------------------------------------------------
// #include "CalorimeterGeom/inc/HexMap.hh"

#include "ana/TGaasSpmcAnaModule.hh"

// #include "ana/InitVirtualDetectors.hh"

ClassImp(TGaasSpmcAnaModule)
//-----------------------------------------------------------------------------
TGaasSpmcAnaModule::TGaasSpmcAnaModule(const char* name, const char* title):
  TStnModule(name,title)
{
  // fPdgCode       = 11;
  // fGeneratorCode = 28;
  fSpmcBlockName = "SpmcBlock";

  fPdgDb = TDatabasePDG::Instance();

  for (int i=0; i<5000; i++) fParticleCache[i] = 0;

  SetParticleCache(   11,fPdgDb->GetParticle(   11)); // e
  SetParticleCache(  -11,fPdgDb->GetParticle(  -11)); // e
  SetParticleCache(   13,fPdgDb->GetParticle(   13)); // mu
  SetParticleCache(  -13,fPdgDb->GetParticle(  -13)); // mu
  SetParticleCache(   22,fPdgDb->GetParticle(   22)); // photon
  SetParticleCache(  211,fPdgDb->GetParticle(  211)); // pi^-
  SetParticleCache( -211,fPdgDb->GetParticle( -211)); // pi^-
  SetParticleCache( 2212,fPdgDb->GetParticle( 2212)); // proton
  SetParticleCache(-2212,fPdgDb->GetParticle(-2212)); // pbar
}

//-----------------------------------------------------------------------------
TGaasSpmcAnaModule::~TGaasSpmcAnaModule() {
}


//-----------------------------------------------------------------------------
void TGaasSpmcAnaModule::BookEventHistograms(HistBase_t* Hist, const char* Folder) {
  //  char name [200];
  //  char title[200];
  EventHist_t* hist = (EventHist_t*) Hist;

  HBook1F(hist->fEventNumber,"evtnum",Form("%s: Event Number",Folder), 1000, 0,  1.e4,Folder);
  HBook1F(hist->fRunNumber  ,"runnum",Form("%s: Run   Number",Folder), 1000, 0,  1.e6,Folder);
  HBook1F(hist->fN1010      ,"n1010" ,Form("%s: N1010"       ,Folder),  100, 0,   100,Folder);
}

//-----------------------------------------------------------------------------
void TGaasSpmcAnaModule::BookSpmcHistograms(HistBase_t* Hist, const char* Folder) {
  //     char name [200];
  //     char title[200];
  StepPointMCHist_t* hist = (StepPointMCHist_t*) Hist;

  HBook1F(hist->fVolumeID[0],"vol_id_0"   ,Form("%s: VolumeID"       ,Folder), 200,    0, 10000,Folder);
  HBook1F(hist->fVolumeID[1],"vol_id_1"   ,Form("%s: VolumeID"       ,Folder), 100, 1000, 1100,Folder);
  HBook1F(hist->fGenIndex,"gen_index",Form("%s: GenIndex"       ,Folder), 200,    0, 10000,Folder);
  HBook1F(hist->fSimID   ,"sim_id"   ,Form("%s: SimID"          ,Folder), 200,    0,  1000,Folder);

  HBook1F(hist->fPDGCode[0] ,"pdg_0" ,Form("%s: PDG code"       ,Folder),  500,  -250,   250,Folder);
  HBook1F(hist->fPDGCode[1] ,"pdg_1" ,Form("%s: PDG code"       ,Folder), 2000, -10000, 10000,Folder);

  HBook1F(hist->fCreationCode   ,"cr_code",Form("%s: Creation code",Folder), 200,   0,   200,Folder);
  HBook1F(hist->fParentSimID    ,"psim_id",Form("%s: Parent SimID",Folder), 200,   0,  1000,Folder);
  HBook1F(hist->fParentPDGCode  ,"ppdg"   ,Form("%s: Parent PDG code",Folder), 200, -100,   100,Folder);
  HBook1F(hist->fEndProcessCode ,"end_code",Form("%s: End process code",Folder), 200,   0,   200,Folder);
  HBook1F(hist->fEDepTot        ,"edep_tot",Form("%s: EDEP tot"        ,Folder), 200,   0,   10 ,Folder);
  HBook1F(hist->fEDepNio        ,"edep_nio",Form("%s: EDEP NIO"        ,Folder), 200,   0,   10 ,Folder);
  HBook1F(hist->fTime           ,"time"    ,Form("%s: Time"            ,Folder), 200,   0,  2000,Folder);
  HBook1F(hist->fStepLength     ,"step"    ,Form("%s: Ltep Length"     ,Folder), 200,   0,   100,Folder);
  HBook1F(hist->fMomentum       ,"mom"     ,Form("%s: Momentum"        ,Folder), 500,   0,   250,Folder);
  HBook1F(hist->fEKin           ,"ekin"    ,Form("%s: kinetic energy"  ,Folder), 400,   0,   100,Folder);

  HBook2F(hist->fYVsX           ,"y_vs_x"     ,Form("%s: Y vs X"       ,Folder), 100, -250,  250, 100, -250, 250, Folder);
  HBook2F(hist->fYVsZ           ,"y_vs_z"     ,Form("%s: Y vs Z"       ,Folder), 500, -250,  250, 500, -250, 250, Folder);
}

//-----------------------------------------------------------------------------
void TGaasSpmcAnaModule::BookGenpHistograms(HistBase_t* Hist, const char* Folder) {
  //     char name [200];
  //     char title[200];
  GenpHist_t* hist = (GenpHist_t*) Hist;

  HBook1F(hist->fPdgCode,"pdg_code",Form("%s: PDG code"      ,Folder), 200,-1000, 1000,Folder);
  HBook1F(hist->fGenCode,"gen_code",Form("%s: generator code",Folder), 100,  -10,   90,Folder);
  HBook1F(hist->fMomentum,"mom"    ,Form("%s: Momentum"      ,Folder), 200,    0, 1000,Folder);
  HBook1F(hist->fPz      ,"pz"     ,Form("%s: Pz"            ,Folder), 200, -500,  500,Folder);
  HBook1F(hist->fZ       ,"z"      ,Form("%s: Z"             ,Folder), 200,    0,   10,Folder);
  HBook1F(hist->fR       ,"r"      ,Form("%s: R"             ,Folder), 500,    0,    5,Folder);
}

//-----------------------------------------------------------------------------
void TGaasSpmcAnaModule::BookHistograms() {

  //  char name [200];
  //  char title[200];

  TFolder* fol;
  TFolder* hist_folder;
  char     folder_name[200];

  DeleteHistograms();
  hist_folder = (TFolder*) GetFolder()->FindObject("Hist");
//-----------------------------------------------------------------------------
// book event histograms
//-----------------------------------------------------------------------------
  int book_event_histset[kNEventHistSets];
  for (int i=0; i<kNEventHistSets; i++) book_event_histset[i] = 0;

  book_event_histset[ 0] = 1;		// all events
  book_event_histset[ 1] = 0;		// just an example - this should be the default

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
// book StepPointMC histograms
//-----------------------------------------------------------------------------
  int book_spmc_histset[kNStepPointMCHistSets];
  for (int i=0; i<kNStepPointMCHistSets; i++) book_spmc_histset[i] = 0;

  book_spmc_histset[0] = 1;		// all steps

  for (int i=0; i<kNStepPointMCHistSets; i++) {
    if (book_spmc_histset[i] != 0) {
      sprintf(folder_name,"spmc_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
      fHist.fStepPointMC[i] = new StepPointMCHist_t;
      BookSpmcHistograms(fHist.fStepPointMC[i],Form("Hist/%s",folder_name));
    }
  }
//-----------------------------------------------------------------------------
// book GENP histograms
//-----------------------------------------------------------------------------
  int book_genp_histset[kNGenpHistSets];
  for (int i=0; i<kNGenpHistSets; i++) book_genp_histset[i] = 0;

  book_genp_histset[0] = 1;		// all particles

  for (int i=0; i<kNGenpHistSets; i++) {
    if (book_genp_histset[i] != 0) {
      sprintf(folder_name,"gen_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
      fHist.fGenp[i] = new GenpHist_t;
      BookGenpHistograms(fHist.fGenp[i],Form("Hist/%s",folder_name));
    }
  }
}

//-----------------------------------------------------------------------------
void TGaasSpmcAnaModule::FillEventHistograms(HistBase_t* Hist) {
//   double            cos_th, xv, yv, rv, zv, p;
//   TLorentzVector    mom;

  EventHist_t* hist = (EventHist_t*) Hist;

  int event_number = GetHeaderBlock()->EventNumber();
  int run_number   = GetHeaderBlock()->RunNumber();

  hist->fEventNumber->Fill(event_number);
  hist->fRunNumber->Fill(run_number);
  hist->fN1010->Fill(fN1010);

}

//-----------------------------------------------------------------------------
void TGaasSpmcAnaModule::FillGenpHistograms(HistBase_t* Hist, TGenParticle* Part) {

  TLorentzVector p;

  Part->Momentum(p);

  GenpHist_t* hist = (GenpHist_t*) Hist;
  
  hist->fPdgCode->Fill(Part->GetPdgCode());
  hist->fGenCode->Fill(Part->GetStatusCode());
  hist->fMomentum->Fill(p.P());
  hist->fPz->Fill(p.Pz());
  hist->fZ->Fill(Part->Vz());
  hist->fR->Fill(Part->R());
}

//-----------------------------------------------------------------------------
void TGaasSpmcAnaModule::FillSpmcHistograms(HistBase_t* Hist, TStepPointMC* Step, SpmcData_t* SpmcData) {

  StepPointMCHist_t* hist = (StepPointMCHist_t*) Hist;
  
  int pdg_code = Step->PDGCode();

  hist->fVolumeID[0]->Fill(Step->VolumeID());
  hist->fVolumeID[1]->Fill(Step->VolumeID());
  hist->fGenIndex->Fill(Step->GenIndex());
  hist->fSimID   ->Fill(Step->SimID());
  hist->fPDGCode[0] ->Fill(pdg_code);
  hist->fPDGCode[1] ->Fill(pdg_code);
  hist->fCreationCode->Fill(Step->CreationCode());
  hist->fParentSimID ->Fill(Step->ParentSimID());
  hist->fParentPDGCode->Fill(Step->ParentPDGCode());
  hist->fEndProcessCode->Fill(Step->EndProcessCode());

  hist->fEDepTot->Fill(Step->EDepTot());
  hist->fEDepNio->Fill(Step->EDepNio());
  hist->fTime   ->Fill(Step->Time());
  hist->fStepLength->Fill(Step->StepLength());

  double p = Step->Mom()->Mag();
  hist->fMomentum->Fill(p);

  
  double m(0);
  if (SpmcData->fParticle) {
    m = SpmcData->fParticle->Mass()*1e3;   // convert GeV -> MeV
  }

  double ekin = sqrt(p*p+m*m)-m;
  hist->fEKin->Fill(ekin);

  float x = Step->Pos()->X();
  float y = Step->Pos()->Y();
  float z = Step->Pos()->Z();

  // hack for stage 2:

  if (fabs(z-2929.) < 0.1) x = x+3904;

  hist->fYVsX->Fill(x,y);		// useful for stage 2
  hist->fYVsZ->Fill(z,y);		// useful for stage 1
}


//-----------------------------------------------------------------------------
// register data blocks and book histograms
//-----------------------------------------------------------------------------
int TGaasSpmcAnaModule::BeginJob() {
//-----------------------------------------------------------------------------
// register data blocks 'SpmcBlock' or 'StepPointMCBlock' (old)
//-----------------------------------------------------------------------------
  RegisterDataBlock("GenpBlock",&fGenpBlock);
  RegisterDataBlock(fSpmcBlockName.Data(),"TStepPointMCBlock",&fSpmcBlock);
//-----------------------------------------------------------------------------
// book histograms
//-----------------------------------------------------------------------------
  BookHistograms();
//-----------------------------------------------------------------------------
// initialize virtual detector offsets - a convenience for histogram filling
//-----------------------------------------------------------------------------
//  InitVirtualDetectors(fVDet,&fNVDet);

  return 0;
}


//-----------------------------------------------------------------------------
void TGaasSpmcAnaModule::FillHistograms() {

//-----------------------------------------------------------------------------
// event histograms
//
// EVT_0: all events
//-----------------------------------------------------------------------------
  FillEventHistograms(fHist.fEvent[0]);
//-----------------------------------------------------------------------------
// StepPointMC histograms
// for beamline studies, there is only one 
//-----------------------------------------------------------------------------
  TStepPointMC* spmc;
  SpmcData_t    spmc_data;

  int nsteps = fSpmcBlock->NStepPoints();

  for (int i=0; i<nsteps; i++) {
    spmc         = fSpmcBlock->StepPointMC(i);
    //    float p      = spmc->Mom()->Mag();
    //    float t      = spmc->Time();
    int pdg_code = spmc->PDGCode();
    int abs_pdg_code = abs(pdg_code);
//-----------------------------------------------------------------------------
// particles of interest are electrons, pions, muons and photons,
// speed up the mass extraction
//-----------------------------------------------------------------------------
    if (abs_pdg_code < 2500) spmc_data.fParticle = GetParticleCache(pdg_code);
    else                     spmc_data.fParticle = NULL;

    if (spmc_data.fParticle == NULL) {
      if (GetDebugBit(3) == 0) printf(">>> WARNING: no particle with PDF code=%i in ROOT particle DB\n",pdg_code);
    }

    FillSpmcHistograms(fHist.fStepPointMC[0],spmc,&spmc_data);
  }

//-----------------------------------------------------------------------------
// fill GENP histograms
//-----------------------------------------------------------------------------
   for (int i=0; i<fNGenp; i++) {
     TGenParticle* genp = fGenpBlock->Particle(i);
     FillGenpHistograms(fHist.fGenp[0],genp);
   }
 
}
  

//_____________________________________________________________________________
int TGaasSpmcAnaModule::BeginRun() {
  //  int rn = GetHeaderBlock()->RunNumber();
  //  TStntuple::Init(rn);
  return 0;
}


//_____________________________________________________________________________
int TGaasSpmcAnaModule::Event(int ientry) {

  fGenpBlock->GetEntry(ientry);
  fSpmcBlock->GetEntry(ientry);

//  fParent = parent;

  fNGenp = fGenpBlock->NParticles();
  int nsteps = fSpmcBlock->NStepPoints();

  TStepPointMC* spmc;
  fN1010 = 0;
  for (int i=0; i<nsteps; i++) {
    spmc = fSpmcBlock->StepPointMC(i);
    if (spmc->VolumeID() == 1010) fN1010 += 1;
  }


//-----------------------------------------------------------------------------
// everything is precalculated, fill histograms
//-----------------------------------------------------------------------------
  FillHistograms();

  Debug();

  return 0;		       
}

//-----------------------------------------------------------------------------
void TGaasSpmcAnaModule::Debug() {

//-----------------------------------------------------------------------------
// bit 4: events with NHitsTF > 1
//-----------------------------------------------------------------------------
  // if (GetDebugBit(4) == 1) {
  //   GetHeaderBlock()->Print(Form("NHits(TF) = %5i",fNGenp));
  // }
}

//_____________________________________________________________________________
int TGaasSpmcAnaModule::EndJob() {
  printf("----- end job: ---- %s\n",GetName());
  return 0;
}

//_____________________________________________________________________________
void TGaasSpmcAnaModule::Test001() {
}
