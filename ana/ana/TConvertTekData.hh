///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#ifndef __TConvertTekData_hh__
#define __TConvertTekData_hh__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>

#include "TObject.h"
#include "TTree.h"
#include "TFile.h"
#include "TBranch.h"
#include "TFolder.h"
#include "TSystem.h"

#include "Stntuple/obj/TStnEvent.hh"
#include "Stntuple/obj/TStnDataBlock.hh"
#include "Stntuple/obj/TStnNode.hh"

#include "gaasAna/ana/TScopeEvent.hh"
#include "gaasAna/ana/InitGaasDataBlock.hh"
#include "gaasAna/ana/InitGaasHeaderBlock.hh"

class TConvertTekData: public TObject {

  TTree*       fTree;
  TString      fDirName;
  TString      fFileName;
  TStnEvent*   fEvent;
  float        fgMaxFileSize;
  int          fgBufferSize   = 64000;  // in bytes
  int          fgSplitLevel   = 99;
  int          fgCompressionLevel = 1;
  int          fgMakeSubdirs  = 1;

  int          fgFileNumber   = 0;
  int          fgOpenNextFile = 0;

  TFile*       fFile;
  TDirectory*  fOldDir;
  TFolder*     fFolder;

  float        fCpuSpeed;

  AbsEvent*    fAbsEvent;

  TScopeEvent  fScopeEvent;
//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
public:
  TConvertTekData();
  ~TConvertTekData();

  int beforeBeginJob();
  int afterBeginJob ();
  
  TStnDataBlock* AddDataBlock(const char*        branch_name,
			      const char*        class_name,
			      Int_t              buffer_size,
			      Int_t              split,
			      Int_t              compression);

  int OpenNewFile   (const char* Filename);

  int Parse(const TString*, char, TObjArray*);

  int ReadGaasData   (const char* Dir, int RunNumber, const char* FnPattern, int Format);
  int ReadGaasDataNew(const char* Dir, int RunNumber, const char* FnPattern, int Format);

  //  ClassDef(TConvertTekData,0)
};

#endif
