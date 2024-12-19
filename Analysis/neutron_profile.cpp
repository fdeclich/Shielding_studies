#include "TTree.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TString.h"
#include "TApplication.h"

#include <iostream>
#include <math.h>
 
void neutron_profile_origin (const TString filename) {
    TFile* myFile = new TFile(filename, "READ");
    TTree* externalTree = myFile->Get<TTree>("ExternalTree");
    float_t origin_vertex_x;
    float_t origin_vertex_z;
    float_t origin_vertex_y;
    int trkID;
    externalTree->SetBranchAddress("origin_vertex.x", &origin_vertex_x);
    externalTree->SetBranchAddress("origin_vertex.z", &origin_vertex_z);
    externalTree->SetBranchAddress("origin_vertex.y", &origin_vertex_y);
    externalTree->SetBranchAddress("trkID", &trkID);
    TH1F* myPlot = new TH1F("Origin Vertex", "Neutron produced in x range", 200, -11000, -9000);
    for (int i = 0; i < externalTree->GetEntries(); i++) {
        externalTree->GetEntry(i);
        if (origin_vertex_z > -2000 && origin_vertex_z < 2000 &&
             origin_vertex_y > -7000 && origin_vertex_y < 7000 && trkID==1) {
            myPlot->Fill(origin_vertex_x);
        }
    }
    TCanvas* c1 = new TCanvas();
    myPlot->Draw();
    c1->SaveAs("Origin vertex analysis");
}

