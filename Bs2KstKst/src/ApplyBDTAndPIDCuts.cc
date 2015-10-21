#include "TLorentzVector.h"

#include "RooRealVar.h"
#include "RooDataSet.h"

#include "Utils.h"

#include "ApplyBDTAndPIDCuts.h"

using namespace std;
using namespace Utils;

Bs2KstKst::ApplyBDTAndPIDCuts::ApplyBDTAndPIDCuts(TString _name, Variables_Analysis *_v):
  Analyser(_name),
  v(_v)
{}

Bs2KstKst::ApplyBDTAndPIDCuts::~ApplyBDTAndPIDCuts(){}

bool Bs2KstKst::ApplyBDTAndPIDCuts::AnalyseEvent() {

  processedEvents ++;

  //
  // Exectute actions
  //
  // L0
  v->min_kaon_ProbNNk               = TMath::Min( v->Kplus_ProbNNk            , v->Kminus_ProbNNk );
  v->min_pion_ProbNNpiKp            = TMath::Min( v->Piplus_ProbNNpiKp            , v->Piminus_ProbNNpiKp );

  v->pass_bdt = false;
  v->pass_pid = false;

  if ( v->year == "2011" ) {
    if ( v->bdtoutput           > -0.02   ) v->pass_bdt = true;
    if ( v->min_kaon_ProbNNk    > 0.22 &&
         v->min_pion_ProbNNpiKp > 0.00    ) v->pass_pid = true;
  }
  else if ( v->year == "2012" ) {
    if ( v->bdtoutput           > 0.00    ) v->pass_bdt = true;
    if ( v->min_kaon_ProbNNk    > 0.26 &&
         v->min_pion_ProbNNpiKp > 0.00    ) v->pass_pid = true;
  }
  else {
    error ( "v->year not recognised: "+v->year );
  }

  passedEvents++;
  return true;
}

