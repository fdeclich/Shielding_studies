#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TString.h"

#include <iostream>

void energy_dep_analysis(TString filename) {
    TFile* myFile = new TFile(filename);
    TTree* water_Tree = myFile->Get<TTree>("Shielding_water_PV");
    TTree* ply_Tree_0 = myFile->Get<TTree>("Shielding_plywood0_PV");
    TTree* poly_Tree_0 = myFile->Get<TTree>("Shielding_poly0_PV");
    TTree* borpol_Tree_0 = myFile->Get<TTree>("Shielding_borpol0_PV");
    TTree* ply_Tree_1 = myFile->Get<TTree>("Shielding_plywood1_PV");
    TTree* ply_Tree_2 = myFile->Get<TTree>("Shielding_plywood2_PV");
    TTree* ste_Tree_0 = myFile->Get<TTree>("Shielding_steel0_PV");
    TTree* ply_Tree_3 = myFile->Get<TTree>("Shielding_plywood3_PV");
    TTree* poly_Tree_1 = myFile->Get<TTree>("Shielding_poly1_PV");
    TTree* ply_Tree_4 = myFile->Get<TTree>("Shielding_plywood4_PV");
    TTree* borpol_Tree_1 = myFile->Get<TTree>("Shielding_borpol1_PV");
    TTree* ply_Tree_5 = myFile->Get<TTree>("Shielding_plywood5_PV");
    TTree* ste_Tree_1 = myFile->Get<TTree>("Shielding_steel1_PV");
    TTree* ste_Tree_2 = myFile->Get<TTree>("Shielding_steel2_PV");

    double edep_water;
    double edep_ply_0;
    double edep_borpol_0;
    double edep_poly_0;
    double edep_ply_1;
    double edep_ply_2;
    double edep_ste_0;
    double edep_ply_3;
    double edep_poly_1;
    double edep_ply_4;
    double edep_borpol_1;
    double edep_ply_5;
    double edep_ste_1;
    double edep_ste_2;
    std::vector<double>* absEnergy_water = nullptr;
    std::vector<double>* TrackWeight_water = nullptr;

    water_Tree->SetBranchAddress("Energy_Deposit", &edep_water);
    water_Tree->SetBranchAddress("AbsEnergy", &absEnergy_water);
    water_Tree->SetBranchAddress("TrackWeight", &TrackWeight_water);
    ply_Tree_0->SetBranchAddress("Energy_Deposit", &edep_ply_0);
    borpol_Tree_0->SetBranchAddress("Energy_Deposit", &edep_borpol_0);
    poly_Tree_0->SetBranchAddress("Energy_Deposit", &edep_poly_0);
    ply_Tree_1->SetBranchAddress("Energy_Deposit", &edep_ply_1);
    ply_Tree_2->SetBranchAddress("Energy_Deposit", &edep_ply_2);
    ste_Tree_0->SetBranchAddress("Energy_Deposit", &edep_ste_0);
    ply_Tree_3->SetBranchAddress("Energy_Deposit", &edep_ply_3);
    poly_Tree_1->SetBranchAddress("Energy_Deposit", &edep_poly_1);
    ply_Tree_4->SetBranchAddress("Energy_Deposit", &edep_ply_4);
    borpol_Tree_1->SetBranchAddress("Energy_Deposit", &edep_borpol_1);
    ply_Tree_5->SetBranchAddress("Energy_Deposit", &edep_ply_5);
    ste_Tree_1->SetBranchAddress("Energy_Deposit", &edep_ste_1);
    ste_Tree_2->SetBranchAddress("Energy_Deposit", &edep_ste_2);

    TH1F* hist_edep_water = new TH1F ("hist_1", "Energy Deposit in Water", 100, -0.5, 0.35);
    TH1F* hist_ene_water = new TH1F ("hist_ene", "Entering Energy in Water", 150, 0, 15);
    TH1F* hist_edep_ste_0 = new TH1F ("hist_2", "Energy Deposit in First Steel", 100, -0.5, 0.35);
    TH1F* hist_edep_ply_0 = new TH1F ("hist_3", "Energy Deposit in First Plywood", 100, -0.5, 0.35);
    TH1F* hist_edep_borpol_0 = new TH1F ("hist_4", "Energy Deposit in First Borated Polyethilene", 100, -0.5, 0.35);
    TH1F* hist_edep_ply_1 = new TH1F ("hist_5", "Energy Deposit in Second Plywood", 100, -0.5, 0.35);
    TH1F* hist_edep_poly_0 = new TH1F ("hist_6", "Energy Deposit in First Polyurethane", 100, -0.5, 0.35);
    TH1F* hist_edep_ply_2 = new TH1F ("hist_7", "Energy Deposit in Third Plywood", 100, -0.5, 0.35);
    TH1F* hist_edep_ste1 = new TH1F ("hist_8", "Energy Deposit in Second Steel", 100, -0.5, 0.35);
    TH1F* hist_edep_ply_3 = new TH1F ("hist_9", "Energy Deposit in Fourth Plywood", 100, -0.5, 0.35);
    TH1F* hist_edep_poly_1 = new TH1F ("hist_10", "Energy Deposit in Second Polyurethane", 100, -0.5, 0.35);
    TH1F* hist_edep_ply_4 = new TH1F ("hist_11", "Energy Deposit in Fifth Plywood", 100, -0.5, 0.35);
    TH1F* hist_edep_borpol_1 = new TH1F ("hist_12", "Energy Deposit in Second Borated Polyethilene", 100, -0.5, 0.35);
    TH1F* hist_edep_ply_5 = new TH1F ("hist_13", "Energy Deposit in Sixth Plywood", 100, -0.5, 0.35);
    TH1F* hist_edep_ste_2 = new TH1F ("hist_14", "Energy Deposit in Third Steel", 100, -0.5, 0.35);

    for (int i = 0; i < water_Tree->GetEntries(); i++) {
        water_Tree->GetEntry(i);
        if (edep_water > 0.01) {
            hist_edep_water->Fill(edep_water);
        }
        for (int k = 0; k < absEnergy_water->size(); k++) {
            hist_ene_water->Fill(absEnergy_water->at(k), TrackWeight_water->at(k));
        }
    }

    for (int i = 0; i < ste_Tree_0->GetEntries(); i++) {
        ste_Tree_0->GetEntry(i);
        if (edep_ste_0 > 0.01) {
            hist_edep_ste_0->Fill(edep_ste_0);
        }
    }
    
    for (int i = 0; i < ply_Tree_0->GetEntries(); i++) {
        ply_Tree_0->GetEntry(i);
        if (edep_ply_0 > 0.01) {
            hist_edep_ply_0->Fill(edep_ply_0);
        }
    }

    for (int i = 0; i < borpol_Tree_0->GetEntries(); i++) {
        borpol_Tree_0->GetEntry(i);
        if (edep_borpol_0 > 0.01) {
            hist_edep_borpol_0->Fill(edep_borpol_0);
        }
    }

    for (int i = 0; i < ply_Tree_1->GetEntries(); i++) {
        ply_Tree_1->GetEntry(i);
        if (edep_ply_1 > 0.01) {
            hist_edep_ply_1->Fill(edep_ply_1);
        }
    }

    for (int i = 0; i < poly_Tree_0->GetEntries(); i++) {
        poly_Tree_0->GetEntry(i);
        if (edep_poly_0 > 0.01) {
            hist_edep_poly_0->Fill(edep_poly_0);
        }
    }

    for (int i = 0; i < ply_Tree_2->GetEntries(); i++) {
        ply_Tree_2->GetEntry(i);
        if (edep_ply_2 > 0.01) {
            hist_edep_ply_2->Fill(edep_ply_2);
        }
    }

    for (int i = 0; i < ste_Tree_1->GetEntries(); i++) {
        ste_Tree_1->GetEntry(i);
        if (edep_ste_1 > 0.01) {
            hist_edep_ste1->Fill(edep_ste_1);
        }
    }

    for (int i = 0; i < ply_Tree_3->GetEntries(); i++) {
        ply_Tree_3->GetEntry(i);
        if (edep_ply_3 > 0.01) {
            hist_edep_ply_3->Fill(edep_ply_3);
        }
    }

    for (int i = 0; i < poly_Tree_1->GetEntries(); i++) {
        poly_Tree_1->GetEntry(i);
        if (edep_poly_1 > 0.01) {
            hist_edep_poly_1->Fill(edep_poly_1);
        }
    }

    for (int i = 0; i < ply_Tree_4->GetEntries(); i++) {
        ply_Tree_4->GetEntry(i);
        if (edep_ply_4 > 0.01) {
            hist_edep_ply_4->Fill(edep_ply_4);
        }
    }

    for (int i = 0; i < borpol_Tree_1->GetEntries(); i++) {
        borpol_Tree_1->GetEntry(i);
        if (edep_borpol_1 > 0.01) {
            hist_edep_borpol_1->Fill(edep_borpol_1);
        }
    }

    for (int i = 0; i < ply_Tree_5->GetEntries(); i++) {
        ply_Tree_5->GetEntry(i);
        if (edep_ply_5 > 0.01) {
            hist_edep_ply_5->Fill(edep_ply_5);
        }
    }

    for (int i = 0; i < ste_Tree_2->GetEntries(); i++) {
        ste_Tree_2->GetEntry(i);
        if (edep_ste_2 > 0.01) {
            hist_edep_ste_2->Fill(edep_ste_2);
        }
    }

    TFile* outputFile = new TFile("edep_" + filename, "RECREATE");
    hist_edep_water->Write();
    hist_edep_ste_0->Write();
    hist_edep_ply_0->Write();
    hist_edep_borpol_0->Write();
    hist_edep_ply_1->Write();
    hist_edep_poly_0->Write();
    hist_edep_ply_2->Write();
    hist_edep_ste1->Write();
    hist_edep_ply_3->Write();
    hist_edep_poly_1->Write();
    hist_edep_ply_4->Write();
    hist_edep_borpol_1->Write();
    hist_edep_ply_5->Write();
    hist_edep_ste_2->Write();
    hist_ene_water->Write();
    outputFile->Close();
    myFile->Close();
} 