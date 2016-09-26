#include "TLorentzVector.h"

#include "RooRealVar.h"
#include "RooDataSet.h"

#include "Utils.h"

#include "CutOnPID.h"

using namespace std;
using namespace Utils;

Bs2KstKst::CutOnPID::CutOnPID(TString _name, Variables_Analysis *_v):
  Analyser(_name),
  v(_v)
{}

Bs2KstKst::CutOnPID::~CutOnPID(){}

bool Bs2KstKst::CutOnPID::AnalyseEvent() {

  processedEvents ++;

  //
  // Exectute actions
  //
  // L0

  if ( !v->pass_pid ) return false;
  if ( v->pass_rhokst ) return false;

  passedEvents++;
  return true;
}

