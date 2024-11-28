//Little macro to analyse all the trees of the output root file of the simulation
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TString.h"

void tree_analysis (TString filename) {
    //File and trees reading
    TFile* myFile = new TFile(filename);
    TTree* water_Tree = myFile->Get<TTree>("Shielding_water_PV");
    TTree* ply_Tree_0 = myFile->Get<TTree>("Shielding_plywood0_PV");
    TTree* poly_Tree_0 = myFile->Get<TTree>("Shielding_poly0_PV");
    TTree* borpol_Tree_0 = myFile->Get<TTree>("Shielding_borpol0_PV");
    TTree* ply_Tree_1 = myFile->Get<TTree>("Shielding_plywood1_PV");
    //Variables definitions
    double en_depo_water = 0;
    double en_depo_ply_0 = 0;
    double en_depo_borpol_0 = 0;
    double en_depo_poly_0 = 0;
    double en_depo_ply_1 = 0;
    //Branch setting
    water_Tree->SetBranchAddress("Energy_Deposit", &en_depo_water);
    ply_Tree_0->SetBranchAddress("Energy_Deposit", &en_depo_ply_0);
    borpol_Tree_0->SetBranchAddress("Energy_Deposit", &en_depo_borpol_0);
    poly_Tree_0->SetBranchAddress("Energy_Deposit", &en_depo_poly_0);
    ply_Tree_1->SetBranchAddress("Energy_Deposit", &en_depo_ply_1);
    //Creating the canvas and all the histos
    TCanvas* c1 = new TCanvas();
    c1->Divide(3,2);
    TH1F* hist_en_depo_water = new TH1F ("en_depo_water", "Deposited Energy in Water", 10, -0.02, 0.02);
    TH1F* hist_en_depo_ply_0 = new TH1F ("en_depo_ply0", "Deposited Energy in First Plywood", 100, -0.02, 0.02);
    TH1F* hist_en_depo_borpol_0 = new TH1F ("en_depo_borpol0", "Deposited Energy in First Borated Polyethilene", 50, -0.02, 0.02);
    TH1F* hist_en_depo_poly_0 = new TH1F ("en_depo_poly0", "Deposited Energy in First Plywood", 20, -0.02, 0.02);
    TH1F* hist_en_depo_ply_1 = new TH1F ("en_depo_ply1", "Deposited Energy in Second Plywood", 5, -0.02, 0.02);
    //Filling all the histos
    for (int i = 0; i < water_Tree->GetEntries(); i ++) {
        hist_en_depo_water->Fill(en_depo_water);
    }
    for (int i = 0; i < ply_Tree_0->GetEntries(); i ++) {
        hist_en_depo_ply_0->Fill(en_depo_ply_0);
    }
    for (int i = 0; i < borpol_Tree_0->GetEntries(); i ++) {
        hist_en_depo_borpol_0->Fill(en_depo_borpol_0);
    }
    for (int i = 0; i < poly_Tree_0->GetEntries(); i ++) {
        hist_en_depo_poly_0->Fill(en_depo_poly_0);
    }
    for (int i = 0; i < ply_Tree_1->GetEntries(); i ++) {
        hist_en_depo_ply_1->Fill(en_depo_ply_1);
    }
    //Drawing all of this
    c1->cd(1);
    hist_en_depo_water->GetXaxis()->SetTitle("Energy deposited [MeV]");
    hist_en_depo_water->GetYaxis()->SetTitle("Number of entries");
    hist_en_depo_water->SetFillColor(kAzure);
    hist_en_depo_water->Draw();
    c1->cd(2);
    hist_en_depo_ply_0->GetXaxis()->SetTitle("Energy deposited [MeV]");
    hist_en_depo_ply_0->GetYaxis()->SetTitle("Number of entries");
    hist_en_depo_ply_0->SetFillColor(kSpring-1);
    hist_en_depo_ply_0->Draw();
    c1->cd(3);
    hist_en_depo_borpol_0->GetXaxis()->SetTitle("Energy deposited [MeV]");
    hist_en_depo_borpol_0->GetYaxis()->SetTitle("Number of entries");
    hist_en_depo_borpol_0->SetFillColor(kGreen);
    hist_en_depo_borpol_0->Draw();
    c1->cd(4);
    hist_en_depo_poly_0->GetXaxis()->SetTitle("Energy deposited [MeV]");
    hist_en_depo_poly_0->GetYaxis()->SetTitle("Number of entries");
    hist_en_depo_poly_0->SetFillColor(kYellow-7);
    hist_en_depo_poly_0->Draw();
    c1->cd(5);
    hist_en_depo_ply_1->GetXaxis()->SetTitle("Energy deposited [MeV]");
    hist_en_depo_ply_1->GetYaxis()->SetTitle("Number of entries");
    hist_en_depo_ply_1->SetFillColor(kSpring-2);
    hist_en_depo_ply_1->Draw();
    
    c1->Print("Tree_analysis.png", ".png");
}