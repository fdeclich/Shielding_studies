#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TString.h"
#include "TCanvas.h"
#include "TKey.h"
#include "TIterator.h"

#include <iostream>
#include <vector>

void comparison (TString filename1, TString filename2) {
    TFile* myFile1 = new TFile(filename1);
    TFile* myFile2 = new TFile(filename2);

    std::vector<std::string> histNames;
    for (int i = 1; i <= 14; i++) {
        histNames.push_back("hist_" + std::to_string(i));
    }

    std::vector<TH1F*> vec_hist_1;
    std::vector<TH1F*> vec_hist_2;

    for (const auto& histName : histNames) {
        TH1F* histogram = dynamic_cast<TH1F*>(myFile1->Get(histName.c_str()));
        vec_hist_1.push_back(histogram);
    }

    for (const auto& histName : histNames) {
        TH1F* histogram = dynamic_cast<TH1F*>(myFile2->Get(histName.c_str()));
        vec_hist_2.push_back(histogram);
    }


    for(int i = 0; i < vec_hist_1.size(); i++) {
        TCanvas* canva = new TCanvas();
        canva->Divide(2,1);
        vec_hist_1.at(i)->GetXaxis()->SetTitle("Energy Deposit");
        vec_hist_2.at(i)->GetXaxis()->SetTitle("Energy Deposit");
        vec_hist_1.at(i)->SetFillColor(kRed);
        vec_hist_2.at(i)->SetFillColor(kAzure);
        canva->cd(1);
        vec_hist_1.at(i)->Draw();
        canva->cd(2);
        vec_hist_2.at(i)->Draw();
        canva->Draw(TString::Format("Histos%d", i));
    }
}