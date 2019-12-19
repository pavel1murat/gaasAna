///////////////////////////////////////////////////////////////////////////////
// different initializations of the quick simulation
///////////////////////////////////////////////////////////////////////////////
#include "gaasAna/ana/GaasqdQuickSim.hh"

GaasqdQuickSim* qs(nullptr);

//-----------------------------------------------------------------------------
// run init_qs , then can do qs->Run(....) - geometry and detector are defined
// so far, assume just one detector, but positioned differently...
//-----------------------------------------------------------------------------
int init_n1816() {

  qs = new GaasqdQuickSim();

  // init sensor dimensions : N1816-PCBGBAC-E2

  double sensor[3] = { 0.2500, 0.0350, 0.00125 };

  // init detectors

  GaasqdQuickSim::Detector_t det;

  det.fDx      = 0.0025;		//  50 um
  det.fDy      = 0.0150;		// 300 um
  det.fDz      = 0.0020; // just to make the diode visible, thickness is not used so far

  det.fSide    = 5;			// Z-side,top
  
  det.fOffsetX = -0.2000;
  det.fOffsetY =  0.0000;
  det.fOffsetZ =  0.0000;

  det.fType    = 0;         // photodiode

  int ndet = 1;
  qs->InitGeometry(sensor, &det, ndet);

  // after that, one can call qs->Run(...)
  return 0;
}


//-----------------------------------------------------------------------------
// run init_qs , then can do qs->Run(....) - geometry and detector are defined
// so far, assume just one detector, but positioned differently...
//-----------------------------------------------------------------------------
int init_suny() {

  qs = new GaasqdQuickSim();

  // init sensor dimensions : sensor used for absorption measurements : 8.0 x 0.7 x 0.025

  double sensor[3] = { 0.400, 0.0350, 0.00125 };

  // init detectors

  GaasqdQuickSim::Detector_t det;

  det.fDx      = 0.0020;	 	//  50 um
  det.fDy      = 0.0025;		// 300 um
  det.fDz      = 0.00125;                // just to make the diode visible, thickness is not used so far

  det.fSide    = 0;			// Z-side,top
  
  det.fOffsetX = -0.4000;
  det.fOffsetY =  0.0000;
  det.fOffsetZ =  0.0000;
  det.fType    = 1;       // fiber with the air gap

  int ndet = 1;
  qs->InitGeometry(sensor, &det, ndet);

  qs->fPosMode = 1;

  // after that, one can call qs->Run(...)
  return 0;
}


//-----------------------------------------------------------------------------
// cross check against SUNY simulation: detect light reaching the edge
//-----------------------------------------------------------------------------
int validate_suny(int DebugLevel = 0) {

  qs = new GaasqdQuickSim();

  // init sensor dimensions : sensor used for absorption measurements : 8.0 x 0.7 x 0.025

  double sensor[3] = { 0.400, 0.0350, 0.00125 };

  // init detectors

  GaasqdQuickSim::Detector_t det;

  det.fDx      = 0.0025;	 	// to make it visible
  det.fDy      = sensor[1];		// 
  det.fDz      = sensor[2];             // 

  det.fSide    = 0;			// X-side, left
  
  det.fOffsetX = -0.4000;
  det.fOffsetY =  0.0000;
  det.fOffsetZ =  0.0000;

  det.fType    = 0;                     // photodiode

  int ndet     = 1;
  qs->InitGeometry(sensor, &det, ndet);

  qs->fPosMode    = 0;
  qs->fDebugLevel = DebugLevel;
  qs->fReflProb   =  1;
  qs->fNPhMean    = -100;
  qs->fAbsLength  = -1;
  
  //  for (int i=0; i<6; i++) qs->fMirror[i] = 1;

  // after that, one can call qs->Run(...)
  return 0;
}
