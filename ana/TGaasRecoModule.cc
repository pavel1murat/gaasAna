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
#include <vector>

#include "TF1.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TEnv.h"
#include "TSystem.h"

#include "Stntuple/loop/TStnAna.hh"
#include "Stntuple/loop/TStnInputModule.hh"
#include "Stntuple/base/TStnDataset.hh"
#include "Stntuple/base/TCalibManager.hh"
#include "Stntuple/obj/TStnHeaderBlock.hh"
#include "Stntuple/obj/TStnNode.hh"
#include "Stntuple/alg/TStntuple.hh"
#include "Stntuple/val/stntuple_val_functions.hh"

#include "gaasAna/ana/TGaasUtils.hh"

using std::vector;
//------------------------------------------------------------------------------
// Mu2e offline includes
//-----------------------------------------------------------------------------
#include "gaasAna/ana/TGaasRecoModule.hh"

ClassImp(TGaasRecoModule)
//-----------------------------------------------------------------------------
TGaasRecoModule::TGaasRecoModule(const char* name, const char* title): TStnModule(name,title)
{
  fFillHistograms = 1;
  //  fNSamples       = 500;                        // should come from the database
  fFirstRun       = -1;
  TClass* cl      = gROOT->GetClass("TGaasCalibData");
  fCalibData      = (TGaasCalibData*) cl->New();
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
//  BookHistograms();

  return 0;
}

//-----------------------------------------------------------------------------
void TGaasRecoModule::BookEventHistograms(HistBase_t* HistR, const char* Folder) {
  //  char name [200];
  //  char title[200];

  EventHist_t* Hist =  (EventHist_t*) HistR;

  HBook1F(Hist->fNumber    ,"en"   ,Form("%s: Event Number"      ,Folder),500, 0,500,Folder);
  HBook1F(Hist->fNChannels ,"nch"  ,Form("%s: N(channels"        ,Folder),100, 0,100,Folder);
}

//-----------------------------------------------------------------------------
// assume taht all channels have the same time sampling
//-----------------------------------------------------------------------------
void TGaasRecoModule::BookChannelHistograms(HistBase_t* HistR, const char* Folder) {
//   char name [200];
//   char title[200];

  ChannelHist_t* Hist =  (ChannelHist_t*) HistR;

  int ns = fCalibData->GetChannel(Hist->fChannelID)->NSamples();

  HBook1F(Hist->fNSamples    ,"nsamples","nsamples"  ,100 ,  0, 1000. ,Folder);
  HBook2F(Hist->fWaveform[0] ,"wf_0"    ,"wf_0[mV]"  , ns ,  0,    ns  ,5000,-250, 250,Folder);
  HBook2F(Hist->fWaveform[1] ,"wf_1"    ,"wf_1[mV]"  , ns ,  0,    ns  ,5000,-250, 250,Folder);
  HBook1F(Hist->fLastWaveform,"last_wf" ,"last_wf"   , ns ,  0,    ns, Folder);
  HBook1F(Hist->fQ[0]        ,"q_0"     ,"Q_0"       ,1000,  0, 5000,Folder);
  HBook1F(Hist->fQ[1]        ,"q_1"     ,"Q_1"       ,1000,  0,  500,Folder);
  HBook1F(Hist->fQ1[0]       ,"q1_0"    ,"Q1_0"      ,1000,  0, 5000,Folder);
  HBook1F(Hist->fQ1[1]       ,"q1_1"    ,"Q1_1"      ,1000,  0,  500,Folder);
  HBook1F(Hist->fV0Max       ,"v0max"   ,"V0Max"     , 500,  0,  250,Folder);
  HBook1F(Hist->fV1Max       ,"v1max"   ,"V1Max"     , 500,  0,  250,Folder);
  HBook1F(Hist->fT0          ,"t0"      ,"T0"        , 400,150,  350,Folder);
  HBook1F(Hist->fPedestal    ,"ped"     ,"Ped, mV"   ,1000,-25,   25,Folder);
  HBook1F(Hist->fSigmaPed    ,"sigped"  ,"SigPed, mV", 500,  0,   50,Folder);
  HBook1F(Hist->fP2P1[0]     ,"p2p1_0"  ,"P2P1_0"    , 200,  0,   10,Folder);
  HBook1F(Hist->fP2P1[1]     ,"p2p1_1"  ,"P2P1_1"    , 200,  0,  100,Folder);
  HBook1F(Hist->fP2P2[0]     ,"p2p2_0"  ,"P2P2_0"    , 200,  0,   10,Folder);
  HBook1F(Hist->fP2P2[1]     ,"p2p2_1"  ,"P2P2_1"    , 200,  0,  100,Folder);
}

