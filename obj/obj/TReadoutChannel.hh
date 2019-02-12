///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#ifndef __gaasAna_obj_TReadoutChannel_hh__
#define __gaasAna_obj_TReadoutChannel_hh__

#include <vector>
#include "TObject.h"

class TReadoutChannel : public TObject {
protected:
  int fNSamples;

  std::vector<float>  fT;
  std::vector<float>  fV0;
  std::vector<float>  fV1;    // corrected for pedestal;

public:
//-----------------------------------------------------------------------------
// constructors and destructor
//-----------------------------------------------------------------------------
  TReadoutChannel();
  TReadoutChannel(int NSamples = -1);
  ~TReadoutChannel();
//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------
  int GetNSamples() { return fNSamples; }
//-----------------------------------------------------------------------------
// overloaded methods of TObject
//-----------------------------------------------------------------------------
  virtual void Clear(Option_t* Opt); 
  
  ClassDef(TReadoutChannel,1)
};

#endif
