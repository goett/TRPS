#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UniformMagField.hh"
#include "G4TransportationManager.hh"
#include "G4FieldManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SDManager.hh"

#include "G4PhysicalConstants.hh"

#include "TESSensitivity.hh"
#include "G4LatticePhysical.hh"
//#include "G4LatticeLogical.hh"
#include "TRPSLatticeLogical.hh"
#include "G4LatticeManager.hh"

#include "G4UserLimits.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

#include <vector>


DetectorConstruction::DetectorConstruction()
 : fConstructed(false), fIfField(true) {
  fLiquidHelium = NULL;
  fTellurium = NULL;
  fOxygen = NULL;
  fTeO2 = NULL;
  fAluminum = NULL;
  fTungsten = NULL;
  fWorldPhys = NULL;
}

DetectorConstruction::~DetectorConstruction() {;}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  if(!fConstructed)
  { 
    fConstructed = true;
    DefineMaterials();
    SetupGeometry();
  }
  return fWorldPhys;
}

void DetectorConstruction::DefineMaterials()
{ 
  G4NistManager* nistManager = G4NistManager::Instance();

  fLiquidHelium = nistManager->FindOrBuildMaterial("G4_Galactic"); // to be corrected
  fTellurium = nistManager->FindOrBuildMaterial("G4_Te");
  fOxygen = nistManager->FindOrBuildMaterial("G4_O");
  //std::vector<G4int> Z(2); 
  std::vector<G4String> Z(2); 
  std::vector<G4double> W(2);
  // Z[0]=52;
  Z[0]="Te";
  W[0]=79.95*perCent;
  // Z[1]=8; 
  Z[1]="O"; 
  W[1]=20.05*perCent;
  fTeO2 = nistManager->ConstructNewMaterial("TeO2",Z,W,6.022*g/cm3,1);
  fAluminum = nistManager->FindOrBuildMaterial("G4_Al");
  fTungsten = nistManager->FindOrBuildMaterial("G4_W");
}

void DetectorConstruction::SetupGeometry()
{
  // World
  G4VSolid* worldSolid = new G4Box("World",16.*cm,16.*cm,16.*cm);
  G4LogicalVolume* worldLogical =
    new G4LogicalVolume(worldSolid,fLiquidHelium,"World");
  worldLogical->SetUserLimits(new G4UserLimits(10*mm, DBL_MAX, DBL_MAX, 0, 0));
  fWorldPhys = new G4PVPlacement(0,G4ThreeVector(),worldLogical,"World",0,
                                 false,0);
  
  // TeO2 cube - this is the volume in which we will propagate phonons
  G4VSolid* fXTALSolid = new G4Box("fXTALSolid",(25.0/2.0)*mm,(25.0/2.0)*mm,(25.0/2.0)*mm);
  //G4VSolid* fXTALSolid = new G4Box("fXTALSolid",(11.2/2.0)*mm,(1.45/2.0)*mm,(7.90/2.0)*mm);
  G4LogicalVolume* fXTALLogical = new G4LogicalVolume(fXTALSolid,fTeO2,"fXTALLogical");
  G4VPhysicalVolume* XTALPhys = new G4PVPlacement(0,G4ThreeVector(),fXTALLogical,"fXTALPhysical",worldLogical,false,0);

  //Germanium lattice information

  // G4LatticeManager gives physics processes access to lattices by volume
  G4LatticeManager* LM = G4LatticeManager::GetLatticeManager();
  TRPSLatticeLogical* TeO2Logical = dynamic_cast<TRPSLatticeLogical*>(LM->LoadLattice(fTeO2, "Te"));

  // G4LatticePhysical assigns G4LatticeLogical a physical orientation
  // The lattice here is rotated pi/4 radians about the z axis
  G4RotationMatrix* lRot = new G4RotationMatrix();
  //rotate about Z axis to get 
  lRot->rotateZ(1.0*pi/4.0*rad);
  G4LatticePhysical* TeO2Physical =
    new G4LatticePhysical(TeO2Logical, lRot);
  LM->RegisterLattice(XTALPhys, TeO2Physical);

  //Test lattice rotation:
  G4ThreeVector A(1.0,0.0,0.0);
  G4ThreeVector B(0.0,1.0,0.0);
  G4ThreeVector C(0.0,0.0,1.0);
  G4ThreeVector D(1.0,1.0,0.0);
  G4cout << "Global direction <1,0,0> is now:" << TeO2Physical->RotateToLocal(A) << G4endl;
  G4cout << "Global direction <0,1,0> is now:" << TeO2Physical->RotateToLocal(B) << G4endl;
  G4cout << "Global direction <0,0,1> is now:" << TeO2Physical->RotateToLocal(C) << G4endl;
  G4cout << "Global direction <1,1,0> is now:" << TeO2Physical->RotateToLocal(D.unit()) << G4endl;

  // detector -- Note : Aluminum electrode sensitivity is attached to crystal because phonons don't step into the Al. 
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  TESSensitivity* electrodeSensitivity = new TESSensitivity("TES");
  SDman->AddNewDetector(electrodeSensitivity);
  fXTALLogical->SetSensitiveDetector(electrodeSensitivity);

  // Visualization attributes
  worldLogical->SetVisAttributes(G4VisAttributes::Invisible);
  G4VisAttributes* XTALVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  XTALVisAtt->SetVisibility(true);
  fXTALLogical->SetVisAttributes(XTALVisAtt);
}
