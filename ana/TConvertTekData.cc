///////////////////////////////////////////////////////////////////////////////
// 2019-02-09 PM
// scope data could have different formats, make format a parameter
// 
// need to revise the data buffer dimensions for new files
///////////////////////////////////////////////////////////////////////////////
#include "gaasAna/ana/TConvertTekData.hh"

// ClassImp(TConvertTekData)
//-----------------------------------------------------------------------------
TConvertTekData::TConvertTekData() {
  fEvent    = nullptr;
  fFile     = nullptr;
  fFileName = "";
  fDirName  = "";
}

//-----------------------------------------------------------------------------
TConvertTekData::~TConvertTekData() {
}

//______________________________________________________________________________
int TConvertTekData::OpenNewFile(const char* Filename) {
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
int TConvertTekData::beforeBeginJob() {
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
int TConvertTekData::afterBeginJob() {

  //  char par[200];

  // if (fgMakeSubdirs && fFile) {
  //   fOldDir->cd();
  // }

  return 0;
}

//-----------------------------------------------------------------------------
TStnDataBlock* TConvertTekData::AddDataBlock(const char*        branch_name,
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
// so far, split by comma
//-----------------------------------------------------------------------------
int TConvertTekData::Parse(const TString* S, const char Del, TObjArray* Words) {

  Words->Delete();
  
  int len = S->Length();
  
  int loc0(0);

  while (loc0 >= 0) {
    int loc = S->Index(Del,loc0);
    if (loc >= 0) {
      TString w = (*S)(loc0,loc-loc0);
      Words->Add(new TObjString(w));
      loc0 = loc+1;
    }
    else {
					// end of string
      TString w = (*S)(loc0,len-loc0);
      Words->Add(new TObjString(w));
      loc0 = loc;
    }
  }
  return Words->GetEntriesFast();
}

//-----------------------------------------------------------------------------
// Format = 1: Inifiniscope (Albany   data)
// Format = 2: Tektronix    (Fermilab data)
//-----------------------------------------------------------------------------
int TConvertTekData::ReadGaasData(const char* Dirname, int RunNumber, const char* FnPattern, int Format) {

//-----------------------------------------------------------------------------
// beginJob
//-----------------------------------------------------------------------------
  fFile     = 0;
  fFileName = Form("gaasqd_fnal.%06i_00000000",RunNumber);
  
  if (! fEvent      ) {
    fEvent       = new TStnEvent();
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
// begin run Dir=/projects/gaas/data/runs-0015-0029
//-----------------------------------------------------------------------------
  //  return 0;

  char buf[1000];

  int  buflen(1000);
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
  fScopeEvent.fStnVersion   = "v7_3_5";
					// assume number of channels doesn't change within the run
  fScopeEvent.fNChannels    = -1;
  
  char* lineptr = (char*) malloc(buflen);

  int  nevents(0);
  
  sprintf(buf,"%s/qdgaas.fnal.%06i.txt",Dirname,RunNumber);
  FILE* f = fopen(buf,"r");
  if (f == nullptr)     {
    printf(" Can\'t open input file %s. BAIL OUT\n",buf);
    return -1;
  }
//-----------------------------------------------------------------------------
// read the data -2 channels
//-----------------------------------------------------------------------------
  int       nlines(0);
  TObjArray data;
  TObjArray words;

  printf(" 0001: start reading\n");
//-----------------------------------------------------------------------------
// read a .CSV file - so far, Tektronix format
//-----------------------------------------------------------------------------
  while (getline(&lineptr,(size_t*) &buflen,f) > 0) {

    printf("line: %s\n",lineptr);

    // skip unneeded lines

    TString s(lineptr);
    s = s.Strip(TString::kTrailing,'\n');
    s = s.Strip();			        // strip trailing spaces

    int loc = s.Index("(u\'GPIB0::");
    if (loc == 0) continue;
    if (s.Index("TEKTRONIX" ) == 0) continue;
    if (s.Index("header"    ) == 0) continue;   // assume header = 0 (no headers)
    if (s.Index("DATA:"     ) == 0) {

      int nw = Parse(&s,';',&words);

      TObjString* os2 = (TObjString*) words.At(2);
      TObjArray w2;

      fScopeEvent.fNChannels = Parse(&os2->String(),',',&w2);

      TObjString* os = (TObjString*) words.At(nw-3);
      fScopeEvent.fNSamples  = strtol(os->String().Data(),nullptr,10);
      
      continue;   // assume 2 channels
    }
    
    if (s.Index("(\'WFMOutpre:CH1:") == 0) {

      Parse(&s,';',&words);
      //      int nw = words.GetEntries();

      TObjString* os = (TObjString*) words.At(9);
      fScopeEvent.fSampleTime  = strtof(os->String().Data(),nullptr);

      os = (TObjString*) words.At(14);
      fScopeEvent.fVOff[0] = strtof(os->String().Data(),nullptr);

      os = (TObjString*) words.At(13);
      fScopeEvent.fVSlp[0] = strtof(os->String().Data(),nullptr);
      
      continue;
    }

    if (s.Index("(\'WFMOutpre:CH2:") == 0) {
//-----------------------------------------------------------------------------
// in general, voltage scales for the two channels are different
//-----------------------------------------------------------------------------
      Parse(&s,';',&words);
      //      int nw = words.GetEntries();

      TObjString* os;

      os = (TObjString*) words.At(14);
      fScopeEvent.fVOff[1]  = strtof(os->String().Data(),nullptr);

      os = (TObjString*) words.At(13);
      fScopeEvent.fVSlp[1]  = strtof(os->String().Data(),nullptr);
      
      continue;
    }

    if (s.Length() == 0) continue;

    if (s.Index("trigger") == 0) {
      TObjArray w;
      Parse(&s,' ',&w);
      TObjString* os = (TObjString*) w.At(2);
      fScopeEvent.fEventNumber = strtol(os->String().Data(),nullptr,10)+1;
//-----------------------------------------------------------------------------
// multiple events in one file, read one event, 20 numbers per line
//-----------------------------------------------------------------------------
      for (int ich=0; ich<fScopeEvent.fNChannels; ich++) {
	int loc = 0;
	TObjArray numbers;
	while (getline(&lineptr,(size_t*) &buflen,f) > 0) {
	  TString line = lineptr;
	  line = line.Strip(TString::kTrailing,'\n');
	  line = line.Strip(TString::kTrailing,' ');
	  line = line.Strip(TString::kTrailing,',');
	  int nw = Parse(&line,',',&numbers);
	  for (int i=0; i<nw; i++) {
	    const char* num = ((TObjString*) numbers.At(i))->String().Data();
	    int val = strtol(num,nullptr,10);
	    fScopeEvent.fV[ich][loc] = (val-fScopeEvent.fVOff[ich])*fScopeEvent.fVSlp[ich];
	    loc++;
	  }
					// next line
	  if (loc >= 500) break;
	}
      }
//-----------------------------------------------------------------------------
// initialize times
//-----------------------------------------------------------------------------
      for (int i=0; i<500; i++) {
	fScopeEvent.fT[i] = fScopeEvent.fSampleTime*(i+0.5);
      }
//-----------------------------------------------------------------------------
// all event data read in,
//-----------------------------------------------------------------------------
    }

    nlines++;
    //    printf("line: %5i nwords: %5i\n", nlines,nwords);
//-----------------------------------------------------------------------------
// data read in
// now need to save as an NTuple
//-----------------------------------------------------------------------------
    fEvent->Init(fAbsEvent,0);
    fTree->Fill();

    nevents++;
  }

  printf(" --- finish after processing: %i events\n",nevents);
  
  fFile->Write();
  fFile->Close();
//-----------------------------------------------------------------------------
// memory cleanup
//-----------------------------------------------------------------------------
  free(lineptr);
  
  return 0;
}
