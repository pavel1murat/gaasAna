/////////////////////T//////////////////////////////////////////////////////////
// different sides - different inefficiencies
// fStop =  1: hit photodiode
// fStop =  2: absorption
// fStop =  3: exited through X side - no internal reflection
// fStop =  4: exited through Y side - no internal reflection
// fStop =  5: exited through the Z top side    - no internal reflection
// fStop =  6: exited through the Z bottom size - no internal reflection
// fStop = 10: too many reflections
// fStop = 13: exited through X side - diffused reflection
// fStop = 14: exited through Y side - diffused reflection
// fStop = 15: exited through Z top  - diffused reflection
// fStop = 16: exited through Z bottom  - diffused reflection
//////////////////////////////////////////////////////////////////////////////

#include "TMath.h"
#include "TROOT.h"
#include "TFile.h"
#include "ana/GaasqdQuickSim.hh"

ClassImp(GaasqdQuickSim)
//-----------------------------------------------------------------------------
GaasqdQuickSim::GaasqdQuickSim(double DiodeDx, double DiodeDy): TNamed() {

  fSensorDx   = 0.2500;
  fSensorDy   = 0.0350;
  fSensorDz   = 0.00125;
					// dimensions for the test
  //  fDiodeDx    = 0.0025;			// N1816-PCBGBAC-E2 : 300x50 um
  fDiodeDx    = DiodeDx;                 
  fDiodeDy    = DiodeDy;                 
  fDiodeDz    = fSensorDz;		// just to make the diode visible, thickness is not used so far

					// nominally, -0.225
  fDiodeDispX = -0.225; // 0.;
					// N(e-h pairs) 
  fYield      = 1000.;			// 250000;    // per MeV
					// by default, fixed plane
  fPosMode    = 0;

  fBottomMirror = 0;

  fGeoManager = new TGeoManager("gm", "gm");
  TGeoElementTable *table = fGeoManager->GetElementTable();
//-----------------------------------------------------------------------------
// just for fun, follow ROOT rules
//-----------------------------------------------------------------------------
  TGeoMixture *air = new TGeoMixture("air",4, 0.00120479);
  air->AddElement(table->FindElement("C" ),0.000124);
  air->AddElement(table->FindElement("N" ),0.755268);
  air->AddElement(table->FindElement("O" ),0.231781);
  air->AddElement(table->FindElement("AR"),0.012827);
  
  TGeoMedium   *med = new TGeoMedium  ("AIR",1,air);

  TGeoVolume   *top = fGeoManager->MakeBox("TOP",med,1,1,1);
  fGeoManager->SetTopVolume(top);
//-----------------------------------------------------------------------------
// describe the GaAs sensor
//-----------------------------------------------------------------------------
  TGeoMixture *gaas = new TGeoMixture("gaas",2, 5.32);
  gaas->AddElement(table->FindElement ("GA"),1);
  gaas->AddElement(table->FindElement ("AS"),1);

  TGeoMedium*  m_gaas = new TGeoMedium  ("GAAS",2,gaas);

  fCrystal   = fGeoManager->MakeBox("gaas_sensor",m_gaas,fSensorDx,fSensorDy,fSensorDz);
  fCrystal->SetTransparency(60);
  top->AddNode(fCrystal,1);
//-----------------------------------------------------------------------------
// add photodiode
//-----------------------------------------------------------------------------
  TGeoMixture *ingaas = new TGeoMixture("ingaas",3, 5.32);
  ingaas->AddElement(table->FindElement ("In"),0.05);
  ingaas->AddElement(table->FindElement ("GA"),0.45);
  ingaas->AddElement(table->FindElement ("AS"),0.50);

  TGeoMedium*  m_ingaas = new TGeoMedium  ("INGAAS",3,ingaas);
  fDiode    = fGeoManager->MakeBox("Diode",m_ingaas, fDiodeDx,fDiodeDy,fDiodeDz);
  fDiode->SetLineColor(kRed+2);
  fDiode->SetTransparency(60);
  
  top->AddNode(fDiode,2,new TGeoTranslation(fDiodeDispX,0.0000,fDiodeDz+fSensorDz));

  fGeoManager->CloseGeometry();

  TGeoNode* tnode = fGeoManager->GetTopNode();

  fDiodePos = tnode->GetDaughter(1)->GetMatrix()->GetTranslation();
  

  // top->Draw();
  //  TView *view = gPad->GetView();
  // view->ShowAxis();
  
  fNPhMean  = fYield; // 13000  LYSO:Ce 26000 photons/MeV

  fPhotoEff        = 0.25;		// not used so far
  fMaxNReflections = 190;

  fRefrIndGaAs     = 3.4;
  fRefrIndAir      = 1.0;
  fRefrIndEpoxy    = 1.5;

  fAbsLength       = 2.2;    // 4 mm is too generous
  fReflProb        = 0.974;		// 0.95;

  fRn = new TRandom3();
//-----------------------------------------------------------------------------
// folders
//-----------------------------------------------------------------------------
  TObject* o = gROOT->GetRootFolder()->FindObject("Ana");
  if (o != 0) {
    gROOT->GetRootFolder()->Remove(o);
    o->Delete();
  }

  fAnaFolder  = gROOT->GetRootFolder()->AddFolder("Ana","TStnAna Folder");
  fFolder     = fAnaFolder->AddFolder("GaasqdQuickSim","GaasqdQuickSim");
  fHistFolder = fFolder   ->AddFolder("Hist"          ,"ListOfHistograms");

  TH1::AddDirectory(0);

  for (int i=0; i<kNEventHistSets; i++) {
    fHist.fEvent[i] = 0;
  }

  for (int i=0; i<kNPhotonHistSets; i++) {
    fHist.fPhoton[i] = 0;
  }

  BookHistograms();

  fDebugLevel = 0;
}


