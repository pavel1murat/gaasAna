//
#include "TInterpreter.h"
#include "gaasAna/ana/scripts/modules.hh"
//-----------------------------------------------------------------------------
int load_stnana_scripts_gaasAna() {
  char        macro[200];
  const char* script[] = {
			  "gaas_init_geometry.C", "PWD",
			  "gaas.C"              , "PWD",
			  0 
  };

  const char* work_dir = gSystem->Getenv("MU2E_SATELLITE_RELEASE");

  TInterpreter* cint = gROOT->GetInterpreter();
  
  for (int i=0; script[i] != 0; i+=2) {
    sprintf(macro,"%s/gaasAna/ana/scripts/%s",work_dir,script[i]);
    if (! cint->IsLoaded(macro)) {
      const char* env_var = script[i+1];
      if (gSystem->Getenv(env_var) != 0) cint->LoadMacro(macro);
    }
  }
  
  return 0;
}
