///////////////////////////////////////////////////////////////////////////////
// 2019-02-09 PM
// scope data could have different formats, make format a parameter
// 
// need to revise the data buffer dimensions for new files
///////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>

#include "TObject.h"
#include "TTree.h"
#include "TFile.h"
#include "TBranch.h"

class TConvertScopeData: public TObject {

  TTree*     fgTree;
  TString    fDirName;
  TString    fgFileName;
  TFile*     fgFile;
  TStnEvent* fgEvent;
  float      fgMaxFileSize;
  int        fgBufferSize   = 64000;  // in bytes
  int        fgSplitLevel   = 99;
  int        fgCompressionLevel = 1;

  int        fgFileNumber   = 0;
  int        fgOpenNextFile = 0;

  float fCpuSpeed;
  
  TConvertScopeData();
  ~TConvertScopeData();

  int beforeBeginJob();
  int afterBeginJob ();
  
  TStnDataBlock* AddDataBlock(const char* branch_name,
			      const char* class_name,
			      Int_t       (*f)(TStnDataBlock*,AbsEvent*,Int_t),
			      Int_t       buffer_size,
			      Int_t       split,
			      Int_t       compression);

  int read_gaas_data(const char* Filename);
};

//-----------------------------------------------------------------------------
TConvertScopeData::TConvertScopeData() {
}

//-----------------------------------------------------------------------------
TConvertScopeData::~TConvertScopeData() {
}

//______________________________________________________________________________
int TConvertScopeData::beforeBeginJob() {
					// return code
  int rc  =  0;
				// give more time to define TModule::fName
  fDirName = GetName();

  if ((fgFileName != "") && (fgFile == 0)) {

					// create a new ROOT File 
    rc = OpenNewFile(fgFileName.Data());
  }

  if (fgFile) {
					// file opened, don't forget to make
					// new directory before booking 
    if (fgMakeSubdirs) {
      fOldDir = gDirectory;
      fgFile->mkdir(fDirName);
      fgFile->cd(fDirName);
    }
  }
  return rc;
}

//______________________________________________________________________________
int TConvertScopeData::afterBeginJob() {

  char par[200];

  if (fgMakeSubdirs && fgFile) {
    fOldDir->cd();
  }

  return 0;
}

  else {
				// can't create branch

    printf(" StntupleModule::AddDataBlock : can\'t add block for");
    printf(" branch %s and class %s\n",branch_name,class_name);
    block = NULL;
  }
  return block;
}


//-----------------------------------------------------------------------------
TStnDataBlock* TConvertScopeData::AddDataBlock(const char* branch_name,
					       const char* class_name,
					       Int_t       (*f)(TStnDataBlock*,AbsEvent*,Int_t),
					       Int_t       buffer_size,
					       Int_t       split,
					       Int_t       compression) 
{
  // adds new branch to fgTree and registers a data block corresponding to it

  TBranch*       branch;
  TStnDataBlock* block;

  int            rc;

  TStnNode*      node;
  node = 0;
  rc  = fgEvent->AddDataBlock(branch_name,class_name,node);

  if (rc == 0) {
				// everything is OK

    branch = fgTree->Branch(branch_name,class_name,
			    node->GetDataBlockAddress(),
			    buffer_size,
			    split);
    branch->SetCompressionLevel(compression);
    block = node->GetDataBlock();
    block->SetExternalInit(f);
    block->SetNode(node);
  }
  else if (rc > 0) {
				// already existing branch

    printf(" :AddDataBlock: ");
    printf(" an attempt to redefine the existing block made for");
    printf(" branch %s and class %s\n",branch_name,class_name);
    block = node->GetDataBlock();
  }
  else {
				// can't create branch

    printf(" AddDataBlock : can\'t add block for");
    printf(" branch %s and class %s\n",branch_name,class_name);
    block = NULL;
  }
  return block;
}

