#ifndef My_Tree_h
#define My_Tree_h
#include "globals.hh"
#include "TTree.h"

const int max_pairs = 50000;

class EventR;

class TRPSTree: public TTree{
	public:
		static TRPSTree* Instance( const char*, const char*);
		virtual ~TRPSTree();
	private: 
		TRPSTree(const char*, const char* );
		static TRPSTree* instance;

};
#endif
