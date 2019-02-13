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
  fNSamples       = 500;                        // should come from the database

  for (int i=0; i<kNChannels; i++) {
    fChannel[i] = new TReadoutChannel(i,fNSamples);
  }
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
void TGaasRecoModule::BookEventHistograms(HistBase_t* HistR, const char* Folder) {
  //  char name [200];
  //  char title[200];

  EventHist_t* Hist =  (EventHist_t*) HistR;

  HBook1F(Hist->fNumber    ,"en"   ,Form("%s: Event Number"      ,Folder),500, 0,500,Folder);
  HBook1F(Hist->fNChannels ,"nch"  ,Form("%s: N(channels"        ,Folder),100, 0,100,Folder);
}

//-----------------------------------------------------------------------------
void TGaasRecoModule::BookChannelHistograms(HistBase_t* HistR, const char* Folder) {
//   char name [200];
//   char title[200];

  ChannelHist_t* Hist =  (ChannelHist_t*) HistR;

  HBook1F(Hist->fNSamples    ,"nsamples","nsamples",100,  0  ,1000. ,Folder);
  HBook2F(Hist->fWaveform    ,"wf"      ,"wf"      ,fNSamples,0,fNSamples,4000,-20,20,Folder);
  HBook1F(Hist->fLastWaveform,"last_wf" ,"last_wf" ,fNSamples,0,fNSamples,Folder);
  HBook1F(Hist->fQ           ,"q"       ,"Q"       ,400      ,-0.2,0.2,Folder);
  HBook1F(Hist->fQ1          ,"q1"      ,"Q1"      ,400      ,-0.2,0.2,Folder);
  HBook1F(Hist->fT0          ,"t0"      ,"T0"      ,200      ,230,250,Folder);
  HBook1F(Hist->fPedestal    ,"ped"     ,"Pedestal",200      ,-1.e-3,1.e-3,Folder);
  HBook1F(Hist->fChi2Ped     ,"chi2ped" ,"Chi2Ped" ,500      , 0, 1.e-5,Folder);
  HBook1F(Hist->fPedP2P      ,"pedp2p"  ,"Ped P2P" ,100      , 0, 10.e-3,Folder);
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
// book track histograms
//-----------------------------------------------------------------------------
  TString*  channel_selection   [kNChannelHistSets];

  for (int i=0; i<kNChannelHistSets; i++) { channel_selection[i] = NULL; }

  channel_selection[  0] = new TString("all channels");
  channel_selection[100] = new TString("P2P < 0.0025");
  channel_selection[200] = new TString("P2P >= 0.0025");

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
  Hist->fNChannels->Fill(fGaasDataBlock->GetNChannels());
}

