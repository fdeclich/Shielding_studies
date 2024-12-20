//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file biasing/B01/exampleB01.cc
/// \brief Main program of the biasing/B01 example
//
//
//
//
// --------------------------------------------------------------
//      GEANT 4 - exampleB01
//
// --------------------------------------------------------------
// Comments
//
// This example intends to show how to use importance sampling and scoring
// in the mass (tracking) geometry.
// A simple geometry consisting of a 180 cm high concrete cylinder
// divided into 18 slabs of 10cm each is created.
// Importance values are assigned to the 18 concrete slabs in the
// detector construction class for simplicity.
// Pairs of G4GeometryCell and importance values are stored in
// the importance store.
// Scoring is carried out by the multifunctional detector (MFD) and
// sensitive detectors
//
// Alex Howard (alexander.howard@cern.ch):
// 22/11/13: Migrated to the new MT compliant design which moves the
//           biasing process to the physicslist constructor - here
//           via the modular physicslists
//

// --------------------------------------------------------------
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4GeometryManager.hh"
#include "G4RunManagerFactory.hh"
#include "G4Types.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4VPhysicalVolume.hh"
#include "G4UIExecutive.hh"

#include <iostream>
#include <stdlib.h>

// user classes
#include "B01ActionInitialization.hh"
#include "B01DetectorConstruction.hh"
#include "FTFP_BERT.hh"

#include "G4ImportanceBiasing.hh"
#include "G4WeightWindowBiasing.hh"
// #include "B01PrimaryGeneratorAction.hh"
// #include "B01RunAction.hh"

// Files specific for biasing and scoring
#include "G4GeometrySampler.hh"
#include "G4IStore.hh"
#include "G4VWeightWindowStore.hh"
#include "G4WeightWindowAlgorithm.hh"

#include <random>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv)
{ 
  if (argc < 2) {
    std::cerr << "First argument not found!";
    return EXIT_FAILURE;
  }
  G4String particlename = argv[1];
  G4String macroname = (argc > 2) ? argv[2] : "";
  G4int mode = 0;
  if (argc > 2) mode = atoi(argv[2]);

  G4int numberOfEvents = 100;
  G4long myseed = static_cast<G4long>(std::time(nullptr));

  /*G4UIExecutive* ui = nullptr;
  if (argc == 1) {
    ui = new G4UIExecutive(argc, argv);
  }*/

  auto* runManager = G4RunManagerFactory::CreateRunManager();
  runManager->SetNumberOfThreads(2);

  G4Random::setTheSeed(myseed);

  G4VWeightWindowAlgorithm* wwAlg = 0;  // pointer for WeightWindow (mode>0)

  // create the detector      ---------------------------
  B01DetectorConstruction* detector = new B01DetectorConstruction();
  detector->SetParticleName(particlename);
  runManager->SetUserInitialization(detector);
  G4GeometrySampler mgs(detector->GetWorldVolume(), particlename);

  G4VModularPhysicsList* physicsList = new FTFP_BERT;
  if (mode == 0) {
    physicsList->RegisterPhysics(new G4ImportanceBiasing(&mgs));
  }
  else {
    wwAlg = new G4WeightWindowAlgorithm(1,  // upper limit factor
                                        1,  // survival factor
                                        100);  // max. number of splitting

    physicsList->RegisterPhysics(new G4WeightWindowBiasing(&mgs, wwAlg, onBoundary));
    // place of action
  }
  runManager->SetUserInitialization(physicsList);

  // Set user action classes through Worker Initialization
  //
  B01ActionInitialization* actions = new B01ActionInitialization;
  runManager->SetUserInitialization(actions);

  //runManager->Initialize();
  
  //if (mode == 0) {
    //detector->CreateImportanceStore();
  //}
  //else {
    //detector->CreateWeightWindowStore();
  //}
  
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  G4UIExecutive* ui = nullptr;
  if (macroname.empty()) {
    // interactive mode
    ui = new G4UIExecutive(argc, argv);
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }
  else {
    // batch mode
    std::ifstream macroFile(macroname);
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command + macroname);
  }

  // open geometry for clean biasing stores clean-up
  //
  G4GeometryManager::GetInstance()->OpenGeometry();

  if (wwAlg) {
    delete wwAlg;
  }

  // mgs.ClearSampling();

  delete runManager;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
