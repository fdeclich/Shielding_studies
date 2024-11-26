#include "B01EventAction.hh"
#include "B01DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4THitsMap.hh"
#include "G4AnalysisManager.hh"

B01EventAction::B01EventAction() {

}

B01EventAction::~B01EventAction() {

}

B01EventAction::BeginOfEventAction(const G4Event*) {
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->AddNtupleRow();
}

B01EventAction::EndOfEventAction(const G4Event*) {
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    G4HCofThisEvent* HCE = aEvent->GetHCofThisEvent();
    if (!HCE) return;
}

