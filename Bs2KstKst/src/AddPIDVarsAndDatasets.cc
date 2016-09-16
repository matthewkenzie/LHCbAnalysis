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
  v->Piplus_ProbNNKpi               = v->Piplus_ProbNNk   * ( 1.-v->Piplus_ProbNNpi );
  v->Piminus_ProbNNKpi              = v->Piminus_ProbNNk  * ( 1.-v->Piminus_ProbNNpi);
  v->Piplus_ProbNNpiKp              = v->Piplus_ProbNNpi  * ( 1.-v->Piplus_ProbNNk )  * ( 1.-v->Piplus_ProbNNp );
  v->Piminus_ProbNNpiKp             = v->Piminus_ProbNNpi * ( 1.-v->Piminus_ProbNNk ) * ( 1.-v->Piminus_ProbNNp );
  v->Piplus_MC12TuneV2_ProbNNpiKp   = v->Piplus_MC12TuneV2_ProbNNpi  * ( 1.-v->Piplus_MC12TuneV2_ProbNNk )  * ( 1.-v->Piplus_MC12TuneV2_ProbNNp );
  v->Piminus_MC12TuneV2_ProbNNpiKp  = v->Piminus_MC12TuneV2_ProbNNpi * ( 1.-v->Piminus_MC12TuneV2_ProbNNk ) * ( 1.-v->Piminus_MC12TuneV2_ProbNNp );
  v->Piplus_MC12TuneV3_ProbNNpiKp   = v->Piplus_MC12TuneV3_ProbNNpi  * ( 1.-v->Piplus_MC12TuneV3_ProbNNk )  * ( 1.-v->Piplus_MC12TuneV3_ProbNNp );
  v->Piminus_MC12TuneV3_ProbNNpiKp  = v->Piminus_MC12TuneV3_ProbNNpi * ( 1.-v->Piminus_MC12TuneV3_ProbNNk ) * ( 1.-v->Piminus_MC12TuneV3_ProbNNp );

  v->Kplus_ProbNNKpi                 = v->Kplus_ProbNNk  * ( 1. - v->Kplus_ProbNNpi  );
  v->Kminus_ProbNNKpi                = v->Kminus_ProbNNk * ( 1. - v->Kminus_ProbNNpi );
  v->Kplus_MC12TuneV2_ProbNNKpi      = v->Kplus_MC12TuneV2_ProbNNk  * ( 1. - v->Kplus_MC12TuneV2_ProbNNpi  );
  v->Kminus_MC12TuneV2_ProbNNKpi     = v->Kminus_MC12TuneV2_ProbNNk * ( 1. - v->Kminus_MC12TuneV2_ProbNNpi );
  v->Kplus_MC12TuneV3_ProbNNKpi      = v->Kplus_MC12TuneV3_ProbNNk  * ( 1. - v->Kplus_MC12TuneV3_ProbNNpi  );
  v->Kminus_MC12TuneV3_ProbNNKpi     = v->Kminus_MC12TuneV3_ProbNNk * ( 1. - v->Kminus_MC12TuneV3_ProbNNpi );

  v->min_kaon_ProbNNk               = TMath::Min( v->Kplus_ProbNNk            , v->Kminus_ProbNNk );
  v->min_kaon_ProbNNKpi             = TMath::Min( v->Kplus_ProbNNKpi          , v->Kminus_ProbNNKpi );
  v->max_kaon_ProbNNKpi             = TMath::Max( v->Kplus_ProbNNKpi          , v->Kminus_ProbNNKpi );
  v->min_kaon_MC12TuneV2_ProbNNk    = TMath::Min( v->Kplus_MC12TuneV2_ProbNNk , v->Kminus_MC12TuneV2_ProbNNk );
  v->min_kaon_MC12TuneV3_ProbNNk    = TMath::Min( v->Kplus_MC12TuneV3_ProbNNk , v->Kminus_MC12TuneV3_ProbNNk );

  v->min_pion_ProbNNKpi             = TMath::Min( v->Piplus_ProbNNKpi             , v->Piminus_ProbNNKpi  );
  v->max_pion_ProbNNKpi             = TMath::Max( v->Piplus_ProbNNKpi             , v->Piminus_ProbNNKpi  );
  v->min_pion_ProbNNpiKp            = TMath::Min( v->Piplus_ProbNNpiKp            , v->Piminus_ProbNNpiKp );
  v->min_pion_MC12TuneV2_ProbNNpiKp = TMath::Min( v->Piplus_MC12TuneV2_ProbNNpiKp , v->Piminus_MC12TuneV2_ProbNNpiKp );
  v->min_pion_MC12TuneV3_ProbNNpiKp = TMath::Min( v->Piplus_MC12TuneV3_ProbNNpiKp , v->Piminus_MC12TuneV3_ProbNNpiKp );

  // corrected vars
  if ( v->itype<0 ) {
    v->Kplus_V3ProbNNKpi_corr       = v->Kplus_V3ProbNNk_corr   * ( 1. - v->Kplus_V3ProbNNpi_corr   );
    v->Kminus_V3ProbNNKpi_corr      = v->Kminus_V3ProbNNk_corr  * ( 1. - v->Kminus_V3ProbNNpi_corr  );
    v->Piplus_V3ProbNNKpi_corr      = v->Piplus_V3ProbNNk_corr  * ( 1. - v->Piplus_V3ProbNNpi_corr  );
    v->Piminus_V3ProbNNKpi_corr     = v->Piminus_V3ProbNNk_corr * ( 1. - v->Piminus_V3ProbNNpi_corr );

    v->min_kaon_V3ProbNNKpi_corr    = TMath::Min( v->Kplus_V3ProbNNKpi_corr, v->Kminus_V3ProbNNKpi_corr );
    v->max_kaon_V3ProbNNKpi_corr    = TMath::Max( v->Kplus_V3ProbNNKpi_corr, v->Kminus_V3ProbNNKpi_corr );
    v->min_pion_V3ProbNNKpi_corr    = TMath::Min( v->Piplus_V3ProbNNKpi_corr, v->Piminus_V3ProbNNKpi_corr );
    v->max_pion_V3ProbNNKpi_corr    = TMath::Max( v->Piplus_V3ProbNNKpi_corr, v->Piminus_V3ProbNNKpi_corr );
  }
  else {
    // just makes it easier later if they have the same name
    v->Kplus_V3ProbNNKpi_corr       = v->Kplus_ProbNNKpi;
    v->Kminus_V3ProbNNKpi_corr      = v->Kminus_ProbNNKpi;
    v->Piplus_V3ProbNNKpi_corr      = v->Piplus_ProbNNKpi;
    v->Piminus_V3ProbNNKpi_corr     = v->Piminus_ProbNNKpi;

    v->min_kaon_V3ProbNNKpi_corr    = v->min_kaon_ProbNNKpi;
    v->max_kaon_V3ProbNNKpi_corr    = v->max_kaon_ProbNNKpi;
    v->min_pion_V3ProbNNKpi_corr    = v->min_pion_ProbNNKpi;
    v->max_pion_V3ProbNNKpi_corr    = v->max_pion_ProbNNKpi;
  }

  // set variables and fill datasets
  w->var("B_s0_DTF_B_s0_M")->setVal( v->B_s0_DTF_B_s0_M );
  w->var("bdtoutput")->setVal( v->bdtoutput );
  w->var("min_kaon_ProbNNk")->setVal( v->min_kaon_ProbNNk );
  w->var("min_kaon_ProbNNKpi")->setVal( v->min_kaon_ProbNNKpi );
  w->var("min_pion_ProbNNpiKp")->setVal( v->min_pion_ProbNNpiKp );

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
  w->factory("min_pion_ProbNNpiKp[0.,1.]");
  w->factory("min_kaon_ProbNNk[0.,1.]");
  w->factory("min_kaon_ProbNNKpi[0.,1.]");
  w->factory("itype[-999,999]");

  observables = new RooArgSet();
  observables->add(*w->var("B_s0_DTF_B_s0_M"));
  observables->add(*w->var("bdtoutput"));
  observables->add(*w->var("min_pion_ProbNNpiKp"));
  observables->add(*w->var("min_kaon_ProbNNk"));
  observables->add(*w->var("min_kaon_ProbNNKpi"));
  observables->add(*w->var("itype"));
  w->defineSet("observables",*observables);
}

void Bs2KstKst::AddPIDVarsAndDatasets::Term() {

  for ( map<int, RooDataSet*>::iterator ds = dsets.begin(); ds != dsets.end(); ds++ ){

    w->import( *(ds->second) );
  }

  w->writeToFile("root/MultiDimCutDatasets.root");
  delete w;

}
