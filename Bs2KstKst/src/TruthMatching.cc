#include "TruthMatching.h"

using namespace std;

Bs2KstKst::TruthMatching::TruthMatching(TString _name, Variables_PreSel *_v):
  Analyser(_name),
  v(_v)
{}

Bs2KstKst::TruthMatching::~TruthMatching(){}

bool Bs2KstKst::TruthMatching::AnalyseEvent() {

  processedEvents ++;

  //
  // Exectute actions
  //
	if ( v->itype < 0 ) {
    // Bs initial states to Kpi Kpi
    if ( v->itype == -70 || v->itype == -80 ||  // Bs -> Kst0 Kst0
         v->itype == -71 || v->itype == -81 ||  // Bs -> Kst0 Kst0 (1430)
         v->itype == -72 || v->itype == -82 ||  // Bs -> Kst0 (1430) Kst0 (1430)
         v->itype == -73 || v->itype == -83 )   // Bs -> Kpi Kpi phase space
    {
      if ( v->B_s0_BKGCAT > 10 ) return false;
    }
    // Bd to Kst Kst
    if ( v->itype == -74 || v->itype == -84 )   // Bs -> Kst0 Kst0
    {
      if ( v->B_s0_BKGCAT > 20 ) return false;
    }
    // Bd to Rho or Phi Kst
    if ( v->itype == -75 || v->itype == -85 ||  // Bd -> phi Kst0
         v->itype == -76 || v->itype == -86 ||  // Bs -> phi Kst0
         v->itype == -77 || v->itype == -87 )   // Bd -> rho Kst0
    {
      if ( v->B_s0_BKGCAT > 30 ) return false;
    }
    // Lb initial states
    if ( v->itype == -78 || v->itype == -88 ||  // Lb -> pKpipi
         v->itype == -79 || v->itype == -89 )   // Lb -> ppipipi
    {
      if ( v->B_s0_BKGCAT > 30 ) return false;
    }
	}
  passedEvents++;
  return true;
}
