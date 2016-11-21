#include "TRPSPrimaryGenerator.hh"

#include "Randomize.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Navigator.hh"
#include "G4SystemOfUnits.hh"
#include "G4RandomDirection.hh"
#include "Randomize.hh"

#include "TF1.h"
#include "G4LatticeManager.hh"
#include "G4LatticePhysical.hh"
#include "G4TransportationManager.hh"

TRPSPrimaryGenerator::TRPSPrimaryGenerator()
: G4VPrimaryGenerator()
{
  fParticleGun = new G4ParticleGun();
  
  //default kinematics
  // handled in GeneratePrimaries
}

TRPSPrimaryGenerator::~TRPSPrimaryGenerator()
{
  delete fParticleGun;
}

void TRPSPrimaryGenerator::GeneratePrimaryVertex(G4Event* anEvent)
{

  //fetch physical lattice for crystal under study
  G4LatticeManager* LM= G4LatticeManager::GetLatticeManager();
  G4Navigator* theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  G4LatticePhysical* theLattice= LM->GetLattice(theNavigator->LocateGlobalPointAndSetup(G4ThreeVector(0.,0.,0.)));

  //Obtain dynamical constants from this volume's lattice
  G4double fBeta=theLattice->GetBeta();
  G4double fGamma=theLattice->GetGamma();
  G4double fLambda=theLattice->GetLambda();
  G4double fMu=theLattice->GetMu();
    
  //Produce vector of energies and polarizations describing phonon spectrum at 20 K
  //We want to simulate a total of 10 GeV
  G4double ETotal = 10*MeV;
  TF1* planck120 = new TF1("p120","pow(x,3.0)/( exp(x/(8.61E-5*20.0))-1 )",0.1*2.8*8.61E-5*20.0,10*2.8*8.61E-5*20.0);
  G4double Esum = 2.5e-5*eV;
  G4double phoE = 0.0;
  G4int pol = 0;
  std::vector<G4double> phoEs = {2.5e-5*eV};
  std::vector<G4int> pPols = {1};
  G4ParticleDefinition* thePhonon;
  do{
	//draw random energy and polarization from distributions
        phoE = planck120->GetRandom();
        pol = ChoosePolarization(theLattice->GetLDOS(),theLattice->GetSTDOS(),theLattice->GetFTDOS());
        Esum += phoE;
        //get particle definition from polarization result
        thePhonon = G4PhononPolarization::Get(pol);
        //setup primary:
        //map the momentum vector to a velocity direction
        G4ThreeVector kdir = G4ThreeVector(0.0,0.0,1.0);
        G4ThreeVector vgroup = theLattice->MapKtoVDir(pol,kdir);
        fParticleGun->SetParticleTime(0.0*ns);
        fParticleGun->SetParticlePosition(G4ThreeVector(0.0,0.0,-0.724*mm));
        fParticleGun->SetParticleMomentumDirection(vgroup);
	fParticleGun->SetParticleEnergy(phoE*eV);
	fParticleGun->GeneratePrimaryVertex(anEvent);
  }
  while(ETotal > Esum);

}

G4int TRPSPrimaryGenerator::ChoosePolarization(G4double Ldos, G4double STdos,
                                                           G4double FTdos) const {
        G4double norm = Ldos + STdos + FTdos;
        G4double cProbST = STdos/norm;
        G4double cProbFT = FTdos/norm + cProbST;

        // NOTE:  Order of selection done to match previous random sequences
        G4double modeMixer = G4UniformRand();
        if (modeMixer<cProbST) return G4PhononPolarization::TransSlow;
        if (modeMixer<cProbFT) return G4PhononPolarization::TransFast;
        return G4PhononPolarization::Long;
}
