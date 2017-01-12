
#ifndef TESHit_h
#define TESHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class G4AttDef;
class G4AttValue;

class TESHit : public G4VHit
{
  public:

      TESHit();
      virtual ~TESHit();
      TESHit(const TESHit &right);
      const TESHit& operator=(const TESHit &right);
      int operator==(const TESHit &right) const;

      inline void *operator new(size_t);
      inline void operator delete(void *aHit);

      virtual void Draw();
      virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
      virtual std::vector<G4AttValue>* CreateAttValues() const;
      virtual void Print();

  private:
      G4double fTime;
      G4double fEdep;
      G4int fPol;
      G4ThreeVector fLocalPos;
      G4ThreeVector fWorldPos;

  public:
      inline void SetTime(G4double t) { fTime = t; }
      inline G4double GetTime() const { return fTime; }
      inline void SetEDep(G4double e) { fEdep = e; }
      inline G4double GetEDep() const { return fEdep; }
      inline void SetPol(G4int pol) { fPol = pol; }
      inline G4int GetPol() const { return fPol; }
      inline void SetLocalPos(G4ThreeVector xyz) { fLocalPos = xyz; }
      inline G4ThreeVector GetLocalPos() const { return fLocalPos; }
      inline void SetWorldPos(G4ThreeVector xyz) { fWorldPos = xyz; }
      inline G4ThreeVector GetWorldPos() const { return fWorldPos; }
};

typedef G4THitsCollection<TESHit> TESHitsCollection;

extern G4Allocator<TESHit> TESHitAllocator;

inline void* TESHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*)TESHitAllocator.MallocSingle();
  return aHit;
}

inline void TESHit::operator delete(void* aHit)
{
  TESHitAllocator.FreeSingle((TESHit*) aHit);
}

#endif


