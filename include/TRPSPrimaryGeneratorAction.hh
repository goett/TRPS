#ifndef TRPSPrimaryGeneratorAction_h
#define TRPSPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

#include "G4PhononLong.hh"
#include "G4PhononPolarization.hh"
#include "G4PhononTransFast.hh"
#include "G4PhononTransSlow.hh"

class G4ParticleGun;
class G4Event;

class TRPSPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  TRPSPrimaryGeneratorAction();    
  virtual ~TRPSPrimaryGeneratorAction();

  public:
    virtual void GeneratePrimaries(G4Event*);

protected:
  //generate random polarization from density of states
  virtual G4int ChoosePolarization(G4double Ldos, G4double STdos, G4double FTdos) const;

  private:
    G4ParticleGun*                fParticleGun;

};


#endif


