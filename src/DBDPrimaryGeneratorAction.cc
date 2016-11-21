#include "DBDPrimaryGeneratorAction.hh"

#include "Randomize.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4RandomDirection.hh"
#include "Randomize.hh"

DBDPrimaryGeneratorAction::DBDPrimaryGeneratorAction()
{
  fParticleGun = new G4ParticleGun();
  
  //default kinematic
  // handled in GeneratePrimaries
}

DBDPrimaryGeneratorAction::~DBDPrimaryGeneratorAction()
{
  delete fParticleGun;
}

void DBDPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4ParticleDefinition* particle;

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  //particleTable ->DumpTable("ALL");


  fParticleGun->SetParticleTime(0.0*ns);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));

  G4int ev_type = 3;  // 1 = photon 2 = electron 3 = alpha
  G4double Ekin;

  if (ev_type == 1) {            // gamma  ======================================================================
    particle = particleTable->FindParticle("gamma");
    fParticleGun->SetParticleDefinition(particle);

    fParticleGun->SetParticleEnergy(1332*keV);
    fParticleGun->SetParticleMomentumDirection(G4RandomDirection());
    fParticleGun->SetParticlePosition(G4ThreeVector(0.0, 0.0,0.0));
    fParticleGun->GeneratePrimaryVertex(anEvent);

  }    // ev_type=1

  if (ev_type == 2) {           // 1 e-
	particle = particleTable->FindParticle("e-");
	fParticleGun->SetParticleDefinition(particle);

	G4double momentum = 2039.0/2.0*keV;
	G4double sigmaMomentum = 0.*keV;
	G4double sigmaAngle = 0.*deg;
	G4double pp = momentum + (G4UniformRand()-0.5)*sigmaMomentum;
	G4double mass = particle->GetPDGMass();
	Ekin = std::sqrt(pp*pp+mass*mass)-mass;
	fParticleGun->SetParticleEnergy(Ekin);
	G4double angle = (G4UniformRand()-0.5)*sigmaAngle;
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1.0));     // straight
    	G4int npart = 1;
    	for (G4int i=1; i<=npart; i++) {
      		fParticleGun->SetParticlePosition(G4ThreeVector(0.00*mm, 0.0*mm ,0.0*mm));
      		fParticleGun->GeneratePrimaryVertex(anEvent);
    	}

  }    // ev_type=2
  
  if (ev_type == 3) {            // alpha  ======================================================================
    particle = particleTable->FindParticle("alpha");
    fParticleGun->SetParticleDefinition(particle);

    fParticleGun->SetParticleEnergy(5.405*MeV);
    fParticleGun->SetParticleMomentumDirection(G4RandomDirection());
    fParticleGun->SetParticlePosition(G4ThreeVector(0.0, 0.0,0.0));
    fParticleGun->GeneratePrimaryVertex(anEvent);

  }    // ev_type=3


}