void neutron_profile_scorer (const TString filename) {
    TFile* myFile = new TFile(filename, "READ");
    TTree* externalTree = myFile->Get<TTree>("ExternalTree");
    float_t scorer_vertex_x;
    float_t scorer_vertex_z;
    float_t scorer_vertex_y;
    float_t scorer_energy;
    int pdgID;
    externalTree->SetBranchAddress("scorer_vertex.x", &scorer_vertex_x);
    externalTree->SetBranchAddress("scorer_vertex.z", &scorer_vertex_z);
    externalTree->SetBranchAddress("scorer_vertex.y", &scorer_vertex_y);
    externalTree->SetBranchAddress("pdgID", &pdgID);
    externalTree->SetBranchAddress("scorer_energy", &scorer_energy);
    TFile* outputFile = new TFile("scorer_vertex_output.root", "RECREATE");
    TH1F* myPlot_south_neu = new TH1F("Scorer Vertex south", "Neutron energy spectrum in south wall", 1500, 0, 15);
    TH1F* myPlot_north_neu = new TH1F("Scorer Vertex north", "Neutron energy spectrum in north wall", 1500, 0, 15);
    TH1F* myPlot_upper_neu = new TH1F("Scorer Vertex upper", "Neutron energy spectrum in upper wall", 1500, 0, 15);
    TH1F* myPlot_lower_neu = new TH1F("Scorer Vertex lower", "Neutron energy spectrum in lower wall", 1500, 0, 15);
    TH1F* myPlot_south_gam = new TH1F("Scorer Vertex south gamma", "Gamma energy spectrum in south wall", 100, 0, 12);
    TH1F* myPlot_north_gam = new TH1F("Scorer Vertex north gamma", "Gamma energy spectrum in north wall", 100, 0, 12);
    TH1F* myPlot_upper_gam = new TH1F("Scorer Vertex upper gamma", "Gamma energy spectrum in upper wall", 100, 0, 12);
    TH1F* myPlot_lower_gam = new TH1F("Scorer Vertex lower gamma", "Gamma energy spectrum in lower wall", 100, 0, 12);
    int counter_neu_south = 0;
    int counter_neu_north = 0;
    int counter_neu_upper = 0;
    int counter_neu_lower = 0;
    int counter_gamma_south = 0;
    int counter_gamma_north = 0;
    int counter_gamma_upper = 0;
    int counter_gamma_lower = 0;
    for (int i = 0; i < externalTree->GetEntries(); i++) {
        externalTree->GetEntry(i);
        if (scorer_vertex_z > -2000 && scorer_vertex_z < 2000 && 
            scorer_vertex_y < -2000 && scorer_vertex_y > -14000 &&
            scorer_vertex_x > -9500 && scorer_vertex_x < -9000 &&
            pdgID==2112) {
                myPlot_south_neu->Fill(scorer_energy);
                counter_neu_south ++;
            }
        else if (scorer_vertex_z > -2000 && scorer_vertex_z < 2000 && 
            scorer_vertex_y < -2000 && scorer_vertex_y > -14000 &&
            scorer_vertex_x > -9500 && scorer_vertex_x < -9000 &&
            pdgID==22) {
                myPlot_south_gam->Fill(scorer_energy);
                counter_gamma_south++;
            }
    }
    for (int i = 0; i < externalTree->GetEntries(); i++) {
        externalTree->GetEntry(i);
        if (scorer_vertex_z > -2000 && scorer_vertex_z < 2000 &&
            scorer_vertex_y < -2000 && scorer_vertex_y > -14000 &&
            scorer_vertex_x > 9000 && scorer_vertex_x < 9500 &&
            pdgID==2112) {
                myPlot_north_neu->Fill(scorer_energy);
                counter_neu_north++;
            }
        else if (scorer_vertex_z > -2000 && scorer_vertex_z < 2000 &&
            scorer_vertex_y < -2000 && scorer_vertex_y > -14000 &&
            scorer_vertex_x > 9000 && scorer_vertex_x < 9500 &&
            pdgID==22) {
                myPlot_north_gam->Fill(scorer_energy);
                counter_gamma_north++;
            }
    }
    for (int i = 0; i < externalTree->GetEntries(); i++) {
        externalTree->GetEntry(i);
        if (scorer_vertex_z > -2000 && scorer_vertex_z < 2000 &&
            scorer_vertex_x > -9000 && scorer_vertex_x < 9000 &&
            scorer_vertex_y < -500 && scorer_vertex_y > -1000 &&
            pdgID==2112) {
                myPlot_upper_neu->Fill(scorer_energy);
                counter_neu_upper ++;
            }
        else if(scorer_vertex_z > -2000 && scorer_vertex_z < 2000 &&
            scorer_vertex_x > -9000 && scorer_vertex_x < 9000 &&
            scorer_vertex_y < -500 && scorer_vertex_y > -1000 &&
            pdgID==22) {
                myPlot_upper_gam->Fill(scorer_energy);
                counter_gamma_upper++;
            }
    }
    for (int i = 0; i < externalTree->GetEntries(); i++) {
        externalTree->GetEntry(i);
        if (scorer_vertex_z > -2000 && scorer_vertex_z < 2000 &&
            scorer_vertex_x > -9000 && scorer_vertex_x < 9000 &&
            scorer_vertex_y < -16500 && scorer_vertex_y > -17000 &&
            pdgID==2112) {
                myPlot_lower_neu->Fill(scorer_energy);
                counter_neu_lower ++;
            }
        else if (scorer_vertex_z > -2000 && scorer_vertex_z < 2000 &&
            scorer_vertex_x > -9000 && scorer_vertex_x < 9000 &&
            scorer_vertex_y < -16500 && scorer_vertex_y > -17000 &&
            pdgID==22) {
                myPlot_lower_gam->Fill(scorer_energy);
                counter_gamma_lower++;
            }
    }
    TCanvas* c2 = new TCanvas("c2");
    TCanvas* c3 = new TCanvas("c3");
    c2->Divide(2,2);
    c2->cd(1);
    myPlot_south_neu->SetFillColor(kOrange);
    myPlot_south_neu->Draw();
    myPlot_south_neu->Write();
    c2->cd(2);
    myPlot_north_neu->SetFillColor(kRed);
    myPlot_north_neu->Draw();
    myPlot_north_neu->Write();
    c2->cd(3);
    myPlot_upper_neu->SetFillColor(kGreen);
    myPlot_upper_neu->Draw();
    myPlot_upper_neu->Write();
    c2->cd(4);
    myPlot_lower_neu->SetFillColor(kAzure);
    myPlot_lower_neu->Draw();
    myPlot_lower_neu->Write();
    c2->SaveAs("Neutron energy plots.png");
    myPlot_south_gam->SetFillColor(kOrange);
    myPlot_south_gam->Scale(1.0/counter_gamma_south);
    myPlot_south_gam->Write();
    myPlot_north_gam->SetFillColor(kRed);
    myPlot_north_gam->Scale(1.0/counter_gamma_north);
    myPlot_north_gam->Write();
    myPlot_upper_gam->SetFillColor(kGreen);
    myPlot_upper_gam->Scale(1.0/counter_gamma_upper);
    myPlot_upper_gam->Write();
    myPlot_lower_gam->SetFillColor(kAzure);
    myPlot_lower_gam->Scale(1.0/counter_gamma_lower);
    myPlot_lower_gam->Write();

    c3->cd();
    myPlot_south_neu->SetLineColor(kOrange);
    myPlot_south_neu->SetFillStyle(0);
    myPlot_south_neu->GetYaxis()->SetRangeUser(0, 50000);
    myPlot_south_neu->Draw();
    myPlot_north_neu->SetLineColor(kRed);
    myPlot_north_neu->SetFillStyle(0);
    myPlot_north_neu->Draw("HIST SAME");
    myPlot_upper_neu->SetLineColor(kGreen);
    myPlot_upper_neu->SetFillStyle(0);
    myPlot_upper_neu->Draw("HIST SAME");
    myPlot_upper_neu->SetLineColor(kGreen);
    myPlot_lower_neu->SetFillStyle(0);
    myPlot_upper_neu->Draw("HIST SAME");
    c3->SaveAs("All neutron spectrums.png");
    std::cout << "---------------------------------NEUTRONS---------------------------" << std::endl;
    std::cout << "Neutron rate normalized for the south wall [#/m^2]: " << (abs(counter_neu_south) /(0.5 * 12)) << "\n" << std::endl;
    std::cout << "Neutron rate normalized for the north wall [#/m^2]: " << (abs(counter_neu_north) /(0.5 * 12)) << "\n" << std::endl;
    std::cout << "Neutron rate normalized for the upper wall [#/m^2]: " << (abs(counter_neu_upper) /(0.5 * 18)) << "\n" << std::endl;
    std::cout << "Neutron rate normalized for the lower wall [#/m^2]: " << (abs(counter_neu_lower) /(0.5 * 18)) << "\n" << std::endl;
    std::cout << "---------------------------------GAMMAS------------------------------" << std::endl;
    std::cout << "Gamma rate normalized for the south wall [#/m^2]: " << (abs(counter_gamma_south) /(0.5 * 12)) << "\n" << std::endl;
    std::cout << "Gamma rate normalized for the north wall [#/m^2]: " << (abs(counter_gamma_north) /(0.5 * 12)) << "\n" << std::endl;
    std::cout << "Gamma rate normalized for the upper wall [#/m^2]: " << (abs(counter_gamma_upper) /(0.5 * 18)) << "\n" << std::endl;
    std::cout << "Gamma rate normalized for the lower wall [#/m^2]: " << (abs(counter_gamma_lower) /(0.5 * 18)) << "\n" << std::endl;
    outputFile->Close();
}   

int main (int argc, char** argv) {
    TString filename = argv[1];
    //neutron_profile_origin(filename);
    neutron_profile_scorer(filename);
    return 0;
}