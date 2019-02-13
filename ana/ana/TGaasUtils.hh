#ifndef TGaasUtils_hh
#define TGaasUtils_hh

#include "TObject.h"

class TGaasUtils : public TObject {

public:

  TGaasUtils ();
  ~TGaasUtils();

  static int FitPol0(const float* V, int IMin, int IMax, double* Par, double* Chi2);
  static int FitPol1(const float* V, int IMin, int IMax, double* Par, double* Chi2);

  ClassDef (TGaasUtils,0)
};



#endif
