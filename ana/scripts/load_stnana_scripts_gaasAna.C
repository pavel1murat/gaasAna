//
#include "TInterpreter.h"
#include "gaasAna/ana/scripts/modules.hh"
//-----------------------------------------------------------------------------
int load_stnana_scripts_gaasAna() {
  char        macro[200];
  const char* script[] = { 
			  "gaas.C",
			  0 
  };

  const char* work_dir = gSystem->Getenv("MU2E_SATELLITE_RELEASE");

  TInterpreter* cint = gROOT->GetInterpreter();
  
  for (int i=0; script[i] != 0; i++) {
    sprintf(macro,"%s/gaasAna/ana/scripts/%s",work_dir,script[i]);
    if (! cint->IsLoaded(macro)) {
      cint->LoadMacro(macro);
    }
  }
  
  return 0;
}