//-----------------------------------------------------------------------------
GaasqdQuickSim::~GaasqdQuickSim() {
}


//_____________________________________________________________________________
void     GaasqdQuickSim::AddHistogram(TObject* hist, const char* FolderName) {
  TFolder* fol = (TFolder*) fFolder->FindObject(FolderName);
  fol->Add(hist); 
}

//_____________________________________________________________________________
void GaasqdQuickSim::HBook1F(TH1F*& Hist, const char* Name, const char* Title,
			 Int_t Nx, Double_t XMin, Double_t XMax,
			 const char* FolderName)
{
  // book 2D histogram, add it to the module's list of histograms and 
  // return pointer to it to the user

  Hist = new TH1F(Name,Title,Nx,XMin,XMax);
  AddHistogram(Hist,FolderName);
}

//_____________________________________________________________________________
void GaasqdQuickSim::HBook2F(TH2F*& Hist, const char* Name, const char* Title,
			 Int_t Nx, Double_t XMin, Double_t XMax,
			 Int_t Ny, Double_t YMin, Double_t YMax,
			 const char* FolderName)
{
  // book 2D histogram, add it to the module's list of histograms and 
  // return pointer to it to the user

  Hist = new TH2F(Name,Title,Nx,XMin,XMax,Ny,YMin,YMax);
  AddHistogram(Hist,FolderName);
}

//_____________________________________________________________________________
void GaasqdQuickSim::HProf(TProfile*& Hist, const char* Name, const char* Title,
		       Int_t Nx, Double_t XMin, Double_t XMax,
		       Double_t YMin, Double_t YMax,
		       const char* FolderName)
{
  // book 2D histogram, add it to the module's list of histograms and 
  // return pointer to it to the user

  Hist = new TProfile(Name,Title,Nx,XMin,XMax,YMin,YMax);
  AddHistogram(Hist,FolderName);
}


