//Little macro to analyse all the trees of the output root file of the simulation
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TString.h"

#include <iostream>

void tree_analysis (TString filename) {
    //File and trees reading
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
    //Unweighted variables definitions
    double pop_water;
    double pop_ply_0;
    double pop_borpol_0;
    double pop_poly_0;
    double pop_ply_1;
    double pop_ply_2;
    double pop_ste_0;
    double pop_ply_3;
    double pop_poly_1;
    double pop_ply_4;
    double pop_borpol_1;
    double pop_ply_5;
    double pop_ste_1;
    double pop_ste_2;

    //Weighted variables definitions
    double popw_water;
    double popw_ply_0;
    double popw_borpol_0;
    double popw_poly_0;
    double popw_ply_1;
    double popw_ply_2;
    double popw_ste_0;
    double popw_ply_3;
    double popw_poly_1;
    double popw_ply_4;
    double popw_borpol_1;
    double popw_ply_5;
    double popw_ste_1;
    double popw_ste_2;

    // Unweighted branches setting
    water_Tree->SetBranchAddress("Population", &pop_water);
    ply_Tree_0->SetBranchAddress("Population", &pop_ply_0);
    borpol_Tree_0->SetBranchAddress("Population", &pop_borpol_0);
    poly_Tree_0->SetBranchAddress("Population", &pop_poly_0);
    ply_Tree_1->SetBranchAddress("Population", &pop_ply_1);
    ply_Tree_2->SetBranchAddress("Population", &pop_ply_2);
    ste_Tree_0->SetBranchAddress("Population", &pop_ste_0);
    ply_Tree_3->SetBranchAddress("Population", &pop_ply_3);
    poly_Tree_1->SetBranchAddress("Population", &pop_poly_1);
    ply_Tree_4->SetBranchAddress("Population", &pop_ply_4);
    borpol_Tree_1->SetBranchAddress("Population", &pop_borpol_1);
    ply_Tree_5->SetBranchAddress("Population", &pop_ply_5);
    ste_Tree_1->SetBranchAddress("Population", &pop_ste_1);
    ste_Tree_2->SetBranchAddress("Population", &pop_ste_2);

    //Weighted branches setting
    water_Tree->SetBranchAddress("Population_W", &popw_water);
    ply_Tree_0->SetBranchAddress("Population_W", &popw_ply_0);
    borpol_Tree_0->SetBranchAddress("Population_W", &popw_borpol_0);
    poly_Tree_0->SetBranchAddress("Population_W", &popw_poly_0);
    ply_Tree_1->SetBranchAddress("Population_W", &popw_ply_1);
    ply_Tree_2->SetBranchAddress("Population_W", &popw_ply_2);
    ste_Tree_0->SetBranchAddress("Population_W", &popw_ste_0);
    ply_Tree_3->SetBranchAddress("Population_W", &popw_ply_3);
    poly_Tree_1->SetBranchAddress("Population_W", &popw_poly_1);
    ply_Tree_4->SetBranchAddress("Population_W", &popw_ply_4);
    borpol_Tree_1->SetBranchAddress("Population_W", &popw_borpol_1);
    ply_Tree_5->SetBranchAddress("Population_W", &popw_ply_5);
    ste_Tree_1->SetBranchAddress("Population_W", &popw_ste_1);
    ste_Tree_2->SetBranchAddress("Population_W", &popw_ste_2);
    //Creating the canvas and all the histos
    TCanvas* c1 = new TCanvas();
    c1->Divide(3,2);
    TH1F* hist_pop_water = new TH1F ("pop_water", "Population in Water", 11, -0.5, 10.5);
    TH1F* hist_pop_ply_0 = new TH1F ("pop_ply0", "Population in First Plywood", 11, -0.5, 10.5);
    TH1F* hist_pop_borpol_0 = new TH1F ("pop_borpol0", "Population in First Borated Polyethilene", 11, -0.5, 10.5);
    TH1F* hist_pop_poly_0 = new TH1F ("pop_poly0", "Population in First Polyethilene", 11, -0.5, 10.5);
    TH1F* hist_pop_ply_1 = new TH1F ("pop_ply1", "Population in Second Plywood", 11, -0.5, 10.5);
    //Filling all the histos-----------------UNWEIGHTED--------------------------------------
    double var_water = 0;
    for (int i = 0; i < water_Tree->GetEntries(); i ++) {
        water_Tree->GetEntry(i);
        var_water += pop_water;
        if (pop_water >0 ) {
            hist_pop_water->Fill(pop_water);
        } 
    }
    double var_ply0 = 0;
    for (int i = 0; i < ply_Tree_0->GetEntries(); i ++) {
        ply_Tree_0->GetEntry(i);
        var_ply0 += pop_ply_0;
        if (pop_ply_0 >0 ) {
            hist_pop_ply_0->Fill(pop_ply_0);
        } 
    }
    double var_borpol0 = 0;
    for (int i = 0; i < borpol_Tree_0->GetEntries(); i ++) {
        borpol_Tree_0->GetEntry(i);
        var_borpol0 += pop_borpol_0;
        if (pop_borpol_0 >0 ) {
            hist_pop_borpol_0->Fill(pop_borpol_0);
        } 
    }
    double var_poly0 = 0;
    for (int i = 0; i < poly_Tree_0->GetEntries(); i ++) {
        poly_Tree_0->GetEntry(i);
        var_poly0 += pop_poly_0;
        if (pop_poly_0 >0 ) {
            hist_pop_poly_0->Fill(pop_poly_0);
        } 
    }
    double var_ply1 = 0;
    for (int i = 0; i < ply_Tree_1->GetEntries(); i ++) {
        ply_Tree_1->GetEntry(i);
        var_ply1 += pop_ply_1;
        if (pop_ply_1 >0 ) {
            hist_pop_ply_1->Fill(pop_ply_1);
        } 
    }
    double var_ply2 = 0;
    for (int i = 0; i < ply_Tree_2->GetEntries(); i++) {
        ply_Tree_2->GetEntry(i);
        var_ply2 += pop_ply_2;
    }
    double var_ste0 = 0;
    for (int i = 0; i < ste_Tree_0->GetEntries(); i++) {
        ste_Tree_0->GetEntry(i);
        var_ste0 += pop_ste_0;
    }
    double var_ply3 = 0;
    for (int i = 0; i < ply_Tree_3->GetEntries(); i++) {
        ply_Tree_3->GetEntry(i);
        var_ply3+= pop_ply_3;
    }
    double var_poly1 = 0;
    for (int i = 0; i < poly_Tree_1->GetEntries(); i++) {
        poly_Tree_1->GetEntry(i);
        var_poly1+= pop_poly_1;
    }
    double var_ply4 = 0;
    for (int i = 0; i < ply_Tree_4->GetEntries(); i++) {
        ply_Tree_4->GetEntry(i);
        var_ply4+= pop_ply_4;
    }
    double var_borpol1 = 0;
    for (int i = 0; i < borpol_Tree_1->GetEntries(); i++) {
        borpol_Tree_1->GetEntry(i);
        var_borpol1+= pop_borpol_1;
    }
    double var_ply5 = 0;
    for (int i = 0; i < ply_Tree_5->GetEntries(); i++) {
        ply_Tree_5->GetEntry(i);
        var_ply5+= pop_ply_5;
    }
    double var_ste1 = 0;
    for (int i = 0; i < ste_Tree_1->GetEntries(); i++) {
        ste_Tree_1->GetEntry(i);
        var_ste1 += pop_ste_1;
    }
    double var_ste2 = 0;
    for (int i = 0; i < ste_Tree_2->GetEntries(); i++) {
        ste_Tree_2->GetEntry(i);
        var_ste2 += pop_ste_2;
    }
    std::cout << "--------------Printing all the unweighted variables------------------------" << std::endl;
    std::cout << "#1 Water variable: " << var_water << std::endl;
    std::cout << "#2 First Steel variable: " << var_ste0 << std::endl;
    std::cout << "#3 First Plywood variable: " << var_ply0 << std::endl;
    std::cout << "#4 First Borated Polyethilene variable: " << var_borpol0 << std::endl;
    std::cout << "#5 Second Plywood variable: " << var_ply1 << std::endl;
    std::cout << "#6 First Polyurethane variable: " << var_poly0 << std::endl;
    std::cout << "#7 Third Plywood variable: " << var_ply2 << std::endl;
    std::cout << "#8 Second Steel variable: " << var_ste1 << std::endl;
    std::cout << "#9 Fourth Plywood variable: " << var_ply3 << std::endl;
    std::cout << "#10 Second Polyurethane variable: " << var_poly1 << std::endl;
    std::cout << "#11 Fifth Plywood variable: " << var_ply4 << std::endl;
    std::cout << "#12 Second Borated Polyethilene variable: " << var_borpol1 << std::endl;
    std::cout << "#13 Sixth Plywood variable: " << var_ply5 << std::endl;
    std::cout << "#14 Third Steel variable: " << var_ste2 << std::endl;

    //Doing the same with the weighted variables
    double varw_water = 0;
    for (int i = 0; i < water_Tree->GetEntries(); i++) {
        water_Tree->GetEntry(i);
        varw_water += popw_water;
    }

    double varw_ste0 = 0;
    for (int i = 0; i < ste_Tree_0->GetEntries(); i++) {
        ste_Tree_0->GetEntry(i);
        varw_ste0 += popw_ste_0;
    }

    double varw_ply0 = 0;
    for (int i = 0; i < ply_Tree_0->GetEntries(); i++) {
        ply_Tree_0->GetEntry(i);
        varw_ply0 += popw_ply_0;
    }

    double varw_borpol0 = 0;
    for (int i = 0; i < borpol_Tree_0->GetEntries(); i++) {
        borpol_Tree_0->GetEntry(i);
        varw_borpol0 += popw_borpol_0;
    }

    double varw_ply1 = 0;
    for (int i = 0; i < ply_Tree_1->GetEntries(); i++) {
        ply_Tree_1->GetEntry(i);
        varw_ply1 += popw_ply_1;
    }

    double varw_poly0 = 0;
    for (int i = 0; i < poly_Tree_0->GetEntries(); i++) {
        poly_Tree_0->GetEntry(i);
        varw_poly0 += popw_poly_0;
    }
    
    double varw_ply2 = 0;
    for (int i = 0; i < ply_Tree_2->GetEntries(); i++) {
        ply_Tree_2->GetEntry(i);
        varw_ply2 += popw_ply_2;
    }

    double varw_ste1 = 0;
    for (int i = 0; i < ste_Tree_1->GetEntries(); i++) {
        ste_Tree_1->GetEntry(i);
        varw_ste1 += popw_ste_1;
    }

    double varw_ply3 = 0;
    for (int i = 0; i < ply_Tree_3->GetEntries(); i++) {
        ply_Tree_3->GetEntry(i);
        varw_ply3 += popw_ply_3;
    }

    double varw_poly1 = 0;
    for (int i = 0; i < poly_Tree_1->GetEntries(); i++) {
        poly_Tree_1->GetEntry(i);
        varw_poly1 += popw_poly_1;
    }

    double varw_ply4 = 0;
    for (int i = 0; i < ply_Tree_4->GetEntries(); i++) {
        ply_Tree_4->GetEntry(i);
        varw_ply4 += popw_ply_4;
    }

    double varw_borpol1 = 0;
    for (int i = 0; i < borpol_Tree_1->GetEntries(); i++) {
        borpol_Tree_1->GetEntry(i);
        varw_borpol1 += popw_borpol_1;
    }

    double varw_ply5 = 0;
    for (int i = 0; i < ply_Tree_5->GetEntries(); i++) {
        ply_Tree_5->GetEntry(i);
        varw_ply5 += popw_ply_5;
    }

    double varw_ste2 = 0;
    for (int i = 0; i < ste_Tree_2->GetEntries(); i++) {
        ste_Tree_2->GetEntry(i);
        varw_ste2 += popw_ste_2;
    }

    std::cout << "--------------Printing all the weighted variables------------------------" << std::endl;
    std::cout << "#1 Water weighted variable: " << varw_water << std::endl;
    std::cout << "#2 First Steel weighted variable: " << varw_ste0 << std::endl;
    std::cout << "#3 First Plywood weighted variable: " << varw_ply0 << std::endl;
    std::cout << "#4 First Borated Polyethilene weighted variable: " << varw_borpol0 << std::endl;
    std::cout << "#5 Second Plywood weighted variable: " << varw_ply1 << std::endl;
    std::cout << "#6 First Polyurethane weighted variable: " << varw_poly0 << std::endl;
    std::cout << "#7 Third Plywood weighted variable: " << varw_ply2 << std::endl;
    std::cout << "#8 Second Steel weighted variable: " << varw_ste1 << std::endl;
    std::cout << "#9 Fourth Plywood weighted variable: " << varw_ply3 << std::endl;
    std::cout << "#10 Second Polyurethane weighted variable: " << varw_poly1 << std::endl;
    std::cout << "#11 Fifth Plywood weighted variable: " << varw_ply4 << std::endl;
    std::cout << "#12 Second Borated Polyethilene weighted variable: " << varw_borpol1 << std::endl;
    std::cout << "#13 Sixth Plywood weighted variable: " << varw_ply5 << std::endl;
    std::cout << "#14 Third Steel weighted variable: " << varw_ste2 << std::endl;
    
    //Drawing all of this
    c1->cd(1);
    hist_pop_water->GetXaxis()->SetTitle("Population");
    hist_pop_water->GetYaxis()->SetTitle("Number of entries");
    hist_pop_water->SetFillColor(kAzure);
    hist_pop_water->Draw();
    c1->cd(2);
    hist_pop_ply_0->GetXaxis()->SetTitle("Population");
    hist_pop_ply_0->GetYaxis()->SetTitle("Number of entries");
    hist_pop_ply_0->SetFillColor(kSpring-1);
    hist_pop_ply_0->Draw();
    c1->cd(3);
    hist_pop_borpol_0->GetXaxis()->SetTitle("Population");
    hist_pop_borpol_0->GetYaxis()->SetTitle("Number of entries");
    hist_pop_borpol_0->SetFillColor(kRed);
    hist_pop_borpol_0->Draw();
    c1->cd(4);
    hist_pop_poly_0->GetXaxis()->SetTitle("Population");
    hist_pop_poly_0->GetYaxis()->SetTitle("Number of entries");
    hist_pop_poly_0->SetFillColor(kYellow-7);
    hist_pop_poly_0->Draw();
    c1->cd(5);
    hist_pop_ply_1->GetXaxis()->SetTitle("Population");
    hist_pop_ply_1->GetYaxis()->SetTitle("Number of entries");
    hist_pop_ply_1->SetFillColor(kSpring-2);
    hist_pop_ply_1->Draw();
    
    c1->Print("Tree_analysis.png", ".png");


}