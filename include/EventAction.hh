#ifndef EventAction_h
#define EventAction_h 

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "RunAction.hh"
#include "EventActionMessenger.hh"
#include "EventR.hh"

//class RunAction;
//class EventActionMessenger;
//class EventR;

class EventAction : public G4UserEventAction
{
public:
  EventAction(RunAction*);
  virtual ~EventAction();

  void  BeginOfEventAction(const G4Event*);
  void    EndOfEventAction(const G4Event*);
    
  void SetPrintModulo(G4int val)  {printModulo = val;};
      
private:
   RunAction*  runAct;
   
   G4int     printModulo;
                             
   EventActionMessenger*  eventMessenger;
};

#endif

    