//_____________________________________________________________________________
void GaasqdQuickSim::ResetHistograms(TFolder* Folder, const char* Opt) {
  // internal method...

  TObject  *o1;

  if (Folder == 0) Folder = fFolder;

  if (strcmp(Opt,"all") == 0) {

    TIter    it1(Folder->GetListOfFolders());

    while ((o1 = it1.Next())) {
      if (o1->InheritsFrom("TFolder")) {
	ResetHistograms((TFolder*) o1,Opt);
      }
      else if (o1->InheritsFrom("TH1")) {
	((TH1*) o1)->Reset();
      }
    }
  }
}


//-----------------------------------------------------------------------------
int GaasqdQuickSim::BookEventHistograms(EventHist_t* Hist, const char* Folder) {

  HBook1F(Hist->fNPhotons   ,"nphot" ,Form("%s: nphotons",Folder),1000,0,1.e5,Folder);
  HBook1F(Hist->fNDetPhotons,"ndphot" ,Form("%s: ndet photons",Folder),1000,0,1.e5,Folder);
  HBook1F(Hist->fEff        ,"eff" ,Form("%s: eff",Folder),1000,0,1,Folder);

  return 0;
}


//-----------------------------------------------------------------------------
int GaasqdQuickSim::BookPhotonHistograms(PhotonHist_t* Hist, const char* Folder) {

  int    nbx, nby;
  double dx, dy;

  TGeoBBox* box = (TGeoBBox*) fCrystal->GetShape();
  dx = box->GetDX();
  dy = box->GetDY();

  nbx = 100;
  nby = 100;

  HBook1F(Hist->fStop        ,"stop"  ,Form("%s: stop"        ,Folder),  20,0, 20,Folder);
  HBook1F(Hist->fNReflections,"nref"  ,Form("%s: nreflections",Folder), 200,0,200,Folder);
  HBook1F(Hist->fPath        ,"path"  ,Form("%s: path"        ,Folder),5000,0,  5,Folder);
  HBook2F(Hist->fYVsX        ,"y_vs_x",Form("%s: y vs x"      ,Folder),nbx,-dx,dx,nby,-dy,dy,Folder);

  return 0;
}


//-----------------------------------------------------------------------------
int GaasqdQuickSim::BookHistograms() {

  char folder_name[200];

  TFolder* hist_folder = (TFolder*) fFolder->FindObject("Hist");
//-----------------------------------------------------------------------------
// event histograms
//-----------------------------------------------------------------------------
  int  book_event_histset[kNEventHistSets];
  for (int i=0; i<kNEventHistSets; i++) book_event_histset[i] = 0;

  book_event_histset[0] = 1; // all 

  for (int i=0; i<kNEventHistSets; i++) {
    if (book_event_histset[i] != 0) {
      sprintf(folder_name,"evt_%i",i);
      TFolder* fol = (TFolder*) hist_folder->FindObject(folder_name);
      if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
      if (fHist.fEvent[i] == 0) fHist.fEvent[i] = new EventHist_t;
      BookEventHistograms(fHist.fEvent[i],Form("Hist/%s",folder_name));
    }
  }
//-----------------------------------------------------------------------------
// photon histograms
//-----------------------------------------------------------------------------
  int  book_photon_histset[kNPhotonHistSets];
  for (int i=0; i<kNPhotonHistSets; i++) book_photon_histset[i] = 0;

  book_photon_histset[0] = 1;                      // all 
  book_photon_histset[1] = 1;                      // photons reached the photodiode
  book_photon_histset[2] = 1;                      // photons not reached the photodiode

  for (int i=0; i<kNPhotonHistSets; i++) {
    if (book_photon_histset[i] != 0) {
      sprintf(folder_name,"pho_%i",i);
      TFolder* fol = (TFolder*) hist_folder->FindObject(folder_name);
      if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
      if (fHist.fPhoton[i] == 0) fHist.fPhoton[i] = new PhotonHist_t;
      BookPhotonHistograms(fHist.fPhoton[i],Form("Hist/%s",folder_name));
    }
  }
    
  return 0;
}