//-----------------------------------------------------------------------------
// for DIO : ultimately, one would need to renormalize the distribution
//-----------------------------------------------------------------------------
void TGaasRecoModule::FillChannelHistograms(HistBase_t* HistR, TReadoutChannel* Channel) {

  ChannelHist_t* Hist = (ChannelHist_t*) HistR;

  int ns = Channel->GetNSamples();

  Hist->fNSamples->Fill(ns);
  Hist->fQ->Fill(Channel->Q());
  Hist->fQ1->Fill(Channel->Q1());
  Hist->fT0->Fill(Channel->T0());
  Hist->fChi2Ped->Fill(Channel->Chi2Ped());
  Hist->fPedestal->Fill(Channel->Pedestal());

  // pedestal "voltage span"
  
  float p2p = Channel->fPar[1]-Channel->fPar[0];
  
  Hist->fPedP2P->Fill(p2p);

  Hist->fLastWaveform->Reset();
  
  for (int i=0; i<ns; i++) {
    float v = fGaasDataBlock->V(0,i);

    int x = i+0.5;
    Hist->fWaveform    ->Fill(x,v*1.e3);
    Hist->fLastWaveform->Fill(x,v*1.e3);
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

  for (int i=0; i<kNChannels; i++) {
    FillChannelHistograms(fHist.fChannel[i], fChannel[i]);

    float p2p1 = fChannel[i]->fPar[1]-fChannel[i]->fPar[0];
    float p2p2 = fChannel[i]->fPar[3]-fChannel[i]->fPar[2];
    
    if ((p2p1 < 0.0025) && (p2p2 < 0.0025)) FillChannelHistograms(fHist.fChannel[100+i], fChannel[i]);
    else                                    FillChannelHistograms(fHist.fChannel[200+i], fChannel[i]);
  }
}


//-----------------------------------------------------------------------------
int TGaasRecoModule::ReconstructChannel(TReadoutChannel* Channel) {

  int ns = fGaasDataBlock->GetNSamples();

  vector<float>* t  = Channel->GetT();
  vector<float>* v0 = Channel->GetV0();
  //  vector<float>* v1 = Channel->GetV1();
  
  Channel->SetNSamples(ns);

  for (int i=0; i<ns; i++) {
    (*t )[i] = fGaasDataBlock->T(i);
    (*v0)[i] = fGaasDataBlock->V(0,i);
  }
//-----------------------------------------------------------------------------
// at this point can proceed with the waveform analysis and pedestal determination
//-----------------------------------------------------------------------------
  static int  _nReports(0);
  
  double    sum, qn, q, q1, v, vmin, vmax, pedestal, slope, _Gain, t0, y, ey, mean, chi2;
  double    _PedSigma ; // , ped_mean;
  int       cmin, found, min_cell, max_cell;
  int       _PulseIntegrationWindow;
  //  TTofData  *ch1, *ch2, *ch;
//-----------------------------------------------------------------------------
// determine pedestal
//-----------------------------------------------------------------------------
  Channel->SetQ (0.);
  Channel->SetQ1(0.);
  if (Channel->GetUsed() == 0) return 0;
//-----------------------------------------------------------------------------
// get calibration constants
// pulse_integration_window - in units of channels
//-----------------------------------------------------------------------------
  // ich                      = Data->GetChannelID()->GetNumber();
  _Gain                     = 1.; // fCalibData->GetChannelGain(ich);
  _PulseIntegrationWindow   = 30; // (int) (fCalibData->GetPulseWindow(ich)*5);
  // ped_mean                 = fCalibData->GetPedestal(ich);
  _PedSigma                   = 0.01; // fCalibData->GetPedSigma(ich);

  sum = 0;
  qn  = 0;

  int _minSample =   0;
  int _maxSample = 200;

  vmin =  999.;
  vmax = -999.;
  
  for (int i=_minSample; i<_maxSample; i++) {
    v   = Channel->V0(i);
    sum += v;
    qn  += 1;
    if (v < vmin) vmin = v;
    if (v > vmax) vmax = v;
  }

  Channel->fPar[0] = vmin;    //
  Channel->fPar[1] = vmax;    //

  pedestal = sum/qn;
  Channel->SetPedestal(pedestal);

  _minSample = 320;
  _maxSample = 499;

  vmin =  999.;
  vmax = -999.;
  
  for (int i=_minSample; i<_maxSample; i++) {
    v   = Channel->V0(i);
    if (v < vmin) vmin = v;
    if (v > vmax) vmax = v;
  }
					// p2p trailing
  Channel->fPar[2] = vmin;    //
  Channel->fPar[3] = vmax;    //
//-----------------------------------------------------------------------------
// assume V0 negative signals, in mV
//-----------------------------------------------------------------------------
  vmin = 999.;
  cmin = -1;
  for (int i=0; i<fNSamples; i++) {
    v = Channel->V0(i)-pedestal;
    if (v < vmin) {
      vmin = v;
      cmin = i;
    }
    
    Channel->SetV1(i,v);
  }

  Channel->SetMin1(cmin,vmin);
//-----------------------------------------------------------------------------
// look, approximately, for the first maximum
//-----------------------------------------------------------------------------
  found    = 0;
  min_cell = -1;
  max_cell = -1;

  float  _minThreshold (-3.0e-3) ; // in mV
  
  for (int i=0; i<fNSamples; i++) {
    if (Channel->V1(i) < _minThreshold) {
      min_cell = i; 
      found    = 1;
      break;
    }
  }
    
  if (found == 0) {
    if (_nReports <= 20) {
      Error("SubtractPedestals",Form("Couldnt find MIN_CELL for channel %i",
				     Channel->ID()));
      _nReports++;
    }
  }

//-----------------------------------------------------------------------------
// approximate T0, then find the first maximum ... which is not used so far...
//-----------------------------------------------------------------------------
  slope = (Channel->V1(min_cell+2)-Channel->V1(min_cell-2))/4.;
  t0    = min_cell-Channel->V1(min_cell)/slope;

  Channel->SetT0(t0);
                                                  
  for (int i=min_cell; i<fNSamples-1; i++) {
    max_cell = i;
    if (Channel->V1(i+1) < Channel->V1(i)) { 
      Channel->SetMin1(i,Channel->V1(i));
      break;
    }
  }
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
//-----------------------------------------------------------------------------
// redefine V1, using real pedestal - fitted for a given event or the mean
// 2012-12-27: use fitted value
//-----------------------------------------------------------------------------
  vmin = 999.;
  cmin = -1;
  for (int i=0; i<fNSamples; i++) {
    v = (Channel->V0(i)-mean);
    if (v < vmin) {
      vmin = v;
      cmin = i;
    }
    
    Channel->SetV1(i,v);
  }

  Channel->SetV1Min(vmin);
  Channel->SetI1Min(cmin);
//-----------------------------------------------------------------------------
// fill histograms
//-----------------------------------------------------------------------------
  Channel->fHist.fV0->Reset();
  Channel->fHist.fV1->Reset();
  Channel->fHist.fShape->Reset();

  for (int cell=0; cell<fNSamples; cell++) {
    y = Channel->V0(cell);
    Channel->fHist.fV0->SetBinContent(cell+1,y);
    
    y  = Channel->V1(cell);
    ey = _PedSigma*1.5+0.05*y;

    Channel->fHist.fV1->SetBinContent(cell+1,y);
    Channel->fHist.fV1->SetBinError  (cell+1,ey);

    y = Channel->V1(cell)/fabs(vmin);
    Channel->fHist.fShape->SetBinContent(cell+1,y);
    Channel->fHist.fShape->SetBinError  (cell+1,ey/fabs(vmin));
  }
//-----------------------------------------------------------------------------
// integrate charge (for non-trigger pulses)
//-----------------------------------------------------------------------------
//  min_cell = (int) (t0-10);
  min_cell = (int) (t0-5);
  if (min_cell < 0) min_cell = 0;
  max_cell = (int) (t0+_PulseIntegrationWindow);
  if (max_cell > 1024) max_cell=1024;

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
  q1 = q*_Gain;
  Channel->SetQ1(q1);

  return 0;
};


//-----------------------------------------------------------------------------
int TGaasRecoModule::ProcessChannels() {

  for (int i=0; i<kNChannels; i++) {
    ReconstructChannel(fChannel[i]);
  }

  return 0;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int TGaasRecoModule::Event(int ientry) {

  fEventNumber = GetHeaderBlock()->EventNumber();

  fGaasDataBlock->GetEntry(ientry);

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
