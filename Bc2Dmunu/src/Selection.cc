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

  // PID fav
  //if ( v->Kminus_PIDK < 2. || v->piplus_PIDK > -2. ) return false;

  // PID sup
  //if ( v->Kminus_PIDK > -5. || v->piplus_PIDK < 5. ) return false;

  if ( v->Kminus_NNK < 0.5 ) return false;
  if ( v->piplus_NNpi < 0.5 ) return false;
  if ( v->muplus_NNmu < 0.5 ) return false;

  if ( v->Kminus_ghost > 0.1 ) return false;
  if ( v->piplus_ghost > 0.1 ) return false;
  if ( v->muplus_ghost > 0.1 ) return false;

  if ( v->Kminus_MINIPCHI2 < 9. ) return false;
  if ( v->piplus_MINIPCHI2 < 9. ) return false;
  if ( v->muplus_MINIPCHI2 < 9. ) return false;

  if ( v->D0_PT < 1500 ) return false;
  if ( v->D0_M < 1850 || v->D0_M > 1880 ) return false;
  if ( v->D0_ENDVERTEX_CHI2 > 2. ) return false;

  if ( v->Bplus_TAU < 0. || v->Bplus_TAU > 0.003 ) return false;
  if ( v->Bplus_Mcorr < 4000. ) return false;
  if ( v->Bplus_ENDVERTEX_CHI2 > 2. ) return false;
  if ( v->Bplus_DIRA_OWNPV < 0.9997 ) return false;

	return true;
}

