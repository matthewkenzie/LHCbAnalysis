#include "TMath.h"
#include "TLorentzVector.h"

#include "TighterSelection.h"

using namespace std;

Bc2Dmunu::TighterSelection::TighterSelection(TString _name, Variables_Analysis *_v):
	Analyser(_name),
  v(_v)
{}

Bc2Dmunu::TighterSelection::~TighterSelection(){}

bool Bc2Dmunu::TighterSelection::AnalyseEvent(){

  // -----------------------------------------------------------------------
  // do physics cuts here:
  //
  // some sanity stuff
  if ( v->K_minus_MIPCHI2PV > 10000 ) return false;
  if ( v->Pi_plus_MIPCHI2PV > 10000 ) return false;
  if ( v->Mu_plus_MIPCHI2PV > 10000 ) return false;
  if ( v->D0_IPCHI2_OWNPV > 10000 ) return false;
  if ( v->B_plus_IPCHI2_OWNPV > 5000 ) return false;
  // Match roughly LHCb-ANA-2016-059
  if ( v->nTracks >=600 ) return false;
  if ( v->Mu_plus_P < 3.e3 || v->Mu_plus_P > 100.e3 ) return false;
  if ( v->Mu_plus_PT < 750. ) return false;
  if ( v->Mu_plus_ETA < 2. || v->Mu_plus_ETA > 5. ) return false;
  if ( v->Pi_plus_ETA < 2. || v->Pi_plus_ETA > 5. ) return false;
  if ( v->Pi_plus_P > 100.e3 ) return false;
  if ( v->K_minus_P > 100.e3 ) return false;
  if ( v->K_minus_ETA < 2. || v->K_minus_ETA > 5. ) return false;
  if ( v->Mu_plus_ProbNNmu < 0.5 ) return false;
  if ( ! v->Mu_plus_isMuon ) return false;
  if ( v->Mu_plus_ETA>=3.6 && v->Mu_plus_P<15.e3 ) return false;
  if ( v->Mu_plus_MIPCHI2PV <= 4.8 ) return false;

  if ( v->B_plus_ENDVERTEX_CHI2 >= 25. ) return false;


	return true;
}

