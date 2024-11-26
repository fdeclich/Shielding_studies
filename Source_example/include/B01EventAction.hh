#ifndef B01EventAction_h
#define B01EventAction_h 1

#include <vector>
#include "G4UserEventAction.hh"
#include "globals.hh"

class B01EventAction:public G4UserEventAction {
    public:
    B01EventAction();
    virtual ~B01EventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    private:
    std::vector<G4String> fSDName;
};

#endif