//-----------------------------------------------------------------------------
// Format = 1: Inifiniscope (Albany   data)
// Format = 2: Tektronix    (Fermilab data)
//-----------------------------------------------------------------------------
int TConvertScopeData::read_gaas_data(const char* Dirname, const char* FnPattern, int Format) {

//-----------------------------------------------------------------------------
// beginJob
//-----------------------------------------------------------------------------
  fFile   = 0;
  fFolder = new TFolder(GetName(),GetName());
  
  if (! fgEvent      ) {
    fgEvent       = new TStnEvent();
    fgStntupleFolder = gROOT->GetRootFolder()->AddFolder("Stntuple",
							 "STNTUPLE folder");
    fgMaxFileSize = 8000;
  }

  beforeBeginJob();

  // book the tree, for ROOT 3 kludge split mode: set it always to
  // "non-split,old"
  // header block, however is always written in split mode

  fgTree   = new TTree("STNTUPLE", "STNTUPLE");

  AddDataBlock("HeaderBlock","TGaasHeaderBlock",
	       InitGaasHeaderBlock,
	       fgBufferSize,
	       0, // 99,                          // fSplitMode.value(), always split
	       fgCompressionLevel);

  AddDataBlock("GaasDataBlock","TGaasDataBlock",
	       InitGaasHeaderBlock,
	       fgBufferSize,
	       0, // 99,                          // fSplitMode.value(), always split
	       fgCompressionLevel);
  
  FILE* pipe;
  pipe = gSystem->OpenPipe(
     "cat /proc/cpuinfo | grep MHz | tail -1 | awk '{print $4}'","r");
  fscanf(pipe,"%f",&fCpuSpeed);
  gSystem->ClosePipe(pipe);

  afterBeginJob();

//-----------------------------------------------------------------------------
// begin run Dir=/projects/gaas/data/Q311_v1_0000 FnPattern=Q333_v1
//-----------------------------------------------------------------------------
  //  return 0;

  char buf[1000];

  TObjArray* list_of_filenames = new TObjArray();
  
  FILE* pipe;
  pipe = gSystem->OpenPipe("ls %s | grep %s | awk '{print $9}'",Dirname,FnPattern);
  fscanf(pipe,"%f",&fCpuSpeed);

  while (fgets(buf,1000,pipe)) { 
    if ( buf[0] == '#')                                     continue;
    fscanf(pipe,"%s",buf);
    list_of_filenames->Add(new TObjString(buf));
  }

  gSystem->ClosePipe(pipe);

  int nfiles = list_of_filenames->GetEntries();
//-----------------------------------------------------------------------------
// want the envets to be ordered in time sequence
//-----------------------------------------------------------------------------
  for (int i=0; i<nfiles; i++) {
    TObjString* s = (TObjString*) list_of_filenames->At(i);
    sprintf(buf,"$s/%s_%04i.csv",Dirname,FnPattern);
//-----------------------------------------------------------------------------
// open the file , fn = X_Y_eventNumber.csv
// it is possible that some files in the sequence are missing 
//-----------------------------------------------------------------------------
    FILE* f = fopen(buf,"r");
    if (f == nullptr)                                       continue;
//-----------------------------------------------------------------------------
// read the data
//-----------------------------------------------------------------------------
    int nlines(0), nevents(-1);
    TObjArray data;

    printf(" 0001\n");

    while (getline(&buf,&buflen,f) > 0) {

      printf("line: %s\n",buf);
    
      // stringstream ss(s);

      // int nf = 0;
      // while (!ss.fail()) {
      // 	//      ss >> data[nlines][nf];
      // 	ss >> adata[nf];
      // 	nf++;
      // }

      // dat[nlines] = new float[nf];

      // for (int i=0; i<nf; i++) {
      // 	dat[nlines][i] = adata[i];
      // }
    
      // if (nevents < 0) nevents = nf-1;
    
      // //    printf("line = %5i nf = %5i data[0] = %16.7g\n",nlines,nf,data[nlines][0]);
      // printf("line = %5i nf = %5i data[0] = %16.7g\n",nlines,nf,adata[0]);
      nlines++;
    }
//-----------------------------------------------------------------------------
// data read in
// now need to save as an NTuple
//-----------------------------------------------------------------------------
    // for (int line=0; line<nlines; line++) {
    //   ev->nbins   = nlines;
    //   ev->t[line] = dat[line][0];
    //   ev->v[line] = dat[line][i];
    // }
    
    // tree->Fill();
  }
  
  // fout->Write();
  // fout->Close();
//-----------------------------------------------------------------------------
// memory cleanup
//-----------------------------------------------------------------------------
  
  return 0;
}