//-----------------------------------------------------------------------------
int GaasqdQuickSim::FillEventHistograms(EventHist_t* Hist) {
  Hist->fNPhotons->Fill(fNPhotons);
  Hist->fNDetPhotons->Fill(fNDetPhotons);

  double eff = fNDetPhotons/fNPhotons;

  Hist->fEff->Fill(eff);

  return 0;
}

//-----------------------------------------------------------------------------
int GaasqdQuickSim::FillPhotonHistograms(PhotonHist_t* Hist) {

  double x    = fLastPoint.X();
  double y    = fLastPoint.Y();
  double path = fLastPoint.S();

  Hist->fStop->Fill(fStop);
  Hist->fNReflections->Fill(fNReflections);

  Hist->fPath->Fill(path);
  Hist->fYVsX->Fill(x,y);

  return 0;
}

//-----------------------------------------------------------------------------
int GaasqdQuickSim::FillHistograms(const char* Mode) {

  if (strcmp(Mode,"event") == 0) {
//-----------------------------------------------------------------------------
// fill event histograms
//-----------------------------------------------------------------------------
    FillEventHistograms(fHist.fEvent[0]);
  }
  else if (strcmp(Mode,"photon") == 0) {
//-----------------------------------------------------------------------------
// fill photon histograms
//-----------------------------------------------------------------------------
    FillPhotonHistograms(fHist.fPhoton[0]);

    if (fStop == 1) FillPhotonHistograms(fHist.fPhoton[1]);
    if (fStop != 1) FillPhotonHistograms(fHist.fPhoton[2]);
  }
  
  return 0;
}

