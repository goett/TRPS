

#ifndef ThermalizationProcess_h
#define ThermalizationProcess_h 1

#include "G4VDiscreteProcess.hh"
#include "G4LatticePhysical.hh"
#include "G4LatticeManager.hh"

#include "G4PhononLong.hh"
#include "G4PhononPolarization.hh"
#include "G4PhononTrackMap.hh"
#include "G4PhononTransFast.hh"
#include "G4PhononTransSlow.hh"

#include "G4PhysicalConstants.hh"
#include "G4RandomDirection.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4VParticleChange.hh"
#include "G4ThreeVector.hh"
#include "Randomize.hh"
#include <cmath>

#include "TF1.h"
#include "EventR.hh"

class PhononExcitation : public G4VDiscreteProcess {
public:
  PhononExcitation(const G4String& processName ="phononExcitation");
  virtual ~PhononExcitation();

  virtual G4VParticleChange* PostStepDoIt(const G4Track&, const G4Step& aStep);
  
  virtual G4bool IsApplicable(const G4ParticleDefinition&);
  
protected:
  virtual G4double GetMeanFreePath(const G4Track&, G4double, G4ForceCondition*);

protected:
  //generate random polarization from density of states in momentum direction v
  virtual G4int ChoosePolarization(G4ThreeVector v);
  //introduce a new phonon with momentum position etc...
  virtual G4Track* CreateSecondary(G4int polarization, const G4ThreeVector &K, G4double energy) const;

protected:
  	G4PhononTrackMap* trackKmap;          // For convenient access by processes
	const G4LatticePhysical* theLattice;

private:
  G4double fBeta, fGamma, fLambda, fMu;	// Local buffers for calculations

  const G4Track* currentTrack;

  // hide assignment operator as private 
  PhononExcitation(PhononExcitation&);
  PhononExcitation& operator=(const PhononExcitation& right);
};

#endif
