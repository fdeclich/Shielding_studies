/**
 * @author      : Daniele Guffanti (daniele.guffanti@mib.infn.it)
 * @file        : B01Absorber
 * @created     : Wednesday Dec 04, 2024 16:11:27 CET
 */

#include "B01AbsorberSD.hh"
#include "G4SDManager.hh"
#include "G4Track.hh"
#include "G4TrackLogger.hh"
#include "G4Neutron.hh"

G4ThreadLocal G4Allocator<AbsorberHit>* AbsorberHitAllocator;

AbsorberHit::AbsorberHit() : G4VHit(), fEnergy(0), fWeight(0.0), fTrackID(0) {}

AbsorberHit::~AbsorberHit() {}

AbsorberHit::AbsorberHit(const AbsorberHit &right) : G4VHit()
{
  fEnergy = right.fEnergy;
  fWeight = right.fWeight;
  fTrackID = right.fTrackID;
}

const AbsorberHit &AbsorberHit::operator=(const AbsorberHit &right)
{
  fEnergy = right.fEnergy;
  fWeight = right.fWeight;
  fTrackID = right.fTrackID;
  return *this;
}

void AbsorberHit::Print()
{
  printf("Absorber[%i] trackID[%i] weight: %g entry energy: %g\n", 
      fAbsorberID, fTrackID, fWeight, fEnergy);
  return;
}

AbsorberSD::AbsorberSD(G4String name) 
  : G4VSensitiveDetector(name), fHCID(-1), fHitsCollection(nullptr)
{
  collectionName.insert(name+"HitsCollection");
}

AbsorberSD::~AbsorberSD() {}

void AbsorberSD::Initialize(G4HCofThisEvent *hce)
{
  fHitsCollection 
    = new AbsorberHitsCollection(SensitiveDetectorName,collectionName[0]);
  if (fHCID<0) { 
    fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection); 
  }

  hce->AddHitsCollection(fHCID,fHitsCollection);
}

G4bool AbsorberSD::ProcessHits(G4Step *aStep, G4TouchableHistory *)
{
  G4ParticleDefinition* particle = aStep->GetTrack()->GetDefinition();
  if (particle != G4Neutron::Neutron()) return false;

  const G4int index = GetIndex(aStep);
  G4TrackLogger& tlog = fCellTrackLogger[index];
  if (tlog.FirstEnterance(aStep->GetTrack()->GetTrackID())) {
    G4double w = aStep->GetPreStepPoint()->GetWeight();
    G4double energy = aStep->GetPreStepPoint()->GetKineticEnergy();

    AbsorberHit* newHit = new AbsorberHit();
    newHit->SetEntryEnergy(energy);
    newHit->SetWeight(w);
    newHit->SetTrackID(aStep->GetTrack()->GetTrackID());
    newHit->SetAbsorberID(index);

    newHit->Print();
    fHitsCollection->insert(newHit);
  }

  return true;
}
