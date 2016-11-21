#ifndef EventR_h
#define EventR_h
#include "TESHit.hh"
#include "G4ios.hh"
#include "G4ThreeVector.hh"

class EventR
{

	public:
		static EventR* Instance();
		~EventR();
		void clear();

		void Print();
		
		void PushTESid(G4int);
		void PushTEStime(G4double);
		void PushTESEdep(G4double);
		void PushVX(G4double);
		void PushVY(G4double);
		void PushVZ(G4double);
		void PushDepX(G4double);
		void PushDepY(G4double);
		void PushDepZ(G4double);
		void PushDepT(G4double);
		void PushDepTemp(G4double);
		void PushDepE(G4double);
		void PushTESX(G4double);
		void PushTESY(G4double);
		void PushTESZ(G4double);
		void PushPol(G4int);
		double GetLatticeTemp(G4ThreeVector,G4double);
  		
		

	private:

		EventR();
		static EventR* instance;
		TESHit* hit;
		
		int n;
 		std::vector<int> TESid;
 		std::vector<double> time;
		std::vector<double> Edep;
		std::vector<int> Pol;
		std::vector<double> vX;
		std::vector<double> vY;
		std::vector<double> vZ;
		std::vector<double> depX;
		std::vector<double> depY;
		std::vector<double> depZ;
		std::vector<double> depT;
		std::vector<double> depTemp;
		std::vector<double> depE;
		std::vector<double> positionsX;
		std::vector<double> positionsY;
		std::vector<double> positionsZ;


		friend class EventAction;
		friend class PrimaryGeneratorAction;
		friend class TESSensitivity;
		friend class TRPSTree;
		friend class PhononExcitation;

};

#endif
