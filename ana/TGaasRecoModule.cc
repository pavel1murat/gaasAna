//////////////////////////////////////////////////////////////////////////////
//
// use of tmp:
//
// use of debug bits: bits 0-2 are reserved
//  0  : all events
//  1  : passed events
//  2  : rejected events
//  3  :
//  4  : interactive scan of low pulse height events
//  5  : used for something
//  6  : events with the LE slope < 0.03 and width > 25
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

  fV2Max[0]       = 0.7; // apparently, mV on input
  fV2Max[1]       = 1.5; // apparently, mV on input
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
// assume that all channels have the same time sampling
//-----------------------------------------------------------------------------
void TGaasRecoModule::BookChannelHistograms(HistBase_t* HistR, const char* Folder) {
//   char name [200];
//   char title[200];

  ChannelHist_t* Hist =  (ChannelHist_t*) HistR;

  // 'ns' is the number of samples, not nanosecond
  
  int ns = fCalibData->GetChannel(Hist->fChannelID)->NSamples();

  HBook1F(Hist->fNSamples    ,"nsamples","nsamples",100      ,  0,  1000, Folder);
  HBook2F(Hist->fWaveform[0] ,"wf_0"    ,"wf_0[mV]", ns      ,  0,   ns,  4000, -200,  200,Folder);
  HBook2F(Hist->fWaveform[1] ,"wf_1"    ,"wf_1[mV]", ns      ,  0,   ns,  5000,-2000, 8000,Folder);
  HBook2F(Hist->fWf2         ,"wf2"     ,"wf2[mV]" , ns      ,  0,   ns,  1000, -1,      4,Folder);

  float tmax = fCalibData->GetChannel(Hist->fChannelID)->SamplingTime()*ns;
  
  HBook2F(Hist->fWf2ns       ,"wf2_ns"  ,"wf2[mV]"        ,  ns      ,  0, tmax,  1000, -1,      4,Folder);
						            
  HBook1F(Hist->fLastWaveform,"last_wf" ,"last_wf"        ,  ns      ,  0,   ns,  Folder);
  HBook1F(Hist->fQ[0]        ,"q_0"     ,"Q_0"            ,  500     ,  0,  500,  Folder);
  HBook1F(Hist->fQ[1]        ,"q_1"     ,"Q_1"            ,  500     ,  0, 2500,  Folder);
  HBook1F(Hist->fQ1[0]       ,"q1_0"    ,"Q1_0"           ,  250     ,  0,   50, Folder);
  HBook1F(Hist->fQ1[1]       ,"q1_1"    ,"Q1_1"           ,  250     ,  0,   50, Folder);
  HBook1F(Hist->fQPc         ,"q_pc"    ,"Q[Pc]"          ,  250     ,  0, 0.05, Folder);
  HBook1F(Hist->fV0Max[0]    ,"v0max_0" ,"V0Max[0]"       ,  500     ,  0,  250,  Folder);
  HBook1F(Hist->fV0Max[1]    ,"v0max_1" ,"V0Max[1]"       ,  200     ,  0,10000,  Folder);
  HBook1F(Hist->fV1Max[0]    ,"v1max_0" ,"V1Max[0]"       ,  500     ,  0,  250,  Folder);
  HBook1F(Hist->fV1Max[1]    ,"v1max_1" ,"V1Max[1]"       ,  200     ,  0,10000,  Folder);
  HBook1F(Hist->fV2Max       ,"v2max"   ,"V2Max"          ,  500     ,  0,    5,  Folder);
  HBook1F(Hist->fT0          ,"t0"      ,"T0"             ,  400     ,100,  300,  Folder);
  HBook1F(Hist->fLeSlope     ,"le_slope","LE slope"       ,  500     ,  0,  0.5,  Folder);
  HBook1F(Hist->fTeSlope     ,"te_slope","TE slope"       ,  200     , -0.2,  0,  Folder);
  HBook1F(Hist->fWidth       ,"width"   ,"Width, channels",  100     ,  0,  100,  Folder);
							     
  HBook1F(Hist->fWidthNs     ,"width_ns","Width, ns"      ,  100     ,  0, 5e-9,  Folder);

  HBook2F(Hist->fW_v_H       ,"w_v_v2"  ,"W_v_V2"  ,1000     ,  0,    2, 100,0,100,Folder);
  HBook1F(Hist->fPedestal[0] ,"ped_0"   ,"Ped_0"   ,500      ,-1e-3,1e-3, Folder);
  HBook1F(Hist->fPedestal[1] ,"ped_1"   ,"Ped, mV" ,500      ,-25,   25,  Folder);
  HBook1F(Hist->fSigmaPed[0] ,"sigped_0","SigPed_0",200      ,  0,    2,  Folder);
  HBook1F(Hist->fSigmaPed[1] ,"sigped_1","SPed, mV",200      ,  0,  200,  Folder);
  HBook1F(Hist->fP2P1[0]     ,"p2p1_0"  ,"P2P1_0"  ,200      ,  0,   10,  Folder);
  HBook1F(Hist->fP2P1[1]     ,"p2p1_1"  ,"P2P1_1"  ,200      ,  0, 1000,  Folder);
  HBook1F(Hist->fP2P2[0]     ,"p2p2_0"  ,"P2P2_0"  ,200      ,  0,   10,  Folder);
  HBook1F(Hist->fP2P2[1]     ,"p2p2_1"  ,"P2P2_1"  ,200      ,  0, 1000,  Folder);
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
      
      channel_selection[     i] = new TString("all channels");
      channel_selection[ 100+i] = new TString("P2P < 0.0040");                 // peak to peak
      channel_selection[ 200+i] = new TString("P2P >= 0.0040");                // peak to peak
      channel_selection[ 300+i] = new TString("Q < fV2Max");                   // charge
      channel_selection[ 400+i] = new TString("fV2Max[0] <= Q < fV2Max[1]");   // charge
      channel_selection[ 500+i] = new TString("Q >= fV2Max[1]");               // charge
      channel_selection[ 600+i] = new TString("LeSlope < 0.03" );              // leading slope
      channel_selection[ 700+i] = new TString("0.03 <= LeSlope < 0.09" );      // leading slope
      channel_selection[ 800+i] = new TString("LeSlope >= 0.09" );             // leading slope
      channel_selection[ 900+i] = new TString("Q <= 1.76" );                   // charge
      channel_selection[1000+i] = new TString("1.76 < Q < 7.04 " );            // charge
      channel_selection[1100+i] = new TString("Q >= 7.04" );                   // charge

      channel_id       [     i] = cid; 
      channel_id       [ 100+i] = cid;
      channel_id       [ 200+i] = cid;
      channel_id       [ 300+i] = cid;
      channel_id       [ 400+i] = cid;
      channel_id       [ 500+i] = cid;
      channel_id       [ 600+i] = cid;
      channel_id       [ 700+i] = cid;
      channel_id       [ 800+i] = cid;
      channel_id       [ 900+i] = cid;
      channel_id       [1000+i] = cid;
      channel_id       [1100+i] = cid;
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

  int   ns = Channel->GetNSamples();
  float dt = Channel->SamplingTime();

  Hist->fNSamples->Fill(ns);
  Hist->fQ [0]->Fill(Channel->Q ());	// charge, pedestal subtracted
  Hist->fQ [1]->Fill(Channel->Q ());
  Hist->fQ1[0]->Fill(Channel->Q1());	// charge, re-calculated to input
  Hist->fQ1[1]->Fill(Channel->Q1());
//-----------------------------------------------------------------------------
// convert charge to pico-coulombs
//-----------------------------------------------------------------------------
  const float Rinp(50.); // 50 Ohm
  
  float qpc = Channel->Q1()*1e-3/Rinp*dt/1e-12;
  Hist->fQPc->Fill(qpc);

  Hist->fT0->Fill(Channel->T0());
  Hist->fLeSlope->Fill(Channel->LeSlope());
  Hist->fTeSlope->Fill(Channel->TeSlope());
  Hist->fWidth->Fill(Channel->Width());

  float width_ns = (Channel->Width()+0.5)*dt;
  
  Hist->fWidthNs->Fill(width_ns);
  
  Hist->fV0Max[0]->Fill(Channel->V0Max());
  Hist->fV0Max[1]->Fill(Channel->V0Max());
  Hist->fV1Max[0]->Fill(Channel->V1Max());
  Hist->fV1Max[1]->Fill(Channel->V1Max());
  Hist->fV2Max->Fill(Channel->V2Max());
					// N(points) >> 1, do not correct for that
  
  float sig_ped = sqrt(Channel->Chi2Ped());
  Hist->fSigmaPed[0]->Fill(sig_ped);
  Hist->fSigmaPed[1]->Fill(sig_ped);
  Hist->fPedestal[0]->Fill(Channel->Pedestal());
  Hist->fPedestal[1]->Fill(Channel->Pedestal());

  // voltage P2P in "pedestal" regions
  
  float p2p_1 = Channel->fPar[1]-Channel->fPar[0];
  float p2p_2 = Channel->fPar[3]-Channel->fPar[2];
  
  Hist->fP2P1[0]->Fill(p2p_1);
  Hist->fP2P1[1]->Fill(p2p_1);

  Hist->fP2P2[0]->Fill(p2p_2);
  Hist->fP2P2[1]->Fill(p2p_2);

  Hist->fLastWaveform->Reset();

  for (int i=0; i<ns; i++) {
    float v = Channel->V0(i);
    float x = i+1+0.5;
    Hist->fWaveform[0] ->Fill(x,v);
    Hist->fWaveform[1] ->Fill(x,v);

    Hist->fWf2         ->Fill(x   ,Channel->V2(i));

    float time         = x*dt;
    Hist->fWf2ns       ->Fill(time,Channel->V2(i));
    
    Hist->fLastWaveform->SetBinContent(i+1,v);
    Hist->fLastWaveform->SetBinError  (i+1,0);
  }
  
  Hist->fW_v_H->Fill(Channel->V2Max(),Channel->Width());

}


