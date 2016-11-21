#include "EventR.hh"

EventR* EventR::instance = NULL;

EventR* EventR::Instance(){
if (!instance)
	instance = new EventR();
return instance;
}

EventR::EventR(){
}

EventR::~EventR(){
}

void EventR::clear(){
	TESid.clear();
	time.clear();
	Edep.clear();
	Pol.clear();
	vX.clear();
	vY.clear();
	vZ.clear();
	depX.clear();
	depY.clear();
	depZ.clear();
	depT.clear();
	depTemp.clear();
	depE.clear();
	positionsX.clear();
	positionsY.clear();
	positionsZ.clear();
}

void EventR::Print(){
	G4cout << " TES: " << '\t' << " time: " << '\t' << " Edep: " << '\t' << " Pol. " << G4endl;
	for(int i = 0; i<Pol.size(); i++)
	{
		G4cout << TESid[i] << '\t' << time[i] << '\t' << Edep[i] << '\t' << Pol[i] << G4endl;
	}
}

void EventR::PushTESid(G4int id){
	TESid.push_back(id);
}
void EventR::PushTEStime(G4double t){
	time.push_back(t);
}
void EventR::PushTESEdep(G4double e){
	Edep.push_back(e);
}
void EventR::PushVX(G4double x){
	vX.push_back(x);
}
void EventR::PushVY(G4double y){
	vY.push_back(y);
}
void EventR::PushVZ(G4double z){
	vZ.push_back(z);
}
void EventR::PushDepX(G4double x){
	depX.push_back(x);
}
void EventR::PushDepY(G4double y){
	depY.push_back(y);
}
void EventR::PushDepZ(G4double z){
	depZ.push_back(z);
}
void EventR::PushDepT(G4double t){
	depT.push_back(t);
}
void EventR::PushDepTemp(G4double temp){
	depTemp.push_back(temp);
}
void EventR::PushDepE(G4double e){
	depE.push_back(e);
}
void EventR::PushTESX(G4double x){
	positionsX.push_back(x);
}
void EventR::PushTESY(G4double y){
	positionsY.push_back(y);
}
void EventR::PushTESZ(G4double z){
	positionsZ.push_back(z);
}
void EventR::PushPol(G4int a){
	Pol.push_back(a);
}
double EventR::GetLatticeTemp(G4ThreeVector stepPos,G4double time){
	//fetch physical lattice for current track and step
	//G4LatticeManager* LM= G4LatticeManager::GetLatticeManager();
	//theLattice= LM->GetLattice(aTrack.GetVolume());

	//Approximate local lattice temperature by distance from other vertices in this event
	G4double dist = 0.0;
	G4double localtemp = 20.0E-3;
	for(int i=0; i<depX.size();i++){
		dist = sqrt(pow(depX[i] - stepPos.x(),2.0) + pow(depY[i] - stepPos.y(),2.0) + pow(depZ[i] - stepPos.z(),2.0) );
		//average speed of ballistic modes in TeO2 is 0.0044 mm/ns 
		if(dist < 0.0044*depT[i]){
			localtemp = depTemp[i];
		}	
	}
	
	return localtemp;
}

