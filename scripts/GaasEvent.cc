#ifndef __GaasEvent_cc__
#define __GaasEvent_cc__

class GaasEvent { // : public TObject {
public:
  int            run;
  int            evt;           // event number
  int            nsd;           // number of straw digis in event
  float          bl;            // 
  float          sig_bl;        // 
  int            ns;            // N(ADC samples)
  float*         time;          //[ns] ns shorts  
  float*         adc;           //[ns] ns shorts
  float          ph;            // pulse height
  float          ms;            // max sample
  GaasEvent();
  ~GaasEvent();

  void Clear();
};

//-----------------------------------------------------------------------------
GaasEvent::GaasEvent() {
  run  = -1;
  evt  = -1;
  ns   = 20000;
  time = new float[ns];
  adc  = new float[ns];
}

GaasEvent::~GaasEvent() {
  delete time;
  delete adc;
}

void GaasEvent::Clear() {
}

#endif

