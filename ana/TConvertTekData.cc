///////////////////////////////////////////////////////////////////////////////
// 2019-02-09 PM
// scope data could have different formats, make format a parameter
// 
// need to revise the data buffer dimensions for new files
// ReadGaasData: single frame mode
// ReadGaasDataNew: single frame mode
///////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <time.h>

#include "gaasAna/ana/TConvertTekData.hh"

// ClassImp(TConvertTekData)
//-----------------------------------------------------------------------------
TConvertTekData::TConvertTekData() {
  fEvent         = nullptr;
  fFile          = nullptr;
  fFileName      = "";
  fDirName       = "";
  fDebugMode     = 0;
  fReadoutMode   = -1;
  fNFrames       = -1;
}

//-----------------------------------------------------------------------------
TConvertTekData::~TConvertTekData() {
  if (fFile) delete fFile;
}

//______________________________________________________________________________
int TConvertTekData::OpenNewFile(const char* Filename) {
  // open next file
  int rc = 0;
  fFile = new TFile(Filename,"RECREATE");
  if (! fFile) {
    Error("beginJob","an attempt to open a new ROOT file %s failed",
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
  fScopeEvent.fRunStartTime = "undefined";
  fScopeEvent.fRunEndTime   = "undefined";

  char fn_txt[200];

  sprintf(fn_txt,"%s/qdgaas.fnal.%06i.txt",Dirname,RunNumber);
  FILE* f = fopen(fn_txt,"r");
  if (f == nullptr)     {
    printf(" Can\'t open input file %s. BAIL OUT\n",fn_txt);
    return -1;
  }

  TString cmd = Form("cat %s | grep RUN_",fn_txt);
  pipe = gSystem->OpenPipe(cmd.Data(),"r");

  TObjArray w0;

  while (fgets(buf,50,pipe)) {
    TString s(buf);
    s = s.Strip(TString::kTrailing,'\n');
    s = s.Strip(TString::kTrailing,'\r');
    if      (s.Index("RUN_START_TIME:") == 0) {
      Parse(&s,' ',&w0);
      fScopeEvent.fRunStartTime = ((TObjString*) w0[1])->String()+" "+((TObjString*) w0[2])->String();
    }
    else if (s.Index("RUN_END_TIME:") == 0) {
      Parse(&s,' ',&w0);
      fScopeEvent.fRunEndTime =  ((TObjString*) w0[1])->String()+" "+((TObjString*) w0[2])->String();
      printf(">>>> found RUN_END_TIME:\n");
    }
  }
  gSystem->ClosePipe(pipe);
  
  for (int i=0; i<TScopeEvent::kMaxNChannels; i++) fScopeEvent.fChannelID[i] = -1;
  
  char* lineptr = (char*) malloc(buflen);

  int  nevents(0);
//-----------------------------------------------------------------------------
// read the data -2 channels
//-----------------------------------------------------------------------------
  int       nlines(0), ichannel(0);
  TObjArray data;
  TObjArray words;

  printf(" 0001: start reading\n");
//-----------------------------------------------------------------------------
// read a .CSV file - so far, Tektronix format
//-----------------------------------------------------------------------------
  
  while (getline(&lineptr,(size_t*) &buflen,f) > 0) {

    printf("line: %s",lineptr);

    // skip unneeded lines

    TString s(lineptr);
    s = s.Strip(TString::kTrailing,'\n');
    s = s.Strip(TString::kTrailing,'\r');
    s = s.Strip();			        // strip trailing spaces

    int loc = s.Index("(u\'GPIB0::");
    if (loc == 0) continue;
    if (s.Index("RUN_"      ) == 0) continue;
    if (s.Index("TEKTRONIX" ) == 0) continue;
    if (s.Index("header"    ) == 0) continue;   // assume header = 0 (no headers)
    if (s.Index("DATA:"     ) == 0) {

      int nw = Parse(&s,';',&words);

      TObjString* os1 = (TObjString*) words.At(1);
      if (os1->String() == "FASTEST") {
	fReadoutMode = 1;                      // FastFrame
      }
      else {
	fReadoutMode = 0;                      // single frame readout
      }

      TObjString* os2 = (TObjString*) words.At(2);
      TObjArray w2;

      fScopeEvent.fNChannels = Parse(&os2->String(),',',&w2);
//-----------------------------------------------------------------------------
// here I'm assuming that the first sample read is always #1
//-----------------------------------------------------------------------------
      TObjString* os = (TObjString*) words.At(nw-3);
      fScopeEvent.fNSamples  = strtol(os->String().Data(),nullptr,10);
      
      continue;   // number of channels is defined at run-time
    }
    
    //    if (s.Index("WFMOutpre:CH1:") == 0) {
    if (s.Index("WFMOutpre:") == 0) {

      Parse(&s,';',&words);
      int nw = words.GetEntries();

      if (nw > 5) {
//-----------------------------------------------------------------------------
// figure out which channel - split by ':'
// chname = "CH1", "CH2" etc
//-----------------------------------------------------------------------------
	TObjArray w0;
	TString s0 = ((TObjString*) words.At(0))->String();
	Parse(&s0,':',&w0);

	TString chname = ((TObjString*) w0.At(1))->String();

	int ich = chname[2]-'1';

	fScopeEvent.fChannelID[ichannel] = ich;
	
	TObjString* os = (TObjString*) words.At(9);
	fScopeEvent.fSampleTime  = strtof(os->String().Data(),nullptr);

	os = (TObjString*) words.At(13);
	fScopeEvent.fVSlp[ichannel] = strtof(os->String().Data(),nullptr);

	os = (TObjString*) words.At(14);
	fScopeEvent.fVOff[ichannel] = strtof(os->String().Data(),nullptr);

	os = (TObjString*) words.At(15);
	fScopeEvent.fVOff2[ichannel] = strtof(os->String().Data(),nullptr);

	ichannel++;
      }
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
	  line = line.Strip(TString::kTrailing,'\r');
	  line = line.Strip(TString::kTrailing,' ');
	  line = line.Strip(TString::kTrailing,',');
	  int nw = Parse(&line,',',&numbers);
	  for (int i=0; i<nw; i++) {
	    const char* num = ((TObjString*) numbers.At(i))->String().Data();
	    int val = strtol(num,nullptr,10);
	    fScopeEvent.fV[ich][loc] = (val-fScopeEvent.fVOff[ich])*fScopeEvent.fVSlp[ich] + fScopeEvent.fVOff2[ich];
	    loc++;
	  }
					// next line
	  if (loc >= fScopeEvent.fNSamples) break;
	}
      }
//-----------------------------------------------------------------------------
// initialize times
//-----------------------------------------------------------------------------
      for (int i=0; i<fScopeEvent.fNSamples; i++) {
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
// end job : memory cleanup, make sure can call twice
//-----------------------------------------------------------------------------
  free(lineptr);

  // delete fTree;
  delete fFile;
  delete fEvent;

  fTree  = nullptr;
  fEvent = nullptr;
  fFile  = nullptr;

  return 0;
}
//-----------------------------------------------------------------------------
// Format = 1: Inifiniscope (Albany   data)
//-----------------------------------------------------------------------------
int TConvertTekData::ReadGaasDataNew(const char* Dirname, int RunNumber, const char* FnPattern, int Format) {

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
  fScopeEvent.fRunStartTime = "undefined";
  fScopeEvent.fRunEndTime   = "undefined";

  char fn_txt[200];

  sprintf(fn_txt,"%s/qdgaas.fnal.%06i.txt",Dirname,RunNumber);
  FILE* f = fopen(fn_txt,"r");
  if (f == nullptr)     {
    printf(" Can\'t open input file %s. BAIL OUT\n",fn_txt);
    return -1;
  }

  TString cmd = Form("cat %s | grep RUN_",fn_txt);
  pipe = gSystem->OpenPipe(cmd.Data(),"r");

  TObjArray w0;

  while (fgets(buf,50,pipe)) {
    TString s(buf);
    s = s.Strip(TString::kTrailing,'\n');
    s = s.Strip(TString::kTrailing,'\r');
    if      (s.Index("RUN_START_TIME:") == 0) {
      Parse(&s,' ',&w0);
      fScopeEvent.fRunStartTime = ((TObjString*) w0[1])->String()+" "+((TObjString*) w0[2])->String();
    }
    else if (s.Index("RUN_END_TIME:") == 0) {
      Parse(&s,' ',&w0);
      fScopeEvent.fRunEndTime =  ((TObjString*) w0[1])->String()+" "+((TObjString*) w0[2])->String();
      printf(">>>> found RUN_END_TIME:\n");
    }
  }
  gSystem->ClosePipe(pipe);


  tm     begin_run_tm   ; // , end_run_tm;
  time_t begin_run_epoch; // , end_run_epoch;

  //  char buf[100];

  strncpy(buf,fScopeEvent.fRunStartTime.Data(),20);
  buf[19] = 0;

  strptime(buf,"%Y-%h-%d %H:%M:%S",&begin_run_tm);
  begin_run_epoch = mktime(&begin_run_tm);

  // strncpy(buf,fScopeEvent.fRunEndTime.Data()+16,20);
  // buf[19] = 0;

  // strptime(buf,"%Y-%h-%d %H:%M:%S",&end_run_tm);
  // end_run_epoch = mktime(&end_run_tm);

  time_t prev_epoch = begin_run_epoch;
  int    prev_usec  = 0;
  
  for (int i=0; i<TScopeEvent::kMaxNChannels; i++) fScopeEvent.fChannelID[i] = -1;
  
  char* lineptr = (char*) malloc(buflen);

  int  nevents(0);
//-----------------------------------------------------------------------------
// read the data -2 channels
//-----------------------------------------------------------------------------
  int       nlines(0), ichannel(0), nblocks(0);
  TObjArray data;
  TObjArray words;

  printf(" 0001: start reading\n");
//-----------------------------------------------------------------------------
// read a .CSV file - so far, Tektronix format
//-----------------------------------------------------------------------------
  
  while (getline(&lineptr,(size_t*) &buflen,f) > 0) {

    printf("line: %s",lineptr);

    // skip unneeded lines

    TString s(lineptr);
    s = s.Strip(TString::kTrailing,'\n');
    s = s.Strip(TString::kTrailing,'\r');
    s = s.Strip();			        // strip trailing spaces

    int loc = s.Index("(u\'GPIB0::");
    if (loc == 0) continue;
    if (s.Index("RUN_"      ) == 0) continue;
    if (s.Index("TEKTRONIX" ) == 0) continue;
    if (s.Index("header"    ) == 0) continue;   // assume header = 0 (no headers)
    if (s.Index("DATA:"     ) == 0) {

      int nw = Parse(&s,';',&words);

      TObjString* os2 = (TObjString*) words.At(2);
      TObjArray w2;

      fScopeEvent.fNChannels = Parse(&os2->String(),',',&w2);
//-----------------------------------------------------------------------------
// here I'm assuming that the first sample read is always #1
//-----------------------------------------------------------------------------
      TObjString* os = (TObjString*) words.At(nw-3);
      fScopeEvent.fNSamples  = strtol(os->String().Data(),nullptr,10);
      
      continue;   // number of channels is defined at run-time
    }

    if (s.Index("WFMOutpre:") == 0) {

      Parse(&s,';',&words);
      int nw = words.GetEntries();

      if (nw > 5) {
//-----------------------------------------------------------------------------
// figure out which channel - split by ':'
// chname = "CH1", "CH2" etc
//-----------------------------------------------------------------------------
	TObjArray w0;
	TString s0 = ((TObjString*) words.At(0))->String();
	Parse(&s0,':',&w0);

	TString chname = ((TObjString*) w0.At(1))->String();

	int ich = chname[2]-'1';

	fScopeEvent.fChannelID[ichannel] = ich;
	
	TObjString* os = (TObjString*) words.At(9);
	fScopeEvent.fSampleTime  = strtof(os->String().Data(),nullptr);

	os = (TObjString*) words.At(13);
	fScopeEvent.fVSlp[ichannel] = strtof(os->String().Data(),nullptr);

	os = (TObjString*) words.At(14);
	fScopeEvent.fVOff[ichannel] = strtof(os->String().Data(),nullptr);

	os = (TObjString*) words.At(15);
	fScopeEvent.fVOff2[ichannel] = strtof(os->String().Data(),nullptr);

	ichannel++;
      }
      continue;
    }

    if (s.Length() == 0) continue;

    if (s.Index(">>>Num_Frames:") == 0) {
//-----------------------------------------------------------------------------
// line '>>>Num_Frames: 20'
//-----------------------------------------------------------------------------
      sscanf(s.Data(),">>>Num_Frames: %i",&fNFrames);
//-----------------------------------------------------------------------------
// read fNFrames timestamps
//-----------------------------------------------------------------------------
      int    evt;

      std::vector<int>     usec, psec, dt;
      std::vector<time_t>  epoch;

      char   day[10], month[10], year[10], time_of_day[10],dot[10];

      TObjArray w;

      for (int i=0; i<fNFrames; i++) {
	int  tt0,tt1, tt2, tt3;

	int nb = getline(&lineptr,(size_t*) &buflen,f);
	
	if (fDebugMode != 0) {
	  printf(" read nb =%i bytes: %s\n",nb,lineptr);
	}
	sscanf(lineptr,"%i: %s %s %s %8s.%3s %3i %3i %3i",&evt,day,month,year,time_of_day,dot,&tt1,&tt2,&tt3);
	tt0 = atoi(dot);
//-----------------------------------------------------------------------------
// save microseconds and picoseconds, convert tm struct into a unix time
//-----------------------------------------------------------------------------
	usec.push_back(1000*tt0+tt1);
	psec.push_back(1000*tt2+tt2);

	char tmm[100];
	sprintf(tmm,"%s-%s-%s %s",year,month,day,time_of_day);

	struct tm  tm;
	//	memset(&tm,0,sizeof(tm));
	strptime(tmm,"%Y-%h-%d %H:%M:%S",&tm);
	// tm.tm_year += 1900;
	// tm.tm_mon  += 1;

	time_t ep = mktime(&tm);

	epoch.push_back(ep);

	int delta_t;                  // in microseconds
	if (i == 0) delta_t = 0;
	else {
	  delta_t = (epoch[i]-prev_epoch)*1000000+(usec[i]-prev_usec);
	}

	dt.push_back(delta_t);

	prev_epoch = epoch[i];
	prev_usec  = usec [i];

	if (fDebugMode != 0) {
	  printf(" time: =%li %s tt0 = %i\n",ep, ctime(&ep),tt0);
	}
      }
//-----------------------------------------------------------------------------
// after frame times, we have fNFrames frames
//-----------------------------------------------------------------------------
      prev_epoch = begin_run_epoch;
      prev_usec  = 0;

      for (int frame=0; frame<fNFrames; frame++) {
	fScopeEvent.fEventNumber = nblocks*fNFrames+frame+1;
//-----------------------------------------------------------------------------
// multiple events in one file, read one event, 20 numbers per line
//-----------------------------------------------------------------------------
	for (int ich=0; ich<fScopeEvent.fNChannels; ich++) {
	  int loc = 0;
	  TObjArray numbers;
	  while (getline(&lineptr,(size_t*) &buflen,f) > 0) {
	    TString line = lineptr;
	    line = line.Strip(TString::kTrailing,'\n');
	    line = line.Strip(TString::kTrailing,'\r');
	    line = line.Strip(TString::kTrailing,' ');
	    line = line.Strip(TString::kTrailing,',');
	    int nw = Parse(&line,',',&numbers);
	    for (int i=0; i<nw; i++) {
	      const char* num = ((TObjString*) numbers.At(i))->String().Data();
	      int val = strtol(num,nullptr,10);
	      fScopeEvent.fV[ich][loc] = (val-fScopeEvent.fVOff[ich])*fScopeEvent.fVSlp[ich] + fScopeEvent.fVOff2[ich];
	      loc++;
	    }
					// next line
	    if (loc >= fScopeEvent.fNSamples) break;
	  }
	}
//-----------------------------------------------------------------------------
// initialize the sample times
//-----------------------------------------------------------------------------
	for (int i=0; i<fScopeEvent.fNSamples; i++) {
	  fScopeEvent.fT[i] = fScopeEvent.fSampleTime*(i+0.5);
	}

	fScopeEvent.fEpoch  = epoch[frame];
	fScopeEvent.fUSec   = usec[frame ];
	fScopeEvent.fPSec   = psec[frame ];
	fScopeEvent.fDeltaT = (epoch[frame]-prev_epoch)+(usec[frame]-prev_usec)/1e6;

//-----------------------------------------------------------------------------
// data read in, save in an NTuple
//-----------------------------------------------------------------------------
	fEvent->Init(fAbsEvent,0);
	fTree->Fill();

	prev_epoch = epoch[frame];
	prev_usec  = usec [frame];

	nevents++;
	nlines++;
      }
    }
//-----------------------------------------------------------------------------
// end of the >>>Num_Frames: block, increment number of blocks
//-----------------------------------------------------------------------------
    nblocks += 1;
  }

  printf(" --- finish after processing: %i events\n",nevents);
  
  fFile->Write();
  fFile->Close();
//-----------------------------------------------------------------------------
// end job : memory cleanup, make sure can call twice
//-----------------------------------------------------------------------------
  free(lineptr);

  // delete fTree;
  delete fFile;
  delete fEvent;

  fTree  = nullptr;
  fEvent = nullptr;
  fFile  = nullptr;

  return 0;
}
