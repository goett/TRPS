
#include "PhononExcitation.hh"
#include "G4PhysicalConstants.hh"
#include "EventR.hh"


PhononExcitation::PhononExcitation(const G4String& aName)
  : G4VDiscreteProcess(aName), currentTrack(0),theLattice(0),fBeta(0.), fGamma(0.), fLambda(0.), fMu(0.) {;}

PhononExcitation::~PhononExcitation() {;}


G4double PhononExcitation::GetMeanFreePath(const G4Track& aTrack,
						 G4double /*previousStepSize*/,
						 G4ForceCondition* condition) {
  *condition = StronglyForced;
  return DBL_MAX;
}



G4VParticleChange* PhononExcitation::PostStepDoIt( const G4Track& aTrack,
							 const G4Step& aStep) {
  aParticleChange.Initialize(aTrack);

  currentTrack = &aTrack;

  //Get energy loss of track
  G4double Eloss = aStep.GetTotalEnergyDeposit();
  
  //if there is no energy deposited, exit with no secondaries produced
  if((Eloss <=0))
  {
	  aParticleChange.SetNumberOfSecondaries(0);
	  return &aParticleChange;
  }

  //fetch physical lattice for current track and step
  G4LatticeManager* LM= G4LatticeManager::GetLatticeManager();
  if(LM->HasLattice(aTrack.GetVolume())){
  	theLattice= LM->GetLattice(aTrack.GetVolume());
  }
  // quick fix to stop process in materials without lattice
  // should find a way to turn processes on/off by material
  else{
	aParticleChange.SetNumberOfSecondaries(0);
	return &aParticleChange;
  }
  
  //Produce vector of energies describing local lattice excitation temperature
  EventR* eventR = EventR::Instance();
  double temp1 = eventR->GetLatticeTemp(currentTrack->GetPosition(),currentTrack->GetGlobalTime()); 
  // assume minor variation of heat capacity at low temperature
  double deltat = Eloss/12483.01; //Baruci et. al. Journal Low Temp. Physics v.123, #5/6
  double temp2 = temp1+deltat;
  TF1* planckSpec = new TF1("p120","pow(x,3.0)/( exp(x/(8.61E-5*[0]))-1 )",0.1*2.8*8.61E-5*temp2,10*2.8*8.61E-5*temp2);
  planckSpec->SetParameter(0,temp2);
  eventR->PushDepTemp(temp2);
  G4double Esum = 2.5e-5*eV;
  G4double phoE = 0.0;
  std::vector<G4double> phoEs = {2.5e-5*eV};
  do{
        phoE = planckSpec->GetRandom();
	phoEs.push_back(phoE*eV);
	Esum += phoE;
  }
  while(Eloss > Esum);
  G4cout << phoEs.size() << " secondary phonons produced for this step." << G4endl;


  //Update number of secondaries
  aParticleChange.SetNumberOfSecondaries(phoEs.size());

  for(G4double pE : phoEs ){
  	//Momentum transfer from interacting particle
  	G4ThreeVector S = -1*(aStep.GetDeltaMomentum());
  	//k-k' = p_pho + G
	//Determine reciprocal lattice vector closest to momentum transfer
	//G4ThreeVector dir = FindReciprocalTransfer(S);
	//Uncomment following lines to create an isotropic distribution: tests show this produces consistent results as reciprocal analysis with less overhead)
	G4ThreeVector dir = G4RandomDirection();
  	//Set polarization based on density of states
  	G4int pol = ChoosePolarization(theLattice->GetLDOS(),theLattice->GetSTDOS(),theLattice->GetFTDOS()); 
  	G4Track* sec = CreateSecondary(pol,dir.unit(),pE);
  	aParticleChange.AddSecondary(sec);
  }

  return &aParticleChange;
}

G4int PhononExcitation::ChoosePolarization(G4double Ldos, G4double STdos,
							   G4double FTdos) const {
	G4double norm = Ldos + STdos + FTdos;
	G4double STprob = STdos/norm;
	G4double FTprob = FTdos/norm + STprob;
	
	// 
	G4double randpol = G4UniformRand();
	if (randpol<STprob) return G4PhononPolarization::TransSlow;
	if (randpol<FTprob) return G4PhononPolarization::TransFast;
	return G4PhononPolarization::Long;
}

G4Track* PhononExcitation::CreateSecondary(G4int polarization,
					   const G4ThreeVector& waveVec,
					   G4double energy) const {
        
	if (verboseLevel>1) {
	  G4cout << GetProcessName() << " CreateSecondary pol " << polarization
	         << " K " << waveVec << " E " << energy << G4endl;
	}

	//Now map the momentum vector to a velocity direction (in the lattice)
	//This function rotates to the local lattice orientation, determines 
	//velocity direction, then rotates back to lab coordinates
	G4ThreeVector vgroup = theLattice->MapKtoVDir(polarization, waveVec);
	
	if (verboseLevel>1) G4cout << " MapKtoVDir returned " << vgroup << G4endl;
        //Check that group velocity direction is a unit vector.	
	if (verboseLevel && std::fabs(vgroup.mag()-1.) > 0.01) {
	  G4cout << "WARNING: " << GetProcessName() << " vgroup not a unit vector: "
	         << vgroup << G4endl;
	}
	
	G4ParticleDefinition* thePhonon = G4PhononPolarization::Get(polarization);
	
	// Secondaries are created at the current track coordinates (at current step)
	G4Track* sec = new G4Track(new G4DynamicParticle(thePhonon, vgroup, energy),
	      		     currentTrack->GetGlobalTime(),
	      		     currentTrack->GetPosition());
	
        //The V direction was returned above, but the velocity magnitude needs to be set.
        //sec->SetMomentumDirection(waveVec);
        sec->SetMomentumDirection(vgroup);
	sec->SetVelocity(theLattice->MapKtoV(polarization,waveVec));    
	sec->UseGivenVelocity(true); // Without this, the velocity is not properly re-set. Not sure why. 

	return sec;
}

G4bool PhononExcitation::IsApplicable(const G4ParticleDefinition& aPD) {
  //Any particle that deposits energy in the lattice excites phonon modes
  if(aPD.GetParticleName() == "phononL") return false;
  if(aPD.GetParticleName() == "phononTF") return false;
  if(aPD.GetParticleName() == "phononTS") return false;
  if(aPD.IsShortLived()) return false;

  return true;
}

