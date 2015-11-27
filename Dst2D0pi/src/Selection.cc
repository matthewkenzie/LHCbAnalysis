#include "TMath.h"
#include "TLorentzVector.h"

#include "Selection.h"

using namespace std;

Dst2D0pi::Selection::Selection(TString _name, Variables_Analysis *_v):
	Analyser(_name),
  v(_v)
{}

Dst2D0pi::Selection::~Selection(){}

bool Dst2D0pi::Selection::AnalyseEvent(){

  // -----------------------------------------------------------------------
  // do physics cuts here:
  //
  //
  //
  if ( ! v->Dst_2010_plus_Hlt1CalibTrackingKPiDecision_TOS &&
       ! v->Dst_2010_plus_Hlt1CalibTrackingKKDecision_TOS  &&
       ! v->Dst_2010_plus_Hlt1CalibTrackingPiPiDecision_TOS   ) return false;

  //if ( v->D0_PT < 4000 ) return false;
  //if ( v->D0_MINIPCHI2 < 5 ) return false;
  //if ( (v->Dst_2010_plus_M - v->D0_M) > 150 ) return false;

  //if ( (v->Dst_M - v->D0_M) > 150 ) return false;
  //if ( v->D0_TAU < 0. ) return false;
  //if ( v->D0_MINIPCHI2 < 3. ) return false;
  //if ( v->Dst_M < 1980 ) return false;
  //if ( v->Dst_M > 2040 ) return false;

	return true;
}

