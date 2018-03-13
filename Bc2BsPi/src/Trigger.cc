#include "TLorentzVector.h"

#include "Trigger.h"

using namespace std;

Bc2BsPi::Trigger::Trigger(TString _name, Variables_PreSel *_v):
  Analyser(_name),
  v(_v)
{}

Bc2BsPi::Trigger::~Trigger(){}

bool Bc2BsPi::Trigger::AnalyseEvent() {

  processedEvents ++;

  //
  // Exectute actions
  //
  // L0
  if ( ! v->B_c_L0MuonDecision_TOS              		   	&&
       ! v->B_c_L0DiMuonDecision_TOS                        ) return false;

  // HLT1
  if ( v->year==TString("2011") || v->year==TString("2012") ) { // Run 1
    if ( ! v->B_c_Hlt1TrackMuonDecision_TOS             &&
         ! v->B_c_Hlt1DiMuonHighMassDecision_TOS        &&
         ! v->B_c_Hlt1SingleMuonHighPTDecision_TOS      &&
         ! v->B_c_Hlt1TrackAllL0Decision_TOS                ) return false;
  }
  else { // Run 2
    if ( ! v->B_c_Hlt1TrackMuonMVADecision_TOS          &&
         ! v->B_c_Hlt1TrackMVADecision_TOS              &&
         ! v->B_c_Hlt1TwoTrackMVADecision_TOS               ) return false;
  }

  // HLT2
  if ( v->year==TString("2011") || v->year==TString("2012") ) { // Run 1
    if ( ! v->B_c_Hlt2TopoMu2BodyBBDTDecision_TOS    		&&
         ! v->B_c_Hlt2TopoMu3BodyBBDTDecision_TOS   		&&
         ! v->B_c_Hlt2TopoMu4BodyBBDTDecision_TOS   		&&
         ! v->B_c_Hlt2DiMuonDetachedJPsiDecision_TOS    &&
         ! v->B_c_Hlt2DiMuonDetachedDecision_TOS        &&
         ! v->B_c_Hlt2DiMuonDetachedHeavyDecision_TOS   &&
         ! v->B_c_Hlt2SingleMuonDecision_TOS               ) return false;
  }
  else { // Run 2
    if ( ! v->B_c_Hlt2TopoMu2BodyDecision_TOS    				&&
         ! v->B_c_Hlt2TopoMu3BodyDecision_TOS   				&&
         ! v->B_c_Hlt2TopoMu4BodyDecision_TOS   		    &&
         ! v->B_c_Hlt2DiMuonDetachedJPsiDecision_TOS    &&
         ! v->B_c_Hlt2DiMuonDetachedDecision_TOS        &&
         ! v->B_c_Hlt2DiMuonDetachedHeavyDecision_TOS   &&
         ! v->B_c_Hlt2SingleMuonDecision_TOS               ) return false;
  }

  passedEvents++;
  return true;
}
