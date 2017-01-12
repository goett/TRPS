#include "TRPSPhysicsList.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
//#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4StoppingPhysics.hh"
#include "G4ProcessManager.hh"
#include "G4SystemOfUnits.hh"


TRPSPhysicsList::TRPSPhysicsList(G4int verbose) : G4VUserPhysicsList() {
  if (verbose) G4cout << "TRPSPhysicsList::constructor" << G4endl;

  SetVerboseLevel(verbose);
  SetDefaultCutValue(0.1*mm);
}

TRPSPhysicsList::~TRPSPhysicsList() {;}

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4PhononLong.hh"
#include "G4PhononTransFast.hh"
#include "G4PhononTransSlow.hh"

void TRPSPhysicsList::ConstructParticle() {
  G4BosonConstructor pBoConstructor;
  pBoConstructor.ConstructParticle();
  
  G4LeptonConstructor pLConstructor;
  pLConstructor.ConstructParticle();
  
  G4MesonConstructor pMConstructor;
  pMConstructor.ConstructParticle();
  
  G4BaryonConstructor pBaConstructor;
  pBaConstructor.ConstructParticle();
  
  G4IonConstructor pIConstructor;
  pIConstructor.ConstructParticle();
  
  G4ShortLivedConstructor pSConstructor;
  pSConstructor.ConstructParticle(); 
	
  G4PhononLong::PhononDefinition();
  G4PhononTransFast::PhononDefinition();
  G4PhononTransSlow::PhononDefinition();
}


void TRPSPhysicsList::ConstructProcess() {
  //Add EM interactions
  ConstructEM();
  //Add phonon processes
  ConstructThermalization();
  //Transportation
  AddTransportation();


  //Add stopping physics
  G4VPhysicsConstructor* stoppingPhysics = new G4StoppingPhysics();
  stoppingPhysics->ConstructProcess();
}

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"
#include "G4MuMultipleScattering.hh"
#include "G4hMultipleScattering.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hIonisation.hh"
#include "G4ionIonisation.hh"

#include "G4NuclearStopping.hh"

void TRPSPhysicsList::ConstructEM(){
  //Only make processes once
  G4VPhysicsConstructor* phEMS = new G4EmStandardPhysics_option3();
  phEMS->ConstructProcess();
  
  /* 
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    if (particleName == "gamma") {
    // gamma
      // Construct processes for gamma
      pmanager->AddDiscreteProcess(new G4GammaConversion());
      pmanager->AddDiscreteProcess(new G4ComptonScattering());
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect());

    } else if (particleName == "e-") {
    //electron
      // Construct processes for electron
      pmanager->AddProcess(new G4eMultipleScattering(),-1, 1, 1);
      pmanager->AddProcess(new G4eIonisation(),       -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung(),   -1, 3, 3);
    } else if (particleName == "alpha") {
    //alpha
      // Construct processes for alphas
      pmanager->AddProcess(new G4hMultipleScattering());
      G4ionIonisation* ionIoni = new G4ionIonisation();
      ionIoni->SetStepFunction(0.1, 1*um);
      pmanager->AddProcess(new G4ionIonisation());
      pmanager->AddProcess(new G4NuclearStopping());
    }
  }*/
}

#include "PhononExcitation.hh"
//#include "AnharmonicDecay.hh"
#include "G4PhononDownconversion.hh"
#include "G4PhononReflection.hh"
#include "G4PhononScattering.hh"

void TRPSPhysicsList::ConstructThermalization(){
  
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    if (particleName == "gamma") {
    // gamma
      // Construct processes for gamma
      pmanager->AddDiscreteProcess(new PhononExcitation());
    } else if (particleName == "e-") {
    //electron
      // Construct processes for electron
      pmanager->AddDiscreteProcess(new PhononExcitation());
    } else if (particleName == "e+") {
    //positron
      // Construct processes for positron
      pmanager->AddDiscreteProcess(new PhononExcitation());
    } else if (particleName == "alpha") {
    //alpha
      // Construct processes for alpha
      pmanager->AddDiscreteProcess(new PhononExcitation());
    }
  }
  
  //G4VProcess* anDec = new AnharmonicDecay();
  G4VProcess* phScat = new G4PhononScattering;
  G4VProcess* phRefl = new G4PhononReflection;
  G4VProcess* phDown = new G4PhononDownconversion;

  // Set process verbosity to match physics list, for diagnostics
  //phScat->SetVerboseLevel(verboseLevel);
  //phRefl->SetVerboseLevel(verboseLevel);
  //phDown->SetVerboseLevel(verboseLevel);
  
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();

    if (phScat->IsApplicable(*particle)) pmanager->AddDiscreteProcess(phScat);
    //Uncomment line below and comment out the following line to enable default downconversion process
    if (phDown->IsApplicable(*particle)) pmanager->AddDiscreteProcess(phDown);
    //if (phDown->IsApplicable(*particle)) pmanager->AddDiscreteProcess(anDec);
    if (phRefl->IsApplicable(*particle)) pmanager->AddDiscreteProcess(phRefl);
  }
}

void TRPSPhysicsList::SetCuts() {
  // These values are used as the default production thresholds
  // for the world volume.
  SetCutsWithDefault();
}

