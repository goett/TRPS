#include "RunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "TRPSTree.hh"
#include "TFile.h"
#include "RunMessenger.hh"
#include <string>

RunAction::RunAction()
{

  runMessenger = new RunMessenger(this);
}

RunAction::~RunAction()
{
delete runMessenger;
}

void RunAction::BeginOfRunAction(const G4Run* aRun)
{ 
	G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

	//inform the runManager to save random number seed
	G4RunManager::GetRunManager()->SetRandomNumberStore(true);

	//initialize output
	TRPSTree* mytree = TRPSTree::Instance("tree","tree");

	// outfile = new TFile("prova.root","RECREATE");
	runMessenger = new RunMessenger(this);
	// set random seeds for each run
	G4RunManager::GetRunManager()->SetRandomNumberStore(true);
	G4RunManager::GetRunManager()->SetRandomNumberStoreDir("random/");

	G4cout << "*******************" << G4endl;
	G4cout << "*** AUTOSEED ON ***" << G4endl;
	G4cout << "*******************" << G4endl;

	long seeds[2];
	time_t systime = time(NULL);
	seeds[0] = (long) systime;
	seeds[1] = (long) (systime*G4UniformRand());
	CLHEP::HepRandom::setTheSeeds(seeds);
	CLHEP::HepRandom::showEngineStatus();
	
}

void RunAction::SetOutputFileName(G4String s){
	//if(outfile){
	//	outfile->Close();
	//	}
	std::string name = s+".root";
	outfile = new TFile(name.c_str(),"RECREATE");
	G4cout<<"file name set to "<<name.c_str()<<G4endl;

}

void RunAction::EndOfRunAction(const G4Run* aRun)
{
	G4int NbOfEvents = aRun->GetNumberOfEvent();
	if (NbOfEvents == 0) return;

	G4cout<<"Writing tree..."<<G4endl;
	outfile->cd();
        TRPSTree* mytree = TRPSTree::Instance("tree","tree");
	mytree->Write();
	G4cout<<"tree written "<<G4endl;
	outfile->Close();
}
