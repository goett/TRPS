#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class PrimaryGeneratorAction;

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    PrimaryGeneratorMessenger(PrimaryGeneratorAction*);
   ~PrimaryGeneratorMessenger();

    void SetNewValue(G4UIcommand*, G4String);

  private:
    PrimaryGeneratorAction*    fPrimaryGenerator;
    G4UIdirectory*             genDir;
    G4UIcmdWithAString*        fSelectCmd;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif