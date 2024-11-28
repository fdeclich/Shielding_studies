#include "B01EventAction.hh"
#include "B01DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4THitsMap.hh"
#include "G4AnalysisManager.hh"

B01EventAction::B01EventAction() {

}

B01EventAction::~B01EventAction() {

}

void B01EventAction::BeginOfEventAction(const G4Event*) {
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    G4int ntuples = analysisManager->GetNofNtuples();
    for (int i =0; i<ntuples; i++) {
      analysisManager->AddNtupleRow(i);
    }
    
}

void B01EventAction::EndOfEventAction(const G4Event* aEvent) {
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4HCofThisEvent* HCE = aEvent->GetHCofThisEvent();
  if (!HCE) return;

  const G4int Ncol = HCE->GetNumberOfCollections();

  for (G4int i = 0; i < Ncol; i++) {  // Loop over HitsCollection
    G4THitsMap<G4double>* EvtMap = 0;
    EvtMap = (G4THitsMap<G4double>*)(HCE->GetHC(i));
    if (EvtMap == nullptr) {
      printf("B01EventAction::EndOfEventAction EvtMap Not Found %d\n", i);
      exit(EXIT_FAILURE); 
    }

    for (const auto& hit : *EvtMap) {
      const G4int& volume_copyNr = hit.first;
      const G4double* val = hit.second; 

      //const G4int column_idx = Ncol * (volume_copyNr - 1) + EvtMap->GetColID();
      const G4int column_idx = EvtMap->GetColID();
      analysisManager->FillNtupleDColumn(volume_copyNr, column_idx, *val);
    }
  }

  return;
}

