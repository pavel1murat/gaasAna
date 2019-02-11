///////////////////////////////////////////////////////////////////////////////
// 2019-02-09 PM
// scope data could have different formats, make format a parameter
// 
// need to revise the data buffer dimensions for new files
///////////////////////////////////////////////////////////////////////////////
#include "gaasAna/ana/TConvertScopeData.hh"

// ClassImp(TConvertScopeData)
//-----------------------------------------------------------------------------
TConvertScopeData::TConvertScopeData() {
  fEvent    = nullptr;
  fFile     = nullptr;
  fFileName = "";
  fDirName  = "";
}

//-----------------------------------------------------------------------------
TConvertScopeData::~TConvertScopeData() {
}

//______________________________________________________________________________
int TConvertScopeData::OpenNewFile(const char* Filename) {
  // open next file
  int rc = 0;
  fFile = new TFile(Filename,"RECREATE");
  if (! fFile) {
    Error("beginJob","an attempxt to open a new ROOT file %s failed",
	  fFileName.Data());
    rc = -1;
  }
  return rc;
}

//______________________________________________________________________________
int TConvertScopeData::beforeBeginJob() {
					// return code
  int rc  =  0;
				// give more time to define TModule::fName
  //  fDirName = GetName();

  if ((fFileName != "") && (fFile == 0)) {

					// create a new ROOT File 
    rc = OpenNewFile(fFileName.Data());
  }

  if (fFile) {
					// file opened, don't forget to make
					// new directory before booking 
    // if (fgMakeSubdirs) {
    //   fOldDir = gDirectory;
    //   fFile->mkdir(fDirName);
    //   fFile->cd(fDirName);
    // }
  }
  return rc;
}

//______________________________________________________________________________
int TConvertScopeData::afterBeginJob() {

  //  char par[200];

  // if (fgMakeSubdirs && fFile) {
  //   fOldDir->cd();
  // }

  return 0;
}

