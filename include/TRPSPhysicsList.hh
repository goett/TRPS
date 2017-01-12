
#ifndef TRPSPhysicsList_h
#define TRPSPhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class TRPSPhysicsList : public G4VUserPhysicsList {
public:
  TRPSPhysicsList(G4int verbose=0);
  ~TRPSPhysicsList();
  
  // Construct particle and physics
  virtual void ConstructParticle();
  virtual void ConstructProcess();
  virtual void ConstructEM();
  virtual void ConstructThermalization();
  virtual void SetCuts();
};

#endif  /* TRPSPhysicsList_h */



