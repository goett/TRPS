#include "RunMessenger.hh"
#include "RunAction.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"

RunMessenger::RunMessenger(RunAction* run):fRunAction(run)
{
  fNameCmd = new G4UIcmdWithAString("/TRPS/setFileName",this);  
  fNameCmd->SetGuidance("Set output file name.");
  fNameCmd->SetParameterName("FName",false);
  fNameCmd->AvailableForStates(G4State_PreInit, G4State_Idle);  

}

void RunMessenger::SetNewValue(G4UIcommand* command,
                                          G4String newValue)
{       
  if( command == fNameCmd )
   { fRunAction->SetOutputFileName(newValue); }
}

RunMessenger::~RunMessenger()
{
  delete fNameCmd;
}

