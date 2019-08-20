//

#include "TGeoManager.h"
// #include "TGeoElementTable.h"
#include "TGeoVolume.h"
// #include "TGeoMixture.h"
#include "TGeoMedium.h"

int test_detector_geometry() {

  TGeoManager* gm = new TGeoManager("emoe", "emoe");
  TGeoElementTable *table = gm->GetElementTable();

  TGeoMixture *air = new TGeoMixture("air",4, 0.00120479);
  air->AddElement(table->FindElement("C" ),0.000124);
  air->AddElement(table->FindElement("N" ),0.755268);
  air->AddElement(table->FindElement("O" ),0.231781);
  air->AddElement(table->FindElement("AR"),0.012827);
  
  TGeoMedium   *med = new TGeoMedium  ("AIR",1,air);

  TGeoVolume   *top = gm->MakeBox("TOP",med,1,1,1);
  gm->SetTopVolume(top);

  TGeoMixture *gaas = new TGeoMixture("gaas",2, 5.32);
  gaas->AddElement(table->FindElement ("GA"),1);
  gaas->AddElement(table->FindElement ("AS"),1);

  TGeoMedium*  m_gaas = new TGeoMedium  ("GAAS",2,gaas);

  double dx = 0.2500;
  double dy = 0.0350;
  double dz = 0.00125;
  
  TGeoVolume *sensor   = gm->MakeBox("Sensor",m_gaas, dx,dy,dz);
  sensor->SetTransparency(50);
  top->AddNode(sensor,1,new TGeoTranslation(0.0000,0.0000,0.0000));
//-----------------------------------------------------------------------------
// add photodiode
//-----------------------------------------------------------------------------
  TGeoMixture *ingaas = new TGeoMixture("ingaas",3, 5.32);
  ingaas->AddElement(table->FindElement ("In"),0.05);
  ingaas->AddElement(table->FindElement ("GA"),0.45);
  ingaas->AddElement(table->FindElement ("AS"),0.50);

  TGeoMedium*  m_ingaas = new TGeoMedium  ("INGAAS",3,ingaas);

  double dx_diode = 0.0050;
  double dy_diode = 0.0250;
  double dz_diode = 0.00005;
  
  TGeoVolume *diode   = gm->MakeBox("Diode",m_ingaas, dx_diode,dy_diode,dz_diode);
  diode->SetLineColor(kRed+2);
  diode->SetTransparency(50);
  top->AddNode(diode,2,new TGeoTranslation(-0.200,0.0000,dz_diode+dz));
  
  gGeoManager->CloseGeometry();
  gm->CloseGeometry();

  return 0;
}
