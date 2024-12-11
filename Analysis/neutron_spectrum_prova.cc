#include <iostream>
#include <fstream>
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TChain.h"
#include "TRatioPlot.h"
#include "TLegend.h"

struct TrkEvent_t {
  std::vector<double>* _energy = {}; 
  std::vector<double>* _weight = {};

  TrkEvent_t() {_energy = new std::vector<double>(); _weight = new std::vector<double>();}
  ~TrkEvent_t() {delete _energy; delete _weight;}

  void clear() {_energy->clear(); _weight->clear();}
};

std::vector<TH1D*> biased_hists(const TString file_list_path)
{
  std::vector<TString> layer_names = {
    "water_PV", "steel0_PV", "plywood0_PV", "borpol0_PV", "plywood1_PV", 
    "poly0_PV", "plywood2_PV", "steel1_PV", "plywood3_PV", "poly1_PV",
    "plywood4_PV", "borpol1_PV", "plywood5_PV", "steel2_PV"
  };

  std::string branch1 = "AbsEnergy";
  std::string branch2 = "TrackWeight";
  std::vector<TH1D*> biased_layer_hists(layer_names.size(), nullptr);
  std::vector<TH1D*> biased_layer_hists_weights(layer_names.size(), nullptr);
  std::vector<TChain> chains(layer_names.size());
  std::vector<TrkEvent_t> layer_events(layer_names.size());

  std::vector<TString> files;
  std::ifstream file_list;
  file_list.open(file_list_path);
  if (!file_list.is_open()) {
    std::cerr << "Error: could not open file list" << std::endl;
    //return 1;
  }
  std::string line; 
  while (std::getline(file_list, line)) {
    files.push_back(line);
    std::cout << "File aggiunto: " << line << std::endl;
  }
  file_list.close();

  //Creating histos into every chain
  for (int i = 0; i < layer_names.size(); i++) {
    TString layer = layer_names[i];
    TH1D* layer_hist = new TH1D (Form("%s", layer.Data()), layer, 100, 0, 16);
    TH1D* layer_hist_weight = new TH1D(Form("%s_w", layer.Data()), layer, 10000, 0, 1);

    biased_layer_hists[i] = layer_hist;
    biased_layer_hists_weights[i] = layer_hist_weight;

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
        biased_layer_hists[i]->Fill((*energy)[s], (*weight)[s]);
        biased_layer_hists_weights[i]->Fill((*weight)[s]);
      }
      i++; 
    }
  }
  //Creating canvas to show everything
  /*
  TCanvas* canvas = new TCanvas("canvas", "Biased Histos Comparison", 1600, 1200);
  canvas->DivideSquare(layer_names.size());
  for (int i = 0; i < layer_names.size(); i++) {
    canvas->cd(i+1);
    biased_layer_hists[i]->SetLineColor(i+1);
    biased_layer_hists[i]->Draw("HIST");
  }
  canvas->SaveAs("Biased Comparison Energy Weighted.png");
  chains.clear();*/
  return biased_layer_hists;
}

