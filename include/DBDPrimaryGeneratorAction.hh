#ifndef DBDPrimaryGeneratorAction_h
#define DBDPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"


class G4ParticleGun;
class G4Event;

class DBDPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  DBDPrimaryGeneratorAction();    
  virtual ~DBDPrimaryGeneratorAction();

  public:
    virtual void GeneratePrimaries(G4Event*);

  private:
    G4ParticleGun*                fParticleGun;

};


#endif


