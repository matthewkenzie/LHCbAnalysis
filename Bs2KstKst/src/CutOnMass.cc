#include "TLorentzVector.h"

#include "RooRealVar.h"
#include "RooDataSet.h"

#include "Utils.h"

#include "CutOnMass.h"

using namespace std;
using namespace Utils;

Bs2KstKst::CutOnMass::CutOnMass(TString _name, Variables_Analysis *_v):
  Analyser(_name),
  v(_v)
{}

Bs2KstKst::CutOnMass::~CutOnMass(){}

bool Bs2KstKst::CutOnMass::AnalyseEvent() {

  processedEvents ++;

  //
  // Exectute actions
  //

  if ( v->pass_massveto_2body || v->pass_rhokst || v->pass_lambdab ) return false;

  passedEvents++;
  return true;
}

