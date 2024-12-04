/**
 * @author      : Daniele Guffanti (daniele.guffanti@mib.infn.it)
 * @file        : B01AbsorberSD
 * @created     : Wednesday Dec 04, 2024 15:09:42 CET
 */

#ifndef B01ABSORBERSD_HH

#define B01ABSORBERSD_HH

#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Step.hh"
#include "G4TrackLogger.hh"
#include "G4Allocator.hh"


class AbsorberHit : public G4VHit
{
  public:
    AbsorberHit();
    virtual ~AbsorberHit();
    AbsorberHit(const AbsorberHit& right);
    const AbsorberHit& operator=(const AbsorberHit& right);
    inline bool operator==(const AbsorberHit& right) const {
      bool ene = (fEnergy == right.fEnergy);
      bool wei = (fWeight == right.fWeight);
      bool trackID = (fTrackID == right.fTrackID);
      bool layerID = (fAbsorberID == right.fAbsorberID);
      return (ene && wei && trackID && layerID);
    }
    
    inline void *operator new(unsigned long);  
    inline void operator delete(void* aHit); 
    void Print();
    inline void SetEntryEnergy(G4double ene) {fEnergy = ene;}
    inline G4double GetEntryEnergy() const {return fEnergy;}
    inline void SetWeight(G4double ww) {fWeight = ww;}
    inline G4double GetWeight() const {return fWeight;}
    inline void SetTrackID(G4int trackID) {fTrackID = trackID;}
    inline G4int GetTrackID() const {return fTrackID;}
    inline void SetAbsorberID(G4int absID) {fAbsorberID = absID;}
    inline G4int GetAbsorberID() const {return fAbsorberID;}

  private:
    G4double fEnergy;
    G4double fWeight;
    G4int fTrackID;
    G4int fAbsorberID;

};

typedef G4THitsCollection<AbsorberHit> AbsorberHitsCollection;
extern G4ThreadLocal G4Allocator<AbsorberHit>* AbsorberHitAllocator;

inline void* AbsorberHit::operator new(unsigned long) {
  if (!AbsorberHitAllocator)
    AbsorberHitAllocator = new G4Allocator<AbsorberHit>;
  return (void*)AbsorberHitAllocator->MallocSingle();
}

inline void AbsorberHit::operator delete(void* aHit) {
  AbsorberHitAllocator->FreeSingle((AbsorberHit*)aHit);
}

class AbsorberSD : public G4VSensitiveDetector
{
  public:
    AbsorberSD(G4String name);
    virtual ~AbsorberSD();
    virtual void Initialize(G4HCofThisEvent* HCE);
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);

  private:
    G4int fHCID;
    AbsorberHitsCollection* fHitsCollection;
    std::map<G4int, G4TrackLogger> fCellTrackLogger;

    inline G4int GetIndex(const G4Step* aStep) {
      G4StepPoint* preStep = aStep->GetPreStepPoint();
      G4TouchableHistory* th = (G4TouchableHistory*)(preStep->GetTouchable());
      return th->GetReplicaNumber(0);
    }
};



#endif /* end of include guard B01ABSORBERSD_HH */

