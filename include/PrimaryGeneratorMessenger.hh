#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h 1

class PrimaryGeneratorAction;
class DBDPrimaryGenerator;

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

#include "globals.hh"
#include "G4UImessenger.hh"

//class G4UIdirectory;
//class G4UIcmdWithABool;
//class G4UIcmdWithoutParameter;
//class G4UIcmdWithAString;
//class G4UIcmdWithADoubleAndUnit;

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
    G4UIcmdWithADoubleAndUnit* QvalueCmd;
    DBDPrimaryGenerator*       DBDgen;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