std::vector<TH1D*> unbiased_hists(const TString file_list_path)
{
  std::vector<TString> layer_names = {
    "water_PV", "steel0_PV", "plywood0_PV", "borpol0_PV", "plywood1_PV", 
    "poly0_PV", "plywood2_PV", "steel1_PV", "plywood3_PV", "poly1_PV",
    "plywood4_PV", "borpol1_PV", "plywood5_PV", "steel2_PV"
  };

  std::string branch1 = "AbsEnergy";
  std::string branch2 = "TrackWeight";
  std::vector<TH1D*> unbiased_layer_hists(layer_names.size(), nullptr);
  std::vector<TH1D*> unbiased_layer_hists_weights(layer_names.size(), nullptr);
  std::vector<TChain> chains_unb(layer_names.size());
  std::vector<TrkEvent_t> layer_events(layer_names.size());

  std::vector<TString> files;
  std::ifstream file_list;
  file_list.open(file_list_path);
  if (!file_list.is_open()) {
    std::cerr << "Error: could not open file list" << std::endl;
    //return 1;
  }
  std::string line; 
  while (std::getline(file_list, line)) {
    files.push_back(line);
    std::cout << "File aggiunto: " << line << std::endl;
  }
  file_list.close();

  //Creating histos into every chain
  for (int i = 0; i < layer_names.size(); i++) {
    TString layer = layer_names[i];
    TH1D* layer_hist = new TH1D (Form("%s_unb", layer.Data()), layer, 100, 0, 16);
    TH1D* layer_hist_weight = new TH1D(Form("%s_w_unb", layer.Data()), layer, 10000, 0, 1);

    unbiased_layer_hists[i] = layer_hist;
    unbiased_layer_hists_weights[i] = layer_hist_weight;

    chains_unb[i].SetName("Shielding_"+layer);

    for (int k = 0; k < files.size(); k++) {
      chains_unb[i].Add(files[k]);
    }

    chains_unb[i].SetBranchAddress(branch1.c_str(), &layer_events[i]._energy);
    chains_unb[i].SetBranchAddress(branch2.c_str(), &layer_events[i]._weight);
  }

  for (const auto& chain : chains_unb) {
    printf("Chain %s has %lli entries\n", chain.GetName(), chain.GetEntries());
  }

  const Long64_t n_entries = chains_unb[0].GetEntries();
  for (int s = 0; s < n_entries; s++) {
    int i=0; 
    for (auto& chain : chains_unb) {
      layer_events[i].clear();
      chain.GetEntry(s);
      auto& energy = layer_events[i]._energy;
      auto& weight = layer_events[i]._weight;
      for (int s = 0; s < energy->size(); s ++) {
        unbiased_layer_hists[i]->Fill((*energy)[s], (*weight)[s]);
        unbiased_layer_hists_weights[i]->Fill((*weight)[s]);
      }
      i++; 
    }
  }
  /*
  //Creating canvas to show everything
  TCanvas* canvas_unb = new TCanvas("canvas_unb", "Unbiased Histos Comparison", 1600, 1200);
  canvas_unb->DivideSquare(layer_names.size());
  for (int i = 0; i < layer_names.size(); i++) {
    canvas_unb->cd(i+1);
    unbiased_layer_hists[i]->SetLineColor(i+1);
    unbiased_layer_hists[i]->Draw("HIST");
  }
  canvas_unb->SaveAs("Unbiased Comparison Energy Weighted.png");
  chains_unb.clear();*/
  return unbiased_layer_hists;
}

void histos_compatibility (std::vector<TH1D*> biased, std::vector<TH1D*> unbiased) {
  std::vector <double_t> chis(biased.size());
  std::vector <TRatioPlot*> res(biased.size());
  std::vector<TCanvas*> canvas(biased.size());
  for (int i = 0; i < biased.size(); i++) {
    //canvas[i].Divide(1,2);
    canvas[i] = new TCanvas(Form("Canvas_%d", i), "Comparison");
    biased[i]->GetXaxis()->SetRange(2, unbiased[i]->GetNbinsX());
    double n_entry_b = biased[i]->Integral(2, biased[i]->GetNbinsX());
    double n_entry_u = unbiased[i]->Integral(2, unbiased[i]->GetNbinsX());
    biased[i]->Scale(1/n_entry_b);
    unbiased[i]->Scale(1/n_entry_u);
    chis[i] = unbiased[i]->Chi2Test(biased[i], "WW");
    res[i] = new TRatioPlot(unbiased[i], biased[i]);
    unbiased[i]->SetLineColor(kRed);
    res[i]->Draw();
    res[i]->GetUpperPad()->cd();
    TLegend* legend = new TLegend(0.15,0.7,0.4,0.85);
    legend->AddEntry(biased[i], "Biased histo", "l");
    legend->AddEntry(unbiased[i], "Unbiased histo", "l");
    legend->Draw();
    canvas[i]->SaveAs(Form ("../Images/Comparison_%d.png", i));
    std::cout << "Chi2 compatibility test result for the " << i << " layer: " << chis[i] << std::endl;
  }
}
int main (int argc, char* argv[]) {
  TString file_list_path_biased = argv[1];
  TString file_list_path_unbiased = argv[2];
  histos_compatibility(biased_hists(file_list_path_biased), unbiased_hists(file_list_path_unbiased));
  return 0;
}