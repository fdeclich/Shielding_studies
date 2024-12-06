#include <iostream>
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TChain.h"

struct TrkEvent_t {
  std::vector<double>* _energy = {}; 
  std::vector<double>* _weight = {};

  TrkEvent_t() {_energy = new std::vector<double>(); _weight = new std::vector<double>();}
  ~TrkEvent_t() {delete _energy; delete _weight;}

  void clear() {_energy->clear(); _weight->clear();}
};

int neutron_spectrum(const TString file_path1, TString file_path2)
{
  TFile* file1 = new TFile(file_path1, "READ");
  TFile* file2 = new TFile(file_path2, "READ");

  std::vector<TString> layer_names = {
    "water_PV", "steel0_PV", "plywood0_PV", "borpol0_PV", "plywood1_PV", 
    "poly0_PV", "plywood2_PV", "steel1_PV", "plywood3_PV", "poly1_PV",
    "plywood4_PV", "borpol1_PV", "plywood5_PV", "steel2_PV"
  };

  std::vector<std::vector<TTree*>> layer_trees(2, std::vector<TTree*>(layer_names.size(), nullptr));
  std::vector<std::vector<TH1D*>> layer_hists(2, std::vector<TH1D*>(layer_names.size(), nullptr));
  std::vector<std::vector<TH1D*>> layer_hists_weights(2, std::vector<TH1D*>(layer_names.size(), nullptr));
  std::vector<std::vector<TrkEvent_t>> layer_events(2, std::vector<TrkEvent_t>(layer_names.size()));

  std::vector<TChain*> layer_chain(layer_names.size(), nullptr);
  
  //Creating histos into every chain
  for (int i = 0; i < layer_names.size(); i++)
  {
    layer_chain[i] = new TChain("Shielding_" + layer_names[i]);
    layer_chain[i]->Add(file_path1);
    layer_chain[i]->Add(file_path2);
    for (int k = 0; k < 2; k++) {
      layer_chain[i]->SetBranchAddress("AbsEnergy", &layer_events[k][i]._energy);
      layer_chain[i]->SetBranchAddress("TrackWeight", &layer_events[k][i]._weight);
    }
    for (int k = 0; k < 2; k++) {
      layer_hists[k][i] = new TH1D(Form("%s_%d", layer_names[i].Data(), k), layer_names[i], 100, 0, 20);
      layer_hists_weights[k][i] = new TH1D(Form("%s_w_%d", layer_names[i].Data(), k), layer_names[i], 10000, 0, 1); 
    }
  }

  //Filling every histo
  for (int i = 0; i < layer_names.size(); i++) {
    for (int k = 0; k < 2; k++) {
      Long64_t n_entries = layer_chain[i]->GetEntries();
      for (Long64_t j = 0; j < n_entries; j++) {
        layer_chain[i]->GetEntry(j);
        auto& event = layer_events[k][i]; // Reference to current TrkEvent_t
        for (size_t l = 0; l < event._energy->size(); l++) {
          double energy = event._energy->at(l);
          double weight = event._weight->at(l);
          layer_hists[k][i]->Fill(energy, weight);
          layer_hists_weights[k][i]->Fill(weight);
        }
      }
    }
}

  //Creating and showing Canvas
  TCanvas* canvas = new TCanvas ("canvas", "Histos comparison", 1600, 1200);
  canvas->Divide(2, layer_names.size());
  for (int s = 0; s < layer_names.size(); s++) {
    for (int k = 0; k < 2; k++) {
      canvas->cd(s*2 + k + 1);
      layer_hists[k][s]->Draw();
    }
  }
  return 0;
}