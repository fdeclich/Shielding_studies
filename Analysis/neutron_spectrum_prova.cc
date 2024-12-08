#include <iostream>
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

int neutron_spectrum_prova(const TString file_path1, TString file_path2)
{
  TFile* file1 = new TFile(file_path1, "READ");
  TFile* file2 = new TFile(file_path2, "READ");

  std::vector<TString> files = {file_path1, file_path2};
  std::vector<TString> layer_names = {
    "water_PV", "steel0_PV", "plywood0_PV", "borpol0_PV", "plywood1_PV", 
    "poly0_PV", "plywood2_PV", "steel1_PV", "plywood3_PV", "poly1_PV",
    "plywood4_PV", "borpol1_PV", "plywood5_PV", "steel2_PV"
  };

  std::string branch1 = "AbsEnergy";
  std::string branch2 = "TrackWeight";
  std::vector<std::vector<TH1D*>> layer_hists(2, std::vector<TH1D*>(layer_names.size(), nullptr));
  std::vector<std::vector<TH1D*>> layer_hists_weights(2, std::vector<TH1D*>(layer_names.size(), nullptr));
  //std::vector<TrkEvent_t> layer_events(layer_names.size());

  //std::vector<TChain*> layer_chain(layer_names.size(), nullptr);
  
  //Creating histos into every chain
  for (int i = 0; i < layer_names.size(); i++) {
    TString layer = layer_names[i];
    for (int k = 0; k < files.size(); k++) {
      TChain chain ("Shielding_" + layer);
      chain.Add(files[k]);
      TH1D* layer_hist = new TH1D (Form("%s_%d", layer.Data(), k), layer, 100, 0, 20);
      TH1D* layer_hist_weight = new TH1D(Form("%s_w_%d", layer.Data(), k), layer, 10000, 0, 1);
      std::vector<double>* energy = nullptr;
      std::vector<double>* weight = nullptr;
      chain.SetBranchAddress(branch1.c_str(), &energy);
      chain.SetBranchAddress(branch2.c_str(), &weight);
      int n_entries = chain.GetEntries();
      for (int s = 0; s < n_entries; s++) {
        chain.GetEntry(s);
        for (int s = 0; s < energy->size(); s ++) {
          layer_hist->Fill((*energy)[i], (*weight)[i]);
          layer_hist_weight->Fill((*weight)[i]);
        }
        
      }
      layer_hists[k][i] = layer_hist;
      layer_hists_weights[k][i] = layer_hist_weight;
    }
  }
  //Creating canvas to show everything
  TCanvas* canvas = new TCanvas("canvas", "Histos Comparison", 1600, 1200);
  canvas->Divide(4, 4);
  for (int i = 0; i < layer_names.size(); i++) {
    int canvas_pad_left = i * 2 + 1;
    //int canvas_pad_right = canvas_pad_left + 1;
    for (int k = 0; k < files.size(); k++) {
      canvas->cd(canvas_pad_left+k);
      layer_hists[k][i]->SetLineColor(k+1);
      layer_hists[k][i]->Draw("HIST");
    }
  }
  canvas->SaveAs("Comparison Energy Weighted.png");
  return 0;
}