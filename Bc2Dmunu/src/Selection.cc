#include "TMath.h"
#include "TLorentzVector.h"

#include "Selection.h"

using namespace std;

Bc2Dmunu::Selection::Selection(TString _name, Variables_Skimming *_v):
	Analyser(_name),
  v(_v)
{}

Bc2Dmunu::Selection::~Selection(){}

bool Bc2Dmunu::Selection::AnalyseEvent(){

  // -----------------------------------------------------------------------
  // do physics cuts here:
  //

  if ( v->K_minus_ProbNNk < 0.2 ) return false;
  if ( v->Pi_plus_ProbNNpi < 0.2 ) return false;
  if ( v->Mu_plus_ProbNNmu < 0.2 ) return false;

  //if ( v->K_minus_ProbNNghost > 0.5 ) return false;
  //if ( v->Pi_plus_ProbNNghost > 0.5 ) return false;
  //if ( v->Mu_plus_ProbNNghost > 0.5 ) return false;

  ////if ( v->K_minus_MINIPCHI2 < 9. ) return false;
  ////if ( v->Pi_plus_MINIPCHI2 < 9. ) return false;
  ////if ( v->Mu_plus_MINIPCHI2 < 9. ) return false;

  //if ( v->D0_PT < 500 ) return false;
  //if ( v->D0_M < 1820 || v->D0_M > 1900 ) return false;
////  if ( v->D0_ENDVERTEX_CHI2/v->D0_ENDVERTEX_NDOF > 3. ) return false;

  //if ( v->B_plus_LTIME < 0. || v->B_plus_LTIME > 0.03 ) return false;
  //if ( v->B_plus_MCORR < 2000. ) return false;
  if ( v->B_plus_DIRA_OWNPV < 0.999 ) return false;

	return true;
}

