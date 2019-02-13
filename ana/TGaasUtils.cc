//
#include "gaasAna/ana/TGaasUtils.hh"

//-----------------------------------------------------------------------------
TGaasUtils::TGaasUtils() {
}

//-----------------------------------------------------------------------------
TGaasUtils::~TGaasUtils() {
}

//-----------------------------------------------------------------------------
// with weights equal to one
//-----------------------------------------------------------------------------
int TGaasUtils::FitPol0(const float* V, int IMin, int IMax, double* Par, double* Chi2) {

  int rc(0);

  double   n, sx, sx2, xm, x2m, sig2;

  n   = 0;
  sx  = 0;
  sx2 = 0;
  
  for (int i=IMin; i<=IMax; i+=1) {
    sx  += V[i];
    sx2 += V[i]*V[i];
    n   += 1;
  }

  xm   = sx/n;
  x2m  = sx2/n;
  sig2 = x2m-xm*xm;

  Par [0] = xm;
  *Chi2   = sig2*n/(n-1+1.e-12);

  return rc;
}

//-----------------------------------------------------------------------------
// with weights equal to one
//-----------------------------------------------------------------------------
int TGaasUtils::FitPol1(const float* V, int IMin, int IMax, double* Par, double* Chi2) {

  int rc(0);

  double   n, x, y, sx, sy, sx2, sxy, sy2, xm, ym, x2m, xym, y2m, sigxx, sigxy, sigyy;

  n   = 0;
  sx  = 0;
  sy  = 0;
  sx2 = 0;
  sxy = 0;
  sy2 = 0;
  
  for (int i=IMin; i<=IMax; i+=1 ) {
    x   = i+ 0.5;
    y   = V[i];
    sx  += x;
    sy  += y;
    sx2 += x*x;
    sxy += x*y;
    sy2 += y*y;
    n   += 1;
  }

  xm  = sx/n;
  ym  = sy/n;
  x2m = sx2/n;
  xym = sxy/n;
  y2m = sy2/n;

  sigxx = x2m -xm*xm;
  sigxy = xym -xm*ym;
  sigyy = y2m -ym*ym;

  Par[1] = sigxy/sigxx;
  Par[0] = ym-Par[1]*xm;
  *Chi2  = (sigyy-sigxy*sigxy/sigxx)*n/(n-2+1.e-12);

  return rc;
}
