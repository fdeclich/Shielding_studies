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
/// \file biasing/B01/src/B01RunAction.cc
/// \brief Implementation of the B01RunAction class
//
//
//
#include "B01RunAction.hh"

#include "B01Run.hh"

//-- In order to obtain detector information.
#include "B01DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4THitsMap.hh"
#include "G4UnitsTable.hh"
#include "G4AnalysisManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// B01RunAction
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Constructor
B01RunAction::B01RunAction()
  : G4UserRunAction(),
    //  fFieldName(15),
    fFieldValue(14)
{
  // - Prepare data member for B01Run.
  //   vector represents a list of MultiFunctionalDetector names.
  fSDName.push_back(G4String("ConcreteSD"));

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFileName("B01_output.root");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Destructor.
B01RunAction::~B01RunAction()
{
  fSDName.clear();
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if (analysisManager->IsOpenFile()) {
    analysisManager->Write();
    analysisManager->CloseFile();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* B01RunAction::GenerateRun()
{
  // Generate new RUN object, which is specially
  // dedicated for MultiFunctionalDetector scheme.
  //  Detail description can be found in B01Run.hh/cc.
  return new B01Run(fSDName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B01RunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile();

  G4RunManager* runmngr = G4RunManager::GetRunManager();
  auto Detector = (B01DetectorConstruction*)runmngr->GetUserDetectorConstruction();
  std::vector<G4VPhysicalVolume*>& pv_vec = Detector->GetPhysicalVolumeVector();

  auto sdman = G4SDManager::GetSDMpointer();
  auto mfd = (G4MultiFunctionalDetector*)sdman->FindSensitiveDetector("ConcreteSD");
  const int n_scorers = mfd->GetNumberOfPrimitives(); 

  //Creating ntuple to record the energy spectrum or other infos
  analysisManager->CreateNtuple("Primary", "Primary");
  analysisManager->CreateNtupleDColumn("Energy");
  analysisManager->FinishNtuple();

  //Create ntuples for every volume
  for (int i = 0; i < pv_vec.size(); i++) {
    G4String vol_name = pv_vec[i]->GetName();
    analysisManager->CreateNtuple("Shielding_" + vol_name, "Shielding_" + vol_name);
    for (int j = 0; j < n_scorers; j++) {
      G4String scorer_name = mfd->GetPrimitive(j)->GetName();
      //G4String full_name = vol_name + "_" + scorer_name;
      analysisManager->CreateNtupleDColumn(scorer_name);
    }

    auto& record = fAbsEnergyRecords[i+1];
    analysisManager->CreateNtupleDColumn("AbsEnergy", record.fAbsEnergy);
    analysisManager->CreateNtupleDColumn("TrackWeight", record.fTrackWeight);
    analysisManager->CreateNtupleIColumn("TrackID", record.fTrackID);
    analysisManager->FinishNtuple();
  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B01RunAction::EndOfRunAction(const G4Run* aRun)
{
  G4cout << " ###### EndOfRunAction  " << G4endl;
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  //
  //- B01Run object.
  B01Run* b01Run = (B01Run*)aRun;
  //--- Dump all socred quantities involved in B01Run.
  // re02Run->DumpAllScorer();
  //---
  G4RunManager* mgr = G4RunManager::GetRunManager();
  //
  B01DetectorConstruction* detector = (B01DetectorConstruction*)mgr->GetUserDetectorConstruction();
  const std::vector<G4VPhysicalVolume*>& pv_vec = detector->GetPhysicalVolumeVector();

  for (G4int i = 0; i < (G4int)fSDName.size(); i++) {
    //---------------------------------------------
    // Dump accumulated quantities for this RUN.
    //  (Display only central region of x-y plane)
    //      0       ConcreteSD/Collisions
    //      1       ConcreteSD/CollWeight
    //      2       ConcreteSD/Population
    //      3       ConcreteSD/TrackEnter
    //      4       ConcreteSD/SL
    //      5       ConcreteSD/SLW
    //      6       ConcreteSD/SLWE
    //      7       ConcreteSD/SLW_V
    //      8       ConcreteSD/SLWE_V
    //---------------------------------------------
    G4THitsMap<G4double>* Collisions = b01Run->GetHitsMap(fSDName[i] + "/Collisions");
    G4THitsMap<G4double>* CollWeight = b01Run->GetHitsMap(fSDName[i] + "/CollWeight");
    G4THitsMap<G4double>* Population = b01Run->GetHitsMap(fSDName[i] + "/Population");
    G4THitsMap<G4double>* PopWeight = b01Run->GetHitsMap(fSDName[i] + "/Population_W");
    G4THitsMap<G4double>* TrackEnter = b01Run->GetHitsMap(fSDName[i] + "/TrackEnter");
    G4THitsMap<G4double>* Termination = b01Run->GetHitsMap(fSDName[i] + "/Termination_W");
    G4THitsMap<G4double>* Energy_Deposit = b01Run->GetHitsMap(fSDName[i] + "/Energy_Deposit");
 
    if (IsMaster()) {
      G4cout << "\n--------------------End of Global Run-----------------------" << G4endl;
      G4cout << " Number of event processed : " << aRun->GetNumberOfEvent() << G4endl;
    }
    else {
      G4cout << "\n--------------------End of Local Run------------------------" << G4endl;
      G4cout << " Number of event processed : " << aRun->GetNumberOfEvent() << G4endl;
    }

    G4cout << "=============================================================" << G4endl;
    G4cout << "=============================================================" << G4endl;

    std::ostream* myout = &G4cout;
    PrintHeader(myout);

    for (size_t iz = 0; iz < pv_vec.size(); iz++) {
      G4double* SumCollisions = (*Collisions)[iz+1];
      G4double* SumCollWeight = (*CollWeight)[iz+1];
      G4double* Populations = (*Population)[iz+1];
      G4double* PopulationsWeighted = (*PopWeight)[iz+1];
      G4double* TrackEnters = (*TrackEnter)[iz+1];
      G4double* Terminations = (*Termination)[iz+1];
      G4double* En_Deposits = (*Energy_Deposit)[iz+1];  
      if (!SumCollisions) SumCollisions = new G4double(0.0);
      if (!SumCollWeight) SumCollWeight = new G4double(0.0);
      if (!Populations) Populations = new G4double(0.0);
      if (!TrackEnters) TrackEnters = new G4double(0.0);
      if (!Terminations) Terminations = new G4double(0.0);
      if (!En_Deposits) En_Deposits = new G4double(0.0);
      G4String cname = detector->GetCellName(iz);
      G4cout << std::setw(fFieldValue) << cname 
        << " |" << std::setw(fFieldValue) << (*SumCollisions)
        << " |" << std::setw(fFieldValue) << (*SumCollWeight) 
        << " |" << std::setw(fFieldValue) << (*Populations) 
        << " |" << std::setw(fFieldValue) << (*PopulationsWeighted)
        << " |" << std::setw(fFieldValue) << (*TrackEnters)
        << " |" << std::setw(fFieldValue) << (*Terminations) 
        << " |" << std::setw(fFieldValue) << (*En_Deposits)
        << " |" << G4endl; 
    }
    G4cout << "=============================================" << G4endl;
  }

  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B01RunAction::PrintHeader(std::ostream* out)
{
  std::vector<G4String> vecScoreName;
  vecScoreName.push_back("Collisions");
  vecScoreName.push_back("Coll*WGT");
  vecScoreName.push_back("Population");
  vecScoreName.push_back("PopulationsWeighted");
  vecScoreName.push_back("Tr.Entering");
  vecScoreName.push_back("Terminations");
  vecScoreName.push_back("En_Deposits");

  // head line
  //   std::string vname;
  //   vname = FillString("Volume", ' ', fFieldName+1);
  //*out << vname << '|';
  *out << std::setw(fFieldValue) << "Volume"
       << " |";
  for (std::vector<G4String>::iterator it = vecScoreName.begin(); it != vecScoreName.end(); it++) {
    //      vname = FillString((*it),
    //                         ' ',
    //                         fFieldValue+1,
    //                         false);
    //      *out << vname << '|';
    *out << std::setw(fFieldValue) << (*it) << " |";
  }
  *out << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::string B01RunAction::FillString(const std::string& name, char c, G4int n, G4bool back)
{
  std::string fname("");
  G4int k = n - name.size();
  if (k > 0) {
    if (back) {
      fname = name;
      fname += std::string(k, c);
    }
    else {
      fname = std::string(k, c);
      fname += name;
    }
  }
  else {
    fname = name;
  }
  return fname;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
