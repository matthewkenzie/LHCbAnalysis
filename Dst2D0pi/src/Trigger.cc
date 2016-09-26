#include "TLorentzVector.h"

#include "Trigger.h"

using namespace std;

Dst2D0pi::Trigger::Trigger(TString _name, Variables_Analysis *_v):
  Analyser(_name),
  v(_v)
{}

Dst2D0pi::Trigger::~Trigger(){}

bool Dst2D0pi::Trigger::AnalyseEvent() {

  processedEvents ++;

  //
  // Exectute actions
  //
  // HLT1
  if ( ! v->Dst_Hlt1CalibTrackingKPiDecision_TOS               ) return false;

  // HLT2
  if ( ! v->Dst_Hlt2CharmHadDstp2D0Pip_D02KmPip_LTUNBTurboDecision_TOS   ) return false;

  passedEvents++;
  return true;
}
