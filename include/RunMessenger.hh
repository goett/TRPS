#ifndef RunMessenger_h
#define RunMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class RunAction;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;

class RunMessenger: public G4UImessenger
{
public:
  
  RunMessenger(RunAction* );
  virtual ~RunMessenger();
    
  virtual void SetNewValue(G4UIcommand*, G4String);
    
private:
  
  RunAction* fRunAction;
  G4UIcmdWithAString*        fNameCmd;
    
};

#endif

