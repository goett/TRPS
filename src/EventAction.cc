#include "EventAction.hh"

#include "RunAction.hh"
#include "EventActionMessenger.hh"

#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"
#include <iomanip>
#include "TRPSTree.hh"
#include "EventR.hh"

EventAction::EventAction(RunAction* run)
:runAct(run),printModulo(1),eventMessenger(0)
{
  eventMessenger = new EventActionMessenger(this);
}

EventAction::~EventAction()
{
  delete eventMessenger;
}

void EventAction::BeginOfEventAction(const G4Event* evt)
{  
  G4int evtNb = evt->GetEventID();
  if (evtNb%printModulo == 0) { 
    G4cout << "\n---> Begin of event: " << evtNb << G4endl;
    //CLHEP::HepRandom::showEngineStatus();
  }
 
 // initialisation per event
 // ....nothing to do here
}

void EventAction::EndOfEventAction(const G4Event* evt)
{
  G4int evtNb = evt->GetEventID();
  TRPSTree* mytree = TRPSTree::Instance("tree","tree"); 
  EventR* eventR = EventR::Instance();

  //Record Primary Vertex Information
  //G4PrimaryVertex* pV = evt->GetPrimaryVertex();
  G4int numPrimaries = evt->GetNumberOfPrimaryVertex();
  for(int i=0; i<numPrimaries; i++){
	G4PrimaryVertex* pV = evt->GetPrimaryVertex(i);
	eventR->PushVX(pV->GetX0());
	eventR->PushVY(pV->GetY0());
	eventR->PushVZ(pV->GetZ0());
  }
  
  mytree->Fill();
  G4cout << "\n---> HC of event: " << evtNb << G4endl;
  G4cout << "\n---> Number of Primaries: " << numPrimaries << G4endl;
  //eventR->Print();
  G4cout << "\n---> End of event: " << evtNb << G4endl;

  eventR->clear();
}  
