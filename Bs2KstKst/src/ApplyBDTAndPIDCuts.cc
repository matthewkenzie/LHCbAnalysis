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

  v->pass_bdt = false;
  v->pass_pid = false;

  if ( v->year == "2011" ) {
    if ( v->bdtoutput           > 0.00    ) v->pass_bdt = true;
    if ( v->min_kaon_V3ProbNNKpi_corr > 0.6 &&
         v->max_pion_V3ProbNNKpi_corr < 0.6    ) v->pass_pid = true;
    //if ( v->min_kaon_ProbNNKpi  > 0.30 &&
         //v->min_pion_ProbNNpiKp > 0.00    ) v->pass_pid = true;
  }
  else if ( v->year == "2012" ) {
    if ( v->bdtoutput           > 0.00    ) v->pass_bdt = true;
    if ( v->min_kaon_V3ProbNNKpi_corr > 0.6 &&
         v->max_pion_V3ProbNNKpi_corr < 0.6    ) v->pass_pid = true;
    //if ( v->min_kaon_ProbNNKpi  > 0.30 &&
         //v->min_pion_ProbNNpiKp > 0.00    ) v->pass_pid = true;
  }
  else {
    error ( "v->year not recognised: "+v->year );
  }

  passedEvents++;
  return true;
}

