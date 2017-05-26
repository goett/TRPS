#include "DBDPrimaryGenerator.hh"

#include "Randomize.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4RandomDirection.hh"
#include "Randomize.hh"

DBDPrimaryGenerator::DBDPrimaryGenerator()
{
  fParticleGun = new G4ParticleGun();

  //default kinematic
  // handled in GeneratePrimaryVertex
}

DBDPrimaryGenerator::~DBDPrimaryGenerator()
{
  delete fParticleGun;
}

void DBDPrimaryGenerator::GeneratePrimaryVertex(G4Event* anEvent)
{
  G4ParticleDefinition* particle;

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();


  fParticleGun->SetParticleTime(0.0*ns);
  G4ThreeVector a = G4RandomDirection();
  //G4ThreeVector a = G4ThreeVector(-1.0,0.0,0.0);
  particle = particleTable->FindParticle("e-");
  fParticleGun->SetParticleDefinition(particle);
  
  //G4double momentum = fQvalue/2.0;
  //G4double sigmaMomentum = 0.*keV;
  //G4double sigmaAngle = 0.*deg;
  //G4double pp = momentum + (G4UniformRand()-0.5)*sigmaMomentum;
  G4double mass = particle->GetPDGMass();
  //G4double Ekin = std::sqrt(pp*pp+mass*mass)-mass;
  //fParticleGun->SetParticleEnergy(Ekin);
  fParticleGun->SetParticleEnergy(fQvalue/2.0);
  //G4double angle = (G4UniformRand()-0.5)*sigmaAngle;
  fParticleGun->SetParticleMomentumDirection(a);
  G4ThreeVector vertexpos = G4ThreeVector(G4UniformRand()*12.5*mm,G4UniformRand()*12.5*mm,G4UniformRand()*12.5*mm);
  fParticleGun->SetParticlePosition(vertexpos);
  fParticleGun->GeneratePrimaryVertex(anEvent);
  fParticleGun->SetParticleMomentumDirection(-1.0*a); //back to back with first primary
  fParticleGun->GeneratePrimaryVertex(anEvent);
  
}

void DBDPrimaryGenerator::SetQValue(double value){
	fQvalue = value;
}
