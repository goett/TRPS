#include "SteppingAction.hh"
#include "EventR.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"

#include "G4OpBoundaryProcess.hh"
#include "G4SDManager.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

SteppingAction::SteppingAction()
: G4UserSteppingAction()
{ 
  fDepVertexCounter = 0;
}

SteppingAction::~SteppingAction()
{ ; }

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  G4int eventNumber = G4RunManager::GetRunManager()->
                                              GetCurrentEvent()->GetEventID();

  G4Track* track = step->GetTrack();

  G4double Edep = step->GetTotalEnergyDeposit();

  if ( Edep>0 ){

	//G4cout << "This step has deposited energy." << G4endl;

	fDepVertexCounter++;

        G4StepPoint* psp = step->GetPostStepPoint();
        G4ThreeVector pos = psp->GetPosition();

	EventR* eventR = EventR::Instance();
	eventR->PushDepX(pos.x());	
	eventR->PushDepY(pos.y());	
	eventR->PushDepZ(pos.z());	
	eventR->PushDepT(psp->GetGlobalTime());	
	eventR->PushDepE(Edep);	

        

  }

}

