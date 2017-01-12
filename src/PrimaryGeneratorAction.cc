#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "G4Event.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fVPrimaryGenerator(0)
{
   fG4Messenger = new PrimaryGeneratorMessenger(this);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fVPrimaryGenerator;
  delete fG4Messenger;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  fVPrimaryGenerator->GeneratePrimaryVertex(anEvent);
}
