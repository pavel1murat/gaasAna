//
#ifndef gaasAna_ana_GaasqdQuickSim_hh
#define gaasAna_ana_GaasqdQuickSim_hh

#include "TNamed.h"
#include "TRandom3.h"
#include "TH2.h"
#include "TF1.h"
#include "TProfile.h"
#include "TFolder.h"
#include "TFile.h"
#include "TGeoManager.h"
#include "TGeoBBox.h"

#include "gaasAna/ana/TTrajectoryPoint.hh"

class GaasqdQuickSim : public TNamed {
public:

  enum  {
	 kNEventHistSets  = 10,
	 kNPhotonHistSets = 10,
  };
					// event histograms
  struct PhotonHist_t {
    TH1F*   fStop;
    TH1F*   fNReflections;
    TH1F*   fPath;
    TH2F*   fYVsX;
  };

  struct EventHist_t {
    TH1F*   fNPhotons;
    TH1F*   fNDetPhotons;
    TH1F*   fEff;
  };
					// analysis hist structure
  struct Hist_t {
    EventHist_t*  fEvent [kNEventHistSets];
    PhotonHist_t* fPhoton[kNPhotonHistSets];
  };

  Hist_t           fHist;
				     // sensor dimensions (half-size)
  double           fSensorDx;
  double           fSensorDy;
  double           fSensorDz;
				     // photodiode dimensions (half size)
  double           fDiodeDx;
  double           fDiodeDy;
  double           fDiodeDz;
  
  double           fDiodeDispX;		// diode displacement in X
  
  TGeoManager*     fGeoManager;

  TGeoVolume*      fCrystal;		// use ROOT class for that
  TGeoVolume*      fDiode;

  TGeoNode*        fDiodeNode;
					// as returned by ROOT
  const double*    fDiodePos;

  int              fNReflections;

  double           fYield;

  double           fX0;
  double           fY0;
  double           fZ0;

  int              fStop;

  double           fNPhMean; 
  double           fNPhotons;		// per event
  double           fNDetPhotons;	// number of detected photons

  TTrajectoryPoint fPoint;
  TTrajectoryPoint fLastPoint;

  TRandom3*        fRn;

  long int         fEventNumber;
					// =0: fixed X plane
					// =1: poits distributed uniformly within the volume
  int              fPosMode;      

  TFolder*         fAnaFolder;
  TFolder*         fFolder;
  TFolder*         fHistFolder;

  int              fDebugLevel;
  int              fMaxNReflections;

  double           fRefrIndGaAs;
  double           fRefrIndAir;
  double           fRefrIndEpoxy;
  double           fAbsLength;
					// probability of mirror reflection, 1-that
					// gives the probability of diffused reflection
  double           fReflProb;		
  double           fPhotoEff;

  int              fBottomMirror;
//-----------------------------------------------------------------------------
//public:
//-----------------------------------------------------------------------------
public:
					// defaults: N1816-PCBGBAC-E2
  
  GaasqdQuickSim (double DiodeDx=0.0025, double DiodeDy = 0.0150);
  ~GaasqdQuickSim(); 
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
  void SetCrystalDimensions(double Dx, double Dy, double Dz) {
    fSensorDx = Dx; fSensorDy = Dy; fSensorDz = Dz; 
  }

  void SetDiodeSize(double Dx, double Dy) {
    fDiodeDx = Dx; fDiodeDy = Dy;
  }
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
  int  BookHistograms  ();
  int  BookEventHistograms (EventHist_t*  Hist, const char* Folder);
  int  BookPhotonHistograms(PhotonHist_t* Hist, const char* Folder);

  void ResetHistograms (TFolder* Folder = 0, const char* Opt = "");

  int  FillEventHistograms (EventHist_t*  Hist);
  int  FillPhotonHistograms(PhotonHist_t* Hist);
  int  FillHistograms      (const char*   Mode);

  int  Run(double Dist, int Nevents = 1);
  int  TracePhoton(TTrajectoryPoint* Start, TGeoBBox* Vol);
//-----------------------------------------------------------------------------
// the following helper methods allow to save 1 line per request, 
// which in case of 100's booked histograms is a non-negligible number
//-----------------------------------------------------------------------------
  void  AddHistogram(TObject* hist, const char* FolderName = "Hist");

  void  HBook1F(TH1F*& Hist, const char* Name, const char* Title,
		Int_t Nx, Double_t XMin, Double_t XMax,
		const char* FolderName = "Hist");

  void  HBook2F(TH2F*& Hist, const char* Name, const char* Title,
		Int_t Nx, Double_t XMin, Double_t XMax,
		Int_t Ny, Double_t YMin, Double_t YMax,
		const char* FolderName = "Hist");

  void  HProf (TProfile*& Hist, const char* Name, const char* Title,
	       Int_t Nx, Double_t XMin, Double_t XMax,
	       Double_t YMin, Double_t YMax,
	       const char* FolderName = "Hist");

  int  SaveFolder(TFolder* Folder, TDirectory* Dir);
  void SaveHist(const char* Filename); 

  ClassDef(GaasqdQuickSim,0)
};

  
#endif