//-----------------------------------------------------------------------------
// return code:
// ------------
// fStop = 1: photon hits the photodiode
// fStop = 2: photon lost due to reflection inefficiency
//-----------------------------------------------------------------------------
int GaasqdQuickSim::TracePhoton(TTrajectoryPoint* Start, TGeoBBox* Vol) {

  double            x, y, z, nx, ny, nz, r, s, sx, sy, sz, smin, p;
  int               reflection;
  TTrajectoryPoint* tp;

  tp = &fLastPoint;

  tp->SetPoint(Start->X (),Start->Y (),Start->Z (),
	       Start->Nx(),Start->Ny(),Start->Nz(),
	       Start->S(),Start->GetPTotal());

  double xhsize = Vol->GetDX();
  double yhsize = Vol->GetDY();
  double zhsize = Vol->GetDZ();

  fStop = 0;

  fNReflections = 0;

  while (fStop == 0) {
					// find next relection
					// calculate path till the X-wall
    x  = tp->X ();
    y  = tp->Y ();
    z  = tp->Z ();
    nx = tp->Nx();
    ny = tp->Ny();
    nz = tp->Nz();

    double deltax, deltay, deltaz;
    
    if (nx > 0) deltax = ( xhsize-x);
    else        deltax = (-xhsize-x);
    sx = deltax/nx;                     // always positive
					// calculate path till the Y-wall
    if (ny > 0) deltay = ( yhsize-y);
    else        deltay = (-yhsize-y);
    sy = deltay/ny;
					// calculate path till the Z-wall
    if (nz > 0) deltaz = ( zhsize-z);
    else        deltaz = (-zhsize-z);
    sz = deltaz/nz;
					// flag the wall reached first
    if (sx < sy) {
      if (sx < sz) { reflection = 1; smin = sx; }
      else         { reflection = 3; smin = sz; }
    }
    else {
      if (sy < sz) { reflection = 2; smin = sy; }
      else         { reflection = 3; smin = sz; }
    }
//-----------------------------------------------------------------------------
// model absorption
//-----------------------------------------------------------------------------
    double abs_len = fRn->Exp(fAbsLength);
    if (abs_len < smin) {
      fStop = 2;
      smin  = abs_len;
    }
//-----------------------------------------------------------------------------
// update coordinates
//-----------------------------------------------------------------------------
    x  = x+smin*nx;
    y  = y+smin*ny;
    z  = z+smin*nz;
    s  = tp->S()+smin;
    p  = tp->GetPTotal();
    
    tp->SetPoint(x,y,z,nx,ny,nz,s,p);

    if (fStop != 0)                                         goto END_OF_PHOTON_SIM;
//-----------------------------------------------------------------------------
// photon didn't get absorbed, handle reflections 
//-----------------------------------------------------------------------------
    if (reflection == 1) {
//-----------------------------------------------------------------------------
// reflection from X=const surface, check if the photon exits 
//-----------------------------------------------------------------------------
      double sinalp = sqrt(ny*ny+nz*nz);
      if (sinalp < fRefrIndAir/fRefrIndGaAs) {
					// photon exits in X - angle too close to 90 deg
	fStop = 3;
      }
      else {
					// check it a photon gets lost due to reflection inefficiency
	r    = fRn->Rndm();
	if (r < fReflProb) {
	  tp->GetDirection()->SetX(-nx);
	}
	else {
//-----------------------------------------------------------------------------
// diffused scattering - in half of all cases the photon exist the crystal
//-----------------------------------------------------------------------------
	  double nx1, ny1, nz1;
	  fRn->Sphere(nx1,ny1,nz1,1);
	  if (nx1*deltax > 0) {
					// photon exits in X 
	    fStop = 13;
	  }
	  else {
					// photon continues with new direction
	    tp->GetDirection()->SetXYZ(nx1,ny1,nz1);
	  }
	}
      }
    }
    else if (reflection == 2) {
//-----------------------------------------------------------------------------
// reflection from Y=const surface, flip sign on NY
//-----------------------------------------------------------------------------
      double sinalp = sqrt(nx*nx+nz*nz);
      if (sinalp < fRefrIndAir/fRefrIndGaAs) {
					// photon exits in Y - angle too close to 90 deg
	fStop = 4;
      }
      else {
	r    = fRn->Rndm();
	if (r < fReflProb) {
	  tp->GetDirection()->SetY(-ny);
	}
	else {
//-----------------------------------------------------------------------------
// simulate diffused scattering - in half of all cases the photon exist the crystal
//-----------------------------------------------------------------------------
	  double nx1, ny1, nz1;
	  fRn->Sphere(nx1,ny1,nz1,1);
	  if (ny1*deltay > 0) {
					// photon exits in Y due to scattering
	    fStop = 14;
	  }
	  else {
					// photon continues with the new direction
	    tp->GetDirection()->SetXYZ(nx1,ny1,nz1);
	  }
	}
      }
    }
    else if (reflection == 3) {
//-----------------------------------------------------------------------------
// Z-boundary, flip sign on NY
//-----------------------------------------------------------------------------
      double sinalp = sqrt(nx*nx+nz*nz);
	
      if (tp->Nz() > 0) {
//-----------------------------------------------------------------------------
// on the top plane there is a photodiode - check if the photon hits it
//-----------------------------------------------------------------------------
	double dx = tp->X()-fDiodePos[0];
	double dy = tp->Y()-fDiodePos[1];

	TGeoBBox* diode_box = (TGeoBBox*) fDiode->GetShape();
	
	if ( (fabs(dx) < diode_box->GetDX()) &&
	     (fabs(dy) < diode_box->GetDY())    ) {
	  fStop = 1;
	}
	else {
//-----------------------------------------------------------------------------
// photon misses the photodiode, consider reflection from the top surface
//-----------------------------------------------------------------------------
	  if (sinalp < fRefrIndAir/fRefrIndGaAs) {
					// photon exists through the top surface
	    fStop = 5;
	  }
	  else {
//-----------------------------------------------------------------------------
// simulate diffused scattering - in half of all cases the photon exist the crystal
//-----------------------------------------------------------------------------
	    r  = fRn->Rndm();
	    if (r < fReflProb) {
	      tp->GetDirection()->SetZ(-nz);
	    }
	    else {
	      double nx1, ny1, nz1;
	      fRn->Sphere(nx1,ny1,nz1,1);
	      if (nz1*deltaz > 0) {
					// photon exits through the top due to scattering
		fStop = 15;
	      }
	      else {
					// photon continues with the new direction
		tp->GetDirection()->SetXYZ(nx1,ny1,nz1);
	      }
	    }
	  }
	}
      }
      else {
//-----------------------------------------------------------------------------
// reflection from the bottom surface
//-----------------------------------------------------------------------------
	if (fBottomMirror != 0) {
					// always mirror reflection, no losses
	  tp->GetDirection()->SetZ(-nz);
	}
	else {
	  double min_sinalp = fRefrIndEpoxy/fRefrIndGaAs;
	  if (sinalp < min_sinalp) {
	    // photon exists through the bottom surface
	    fStop = 6;
	  }
	  else {
	    //-----------------------------------------------------------------------------
	    // simulate diffused scattering - in half of all cases the photon exist the crystal
	    //-----------------------------------------------------------------------------
	    r  = fRn->Rndm();
	    if (r < fReflProb) {
	      tp->GetDirection()->SetZ(-nz);
	    }
	    else {
	      double nx1, ny1, nz1;
	      fRn->Sphere(nx1,ny1,nz1,1);
	      if (nz1*deltaz > 0) {
		// photon exits through the top due to scattering
		fStop = 16;
	      }
	      else {
		// photon continues with the new direction
		tp->GetDirection()->SetXYZ(nx1,ny1,nz1);
	      }
	    }
	  }
	}
      }
    }

  END_OF_PHOTON_SIM:
    if (fStop == 0) fNReflections++;
//-----------------------------------------------------------------------------
// to avoid infinite loops, limite max number of reflections
//-----------------------------------------------------------------------------
    if (fNReflections >= fMaxNReflections) fStop = 10;
  }

  return 0;
}


