#include "TruthMatching.h"

using namespace std;

Bc2Dmunu::TruthMatching::TruthMatching(TString _name, Variables_Skimming *_v):
  Analyser(_name),
  v(_v)
{}

Bc2Dmunu::TruthMatching::~TruthMatching(){}

bool Bc2Dmunu::TruthMatching::AnalyseEvent() {

  processedEvents ++;

  //
  // Exectute actions
  //
  // SIG Bc2Dmunu MC
  if ( v->itype==-20 ) {
    if ( v->B_plus_BKGCAT!=40 ) return false;
  }
  // BKG Bp2Dmunu MC
  if ( v->itype==-29 ) {
    if ( v->B_plus_BKGCAT!=50) return false;
  }

  passedEvents++;
  return true;
}
