///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#ifndef __gaasAna_obj_TReadoutChannel_hh__
#define __gaasAna_obj_TReadoutChannel_hh__

#include <vector>
#include "TObject.h"

class TH1F;

using std::vector;

class TReadoutChannel : public TObject {
public:
  int            fID;		        // channel number (or channel ID)
  int            fNSamples;
  int            fUsed;

  vector<float>  fT;
  vector<float>  fV0;
  vector<float>  fV1;			// corrected for pedestal;

  float          fQ;			// 
  float          fQ1;			//
  float          fPedestal;             //
  float          fChi2Ped;              //

  float          fV1Min;                // minimal V1 value for the whole waveform
  int            fI1Min;                // sample corresponding to minimal V1
  float          fT0;			//

  struct Hist_t {
    TH1F*        fV0;
    TH1F*        fV1;
    TH1F*        fShape;
  } fHist;

  float          fPar[100];             // for development
public:
//-----------------------------------------------------------------------------
// constructors and destructor
//-----------------------------------------------------------------------------
  TReadoutChannel();
  TReadoutChannel(int ID, int NSamples);
  ~TReadoutChannel();
//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------
  int           GetNSamples() { return fNSamples; }
  int           GetUsed    () { return fUsed;     }
  int           ID         () { return fID;       }

  vector<float>* GetT      ()  { return &fT ; }
  vector<float>* GetV0     ()  { return &fV0; }
  vector<float>* GetV1     ()  { return &fV1; }

  float&          T (int I) { return fT [I]; }
  float&          V0(int I) { return fV0[I]; }
  float&          V1(int I) { return fV1[I]; }
  float           Q       () { return fQ;     }
  float           Q1      () { return fQ1;    }
  float           T0      () { return fT0;    }
  float           Chi2Ped () { return fChi2Ped;  }
  float           Pedestal() { return fPedestal; }
//-----------------------------------------------------------------------------
// setters
//-----------------------------------------------------------------------------
  void SetQ        (float Q)    { fQ        = Q;    }
  void SetQ1       (float Q)    { fQ1       = Q;    }
  void SetPedestal (float P)    { fPedestal = P;    }
  void SetChi2Ped  (float Chi2) { fChi2Ped  = Chi2; }
  void SetV1(int I, float V)    { fV1[I]    = V;    }
  void SetT0       (float T)    { fT0       = T;    }

  void SetV1Min    (float V) { fV1Min    = V; }
  void SetI1Min    (int   I) { fI1Min    = I; }

  void SetMin1     (int I, float V) {fI1Min = I; fV1Min = V; }
  
  void SetNSamples(int NSamples);
  
//-----------------------------------------------------------------------------
// overloaded methods of TObject
//-----------------------------------------------------------------------------
  virtual void Clear(Option_t* Opt); 
  
  ClassDef(TReadoutChannel,1)
};

#endif
