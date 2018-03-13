#include "TruthMatching.h"

using namespace std;

Bc2BsPi::TruthMatching::TruthMatching(TString _name, Variables_PreSel *_v):
  Analyser(_name),
  v(_v)
{}

Bc2BsPi::TruthMatching::~TruthMatching(){}

bool Bc2BsPi::TruthMatching::AnalyseEvent() {

  processedEvents ++;

  //
  // Exectute actions
  //
	if ( v->itype < 0 ) {
	  if ( v->itype==-80 ) { // Bc2BsPi
      if ( v->B_c_BKGCAT!=0 ) return false;
      if ( TMath::Abs(v->Pibach_TRUEID)!=211 ) return false;
    }
    if ( v->itype==-81 ) { // BsPrompt
      // truth criteria ?
      if ( v->B_s0_BKGCAT!=0 ) return false;
      if ( TMath::Abs(v->Pibach_TRUEID)!=211 ) return false;
    }
  }
  passedEvents++;
  return true;
}
