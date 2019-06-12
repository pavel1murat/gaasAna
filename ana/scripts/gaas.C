///////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////
#include "gaasAna/ana/scripts/modules.hh"

def_name gaas_001("gaas_reco");
def_name gaas_002("spmc_ana");
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void  gaas_reco(float V0 = -1., float V1 = -1., int DebugBit = -1) {
  m_gre = (TGaasRecoModule*) g.x->AddModule("TGaasRecoModule",0);  

  if (V0       >= 0) m_gre->SetV1Max(V0,V1);
  if (DebugBit >= 0) m_gre->SetDebugBit(DebugBit,1);
}


void  spmc_ana(int DebugBit = -1) {
  m_spmc = (TGaasSpmcAnaModule*) g.x->AddModule("TGaasSpmcAnaModule",0);  

  if (DebugBit >= 0) m_spmc->SetDebugBit(DebugBit,1);
}
