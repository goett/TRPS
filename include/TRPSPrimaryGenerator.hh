#ifndef TRPSPrimaryGenerator_h
#define TRPSPrimaryGenerator_h 1

#include "G4VPrimaryGenerator.hh"
#include "globals.hh"

#include "G4PhononLong.hh"
#include "G4PhononPolarization.hh"
#include "G4PhononTransFast.hh"
#include "G4PhononTransSlow.hh"

class G4ParticleGun;
class G4Event;

class TRPSPrimaryGenerator : public G4VPrimaryGenerator
{
public:
  TRPSPrimaryGenerator();    
  virtual ~TRPSPrimaryGenerator();

  public:
    virtual void GeneratePrimaryVertex(G4Event*);

protected:
  //generate random polarization from density of states
  virtual G4int ChoosePolarization(G4double Ldos, G4double STdos, G4double FTdos) const;

  private:
    G4ParticleGun*                fParticleGun;

};


#endif


