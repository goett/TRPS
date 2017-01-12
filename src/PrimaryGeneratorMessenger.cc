#include "globals.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"

// Include all generators here
#include "TRPSPrimaryGenerator.hh"
#include "G4GeneralParticleSource.hh"
#include "DBDPrimaryGenerator.hh"

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* generator)
: fPrimaryGenerator(generator)
{
  genDir = new G4UIdirectory("/TRPS/generator/");
  genDir->SetGuidance("Primary generator selection");
  genDir->SetGuidance("/TRPS/generator/select: Select generator.");

  // /TRPS/generator/select command
  fSelectCmd = new G4UIcmdWithAString("/TRPS/generator/select", this);
  fSelectCmd->SetGuidance("Selects generator for events.");
  fSelectCmd->SetGuidance("Options are:");
  fSelectCmd->SetGuidance("DBD");
  fSelectCmd->SetGuidance("GPS: Geant 4 SPS Generator.");
  fSelectCmd->SetGuidance("Hurley");

  // For each new generator, make sure to add string as new candidate
  G4String candidates = "DBD GPS Hurley";
  fSelectCmd->SetCandidates(candidates);
  fSelectCmd->SetDefaultValue("DBD");

}

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete genDir;
  delete fSelectCmd;
}


void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if( command == fSelectCmd )
   {
      // For a new generator, add in command here
      if(newValue == "DBD"){
        fPrimaryGenerator->SetGenerator(new DBDPrimaryGenerator);
      }
      else if(newValue == "GPS")
      {
        fPrimaryGenerator->SetGenerator(new G4GeneralParticleSource);
      }
      else if(newValue == "Hurley")
      {
        fPrimaryGenerator->SetGenerator(new TRPSPrimaryGenerator); 
      }
   }

}

