#include "TLorentzVector.h"

#include "RooRealVar.h"
#include "RooDataSet.h"

#include "Utils.h"

#include "CutOnBDTPIDAndMass.h"

using namespace std;
using namespace Utils;

Bs2KstKst::CutOnBDTPIDAndMass::CutOnBDTPIDAndMass(TString _name, Variables_Analysis *_v):
  Analyser(_name),
  v(_v)
{}

Bs2KstKst::CutOnBDTPIDAndMass::~CutOnBDTPIDAndMass(){}

bool Bs2KstKst::CutOnBDTPIDAndMass::AnalyseEvent() {

  processedEvents ++;

  //
  // Exectute actions
  //

  if ( !v->pass_bdtpidmass ) return false;

  passedEvents++;
  return true;
}

