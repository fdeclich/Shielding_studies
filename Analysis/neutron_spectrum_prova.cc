#include <iostream>
#include <fstream>
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TChain.h"
#include "TLegend.h"

struct TrkEvent_t {
  std::vector<double>* _energy = {}; 
  std::vector<double>* _weight = {};

  TrkEvent_t() {_energy = new std::vector<double>(); _weight = new std::vector<double>();}
  ~TrkEvent_t() {delete _energy; delete _weight;}

  void clear() {_energy->clear(); _weight->clear();}
};

int neutron_spectrum_prova(const TString file_list_path)
{
  std::vector<TString> layer_names = {
    "water_PV", "steel0_PV", "plywood0_PV", "borpol0_PV", "plywood1_PV", 
    "poly0_PV", "plywood2_PV", "steel1_PV", "plywood3_PV", "poly1_PV",
    "plywood4_PV", "borpol1_PV", "plywood5_PV", "steel2_PV"
  };

  std::string branch1 = "AbsEnergy";
  std::string branch2 = "TrackWeight";
  std::vector<TH1D*> layer_hists(layer_names.size(), nullptr);
  std::vector<TH1D*> layer_hists_weights(layer_names.size(), nullptr);
  std::vector<TChain> chains(layer_names.size());
  std::vector<TrkEvent_t> layer_events(layer_names.size());

  std::vector<TString> files;
  std::ifstream file_list;
  file_list.open(file_list_path);
  if (!file_list.is_open()) {
    std::cerr << "Error: could not open file list" << std::endl;
    return 1;
  }
  std::string line; 
  while (std::getline(file_list, line)) {
    files.push_back(line);
  }
  file_list.close();

  //Creating histos into every chain
  for (int i = 0; i < layer_names.size(); i++) {
    TString layer = layer_names[i];
    TH1D* layer_hist = new TH1D (Form("%s", layer.Data()), layer, 100, 0, 20);
    TH1D* layer_hist_weight = new TH1D(Form("%s_w", layer.Data()), layer, 10000, 0, 1);

    layer_hists[i] = layer_hist;
    layer_hists_weights[i] = layer_hist_weight;

    chains[i].SetName("Shielding_"+layer);

    for (int k = 0; k < files.size(); k++) {
      chains[i].Add(files[k]);
    }

    chains[i].SetBranchAddress(branch1.c_str(), &layer_events[i]._energy);
    chains[i].SetBranchAddress(branch2.c_str(), &layer_events[i]._weight);
  }

  for (const auto& chain : chains) {
    printf("Chain %s has %lli entries\n", chain.GetName(), chain.GetEntries());
  }

  const Long64_t n_entries = chains[0].GetEntries();
  for (int s = 0; s < n_entries; s++) {
    int i=0; 
    for (auto& chain : chains) {
      layer_events[i].clear();
      chain.GetEntry(s);
      auto& energy = layer_events[i]._energy;
      auto& weight = layer_events[i]._weight;
      for (int s = 0; s < energy->size(); s ++) {
        layer_hists[i]->Fill((*energy)[s], (*weight)[s]);
        layer_hists_weights[i]->Fill((*weight)[s]);
      }
      i++; 
    }
  }
  //Creating canvas to show everything
  TCanvas* canvas = new TCanvas("canvas", "Histos Comparison", 1600, 1200);
  canvas->DivideSquare(layer_names.size());
  for (int i = 0; i < layer_names.size(); i++) {
    canvas->cd(i+1);
    layer_hists[i]->SetLineColor(i+1);
    layer_hists[i]->Draw("HIST");
  }
  canvas->SaveAs("Comparison Energy Weighted.png");
  return 0;
}
