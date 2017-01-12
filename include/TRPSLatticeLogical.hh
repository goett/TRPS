
#ifndef TRPSLatticeLogical_h
#define TRPSLatticeLogical_h

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4LatticeLogical.hh"
#include <iosfwd>


class TRPSLatticeLogical:public G4LatticeLogical {
public:
  void SetKbConstant(G4double b) { fb=b; }
  void SetKaConstant(G4double a) { fa=a; }

  G4double GetKbConstant() const { return fb; }
  G4double GetKaConstant() const { return fa; }

private:
  G4double fa;       //Klemens anharmonic decay constant a
  G4double fb;       ///Klemens anharmonic decay constant b
};

#endif
