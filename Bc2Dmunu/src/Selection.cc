#include "TMath.h"
#include "TLorentzVector.h"

#include "Selection.h"

using namespace std;

Bc2Dmunu::Selection::Selection(TString _name, Variables_Analysis *_v):
	Analyser(_name),
  v(_v)
{}

Bc2Dmunu::Selection::~Selection(){}

bool Bc2Dmunu::Selection::AnalyseEvent(){

  // -----------------------------------------------------------------------
  // do physics cuts here:
  //

  if ( v->Bplus_TAU < 0. ) return false;
  if ( v->muplus_PIDK > -2. ) return false;
  if ( v->muplus_MINIPCHI2 < 9. ) return false;

  // PID fav
  if ( v->Kminus_PIDK < 2. || v->piplus_PIDK > -2. ) return false;

  // PID sup
  //if ( v->Kminus_PIDK > -5. || v->piplus_PIDK < 5. ) return false;

	return true;
}

