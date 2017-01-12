
#include "TESHit.hh"

#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UnitsTable.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

G4Allocator<TESHit> TESHitAllocator;

TESHit::TESHit()
{
  fTime = 0.;
  fEdep = 0.;
  fPol = 10.;
}

TESHit::~TESHit()
{;}

TESHit::TESHit(const TESHit &right)
: G4VHit() {
  fTime = right.fTime;
  fEdep = right.fEdep;
  fPol = right.fPol;
  fWorldPos = right.fWorldPos;
  fLocalPos = right.fLocalPos;
}

const TESHit& TESHit::operator=(const TESHit &right)
{
  fTime = right.fTime;
  fEdep = right.fEdep;
  fPol = right.fPol;
  fWorldPos = right.fWorldPos;
  fLocalPos = right.fLocalPos;
  return *this;
}

int TESHit::operator==(const TESHit &/*right*/) const
{
  return 0;
}

void TESHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(fWorldPos);
    circle.SetScreenSize(15);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(0.65,0.65,0.);
    G4VisAttributes attribs(colour);
    attribs.SetStartTime(fTime);
    attribs.SetEndTime(fTime+1*ms);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

const std::map<G4String,G4AttDef>* TESHit::GetAttDefs() const
{
  G4bool isNew;
  std::map<G4String,G4AttDef>* store
    = G4AttDefStore::GetInstance("TESHit",isNew);
  if (isNew) {
    G4String HitType("HitType");
    (*store)[HitType] = G4AttDef(HitType,"Hit Type","Physics","","G4String");

    G4String Time("Time");
    (*store)[Time] = G4AttDef(Time,"Time","Physics","G4BestUnit","G4double");

    G4String EDep("EDep");
    (*store)[EDep] = G4AttDef(Time,"EDep","Physics","G4BestUnit","G4double");
    
    G4String Pol("Pol");
    (*store)[Pol] = G4AttDef(Time,"Pol","Physics","G4BestUnit","G4int");

    G4String Pos("Pos");
    (*store)[Pos] = G4AttDef(Pos, "Position",
                      "Physics","G4BestUnit","G4ThreeVector");
  }
  return store;
}

std::vector<G4AttValue>* TESHit::CreateAttValues() const
{
  std::vector<G4AttValue>* values = new std::vector<G4AttValue>;

  values->push_back(G4AttValue("HitType","TESHit",""));

  values->push_back
    (G4AttValue("Time",G4BestUnit(fTime,"Time"),""));

  values->push_back
    (G4AttValue("EDep",G4BestUnit(fEdep,"Energy"),""));
  
values->push_back
    (G4AttValue("Pol",G4BestUnit(fPol,"Polarization"),""));

  values->push_back
    (G4AttValue("Pos",G4BestUnit(fWorldPos,"Length"),""));

  return values;
}

void TESHit::Print()
{
  G4cout << "  time " << fTime/ns << " (nsec) : at " << fLocalPos
         << "  -- fEdep = " << fEdep/eV << " [eV]   with polarization: " << fPol << G4endl;
}


