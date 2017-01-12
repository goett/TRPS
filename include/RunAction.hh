#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"


class G4Run;
class TFile;
class RunMessenger;

class RunAction : public G4UserRunAction
{
public:
  RunAction();
  virtual ~RunAction();

  void BeginOfRunAction(const G4Run*);
  void   EndOfRunAction(const G4Run*);
    
  void fillPerEvent(G4double, G4double); 
  void SetOutputFileName(G4String);

private:
  RunMessenger* runMessenger;
  TFile* outfile;
};

#endif

