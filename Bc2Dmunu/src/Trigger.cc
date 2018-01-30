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
  if ( ! v->Mu_plus_L0MuonDecision_TOS ) return false;

  // RUN 1
  if ( v->year == "2011" || v->year == "2012" ) {
      // HLT1
      if ( !v->Mu_plus_Hlt1TrackMuonDecision_TOS ) return false;
      // HLT2
      if ( ! v->B_plus_Hlt2TopoMu2BodyBBDTDecision_TOS &&
           ! v->B_plus_Hlt2TopoMu3BodyBBDTDecision_TOS &&
           ! v->B_plus_Hlt2SingleMuonDecision_TOS         ) return false;
  }
  // RUN 2
  if ( v->year == "2015" || v->year == "2016" || v->year == "2017" ) {
      // HLT1
      if ( !v->Mu_plus_Hlt1TrackMuonDecision_TOS ) return false;
      // HLT2
      if ( ! v->B_plus_Hlt2TopoMu3BodyDecision_TOS &&
          //! v->B_plus_Hlt2TopoMu2BodyDecision_TOS && // missing because of bug
           ! v->B_plus_Hlt2SingleMuonDecision_TOS         ) return false;
  }


	return true;
}

