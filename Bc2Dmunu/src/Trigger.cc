#include "TMath.h"
#include "TLorentzVector.h"

#include "Trigger.h"

using namespace std;

Bc2Dmunu::Trigger::Trigger(TString _name, Variables_Analysis *_v):
	Analyser(_name),
  v(_v)
{}

Bc2Dmunu::Trigger::~Trigger(){}

bool Bc2Dmunu::Trigger::AnalyseEvent(){

  // -----------------------------------------------------------------------
  // do physics cuts here:
  //

  // L0
  if ( ! v->Bplus_L0MuonDecision_TOS ) return false;

  // HLT1
  if ( ! v->Bplus_Hlt1TrackMuonDecision_TOS ) return false;

  // HLT2
  if ( ! v->Bplus_Hlt2TopoMu2BodyBBDTDecision_TOS &&
       ! v->Bplus_Hlt2TopoMu3BodyBBDTDecision_TOS &&
       ! v->Bplus_Hlt2SingleMuonDecision_TOS         ) return false;

	return true;
}