//-----------------------------------------------------------------------------
// returns 1 for good pulse
//-----------------------------------------------------------------------------
int TGaasRecoModule::GoodPulse(TReadoutChannel* Ch, TGaasCalibChannel* Calib) {
  int ok(0);

  float p2p_1 = Ch->fPar[1]-Ch->fPar[0];
  float p2p_2 = Ch->fPar[3]-Ch->fPar[2];
    
  if ((p2p_1                <  Calib->fMaxP2P) && 
      (p2p_2                <  Calib->fMaxP2P) && 
      (fabs(Ch->Pedestal()) <  Calib->fMaxThr) && 
      (Ch->Q1()             >= Calib->MinQ() )    ) {
    ok = 1;
  }

  return ok;
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
      
    int good_pulse = GoodPulse(ch,cch);

    if (good_pulse) FillChannelHistograms(fHist.fChannel[100+cid], ch);
    else            FillChannelHistograms(fHist.fChannel[200+cid], ch);

    float v2max = ch->V2Max();
    float width = ch->Width();
    
    if (good_pulse) {
//-----------------------------------------------------------------------------
// determine pulse type:
// wide pulses: type = 1. narrow ones: type=2
// if width is not analyzed, small pulses are type = 1; higher ones : type=2
//                           type=3: very large pulses
//-----------------------------------------------------------------------------
      int pulse_type(-1);

      if (cch->MinWidth() > 0) {
	if (width >= cch->MinWidth()) pulse_type = 1;
	else                          pulse_type = 2;
      }
      else {
	float min_ph = -cch->MinWidth();
	if      (v2max  <  min_ph)                              pulse_type = 1;
	else                                                    pulse_type = 2;

	//	else if (v2max  >= fV2Max[1])                           pulse_type = 3;
      }


      if      (pulse_type == 1) FillChannelHistograms(fHist.fChannel[300+cid], ch);
      else if (pulse_type == 2) FillChannelHistograms(fHist.fChannel[400+cid], ch);
      else if (pulse_type == 3) FillChannelHistograms(fHist.fChannel[500+cid], ch);
//-----------------------------------------------------------------------------
// look at the waveforms with different LE slopes
//-----------------------------------------------------------------------------
      if (ch->fLeSlope < 0.03) { 
	FillChannelHistograms(fHist.fChannel[600+cid], ch);
	
	if (GetDebugBit(6) == 1) {
	  if (ch->Width() > 25) {
	    GetHeaderBlock()->Print(Form("bit006: LE slope = %10.3f width = %10.3f",
					 ch->LeSlope(),ch->Width()));
	  }
	}
      }
      else if ((ch->fLeSlope >= 0.03) && (ch->fLeSlope < 0.09)) {
	FillChannelHistograms(fHist.fChannel[700+cid], ch);
      }
      else FillChannelHistograms(fHist.fChannel[800+cid], ch);
    }

    if (GetDebugBit(5) == 1) {
      if ((ch->fQ > 2000.) && (ch->fQ < 8000.)) {
	GetHeaderBlock()->Print(Form("look at the event : Q = %10.3f",ch->fQ));
      }
    }

//-----------------------------------------------------------------------------
// look at the waveforms with different charges
//-----------------------------------------------------------------------------

    if (ch->fQ1 <= 2000/1136.) {
      FillChannelHistograms(fHist.fChannel[900+cid], ch);
    }
    else if ((ch->fQ1 > 2000/1136.) && (ch->fQ1 < 8000/1136.)) {
      FillChannelHistograms(fHist.fChannel[1000+cid], ch);
    }
    else if (ch->fQ1 >= 8000/1136.) {
      FillChannelHistograms(fHist.fChannel[1100+cid], ch);
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

  return 0;
}

//-----------------------------------------------------------------------------
int TGaasRecoModule::ReconstructChannel(TReadoutChannel* Channel, TGaasCalibChannel* Calib) {

  if (Channel->GetUsed() == 0) return 0;

  int ns            = Channel->GetNSamples();
//-----------------------------------------------------------------------------
// at this point can proceed with the waveform analysis and pedestal determination
//-----------------------------------------------------------------------------
  static int  _nErrors            (0);
  static int  _printErrorThreshold(0);
  
  double    sum, qn, q, q1, v, vmin, vmax, pedestal, slope, t0, t1, y, ey, mean, chi2;
  double    _PedSigma ; // , ped_mean;
  int       found, min_cell, max_cell, min_cell_05, max_cell_05;
  float     width;
//-----------------------------------------------------------------------------
// determine pedestal
//-----------------------------------------------------------------------------
  Channel->SetQ (0.);
  Channel->SetQ1(0.);
  Channel->SetSamplingTime(Calib->SamplingTime());
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
    Channel->SetV2(i,v/Calib->fGain);
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

  for (int i=cmax; i>=0; i--) {
    if (Channel->V1(i) > 0.5*vmax) {
      min_cell_05 = i;
    }
    else break;
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
  slope = slope/Calib->fGain;
  Channel->SetLeSlope(slope);
//-----------------------------------------------------------------------------
// recalculate pedestal using [T0-60,T0-10] samples interval
//-----------------------------------------------------------------------------
  min_cell = int(t0-60);
  if (min_cell <= 0) min_cell=0;
  max_cell = min_cell+50;
  if (max_cell > 1023) max_cell = 1023;
//-----------------------------------------------------------------------------
// FitPol0 returns chi2/ndof
//-----------------------------------------------------------------------------
  TGaasUtils::FitPol0(&Channel->V0(0),min_cell,max_cell,&mean,&chi2);
  Channel->SetPedestal(mean);
  Channel->SetChi2Ped (chi2);
  int npt = max_cell-min_cell+1;
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
    Channel->SetV2(i,v/Calib->fGain);
  }

  Channel->SetV0Max(v0max);
  Channel->SetI0Max(c0max);

  Channel->SetV1Max(vmax);
  Channel->SetV2Max(vmax/Calib->fGain);
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
// calculate trailing edge slope
// set upper bound of integration using linear fit
//-----------------------------------------------------------------------------

  for (int i=cmax; i<=ns; i++) {
    if (Channel->V1(i) > 0.9*vmax) {
      max_cell = i; 
    }
    else if (Channel->V1(i) < 0.1*vmax) {
      min_cell = i;
      break;
    }
  }

  for (int i=cmax; i<=ns; i++) {
    if (Channel->V1(i) > 0.5*vmax) {
      max_cell_05 = i; 
    }
    else break;
  }

  slope = (Channel->V1(max_cell)-Channel->V1(min_cell))/(max_cell-min_cell);
  t1    = max_cell-Channel->V1(max_cell)/slope;

  slope = slope/Calib->fGain;
  Channel->SetTeSlope(slope);

  width = max_cell_05-min_cell_05;
  Channel->SetWidth(width);
//-----------------------------------------------------------------------------
// integrate charge (for non-trigger pulses)
// poor-man's calib DB
//-----------------------------------------------------------------------------
  min_cell = t0;
  if (min_cell < 0) min_cell = 0;

//-----------------------------------------------------------------------------
// if Calib->fPulseIntWindow > 0, use fixed length integration window, 
// otherwise integrate till the waveform crosses zero
//-----------------------------------------------------------------------------
  if (Calib->fPulseIntWindow > 0) max_cell = (int) (t0+Calib->fPulseIntWindow);
  else                            max_cell = t1;

  if (max_cell > ns) max_cell=ns;

  q = 0;
  for (int cell=min_cell; cell<max_cell; cell++) {
    float v1 = Channel->V1(cell);
    q += v1;
  }
//-----------------------------------------------------------------------------
// account for calibrations and convert charge into pC assuming 50 Ohm load
// q : charge on input of the scope
// q1: charge on input of the first amplifier
// remember, that the voltage is currently in mV, so need to convert it to volts
// do the conversion when filling the histograms - cuts depend on the current definition
//-----------------------------------------------------------------------------
//  q  = q/50*Calib->SamplingTime()/1.e-12*1.e-3;
  Channel->SetQ(q);
				// "charge" converted to "mV on input"
  q1 = q/Calib->fGain;
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
