
#include "TESSensitivity.hh"

#include "TESHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4Navigator.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "EventR.hh"


using namespace std;

TESHitsCollection* 
TESSensitivity::fHitsCollection = NULL;

TESSensitivity::TESSensitivity(G4String name)
:G4VSensitiveDetector(name)
{
  G4String HCname;
  collectionName.insert(HCname="TESHit");
  fHCID = -1;
}

TESSensitivity::~TESSensitivity(){
}

TESHitsCollection* 
TESSensitivity::GetHitsCollection(){
  return fHitsCollection;
}

void TESSensitivity::Initialize(G4HCofThisEvent*HCE)
{
  fHitsCollection = new TESHitsCollection
                   (SensitiveDetectorName,collectionName[0]);
  if(fHCID<0)
  { fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection); }
  HCE->AddHitsCollection(fHCID,fHitsCollection);
}

G4bool TESSensitivity::ProcessHits(G4Step* aStep,
                                                  G4TouchableHistory* /*ROhist*/)
{
  //if(aStep->GetTrack()->GetDefinition()!=Phonon::PhononDefinition()) return true;
  G4double edpNI = aStep->GetNonIonizingEnergyDeposit();
  //G4double edp = aStep->GetTotalEnergyDeposit();
  //Since phonon is massless, all energy deposited is kinetic energy
  G4double edp = (aStep->GetTrack()->GetKineticEnergy())/eV;
  if(edp==0. && edpNI==0.) return true;
  G4String p_name; 
  p_name = aStep->GetTrack()->GetDefinition()->GetParticleName();
 
  //If energy deposition is from a standard physics process pol is negative
  //otherwise the hit collection will register a negative polarization
  G4int pol = -10;
  if( p_name == "phononTF") pol = 1;
  else if( p_name == "phononTS") pol = 2;
  else if( p_name == "phononL") pol = 3;

  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  G4StepPoint* postStepPoint = aStep->GetPostStepPoint();
  G4TouchableHistory* theTouchable
    = (G4TouchableHistory*)(preStepPoint->GetTouchable());
  G4int copynum = theTouchable->GetVolume()->GetCopyNo();
  G4ThreeVector fWorldPos = postStepPoint->GetPosition();
  G4ThreeVector fLocalPos
    = theTouchable->GetHistory()->GetTopTransform().TransformPoint(fWorldPos);

  TESHit* aHit = new TESHit();
  aHit->SetTime(postStepPoint->GetGlobalTime());
  if((pol<0) )aHit->SetEDep(edp);
  else aHit->SetEDep(edpNI);
  aHit->SetPol(pol);
  aHit->SetWorldPos(fWorldPos);
  aHit->SetLocalPos(fLocalPos);

  fHitsCollection->insert(aHit);

  EventR* eventR = EventR::Instance();
  eventR->PushTESid(copynum);
  eventR->PushTEStime(postStepPoint->GetGlobalTime());
  eventR->PushTESEdep(edp);
  eventR->PushPol(pol);
  eventR->PushTESX(fWorldPos.getX()/mm);
  eventR->PushTESY(fWorldPos.getY()/mm);
  eventR->PushTESZ(fWorldPos.getZ()/mm);
  //eventR->Print();

  return true;
}

void TESSensitivity::EndOfEvent(G4HCofThisEvent* /*HCE*/)
{;}

