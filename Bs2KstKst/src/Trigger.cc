#include "TLorentzVector.h"

#include "Trigger.h"

using namespace std;

Bs2KstKst::Trigger::Trigger(TString _name, Variables_PreSel *_v):
  Analyser(_name),
  v(_v)
{}

Bs2KstKst::Trigger::~Trigger(){}

bool Bs2KstKst::Trigger::AnalyseEvent() {

  processedEvents ++;

  //
  // Exectute actions
  //
  // L0
  if ( ! v->B_s0_L0HadronDecision_TOS                 &&
       ! v->B_s0_L0Global_TIS														 ) return false;

  // HLT1
  if ( ! v->B_s0_Hlt1TrackAllL0Decision_TOS               ) return false;

  // HLT2
  if ( ! v->B_s0_Hlt2Topo2BodyBBDTDecision_TOS    		&&
       ! v->B_s0_Hlt2Topo3BodyBBDTDecision_TOS   			&&
       ! v->B_s0_Hlt2Topo4BodyBBDTDecision_TOS   			    ) return false;

  passedEvents++;
  return true;
}
