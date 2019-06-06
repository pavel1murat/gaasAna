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
void  gaas_reco(int DebugBit = -1) {
  m_gre = (TGaasRecoModule*) g.x->AddModule("TGaasRecoModule",0);  

  if (DebugBit >= 0) m_gre->SetDebugBit(DebugBit,1);
}


void  spmc_ana(int DebugBit = -1) {
  m_spmc = (TGaasSpmcAnaModule*) g.x->AddModule("TGaasSpmcAnaModule",0);  

  if (DebugBit >= 0) m_spmc->SetDebugBit(DebugBit,1);
}
