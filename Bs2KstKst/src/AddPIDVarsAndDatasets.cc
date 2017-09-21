#include "TLorentzVector.h"

#include "RooRealVar.h"
#include "RooDataSet.h"

#include "AddPIDVarsAndDatasets.h"

using namespace std;

Bs2KstKst::AddPIDVarsAndDatasets::AddPIDVarsAndDatasets(TString _name, Variables_Analysis *_v):
  Analyser(_name),
  v(_v)
{}

Bs2KstKst::AddPIDVarsAndDatasets::~AddPIDVarsAndDatasets(){}

bool Bs2KstKst::AddPIDVarsAndDatasets::AnalyseEvent() {

  processedEvents ++;

  //
  // Exectute actions
  //
  //
  // The data contains the PID vars along the lines of V3ProbNN etc.
  // The MC PID gets corrected before the presel stage and these
  // go into branches like ProbNNk_corr (which is the V3 corrected)
  //

  // Data vars
  v->Kplus_ProbNNKpi                = v->Kplus_ProbNNk    * ( 1. - v->Kplus_ProbNNpi  );
  v->Kminus_ProbNNKpi               = v->Kminus_ProbNNk   * ( 1. - v->Kminus_ProbNNpi );
  v->Piplus_ProbNNpiK               = v->Piplus_ProbNNpi  * ( 1. - v->Piplus_ProbNNk  );
  v->Piminus_ProbNNpiK              = v->Piminus_ProbNNpi * ( 1. - v->Piminus_ProbNNk );

  // Then take the minimums
  v->min_kaon_ProbNNk               = TMath::Min( v->Kplus_ProbNNk            , v->Kminus_ProbNNk    );
  v->min_kaon_ProbNNKpi             = TMath::Min( v->Kplus_ProbNNKpi          , v->Kminus_ProbNNKpi  );
  v->min_pion_ProbNNpi              = TMath::Min( v->Piplus_ProbNNpi          , v->Piminus_ProbNNpi  );
  v->min_pion_ProbNNpiK             = TMath::Min( v->Piplus_ProbNNpiK         , v->Piminus_ProbNNpiK );

  // If MC use the corrected ones instead
  if ( v->itype < 0 ) {
    v->Kplus_ProbNNKpi               = v->Kplus_ProbNNk_corr    * ( 1. - v->Kplus_ProbNNpi_corr  );
    v->Kminus_ProbNNKpi              = v->Kminus_ProbNNk_corr   * ( 1. - v->Kminus_ProbNNpi_corr );
    v->Piplus_ProbNNpiK              = v->Piplus_ProbNNpi_corr  * ( 1. - v->Piplus_ProbNNk_corr  );
    v->Piminus_ProbNNpiK             = v->Piminus_ProbNNpi_corr * ( 1. - v->Piminus_ProbNNk_corr );
    v->min_kaon_ProbNNk              = TMath::Min( v->Kplus_ProbNNk_corr     , v->Kminus_ProbNNk_corr    );
    v->min_pion_ProbNNpi             = TMath::Min( v->Piplus_ProbNNpi_corr   , v->Piminus_ProbNNpi_corr  );
    v->min_kaon_ProbNNKpi            = TMath::Min( v->Kplus_ProbNNKpi        , v->Kminus_ProbNNKpi       );
    v->min_pion_ProbNNpiK            = TMath::Min( v->Piplus_ProbNNpiK       , v->Piminus_ProbNNpiK      );
  }


  // set variables and fill datasets
  w->var("B_s0_DTF_B_s0_M")->setVal( v->B_s0_DTF_B_s0_M );
  w->var("bdtoutput")->setVal( v->bdtoutput );
  w->var("min_kaon_ProbNNk")  ->setVal( v->min_kaon_ProbNNk   );
  w->var("min_kaon_ProbNNKpi")->setVal( v->min_kaon_ProbNNKpi );
  w->var("min_pion_ProbNNpi") ->setVal( v->min_pion_ProbNNpi  );
  w->var("min_pion_ProbNNpiK")->setVal( v->min_pion_ProbNNpiK );

  // if no dset found then make it
  if ( dsets.find( v->itype ) == dsets.end() ) {
    dsets[ v->itype ] = new RooDataSet( v->evname, v->evname, *w->set("observables") );
  }
  // otherwise fill it
  else {
    dsets[ v->itype ]->add( *w->set("observables") );
  }

  passedEvents++;
  return true;
}

void Bs2KstKst::AddPIDVarsAndDatasets::Init() {

  w = new RooWorkspace("w","w");

  w->factory("B_s0_DTF_B_s0_M[5000,5800]");
  w->var("B_s0_DTF_B_s0_M")->setBins(100); // IMPORTANT THIS
  w->factory("bdtoutput[-1.,1.]");
  w->factory("min_kaon_ProbNNk[0.,1.]");
  w->factory("min_kaon_ProbNNKpi[0.,1.]");
  w->factory("min_pion_ProbNNpi[0.,1.]");
  w->factory("min_pion_ProbNNpiK[0.,1.]");
  w->factory("itype[-999,999]");

  observables = new RooArgSet();
  observables->add(*w->var("B_s0_DTF_B_s0_M"));
  observables->add(*w->var("bdtoutput"));
  observables->add(*w->var("min_kaon_ProbNNk"));
  observables->add(*w->var("min_kaon_ProbNNKpi"));
  observables->add(*w->var("min_pion_ProbNNpi"));
  observables->add(*w->var("min_pion_ProbNNpiK"));
  observables->add(*w->var("itype"));
  w->defineSet("observables",*observables);
}

void Bs2KstKst::AddPIDVarsAndDatasets::Term() {

  for ( map<int, RooDataSet*>::iterator ds = dsets.begin(); ds != dsets.end(); ds++ ){

    w->import( *(ds->second) );
  }

  system("mkdir -p root/CutOptimisation");
  w->writeToFile("root/CutOptimisation/CutOptimisationDatasets.root");
  delete w;

}
