/**
 * @author      : Daniele Guffanti (daniele.guffanti@mib.infn.it)
 * @file        : neutron_spectrum.cc
 * @created     : Thursday Dec 05, 2024 15:34:59 CET
 */

#include <iostream>
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"

struct TrkEvent_t {
  std::vector<double>* _energy = {}; 
  std::vector<double>* _weight = {};

  TrkEvent_t() {_energy = new std::vector<double>(); _weight = new std::vector<double>();}
  ~TrkEvent_t() {delete _energy; delete _weight;}

  void clear() {_energy->clear(); _weight->clear();}
};

int neutron_spectrum(const TString file_path)
{
  TFile* file = new TFile(file_path, "READ");

  std::vector<TString> layer_names = {
    "water_PV", "steel0_PV", "plywood0_PV", "borpol0_PV", "plywood1_PV", 
    "poly0_PV", "plywood2_PV", "steel1_PV", "plywood3_PV", "poly1_PV",
    "plywood4_PV", "borpol1_PV", "plywood5_PV", "steel2_PV"
  };

  std::vector<TTree*> layer_trees(layer_names.size(), nullptr);
  std::vector<TH1D*> layer_hists(layer_names.size(), nullptr);
  std::vector<TH1D*> layer_hists_weights(layer_names.size(), nullptr);
  std::vector<TrkEvent_t> layer_events(layer_names.size());

  for (int i = 0; i < layer_names.size(); i++)
  {
    printf("layer_name: %s\n", layer_names[i].Data());
    layer_trees[i] = (TTree*)file->Get("Shielding_"+layer_names[i]);
    layer_trees[i]->SetBranchAddress("AbsEnergy", &layer_events[i]._energy);
    layer_trees[i]->SetBranchAddress("TrackWeight", &layer_events[i]._weight);
    layer_hists[i] = new TH1D(layer_names[i], layer_names[i], 100, 0, 20);
    layer_hists_weights[i] = new TH1D(layer_names[i]+"_w", layer_names[i], 10000, 0, 1);
  }

  const Long64_t n_entries = layer_trees[0]->GetEntries();

  for (Long64_t i = 0; i < n_entries; i++)
  {
    for (int j = 0; j < layer_names.size(); j++)
    {
      layer_trees[j]->GetEntry(i);
      for (int k = 0; k < layer_events[j]._energy->size(); k++)
      {
        layer_hists[j]->Fill(layer_events[j]._energy->at(k), layer_events[j]._weight->at(k));
        layer_hists_weights[j]->Fill(layer_events[j]._weight->at(k));
      }
      layer_events[j].clear();
    }
  }

  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 600);
  canvas->DivideSquare( layer_names.size() );
  for (int i = 0; i < layer_names.size(); i++)
  {
    canvas->cd(i+1);
    layer_hists[i]->Draw();
  }
    
  return 0;
}