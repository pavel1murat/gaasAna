#ifndef TGaasHeaderBlock_hh
#define TGaasHeaderBlock_hh

#include "Stntuple/obj/TStnDataBlock.hh"

// class AbsEvent;
// class TGaas;

class TGaasHeaderBlock: public TStnDataBlock {
public:

  friend class InitGaasHeaderBlock;
  
//   friend Int_t TGaas::InitVme1742DataBlock(TStnDataBlock* block, 
// 					      AbsEvent*      event, 
// 					      int            mode);

  enum {
	kNFreeInts   =  5,                    // put in starting right from version 1
	kNFreeFloats =  5
  };

protected:
  Int_t             fVersion;
  Int_t             fEventNumber;
  Int_t             fRunNumber;
  Int_t             fSubrunNumber;	  // run section number within the run
  Int_t             fMcFlag;		  // MC flag, 0 for real data
  Int_t             fGoodRun;		  // run flag
  Int_t             fBrCode;		  // 
  Int_t             fGoodTrig;
  Int_t             fTrigWord;		  // 
  Int_t             fCpu;                 // packed word with processing time
  int               fInt  [kNFreeInts  ]; // provision for future I/O expansion
  float             fFloat[kNFreeFloats]; // provision for future I/O expansion
  TString           fStnVersion;          // STNTUPLE version, like "dev_243_16"
//-----------------------------------------------------------------------------
//  functions
//-----------------------------------------------------------------------------
public:
					// ****** constructors and destructor
  TGaasHeaderBlock();
  virtual ~TGaasHeaderBlock();
					// ****** accessors

  Int_t  EventNumber  () const { return fEventNumber;   }
  Int_t  RunNumber    () const { return fRunNumber;     }
  Int_t  SubrunNumber () const { return fSubrunNumber;  }
  Int_t  McFlag       () const { return fMcFlag;        }

  Float_t CpuTime     () const { return ((fCpu>>8)/10.0);   } // in s
  Float_t CpuSpeed    () const { return ((fCpu&0xFF)/5.0); } // in GHz
  const TString& StnVersion () const { return fStnVersion;    }
//-----------------------------------------------------------------------------
// modifiers
//-----------------------------------------------------------------------------
					// use free space to save the run number
					// not implemented yet
  void    SetRunNumber(int RunNumber) { 
  }

//-----------------------------------------------------------------------------
// overloaded methods of TObject
//-----------------------------------------------------------------------------
  virtual void    Clear(Option_t* opt = "");
  virtual void    Print(Option_t* opt = "") const;

  ClassDef(TGaasHeaderBlock,1)	// Event Header block for GaAs data
};

#endif