//_____________________________________________________________________________
void TGaasRecoModule::BookHistograms() {

  //  char name [200];
  //  char title[200];

  TFolder* fol;
  TFolder* hist_folder;
  char     folder_name[200];

  DeleteHistograms();

  //  TH1::SetDefaultSumw2(kTRUE);	

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
// book channel histograms
//-----------------------------------------------------------------------------
  TString*  channel_selection   [kNChannelHistSets];
  int       channel_id          [kNChannelHistSets];
  

  for (int i=0; i<kNChannelHistSets; i++) { channel_selection[i] = NULL; }

  int nch = fCalibData->GetNChannels();

  for (int i=0; i<nch; i++) {
    if (fCalibData->GetChannel(i)->Used() != 0) {
      int cid = fCalibData->GetChannel(i)->ID();
      
      channel_selection[    i] = new TString("all channels");
      channel_selection[100+i] = new TString("P2P < 0.0040");
      channel_selection[200+i] = new TString("P2P >= 0.0040");
      channel_selection[300+i] = new TString("P2P < 0.0040 and 3 < V1Min < 8"); // "QD signal" for run 14
      channel_selection[400+i] = new TString("P2P < 0.0040 and V1Min > 8"    ); // "PD signal" for run 14

      channel_id       [    i] = cid;
      channel_id       [100+i] = cid;
      channel_id       [200+i] = cid;
      channel_id       [300+i] = cid;
      channel_id       [400+i] = cid;
    }
  }

  const char* folder_title;
  for (int i=0; i<kNChannelHistSets; i++) {
    if (channel_selection[i] != nullptr) {
      sprintf(folder_name,"chn_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      folder_title = folder_name;
      folder_title = channel_selection[i]->Data();
      if (! fol) fol = hist_folder->AddFolder(folder_name,folder_title);
      fHist.fChannel[i] = new ChannelHist_t;
      fHist.fChannel[i]->fChannelID = channel_id[i];
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
  Hist->fNChannels->Fill(fGaasDataBlock->GetNChannels());
}

//-----------------------------------------------------------------------------
// for DIO : ultimately, one would need to renormalize the distribution
//-----------------------------------------------------------------------------
void TGaasRecoModule::FillChannelHistograms(HistBase_t* HistR, TReadoutChannel* Channel) {

  ChannelHist_t* Hist = (ChannelHist_t*) HistR;

  int ns = Channel->GetNSamples();

  Hist->fNSamples->Fill(ns);
  Hist->fQ [0]->Fill(Channel->Q ());
  Hist->fQ [1]->Fill(Channel->Q ());
  Hist->fQ1[0]->Fill(Channel->Q1());
  Hist->fQ1[1]->Fill(Channel->Q1());
  
  Hist->fT0->Fill(Channel->T0());
  Hist->fV0Max->Fill(Channel->V0Max());
  Hist->fV1Max->Fill(Channel->V1Max());
					// N(points) >> 1, do not correct for that
  
  float sig_ped = sqrt(Channel->Chi2Ped());
  Hist->fSigmaPed->Fill(sig_ped);
  Hist->fPedestal->Fill(Channel->Pedestal());

  // voltage P2P in "pedestal" regions
  
  float p2p_1 = Channel->fPar[1]-Channel->fPar[0];
  float p2p_2 = Channel->fPar[3]-Channel->fPar[2];
  
  Hist->fP2P1[0]->Fill(p2p_1);
  Hist->fP2P1[1]->Fill(p2p_1);

  Hist->fP2P2[0]->Fill(p2p_2);
  Hist->fP2P2[1]->Fill(p2p_2);

  Hist->fLastWaveform->Reset();
  
  for (int i=0; i<ns; i++) {
    float v0 = Channel->V0(i);
    float v1 = Channel->V1(i);
    int   x = i+0.5;
    Hist->fWaveform[0]    ->Fill(x,v0);

    int x2 = i-Channel->T0()+200+0.5;
    if ((x2>=0) && (x2<500)) Hist->fWaveform[1]->Fill(x2,v1);

    Hist->fLastWaveform->SetBinContent(i+1,v0);
    Hist->fLastWaveform->SetBinError  (i+1,0);
  }
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

  // so far have just one channel, pretend there are many

  int nch = fGaasDataBlock->GetNChannels();
  
  for (int i=0; i<nch; i++) {
    TReadoutChannel*   ch  = fChannel[i];

    int cid = ch->ID();
    TGaasCalibChannel* cch = fCalibData->GetChannel(cid);
    
    FillChannelHistograms(fHist.fChannel[cid], ch);
      
    float p2p_1 = ch->fPar[1]-ch->fPar[0];
    float p2p_2 = ch->fPar[3]-ch->fPar[2];
    
    if ((p2p_1 < cch->fMaxP2P) && (p2p_2 < cch->fMaxP2P) && (fabs(ch->Pedestal()) < cch->fMaxThr)) {
      FillChannelHistograms(fHist.fChannel[100+cid], ch);
    }
    else {
      FillChannelHistograms(fHist.fChannel[200+cid], ch);
    }

    float v1max = ch->V1Max();
    
    if ((p2p_1 < cch->fMaxP2P) && (p2p_2 < cch->fMaxP2P) && (fabs(ch->Pedestal()) < cch->fMaxThr)) {
      if      ((v1max >  3.) && (v1max < 8.)) FillChannelHistograms(fHist.fChannel[300+cid], ch);
      else if ( v1max >  8.)                  FillChannelHistograms(fHist.fChannel[400+cid], ch);
    }
  }
}

//-----------------------------------------------------------------------------
int TGaasRecoModule::BeginRun() {
  //  char calib_name[200];

  int rn = GetHeaderBlock()->RunNumber();

  TCalibManager* cm = TCalibManager::Instance();

  fCalibData->Init(rn,cm);

// //-----------------------------------------------------------------------------
// // this is, obviously, a hack
// //-----------------------------------------------------------------------------
//   sprintf(calib_name,"config");
// 
//   crr = cm->GetRunRange("gaas","config",rn);
//   if (crr == 0) {
//     Error("BeginRun",Form("missing Gaas Reco CONFIG table for run number %8i\n",rn));
//     return -1;
//   }
// //-----------------------------------------------------------------------------
// // if new run range, load the file
// //-----------------------------------------------------------------------------
//   if (fCalibRunRange != crr) {
// 
//     FILE* f  = fopen(crr->GetFilename(),"r");
//     if (f == 0) {
//       Error("Init",Form("missing file %s\n",crr->GetFilename()));
//       return -2;
//     }
// 
//     fCalibRunRange = crr;
// 
//     gInterpreter->LoadMacro(crr->GetFilename());
//   }
// 
  if (fLastRun == -1) {
//------------------------------------------------------------------------------
// book histograms after the first database access
//-----------------------------------------------------------------------------
    BookHistograms();
//-----------------------------------------------------------------------------
// now, that we know home many channels are processed, initialize them
//-----------------------------------------------------------------------------
    int nch = 0;
    for (int i=0; i<fCalibData->GetNChannels(); i++) {
      TGaasCalibChannel* cch = fCalibData->GetChannel(i);
      if (cch->Used() != 0) {
	fChannel[nch] = new TReadoutChannel(cch->fID,cch->fNSamples);
	nch++;
      }
    }
    
    fLastRun = rn;
  }

  // printf(" .... executing %s::GaasRecoModule_init_run\n",crr->GetFilename());
  // gInterpreter->ProcessLine(Form("GaasRecoModule_init_run(%i);",rn));
  // printf(" .... back from %s::GaasRecoModule_init_run\n",crr->GetFilename());

 
//   fGain           = 1.; // fCalibData->GetChannelGain(ich);

//   if (rn < 15) {
//     fPolarity     =  -1;
//     fMinSample[0] =   0;
//     fMaxSample[0] = 200;
//     fMinSample[1] = 320;
//     fMaxSample[1] = 499;

//     fMaxP2P       = 0.004;
//     fMaxThreshold = 1.0e-3 ; // in mV

//     if      (rn <=  6) {
//       fFirstCell                = 235; // (int) (t0-5);
//       fPulseIntegrationWindow   = 40 ; // (int) (fCalibData->GetPulseWindow(ich)*5);
//     }
//     else if (rn <= 12) {
//       fFirstCell                = 230;
//       fPulseIntegrationWindow   = 70 ;
//     }
//     else if (rn <= 14) {
//       fFirstCell                = 235;
//       fPulseIntegrationWindow   = 40 ;
//     }
//   }
//   if ((rn >= 15) && (rn <= 29)) {
// //-----------------------------------------------------------------------------
// // calibration of alpha sources
// //-----------------------------------------------------------------------------
//     fPolarity     =  1;
    
//     if (rn == 15) {
//       fMinSample[0] =   0;
//       fMaxSample[0] = 150;
//       fMinSample[1] = 320;
//       fMaxSample[1] = 499;

//       fMaxP2P       = 4.;
//       fMaxThreshold = 100; // in mV

//       fFirstCell              = 180;
//       fPulseIntegrationWindow = 100;
//     }

//     if (rn == 16) {
//       fMinSample[0] =   0;
//       fMaxSample[0] = 150;
//       fMinSample[1] = 320;
//       fMaxSample[1] = 499;

//       fMaxP2P       = 4.;
//       fMaxThreshold = 100; // in mV

//       fFirstCell              = 180;
//       fPulseIntegrationWindow = 100;
//     }

//     if (rn == 17) {
//       fMinSample[0] =   0;
//       fMaxSample[0] = 140;
//       fMinSample[1] = 320;
//       fMaxSample[1] = 499;

//       fMaxP2P       = 4.;
//       fMaxThreshold = 100; // in mV

//       fFirstCell              = 150;
//       fPulseIntegrationWindow = 100;
//     }

//     if (rn <= 28) {
//       fMinSample[0] =   0;
//       fMaxSample[0] = 140;
//       fMinSample[1] = 320;
//       fMaxSample[1] = 499;

//       fMaxP2P       = 4.;
//       fMaxThreshold = 100; // in mV

//       fFirstCell              = 150;
//       fPulseIntegrationWindow = 100;
//     }
//     else if (rn == 29) {
//       fMinSample[0] =   0;
//       fMaxSample[0] = 140;
//       fMinSample[1] = 280;
//       fMaxSample[1] = 499;

//       fMaxP2P       = 4.;
//       fMaxThreshold = 100; // in mV

//       fFirstCell              = 150;
//       fPulseIntegrationWindow = 100;
//     }
//   }
//   else if (rn <= 40) {
// //-----------------------------------------------------------------------------
// // calibration of alpha sources
// //-----------------------------------------------------------------------------
//     fPolarity     =  1;
//     if (rn == 30) {
//       fMinSample[0] =   0;
//       fMaxSample[0] = 140;
//       fMinSample[1] = 280;
//       fMaxSample[1] = 499;

//       fMaxP2P       = 4.;
//       fMaxThreshold = 100; // in mV

//       fFirstCell              = 150;
//       fPulseIntegrationWindow = 100;
//     }
//     else if (rn == 31) {
//       fMinSample[0] =   0;
//       fMaxSample[0] = 140;
//       fMinSample[1] = 280;
//       fMaxSample[1] = 499;

//       fMaxP2P       = 4.;
//       fMaxThreshold = 100; // in mV

//       fFirstCell              = 150;
//       fPulseIntegrationWindow = 100;
//     }
//     else if (rn == 32) {
//       fMinSample[0] =   0;
//       fMaxSample[0] = 140;
//       fMinSample[1] = 280;
//       fMaxSample[1] = 499;

//       fMaxP2P       = 4.;
//       fMaxThreshold = 100; // in mV

//       fFirstCell              = 150;
//       fPulseIntegrationWindow = 100;
//     }
//     else if (rn == 33) {
//       fMinSample[0] =   0;
//       fMaxSample[0] = 140;
//       fMinSample[1] = 280;
//       fMaxSample[1] = 499;

//       fMaxP2P       = 4.;
//       fMaxThreshold = 100; // in mV

//       fFirstCell              = 150;
//       fPulseIntegrationWindow = 100;
//     }
//     else if (rn == 34) {
//       fMinSample[0] =   0;
//       fMaxSample[0] = 140;
//       fMinSample[1] = 280;
//       fMaxSample[1] = 499;

//       fMaxP2P       = 4.;
//       fMaxThreshold = 100; // in mV

//       fFirstCell              = 150;
//       fPulseIntegrationWindow = 100;
//     }
//     else {
//       fMinSample[0] =   0;
//       fMaxSample[0] = 140;
//       fMinSample[1] = 280;
//       fMaxSample[1] = 499;

//       fMaxP2P       = 4.;
//       fMaxThreshold = 100; // in mV

//       fFirstCell              = 150;
//       fPulseIntegrationWindow = 100;
//     }
//   }
//   else if (rn < 48) {
// //-----------------------------------------------------------------------------
// // continue with the source calibrations:
// // run 41: internal pulser + PO-210
// // run 42: PO-210 only
// //-----------------------------------------------------------------------------
//     fPolarity     =  1;
//     fMinSample[0] =   0;
//     fMaxSample[0] = 140;
//     fMinSample[1] = 280;
//     fMaxSample[1] = 499;

//     fMaxP2P       = 4.;
//     fMaxThreshold = 100; // in mV

//     fFirstCell              = 150;
//     fPulseIntegrationWindow = 100;
//   }

  return 0;
}

//-----------------------------------------------------------------------------
int TGaasRecoModule::ReconstructChannel(TReadoutChannel* Channel, TGaasCalibChannel* Calib) {

  if (Channel->GetUsed() == 0) return 0;

  int ns            = Channel->GetNSamples();
  // vector<float>* t  = Channel->GetT();
  // vector<float>* v0 = Channel->GetV0();
//-----------------------------------------------------------------------------
// at this point can proceed with the waveform analysis and pedestal determination
//-----------------------------------------------------------------------------
  static int  _nErrors            (0);
  static int  _printErrorThreshold(0);
  
  double    sum, qn, q, q1, v, vmin, vmax, pedestal, slope, t0, y, ey, mean, chi2;
  double    _PedSigma ; // , ped_mean;
  int       found, min_cell, max_cell;
//-----------------------------------------------------------------------------
// determine pedestal
//-----------------------------------------------------------------------------
  Channel->SetQ (0.);
  Channel->SetQ1(0.);
//-----------------------------------------------------------------------------
// get calibration constants
// pulse_integration_window - in units of channels
//-----------------------------------------------------------------------------
  _PedSigma                   = 1.; // fCalibData->GetPedSigma(ich);

  sum = 0;
  qn  = 0;
//-----------------------------------------------------------------------------
// P2P before the signal
//-----------------------------------------------------------------------------
  vmin =  1.e6;
  vmax = -1.e6;
  
  for (int i=Calib->fMinSample[0]; i<Calib->fMaxSample[0]; i++) {
    v   = Channel->V0(i);
    sum += v;
    qn  += 1;
    if (v < vmin) vmin = v;
    if (v > vmax) vmax = v;
  }
					// p2p before the signal
  Channel->fPar[0] = vmin;    //
  Channel->fPar[1] = vmax;    //

  pedestal = sum/qn;
  Channel->SetPedestal(pedestal);
//-----------------------------------------------------------------------------
// P2P after the signal
//-----------------------------------------------------------------------------
  vmin =  1.e6;
  vmax = -1.e6;
  
  for (int i=Calib->fMinSample[1]; i<Calib->fMaxSample[1]; i++) {
    v   = Channel->V0(i);
    if (v < vmin) vmin = v;
    if (v > vmax) vmax = v;
  }
					// p2p after the signal
  Channel->fPar[2] = vmin;    //
  Channel->fPar[3] = vmax;    //
//-----------------------------------------------------------------------------
// don't make assumptions about the signal polarity
// find channel corresponding to the waveform maximum
//-----------------------------------------------------------------------------
  vmax     = -1.e6;
  int cmax = -1;
  for (int i=0; i<ns; i++) {
    v = (Channel->V0(i)-pedestal)*Calib->fPolarity;
    if (v > vmax) {
      vmax = v;
      cmax = i;				// sample corresponding to the waveform maximum
    }
					// v1 - baseline subtracted, positive
    Channel->SetV1(i,v);
  }
//-----------------------------------------------------------------------------
// try to determine the waveform T0
//-----------------------------------------------------------------------------
  found    =  0;
  min_cell = -1;
  max_cell = -1;

  for (int i=cmax; i>=0; i--) {
    if (Channel->V1(i) > 0.9*vmax) {
      max_cell = i;
    }
    else if (Channel->V1(i) < 0.1*vmax) {
      min_cell = i; 
      found    = 1;
      break;
    }
  }
    
  if (found == 0) {
    fPedError    = 1;
    _nErrors    += 1;
    if (_nErrors >= _printErrorThreshold) {
      GetHeaderBlock()->Print(Form("SubtractPedestals ERROR: %8i Couldnt find MIN_CELL for channel %i",
				     _nErrors, Channel->ID()));
      _printErrorThreshold = _printErrorThreshold*2;
    }
  }
//-----------------------------------------------------------------------------
// approximate T0, then find the first maximum ... which is not used so far...
//-----------------------------------------------------------------------------
  slope = (Channel->V1(max_cell)-Channel->V1(min_cell))/(max_cell-min_cell);
  t0    = min_cell-Channel->V1(min_cell)/slope;

  Channel->SetT0(t0);
//-----------------------------------------------------------------------------
// recalculate pedestal using [0,T0-20] samples interval
//-----------------------------------------------------------------------------
  max_cell = int(t0-20);
  if (min_cell <= 0) max_cell = 0;
  if (max_cell > ns) max_cell = ns;
  min_cell = 0;
//-----------------------------------------------------------------------------
// FitPol0 returns chi2/ndof
//-----------------------------------------------------------------------------
  TGaasUtils::FitPol0(&Channel->V0(0),min_cell,max_cell,&mean,&chi2);
  Channel->SetPedestal(mean);
  Channel->SetChi2Ped (chi2);
  int npt = max_cell-min_cell;
  Channel->SetNptPed  (npt);
//-----------------------------------------------------------------------------
// redefine V1, using real pedestal - fitted for a given event or the mean
// 2012-12-27: use fitted value
//-----------------------------------------------------------------------------
  vmax = -1.e6;
  cmax = -1;

  float v0max = -1.e6;
  int   c0max = -1;
  
  for (int i=0; i<ns; i++) {
    v = (Channel->V0(i)-mean)*Calib->fPolarity;
    if (v > vmax) {
      vmax = v;
      cmax = i;
    }

    float v0_plus = Channel->V0(i)*Calib->fPolarity;
    if (v0_plus > v0max) {
      v0max = v0_plus;
      c0max = i;
    }
    
    Channel->SetV1(i,v);
  }

  Channel->SetV0Max(v0max);
  Channel->SetI0Max(c0max);

  Channel->SetV1Max(vmax);
  Channel->SetI1Max(cmax);
//-----------------------------------------------------------------------------
// fill histograms
//-----------------------------------------------------------------------------
  Channel->fHist.fV0->Reset();
  Channel->fHist.fV1->Reset();
  Channel->fHist.fShape->Reset();

  for (int cell=0; cell<ns; cell++) {
    y = Channel->V0(cell);
    Channel->fHist.fV0->SetBinContent(cell+1,y);
    
    y  = Channel->V1(cell);
    ey = _PedSigma*1.5+0.05*y;

    Channel->fHist.fV1->SetBinContent(cell+1,y);
    Channel->fHist.fV1->SetBinError  (cell+1,ey);

    y = Channel->V1(cell)/fabs(vmax);
    Channel->fHist.fShape->SetBinContent(cell+1,y);
    Channel->fHist.fShape->SetBinError  (cell+1,ey/fabs(vmin));
  }
//-----------------------------------------------------------------------------
// integrate charge (for non-trigger pulses)
// poor-man's calib DB
//-----------------------------------------------------------------------------
//  min_cell = (int) (t0-10);
//  min_cell = Calib->fS1;
  min_cell = t0;
  if (min_cell < 0) min_cell = 0;
  max_cell = (int) (t0+Calib->fPulseIntWindow);
  if (max_cell > ns) max_cell=ns;

  q = 0;
  for (int cell=min_cell; cell<max_cell; cell++) {
    v = Channel->V1(cell);
    q += v;
  }
//-----------------------------------------------------------------------------
// account for calibrations and convert into picocoulombs
//-----------------------------------------------------------------------------
  float _QScale ( 1.);
  
  q  = q/_QScale;
  Channel->SetQ(q);
  q1 = q*Calib->fGain;
  Channel->SetQ1(q1);

  return 0;
};


//-----------------------------------------------------------------------------
int TGaasRecoModule::ProcessChannels() {

  int ns  = fGaasDataBlock->GetNSamples();
  int nch = fGaasDataBlock->GetNChannels();

  for (int ich=0; ich<nch; ich++) {
    TReadoutChannel* ch = fChannel[ich];

    vector<float>* t  = ch->GetT();
    vector<float>* v0 = ch->GetV0();
  
    ch->SetNSamples(ns);

    int cid = fGaasDataBlock->ChannelID(ich);
    ch->SetID(cid);
//-----------------------------------------------------------------------------
// convert V --> mV, T is still in units of channels 
//-----------------------------------------------------------------------------
    for (int i=0; i<ns; i++) {
      (*t )[i] = fGaasDataBlock->T(i);
      (*v0)[i] = fGaasDataBlock->V(ich,i)*1.e3;  // V --> mV
    }

    ReconstructChannel(ch,fCalibData->GetChannel(cid));
  }

  return 0;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int TGaasRecoModule::Event(int ientry) {

  fEventNumber = GetHeaderBlock()->EventNumber();

  fGaasDataBlock->GetEntry(ientry);
  fPedError = 0;

  ProcessChannels();

  if (fFillHistograms) FillHistograms();

  Debug();

  return 0;		       
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void TGaasRecoModule::Debug() {

  //  char        text[500];
  int nch = fGaasDataBlock->GetNChannels();
//-----------------------------------------------------------------------------
// bit 0: All Events
//-----------------------------------------------------------------------------
  if (GetDebugBit(2) == 1) {
    
    for (int i=0; i<nch; i++) {
      TGaasCalibChannel* cch = fCalibData->GetChannel(i);
      
      float p2p1 = fChannel[i]->fPar[1]-fChannel[i]->fPar[0];
      float p2p2 = fChannel[i]->fPar[3]-fChannel[i]->fPar[2];
    
      if ((p2p1 > cch->fMaxP2P) || (p2p2 > cch->fMaxP2P)) {
	GetHeaderBlock()->Print(Form("TGaasRecoModule:bit002: channel: %4i background: p2p > %f",i,cch->fMaxP2P));
      }
    }
  }

  if (GetDebugBit(3) == 1) {
    for (int i=0; i<nch; i++) { 
      if (fChannel[i]->fQ > -0.020) {
	GetHeaderBlock()->Print(Form("TGaasRecoModule:bit003: channel: %4i q = %10.5f",
				     i,fChannel[i]->fQ));
      }
    }
  }
  
}

//-----------------------------------------------------------------------------
int TGaasRecoModule::EndJob() {
  printf("----- end job: ---- %s\n",GetName());

  return 0;
}
