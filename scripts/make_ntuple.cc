///////////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <filesystem> // Required for std::filesystem
#include <format>

#include "stdio.h"

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TClonesArray.h"
#include "TObject.h"

#include "daqana/obj/DaqStrawDigi.hh"
#include "daqana/obj/DaqStrawHit.hh"
#include "daqana/obj/DaqTimeCluster.hh"

class GaasEvent { // : public TObject {
public:
  int            run;
  int            evt;           // event number
  int            nsd;           // number of straw digis in event
  float          bl;            // 
  float          sig_bl;        // 
  int            ns;            //      N(ADC samples)
  float*         time;          //      [ns] ns shorts  
  float*         adc;           //      [ns] ns shorts
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
  ns   = 502;
  time = new float[ns];
  adc  = new float[ns];
}

GaasEvent::~GaasEvent() {
  delete time;
  delete adc;
}

void GaasEvent::Clear() {
}

//-----------------------------------------------------------------------------
int make_ntuple(const char* Dir) {

  TFile* file      = new TFile("aaa.root","RECREATE");
  TTree* tree      = new TTree("digis","digis");
  GaasEvent* event = new GaasEvent();

  TBranch* branch = tree->Branch("evt","GaasEvent",event,32000,99);
  branch->SetAutoDelete(kFALSE);


  event->run = 1;
  event->evt = 1;
  for (const auto& entry : std::filesystem::directory_iterator(Dir)) {
    std::filesystem::path current_path = entry.path();

    if (std::filesystem::is_regular_file(current_path)) {
      std::string   file_path = entry.path().string();

      std::ifstream input_file(file_path);

      if (input_file.is_open()) {
//-----------------------------------------------------------------------------
// new event
//-----------------------------------------------------------------------------
        event->Clear();
        
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(input_file, line)) {
//-----------------------------------------------------------------------------
// skip the header
//-----------------------------------------------------------------------------
          lines.emplace_back(line);
        }

        // skip first 5 lines - the header
        int nlines = lines.size();
        
        for (int i=5; i<nlines; i++) {
          float t, h;
          sscanf(lines[i].data(),"%e,%f",&t,&h);
          // std::cout << std::format("t:{} h:{}\n",t,h);
          int loc = i-5;
          event->time[loc] = t;
          event->adc [loc] = h;
        }
//-----------------------------------------------------------------------------
// pre-calculate the baseline
//-----------------------------------------------------------------------------
        double qn(0), sy(0), sy2(0);
        for (int i=0; i<200; i++) {
          qn  += 1;
          sy  += event->adc [i];
          sy2 += event->adc [i]*event->adc [i];
        }
        double ym  = sy/qn;
        double ym2 = sy2/qn;

        double sig = sqrt(ym2-ym*ym);
        
        event->bl     = ym;
        event->sig_bl = sig;

        event->ph = -1e6;
        for (int i=201; i<502; i++) {
          float dy = event->bl-event->adc [i];
          if (dy > event->ph) {
            event->ph = dy;
            event->ms = i;
          }
        }

        input_file.close(); // Close the file
//-----------------------------------------------------------------------------
// save event
//-----------------------------------------------------------------------------
        tree->Fill();
        event->evt++;
      }
    }
  }

  file->Write();
  file->Close();

  delete file;

  return 0;
}
