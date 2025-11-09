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

#include "GaasEvent.cc"

//-----------------------------------------------------------------------------
int make_ntuple_2(const char* Dir, int RunNumber) {

  std::string output_fn = std::format("gaasqd.{:06d}.root",RunNumber);
  TFile* file      = new TFile(output_fn.data(),"RECREATE");
  TTree* tree      = new TTree("wf","wf");
  GaasEvent* event = new GaasEvent();

  TBranch* branch = tree->Branch("evt","GaasEvent",event,32000,99);
  branch->SetAutoDelete(kFALSE);


  event->run = 1;
  event->evt = 0;
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

        // it looks that each line: t and 14 pulse heights, read them all in 
        
        int nlines = lines.size();
        std::cout << std::format("nlines:{}\n",nlines);

        std::vector<float> time;
        std::vector<float> ph[14];

        float t, h[14];

        for (int i=0; i<nlines; i++) {
          sscanf(lines[i].data(),"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
                 &t,&h[0],&h[1],h+2,h+3,h+4,h+5,h+6,h+7,h+8,h+9,h+10,h+11,h+12,h+13);
          time.push_back(t);
          for (int k=0;k<14; k++) ph[k].push_back(h[k]);
        }
//-----------------------------------------------------------------------------
// make 10 events - the last 4 columns are not used, all zeroes
//-----------------------------------------------------------------------------
        for (int ievent=0; ievent<14; ievent++) {
          event->evt++;
          event->run = RunNumber;
          event->ns  = nlines;
          for (int is=0; is<nlines; is++) {
            event->time[is] = time[is];
            event->adc [is] = ph[ievent].at(is);
          }
//-----------------------------------------------------------------------------
// pre-calculate the baseline
//-----------------------------------------------------------------------------
          double qn(0), sy(0), sy2(0);
          for (int i=3000; i<4200; i++) {
            qn  += 1;
            sy  += event->adc [i];
            sy2 += event->adc [i]*event->adc [i];
          }
          
          double ym  = sy/qn;
          double ym2 = sy2/qn;

          double sig = sqrt(ym2-ym*ym);
        
          event->bl     = ym;
          event->sig_bl = sig;
//-----------------------------------------------------------------------------
// determine the pulse height
//-----------------------------------------------------------------------------
          event->ph = -1e6;
          for (int i=4000; i<5500; i++) {
            float dy = event->bl-event->adc [i];
            if (dy > event->ph) {
              event->ph = dy;
              event->ms = i;
            }
          }
//-----------------------------------------------------------------------------
// fill the tree
//-----------------------------------------------------------------------------
          tree->Fill();
        }

        input_file.close(); // Close the file
      }
    }
  }

  file->Write();
  file->Close();

  delete file;

  return 0;
}
