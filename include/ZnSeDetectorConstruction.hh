#ifndef ZnSeDetectorConstruction_h
#define ZnSeDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4Material;
class G4VPhysicalVolume;

class ZnSeDetectorConstruction : public G4VUserDetectorConstruction {
public:
  ZnSeDetectorConstruction();
  virtual ~ZnSeDetectorConstruction();
  
public:
  virtual G4VPhysicalVolume* Construct();
  
private:
  void DefineMaterials();
  void SetupGeometry();
  
private:
  G4Material* fLiquidHelium;
  G4Material* fZn;
  G4Material* fSe;
  G4Material* fZnSe;
  G4Material* fAluminum;
  G4Material* fTungsten;
  G4VPhysicalVolume* fWorldPhys;
  G4bool fConstructed;
  G4bool fIfField;
  
public:
  inline void Field(G4bool bl) { fIfField = bl; }
};

#endif