//-----------------------------------------------------------------------------
int GaasqdQuickSim::Run(double Dist, int NEvents) {

    float  rn[2];  // to generate phi and theta

    double nx, ny, nz, theta, phi; // , x, y;


  ResetHistograms(0,"all");

  // simulate events, trace photons, andcount the number of detected oned

  for (int ievent=0; ievent<NEvents; ievent++) {
    fEventNumber      = ievent;
    if (fDebugLevel > 0) {
      printf(" >>>> Start event number %10i\n",ievent);
    }
//-----------------------------------------------------------------------------
// reset profile histograms in the beginning
//-----------------------------------------------------------------------------
    ResetHistograms(0,"event");

    fRn->RndmArray(3,rn);

    TGeoBBox* cPos = (TGeoBBox*) fCrystal->GetShape();
    
    if (fPosMode == 0) {
//-----------------------------------------------------------------------------
// fixed distance in X from the photodiode center , Y=0, Z=0
//-----------------------------------------------------------------------------
      fX0 = fDiodePos[0]+Dist;
      fY0 = 0.; // 2*(rn[1]-0.5)*cPos->GetDY();
      fZ0 = 0.; // 2*(rn[2]-0.5)*cPos->GetDZ();
    }
    else if (fPosMode == 1) {
//-----------------------------------------------------------------------------
// randomly within the volume...
//-----------------------------------------------------------------------------      
      fX0 = 2*(rn[0]-0.5)*cPos->GetDX();
      fY0 = 2*(rn[1]-0.5)*cPos->GetDY();
      fZ0 = 2*(rn[2]-0.5)*cPos->GetDZ();
    }
      					// number of photons in this event
    
    fNPhotons    = fRn->Poisson(fNPhMean);
    fNDetPhotons = 0;

    if (fDebugLevel > 0) {
      printf(" >>>> GaasQuickSim::Run nphotons:  %12.0f\n",fNPhotons);
    }
					// generate parameters of i-th photo::n

    for (int iph=0; iph<fNPhotons; iph++) {
      if (fDebugLevel > 0) {
	printf(" >>>> GaasQuickSim::Run simulate photon number:  %10i\n",iph);
      }

      fRn->RndmArray(2,rn);

      phi   = TMath::TwoPi()*rn[0];
      theta = TMath::ASin(2*(rn[1]-0.5));
    
      nx  = TMath::Cos(theta)*TMath::Cos(phi);
      ny  = TMath::Cos(theta)*TMath::Sin(phi);
      nz  = TMath::Sin(theta);     
//-----------------------------------------------------------------------------
// trace photon till it reaches the photodiode of gets absorbed, reflecting
// it from the other faces
//-----------------------------------------------------------------------------
      fPoint.SetPoint(fX0,fY0,fZ0,nx,ny,nz,0,1);

      TracePhoton(&fPoint,(TGeoBBox*) fCrystal->GetShape());
    
      if (fStop == 1) {
//-----------------------------------------------------------------------------
// photon reached the photodiode
//-----------------------------------------------------------------------------
	fNDetPhotons += 1;
      }

      if (fDebugLevel > 0) {
	printf(">>>> GaasQuickSim::Run done simulating photon number:  %10i\n",iph);
      }
//-----------------------------------------------------------------------------
// end of photon tracing - fill "per photon" histograms
//-----------------------------------------------------------------------------
      FillHistograms("photon");
    }
//-----------------------------------------------------------------------------
// end of event processing - fill "per event" histograms
//-----------------------------------------------------------------------------
    FillHistograms("event");
  }

//-----------------------------------------------------------------------------
// on exit, print efficiency
//-----------------------------------------------------------------------------
  double eff    = fHist.fEvent[0]->fEff->GetMean();
  double efferr = fHist.fEvent[0]->fEff->GetMeanError();

  printf(" >> eff, efferr = %12.6f %12.6f\n",eff, efferr);
  return 0;
}
  
