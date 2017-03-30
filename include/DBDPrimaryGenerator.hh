#ifndef DBDPrimaryGenerator_h
#define DBDPrimaryGenerator_h 1

#include "G4VPrimaryGenerator.hh"
#include "globals.hh"


class G4ParticleGun;
class G4Event;

class DBDPrimaryGenerator : public G4VPrimaryGenerator
{
public:
  DBDPrimaryGenerator();    
  virtual ~DBDPrimaryGenerator();

  public:
    virtual void GeneratePrimaryVertex(G4Event*);
    virtual void SetQValue(double);

  private:
    G4ParticleGun*                fParticleGun;
    double                	  fQvalue;

};


#endif