//-----------------------------------------------------------------------------
TStnDataBlock* TConvertScopeData::AddDataBlock(const char*        branch_name,
					       const char*        class_name,
					       Int_t              buffer_size,
					       Int_t              split,
					       Int_t              compression) 
{
  // adds new branch to fTree and registers a data block corresponding to it

  TBranch*       branch;
  TStnDataBlock* block;

  int            rc;

  TStnNode*      node;
  node = 0;
  rc  = fEvent->AddDataBlock(branch_name,class_name,node);

  if (rc == 0) {
				// everything is OK

    branch = fTree->Branch(branch_name,class_name,
			   node->GetDataBlockAddress(),
			   buffer_size,
			   split);
    branch->SetCompressionLevel(compression);
    block = node->GetDataBlock();
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
int TConvertScopeData::ReadGaasData(const char* Dirname, int RunNumber, const char* FnPattern, int Format) {

//-----------------------------------------------------------------------------
// beginJob
//-----------------------------------------------------------------------------
  fFile     = 0;
  //  fFolder   = new TFolder(GetName(),GetName());
  fFileName = Form("gaasqd_fnal.%06i_00000000",RunNumber);
  
  if (! fEvent      ) {
    fEvent       = new TStnEvent();
    // fgStntupleFolder = gROOT->GetRootFolder()->AddFolder("Stntuple",
    // 							 "STNTUPLE folder");
    fgMaxFileSize = 8000;
  }

  beforeBeginJob();

  // book the tree, for ROOT 3 kludge split mode: set it always to
  // "non-split,old"
  // header block, however is always written in split mode

  fTree   = new TTree("STNTUPLE", "STNTUPLE");

  TStnDataBlock* block;

  block = AddDataBlock("HeaderBlock","TGaasHeaderBlock",
		       fgBufferSize,
		       0, // 99,                          // fSplitMode.value(), always split
		       fgCompressionLevel);

  InitGaasHeaderBlock* ighb = new InitGaasHeaderBlock();
  ighb->SetScopeEvent(&fScopeEvent);
  block->SetInitBlock(ighb);

  block = AddDataBlock("GaasDataBlock","TGaasDataBlock",
		       fgBufferSize,
		       0, // 99,                          // fSplitMode.value(), always split
		       fgCompressionLevel);

  InitGaasDataBlock* igdb = new InitGaasDataBlock();
  igdb->SetScopeEvent(&fScopeEvent);
  block->SetInitBlock(igdb);
  
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

  char buf[1000], cmd[1000];

  int buflen(1000);

  TObjArray* list_of_filenames = new TObjArray();
  
  sprintf(cmd,"ls -l %s | grep %s | awk '{print $9}'",Dirname,FnPattern);

  pipe = gSystem->OpenPipe(cmd,"r");

  while (fgets(buf,1000,pipe)) { 
    if ( buf[0] == '#')                                     continue;
    //    fscanf(pipe,"%s",buf);
    TString fn(buf);
    TString s = fn.Strip(TString::kTrailing,'\n');
    list_of_filenames->Add(new TObjString(s));
  }

  gSystem->ClosePipe(pipe);

  int nfiles = list_of_filenames->GetEntries();
//-----------------------------------------------------------------------------
// want "events" to be ordered in time sequence,
// event == file here
// form header
//-----------------------------------------------------------------------------
  fScopeEvent.fRunNumber    = RunNumber;
  fScopeEvent.fSubrunNumber = 0;
  fScopeEvent.fMcFlag       = 0;
  fScopeEvent.fVersion      = 1;
  fScopeEvent.fBrCode       = 0;
  fScopeEvent.fGoodTrig     = 1;
  fScopeEvent.fTrigWord     = 0;
  fScopeEvent.fCpu          = 0;
  fScopeEvent.fStnVersion   = "v7_3_0";

  fScopeEvent.fNChannels    = -1;
  
  char* lineptr = (char*) malloc(buflen);

  int  nevents(0);
  
  for (int i=0; i<nfiles; i++) {
    sprintf(buf,"%s/%s_%04i.csv",Dirname,FnPattern,i);
//-----------------------------------------------------------------------------
// open the file , fn = X_Y_eventNumber.csv
// it is possible that some files in the sequence are missing 
//-----------------------------------------------------------------------------
    FILE* f = fopen(buf,"r");
    if (f == nullptr)                                       continue;

    fScopeEvent.fEventNumber = i;
//-----------------------------------------------------------------------------
// read the data
//-----------------------------------------------------------------------------
    int nlines(0);
    TObjArray data;

    printf(" 0001: start reading\n");
//-----------------------------------------------------------------------------
// read a .CSV file - so far, Tektronix format
//-----------------------------------------------------------------------------
    while (getline(&lineptr,(size_t*) &buflen,f) > 0) {

      printf("line: %s\n",lineptr);

      TString s(lineptr);
      s = s.Strip(TString::kTrailing,'\n');
      s = s.Strip();			        // strip trailing spaces

      TObjArray words;

      int len = s.Length();

      int loc0 = 0;

      while (loc0 >= 0) {
	int loc = s.Index(',',loc0);
	if (loc >= 0) {
	  TString w = s(loc0,loc-loc0);
	  words.Add(new TObjString(w));
	  loc0 = loc+1;
	}
	else {
	  // end of string
	  TString w = s(loc0,len-loc0);
	  words.Add(new TObjString(w));
	  loc0 = loc;
	}
      }
      
      int nwords = words.GetEntries();

      if (fScopeEvent.fNChannels == -1) {
	fScopeEvent.fNChannels = nwords-4;
      }

      TObjString* w0 = (TObjString*) words.At(0);

      TObjString* w;
      
      if      (w0->String() == "\"Record Length\"") {
	w = (TObjString*) words.At(1);
	sscanf(w->String().Data(),"%i",&fScopeEvent.fNSamples);
      }
      else if (w0->String() == "\"Sample Interval\"") {
	w = (TObjString*) words.At(1);
	sscanf(w->String().Data(),"%f",&fScopeEvent.fSampleTime);
      }
      else if (w0->String() == "\"Trigger Point\"") {
	w = (TObjString*) words.At(1);
	sscanf(w->String().Data(),"%i",&fScopeEvent.fTriggerSample);
      }
      else if (w0->String() == "\"Trigger Time\"") {
	w = (TObjString*) words.At(1);
	sscanf(w->String().Data(),"%f",&fScopeEvent.fTriggerTime);
      }
//-----------------------------------------------------------------------------
// times and voltages are always there
//-----------------------------------------------------------------------------
      w = (TObjString*) words.At(3);
      sscanf(w->String().Data(),"%f",&fScopeEvent.fT[nlines]);

      for (int i=4; i<nwords; i++) {
	w = (TObjString*) words.At(i);
	sscanf(w->String().Data(),"%f",&fScopeEvent.fV[i-4][nlines]);
      }
      
      nlines++;
      printf("line: %5i nwords: %5i\n", nlines,nwords);
    }

    nevents++;
//-----------------------------------------------------------------------------
// data read in
// now need to save as an NTuple
//-----------------------------------------------------------------------------
    fEvent->Init(fAbsEvent,0);
    fTree->Fill();
  }

  printf(" --- finish after processing: %i events\n",nevents);
  
  fFile->Write();
  fFile->Close();
//-----------------------------------------------------------------------------
// memory cleanup
//-----------------------------------------------------------------------------
  
  return 0;
}
