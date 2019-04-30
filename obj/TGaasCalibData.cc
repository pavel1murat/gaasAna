///////////////////////////////////////////////////////////////////////////////
// calibration: Ch[i] = Offset[i] + Gain[i]*V
///////////////////////////////////////////////////////////////////////////////

#include "gaasAna/obj/TGaasCalibData.hh"
#include "Stntuple/base/TCalibManager.hh"
#include "Stntuple/base/TCalibRunRange.hh"

ClassImp(TGaasCalibData)

//_____________________________________________________________________________
TGaasCalibData::TGaasCalibData() {
//-----------------------------------------------------------------------------
// initialize the look up tables to indicate all the channels are good
//-----------------------------------------------------------------------------
  fNChannels = 4;
  Clear();
}

//_____________________________________________________________________________
TGaasCalibData::~TGaasCalibData() {
}

//-----------------------------------------------------------------------------
int TGaasCalibData::InitReadoutMap(int RunNumber, TCalibManager* Manager) {
  int   rc(0);

  FILE            *f;
  int             done(0);
  char            c[1000], calib_name[100];
  TCalibRunRange  *crr(0);

  sprintf(calib_name,"readout_map");

  crr = Manager->GetRunRange("gaas",calib_name,RunNumber);
  if (crr == 0) {
    Error("Init",Form("missing Gaas %s table for run number %8i\n",
		      calib_name,RunNumber));
    return -1;
  }
					// check if the same run range
  if (fCalibRunRange == crr) return 0;
  fCalibRunRange = crr;
					// new run range, need to read
  f  = fopen(crr->GetFilename(),"r");
  if (f == 0) {
    Error("Init",Form("missing file %s\n",crr->GetFilename()));
    return -2;
  }

  int     ich, used, polarity, ns, min_s0, max_s0, min_s1, max_s1, s1, win ;
  float   max_p2p, max_thr, gain;

  fNChannels = 0;
  
  while ( ((c[0]=getc(f)) != EOF) && !done) {
					// check if it is a comment line
    if (c[0] != '#') {
      ungetc(c[0],f);
					// parse line
      fscanf(f,"%i" ,&ich      );
      fscanf(f,"%i" ,&used     );
      fscanf(f,"%i" ,&ns       );
      fscanf(f,"%i" ,&polarity );
      fscanf(f,"%i" ,&min_s0   );
      fscanf(f,"%i" ,&max_s0   );
      fscanf(f,"%i" ,&min_s1   );
      fscanf(f,"%i" ,&max_s1   );
      fscanf(f,"%f" ,&max_p2p  );
      fscanf(f,"%f" ,&max_thr  );
      fscanf(f,"%i" ,&s1       );
      fscanf(f,"%i" ,&win      );
      fscanf(f,"%f" ,&gain     );
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
      fChannel[fNChannels].fChannelID      = ich;
      fChannel[fNChannels].fUsed           = used;
      fChannel[fNChannels].fNSamples       = ns;
      fChannel[fNChannels].fPolarity       = polarity;
      fChannel[fNChannels].fMinSample[0]   = min_s0;
      fChannel[fNChannels].fMaxSample[0]   = max_s0;
      fChannel[fNChannels].fMinSample[1]   = min_s1;
      fChannel[fNChannels].fMaxSample[1]   = max_s1;
      fChannel[fNChannels].fMaxP2P         = max_p2p;
      fChannel[fNChannels].fMaxThr         = max_thr;
      fChannel[fNChannels].fS1             = s1;
      fChannel[fNChannels].fPulseIntWindow = win;
      fChannel[fNChannels].fGain           = gain;
      
      fNChannels++;
    }
					// skip line
    fgets(c,100,f);
  }

  fclose(f);

  return rc;
}

//-----------------------------------------------------------------------------
int TGaasCalibData::Init(int RunNumber, TCalibManager* Manager) {
//-----------------------------------------------------------------------------
// read calibrations for a given run
//-----------------------------------------------------------------------------
  int rc(0);

  rc = InitReadoutMap(RunNumber,Manager);
  return rc;
}

//_____________________________________________________________________________
void TGaasCalibData::Clear(Option_t* Opt) {
  fCalibRunRange = 0;
}

//_____________________________________________________________________________
void TGaasCalibData::Print(Option_t* opt) const {
  Error("Print","not implemented yet");
}
  

