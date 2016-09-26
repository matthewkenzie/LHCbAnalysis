#include "TLorentzVector.h"

#include "RooRealVar.h"
#include "RooDataSet.h"

#include "Utils.h"

#include "CutOnBDT.h"

using namespace std;
using namespace Utils;

Bs2KstKst::CutOnBDT::CutOnBDT(TString _name, Variables_Analysis *_v):
  Analyser(_name),
  v(_v)
{}

Bs2KstKst::CutOnBDT::~CutOnBDT(){}

bool Bs2KstKst::CutOnBDT::AnalyseEvent() {

  processedEvents ++;

  //
  // Exectute actions
  //

  if ( !v->pass_bdt ) return false;

  passedEvents++;
  return true;
}

