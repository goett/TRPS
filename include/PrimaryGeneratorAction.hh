
#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4VPrimaryGenerator.hh"

class G4Event;
class PrimaryGeneratorMessenger;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();
   ~PrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event*);

    void SetGenerator(G4VPrimaryGenerator *gene) { fVPrimaryGenerator = gene; }

  private:

  	G4VPrimaryGenerator *fVPrimaryGenerator;
    PrimaryGeneratorMessenger *fG4Messenger;

};

#endif


