#include "TH1D.h"
#include "TFile.h"
#include "TString.h"

#include <ostream>
#include <iostream>
#include <fstream>

void spacchettamento (const TString filename) {
    TFile* myFile(TFile::Open(filename));
    TH1D* myHist = myFile->Get<TH1D>("hEnergyNeutron");
    std::ofstream outfile("Neutron_spectrum.dat");
    //outfile << "#Energy\tWeight\n";
    //myHist->Rebin(10);
    for (int i = 0; i < myHist->GetNbinsX(); i++) {
        double bincenter = myHist->GetBinCenter(i);
        double binvalue = myHist->GetBinContent(i);
        outfile << bincenter << " " << binvalue << "\n";
    }
    outfile.close();
}