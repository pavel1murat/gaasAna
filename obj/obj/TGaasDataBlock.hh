#ifndef TGaasDataBlock_hh
#define TGaasDataBlock_hh

#include "TVectorT.h"
#include "TMatrixT.h"

#include "Stntuple/obj/TStnDataBlock.hh"

// class AbsEvent;
// class TGaas;

class TGaasDataBlock: public TStnDataBlock {
public:
  
//   friend Int_t TGaas::InitVme1742DataBlock(TStnDataBlock* block, 
// 					      AbsEvent*      event, 
// 					      int            mode);
  enum {
	kNFreeInts   =  5,                    // put in starting right from version 1
	kNFreeFloats =  5
  };

protected:

  int             fNSamples;		// number of samples in the waveform
  int             fNChannels;		// number of channels
  int             fTriggerSample;       // sample on which the scope triggered
  int             fInt  [kNFreeInts  ]; // provision for future I/O expansion
  float           fSampleTime;		// sampling time, sec
  float           fTriggerTime;	        // not sure what exactly this is, but may be useful
  float           fFloat[kNFreeFloats]; // provision for future I/O expansion
  
  TVectorT<float> fT;			// time samples
  TMatrixT<float> fV;			// data: [nchannels][nsamples]
//-----------------------------------------------------------------------------
//  functions
//-----------------------------------------------------------------------------
public:
					// ****** constructors and destructor
  TGaasDataBlock();
  virtual ~TGaasDataBlock();
					// ****** accessors

  int       GetNSamples     () { return fNSamples;   }
  int       GetNChannels    () { return fNChannels;  }
  float     GetSampleTime   () { return fSampleTime; }
  int       GetTriggerSample() { return fTriggerSample; }
  float     GetTriggerTime  () { return fTriggerTime;   }
  TVectorT<float>* GetT     () { return &fT;            }
  TMatrixT<float>* GetV     () { return &fV;            }
//-----------------------------------------------------------------------------
// modifiers
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// overloaded methods of TObject
//-----------------------------------------------------------------------------
  void    Clear(Option_t* opt = "");
  void    Print(Option_t* opt = "") const;

  ClassDef(TGaasDataBlock,1)	// oscilloscope data block
};

#endif
