#include "B01EventAction.hh"
#include "B01DetectorConstruction.hh"
#include "B01AbsorberSD.hh"
#include "B01RunAction.hh"
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

    B01RunAction* runAction = (B01RunAction*)(G4RunManager::GetRunManager()->GetUserRunAction());
    auto& track_records = runAction->GetAbsEnergyRecords();
    for (auto& record : track_records) {
      record.second.reset();
      record.second.init();
    }
    
}

void B01EventAction::EndOfEventAction(const G4Event* aEvent) {
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4HCofThisEvent* HCE = aEvent->GetHCofThisEvent();
  if (!HCE) return;

  const G4int Ncol = HCE->GetNumberOfCollections();
  for (G4int i = 0; i < Ncol; i++) {  // Loop over HitsCollection
    G4THitsMap<G4double>* EvtMap = 0;

    if ( i < Ncol -1 ) {
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
    else {
      AbsorberHitsCollection* hc = static_cast<AbsorberHitsCollection*>(HCE->GetHC(i));
      G4RunManager* runManager = G4RunManager::GetRunManager();
      B01RunAction* runAction = (B01RunAction*)(runManager->GetUserRunAction());
      auto& track_records = runAction->GetAbsEnergyRecords();

      for (G4int j = 0; j < hc->entries(); j++) {
        AbsorberHit* hit = (*hc)[j];
        G4int volume_copyNr = hit->GetAbsorberID();
        auto& record = track_records[volume_copyNr];
        record.fAbsEnergy.push_back(hit->GetEntryEnergy());
        record.fTrackWeight.push_back(hit->GetWeight());
        record.fTrackID.push_back(hit->GetTrackID());
      }

    }
  }
  return;
}