//_____________________________________________________________________________
int  GaasqdQuickSim::SaveFolder(TFolder* Folder, TDirectory* Dir) {
  // save Folder into a subdirectory
  // do not write TStnModule's - for each TStnModule save contents of its
  // fFolder

  //  TFolder*     fol;
  TDirectory*  dir;
  TObject*     o;
//-----------------------------------------------------------------------------
// create new subdirectory in Dir to save Folder
//-----------------------------------------------------------------------------
  Dir->cd();
  //  dir = new TDirectory(Folder->GetName(),Folder->GetName(),"");
  dir = Dir->mkdir(Folder->GetName(),Folder->GetName());
  dir->cd();

//   printf(" ------------------- Dir: %s, new dir: %s\n",
// 	 Dir->GetName(),dir->GetName());


  TIter  it(Folder->GetListOfFolders());
  while ((o = it.Next())) {
//     printf(" o->GetName, o->ClassName : %-20s %-20s\n",
// 	   o->GetName(),
// 	   o->ClassName());

    if (strcmp(o->ClassName(),"TFolder") == 0) {
      SaveFolder((TFolder*) o, dir);
      //      dir->cd();
    }
    else if (! o->InheritsFrom("TStnModule")) {
      //      printf("gDirectory->GetPath = %s\n",gDirectory->GetPath());
      o->Write();
      //      gDirectory->GetListOfKeys()->Print();
    }
  }

  Dir->cd();
  return 0;
}

//_____________________________________________________________________________
void GaasqdQuickSim::SaveHist(const char* Filename) {
  // save histograms booked by all the modules into a file with the given name
  // Mode = 2: save directories

  TFile* f = new TFile(Filename,"recreate");

  SaveFolder(fAnaFolder,f);

  f->Close();
  delete f;
}


