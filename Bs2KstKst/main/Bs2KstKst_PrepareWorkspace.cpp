#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "TROOT.h"
#include "TColor.h"
#include "TH1D.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TLegend.h"
#include "TRandom3.h"

#include "RooWorkspace.h"
#include "RooCategory.h"
#include "RooSimultaneous.h"
#include "RooDataSet.h"
#include "RooArgSet.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooIpatia2.h"
#include "RooPhysBkg.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"

using namespace std;
using namespace RooFit;

void flagMultCands( TString fname, TString tname ) {

  TFile *inFile = new TFile( fname, "UPDATE" );
  TTree *tree = (TTree*)inFile->Get( tname );

  if ( tree->FindBranch( "pass_multcand" ) ) {
    cout << "pass_multcand branch already exists so I won't add it" << endl;
    delete inFile;
    return;
  }

  ULong64_t  eventNumber;
  int        itype;
  bool       pass_bdtpidmass;
  UInt_t     nCandidate;
  ULong64_t  totCandidates;
  tree->SetBranchAddress(  "eventNumber"                 , &eventNumber                 );
  tree->SetBranchAddress(  "itype"                       , &itype                       );
  tree->SetBranchAddress(  "pass_bdtpidmass"             , &pass_bdtpidmass             );
  tree->SetBranchAddress(  "totCandidates"               , &totCandidates               );
  tree->SetBranchAddress(  "nCandidate"                  , &nCandidate                  );

  map< ULong64_t, ULong64_t > multCandEventNumbers;

  cout << "Finding multiple candidates" << endl;
  // first loop tree and save number of all multiple candidates
  for ( int ev=0; ev<tree->GetEntries(); ev++ ) {
    tree->GetEntry(ev);
    if ( ev%10000==0 ) cout << ev << "/" << tree->GetEntries() << endl;
    if ( itype>0 && pass_bdtpidmass && totCandidates > 1 ) {
      multCandEventNumbers[eventNumber] = totCandidates;
    }
  }

  // now randomly select which one to keep
  map< ULong64_t, UInt_t > eventToKeep;
  TRandom3 rand;
  rand.SetSeed(2016);
  for ( map<ULong64_t,ULong64_t>::iterator it=multCandEventNumbers.begin(); it!=multCandEventNumbers.end(); it++) {
    UInt_t keep = rand.Integer( it->second );
    eventToKeep[it->first] = keep;
  }

  cout << "Adding flag pass_multcand to tree" << endl;
  // then loop again and write the random choice back in
  bool pass_multcand;
  TBranch *bpt = tree->Branch( "pass_multcand", &pass_multcand, "pass_multcand/O" );

  for ( int ev=0; ev<tree->GetEntries(); ev++ ) {
    tree->GetEntry(ev);
    if ( ev%10000==0 ) cout << ev << "/" << tree->GetEntries() << endl;
    pass_multcand = true;
    if ( totCandidates > 1 && eventToKeep[eventNumber] != nCandidate ) pass_multcand = false;
    bpt->Fill();
  }
  tree->Write();
  delete inFile;
}

void defineDatasets( RooWorkspace *w, bool applyMassVeto ) {

  if ( applyMassVeto ) w->factory( "B_s0_DTF_B_s0_M [5200,5600]" );
  else w->factory( "B_s0_DTF_B_s0_M [5000,5800]" );
  w->factory( "eventNumber [0, 10e10]" );
  w->factory( "DataCat[HadronTOS2011,GlobalTIS2011,HadronTOS2012,GlobalTIS2012]" );

  w->var("B_s0_DTF_B_s0_M")->SetTitle("m(K^{+}#pi^{-}K^{-}#pi^{+})");
  w->var("B_s0_DTF_B_s0_M")->setUnit("MeV");

  RooArgSet *observables = new RooArgSet();
  observables->add( *w->var("B_s0_DTF_B_s0_M") );
  observables->add( *w->var("eventNumber") );
  observables->add( *w->cat("DataCat") );
  w->defineSet( "observables", *observables );
  delete observables;

  RooArgSet *categories = new RooArgSet();
  categories->add( *w->cat("DataCat") );
  w->defineSet( "categories", *categories );
  delete categories;

  vector<TString> dsets;
  dsets.push_back( "Data" );
  dsets.push_back( "Data2011" );
  dsets.push_back( "Data2012" );
  dsets.push_back( "Data2011HadronTOS" );
  dsets.push_back( "Data2011GlobalTIS" );
  dsets.push_back( "Data2012HadronTOS" );
  dsets.push_back( "Data2012GlobalTIS" );
  dsets.push_back( "Bs2KstKst"           );
  dsets.push_back( "Bs2KstKst1430"       );
  dsets.push_back( "Bs2Kst1430Kst1430"   );
  dsets.push_back( "Bs2KpiKpiPhaseSpace" );
  dsets.push_back( "Bd2KstKst"           );
  dsets.push_back( "Bd2PhiKst"           );
  dsets.push_back( "Bs2PhiKst"           );
  dsets.push_back( "Bd2RhoKst"           );
  dsets.push_back( "Lb2pKpipi"           );
  dsets.push_back( "Lb2ppipipi"          );

  for (int i=0; i<dsets.size(); i++) {
    RooDataSet *data = new RooDataSet( dsets[i], dsets[i], *w->set("observables") );
    w->import( *data );
    delete data;
  }

  dsets.push_back( "PartReco" );
  dsets.push_back( "Combinatorial" );

  w->factory( "B_s0_DTF_B_s0_M_forPartReco [5000,5250]" );
  RooDataSet *part_reco = new RooDataSet( "PartReco", "PartReco", *w->var("B_s0_DTF_B_s0_M_forPartReco") );
  w->import( *part_reco );
  delete part_reco;

  w->factory(" B_s0_DTF_B_s0_M_forCombinatorial [5600,5800]" );
  RooDataSet *combinatorial = new RooDataSet( "Combinatorial", "Combinatorial", *w->var("B_s0_DTF_B_s0_M_forCombinatorial") );
  w->import( *combinatorial );
  delete combinatorial;
}

void fillDatasets( TString fname, TString tname, TString outfname, bool applyMassVeto=true ) {

  TFile *inFile = TFile::Open( fname );
  TTree *tree = (TTree*)inFile->Get( tname );

  ULong64_t eventNumber;
  TString   *year = 0;
  double    B_s0_DTF_B_s0_M;
  int       itype;
  bool      pass_bdt;
  bool      pass_pid;
  bool      pass_rhokst;
  bool      pass_lambdab;
  bool      pass_massveto_2body;
  bool      pass_massveto_4body;
  bool      pass_multcand;
  bool      pass_bdtpidmass;
  bool      B_s0_L0HadronDecision_TOS;
  bool      B_s0_L0Global_TIS;
  double    B_s0_DTF_KST1_M;
  double    B_s0_DTF_KST2_M;
  // use these to get the part-reco shape (from vetoed stuff)
  double    B_s0_M_KpPimpbPip;
  double    B_s0_M_pPimKmPip;
  double    B_s0_M_KpKmKmPip;
  double    B_s0_M_KpPimKmKp;

  tree->SetBranchAddress(  "eventNumber"                 , &eventNumber                 );
  tree->SetBranchAddress(  "year"                        , &year                        );
  tree->SetBranchAddress(  "B_s0_DTF_B_s0_M"             , &B_s0_DTF_B_s0_M             );
  tree->SetBranchAddress(  "itype"                       , &itype                       );
  tree->SetBranchAddress(  "pass_bdt"                    , &pass_bdt                    );
  tree->SetBranchAddress(  "pass_pid"                    , &pass_pid                    );
  tree->SetBranchAddress(  "pass_rhokst"                 , &pass_rhokst                 );
  tree->SetBranchAddress(  "pass_lambdab"                , &pass_lambdab                );
  tree->SetBranchAddress(  "pass_massveto_2body"         , &pass_massveto_2body         );
  tree->SetBranchAddress(  "pass_massveto_4body"         , &pass_massveto_4body         );
  tree->SetBranchAddress(  "pass_multcand"               , &pass_multcand               );
  tree->SetBranchAddress(  "pass_bdtpidmass"             , &pass_bdtpidmass             );
  tree->SetBranchAddress(  "B_s0_L0HadronDecision_TOS"   , &B_s0_L0HadronDecision_TOS   );
  tree->SetBranchAddress(  "B_s0_L0Global_TIS"           , &B_s0_L0Global_TIS           );
  tree->SetBranchAddress(  "B_s0_DTF_KST1_M"             , &B_s0_DTF_KST1_M             );
  tree->SetBranchAddress(  "B_s0_DTF_KST2_M"             , &B_s0_DTF_KST2_M             );
  //tree->SetBranchAddress(  "B_s0_M_KpPimpbPip"           , &B_s0_M_KpPimpbPip           );
  //tree->SetBranchAddress(  "B_s0_M_pPimKmPip"            , &B_s0_M_pPimKmPip            );
  //tree->SetBranchAddress(  "B_s0_M_KpKmKmPip"            , &B_s0_M_KpKmKmPip            );
  //tree->SetBranchAddress(  "B_s0_M_KpPimKmKp"            , &B_s0_M_KpPimKmKp            );

  RooWorkspace *w = new RooWorkspace("w","w");
  defineDatasets( w, applyMassVeto );

  for (int ev=0; ev<tree->GetEntries(); ev++) {

    tree->GetEntry(ev);

    if ( ev%10000==0 ) cout << ev << " / " << tree->GetEntries() << endl;

    // fill the datasets for the combinatorial and part reco first
    w->var("B_s0_DTF_B_s0_M_forPartReco")->setVal( B_s0_DTF_B_s0_M );
    if ( itype>0 && pass_bdtpidmass && pass_massveto_4body ) {
      w->data("PartReco")->add( *w->var("B_s0_DTF_B_s0_M_forPartReco") );
    }
    w->var("B_s0_DTF_B_s0_M_forCombinatorial")->setVal( B_s0_DTF_B_s0_M );
    if ( itype>0 && pass_bdt && !pass_rhokst && !pass_lambdab && !pass_massveto_2body && B_s0_DTF_B_s0_M >= 5600 && B_s0_DTF_B_s0_M <= 5800 ) {
      w->data("Combinatorial")->add( *w->var("B_s0_DTF_B_s0_M_forCombinatorial") );
    }

    // set workspace values
    w->var("B_s0_DTF_B_s0_M")->setVal( B_s0_DTF_B_s0_M );
    w->var("eventNumber")->setVal( eventNumber );
    if ( *year==TString("2011") ) {
      if ( B_s0_L0HadronDecision_TOS ) w->cat("DataCat")->setLabel("HadronTOS2011");
      else if ( B_s0_L0Global_TIS && !B_s0_L0HadronDecision_TOS ) w->cat("DataCat")->setLabel("GlobalTIS2011");
      else continue;
    }
    else if ( *year==TString("2012") ) {
      if ( B_s0_L0HadronDecision_TOS ) w->cat("DataCat")->setLabel("HadronTOS2012");
      else if ( B_s0_L0Global_TIS && !B_s0_L0HadronDecision_TOS ) w->cat("DataCat")->setLabel("GlobalTIS2012");
      else continue;
    }
    else continue;

    // cut events outside the mass window
    if ( B_s0_DTF_B_s0_M < 5000 || B_s0_DTF_B_s0_M > 5800 ) continue;
    if ( applyMassVeto && (B_s0_DTF_B_s0_M < 5200 || B_s0_DTF_B_s0_M > 5600) ) continue;
    if ( B_s0_DTF_KST1_M < 750  || B_s0_DTF_KST1_M > 1600 ) continue;
    if ( B_s0_DTF_KST2_M < 750  || B_s0_DTF_KST2_M > 1600 ) continue;

    // for the most case we put the bdt and pid requirement in
    // for low stats MC samples we don't use it
    //
    // NO REQUIREMENT:
    // Lb2pKpipi MC
    if ( itype == -78 || itype == -88 ) {
      w->data("Lb2pKpipi")->add( *w->set("observables") );
    }
    // Lb2ppipipi MC
    else if ( itype == -79 || itype == -89 ) {
        w->data("Lb2ppipipi")->add( *w->set("observables") );
    }
    // Bd2PhiKst MC
    else if ( itype == -75 || itype == -85 ) {
      w->data("Bd2PhiKst")->add( *w->set("observables") );
    }
    // Bs2PhiKst MC
    else if ( itype == -76 || itype == -86 ) {
      w->data("Bs2PhiKst")->add( *w->set("observables") );
    }
    // Bd2RhoKst MC
    else if ( itype == -77 || itype == -87 ) {
      w->data("Bd2RhoKst")->add( *w->set("observables") );
    }

    // FROM HERE BDT, PID AND MASS VETO REQUIREMENTS (insert pass_all here?)
    if ( pass_bdtpidmass && pass_multcand ) {
      if ( applyMassVeto && !pass_massveto_4body ) continue;
      // Data 2011
      if ( itype == 71 ) {
        w->data("Data")->add( *w->set("observables") );
        w->data("Data2011")->add( *w->set("observables") );
        if ( B_s0_L0HadronDecision_TOS ) {
          w->data("Data2011HadronTOS")->add( *w->set("observables") );
        }
        if ( B_s0_L0Global_TIS && !B_s0_L0HadronDecision_TOS ) {
          w->data("Data2011GlobalTIS")->add( *w->set("observables") );
        }
      }
      // Data 2012
      else if ( itype == 81 ) {
        w->data("Data")->add( *w->set("observables") );
        w->data("Data2012")->add( *w->set("observables") );
        if ( B_s0_L0HadronDecision_TOS ) {
          w->data("Data2012HadronTOS")->add( *w->set("observables") );
        }
        if ( B_s0_L0Global_TIS && !B_s0_L0HadronDecision_TOS ) {
          w->data("Data2012GlobalTIS")->add( *w->set("observables") );
        }
      }
      // Bs2KstKst MC
      else if ( itype == -70 || itype == -80 ) {
        w->data("Bs2KstKst")->add( *w->set("observables") );
      }
      // Bs2KstKst1430 MC
      else if ( itype == -71 || itype == -81 ) {
        w->data("Bs2KstKst1430")->add( *w->set("observables") );
      }
      // Bs2Kst1430Kst1430 MC
      else if ( itype == -72 || itype == -82 ) {
        w->data("Bs2Kst1430Kst1430")->add( *w->set("observables") );
      }
      // Bs2KPiKPi PhaseSpace
      else if ( itype == -73 || itype == -83 ) {
        w->data("Bs2KpiKpiPhaseSpace")->add( *w->set("observables") );
      }
      // Bd2KstKst MC
      else if ( itype == -74 || itype == -84 ) {
        w->data("Bd2KstKst")->add( *w->set("observables") );
      }
    }
  }

  // combined data
  map<string,RooDataSet*> dsetMap;
  dsetMap[ "HadronTOS2011" ] = (RooDataSet*)w->data("Data2011HadronTOS") ;
  dsetMap[ "GlobalTIS2011" ] = (RooDataSet*)w->data("Data2011GlobalTIS") ;
  dsetMap[ "HadronTOS2012" ] = (RooDataSet*)w->data("Data2012HadronTOS") ;
  dsetMap[ "GlobalTIS2012" ] = (RooDataSet*)w->data("Data2012GlobalTIS") ;

  RooDataSet *DataComb = new RooDataSet( "DataCombined", "DataCombined", *w->set("observables"), Index(*w->cat("DataCat")), Import(dsetMap) );
  w->import(*DataComb);
  delete DataComb;

  inFile->Close();
  delete inFile;
  delete year;

  w->writeToFile( outfname );

}

void drawData( RooWorkspace *w ) {

  w->var("B_s0_DTF_B_s0_M")->setBins(100);

  TH1 *Data              = w->data("Data")->createHistogram( "Data", *w->var("B_s0_DTF_B_s0_M") );
  Data->SetLineWidth(2);
  Data->SetLineColor(kBlack);

  TH1 *Data2011          = w->data("Data2011")->createHistogram( "Data2011", *w->var("B_s0_DTF_B_s0_M") );
  Data2011->SetLineWidth(4);
  Data2011->SetLineColor(kBlue);
  Data2011->SetFillColor(0);
  Data2011->SetFillStyle(0);

  TH1 *Data2012          = w->data("Data2012")->createHistogram( "Data2012", *w->var("B_s0_DTF_B_s0_M") );
  Data2012->SetLineWidth(4);
  Data2012->SetLineColor(kRed);
  Data2012->SetFillColor(0);
  Data2012->SetFillStyle(0);

  int col1 = TColor::GetColor( "#b2e2e2" );
  int col2 = TColor::GetColor( "#66c2a4" );
  int col3 = TColor::GetColor( "#2ca25f" );
  int col4 = TColor::GetColor( "#006d2c" );

  TH1 *Data2011HadronTOS = w->data("Data2011HadronTOS")->createHistogram( "Data2011HadronTOS", *w->var("B_s0_DTF_B_s0_M") );
  Data2011HadronTOS->SetLineColor(col1);
  Data2011HadronTOS->SetFillColor(col1);

  TH1 *Data2011GlobalTIS = w->data("Data2011GlobalTIS")->createHistogram( "Data2011GlobalTIS", *w->var("B_s0_DTF_B_s0_M") );
  Data2011GlobalTIS->SetLineColor(col2);
  Data2011GlobalTIS->SetFillColor(col2);

  TH1 *Data2012HadronTOS = w->data("Data2012HadronTOS")->createHistogram( "Data2012HadronTOS", *w->var("B_s0_DTF_B_s0_M") );
  Data2012HadronTOS->SetLineColor(col3);
  Data2012HadronTOS->SetFillColor(col3);

  TH1 *Data2012GlobalTIS = w->data("Data2012GlobalTIS")->createHistogram( "Data2012GlobalTIS", *w->var("B_s0_DTF_B_s0_M") );
  Data2012GlobalTIS->SetLineColor(col4);
  Data2012GlobalTIS->SetFillColor(col4);


  THStack *hsData = new THStack("DataHs","");
  hsData->Add( Data2011HadronTOS );
  hsData->Add( Data2011GlobalTIS );
  hsData->Add( Data2012HadronTOS );
  hsData->Add( Data2012GlobalTIS );

  TH1 *DataYears = (TH1*)Data2011->Clone("DataYears");
  DataYears->Add( Data2012 );
  DataYears->SetLineColor(kRed);
  DataYears->SetLineWidth(4);

  TLegend *leg = new TLegend(0.6,0.4,0.89,0.89);
  leg->SetFillColor(0);
  leg->AddEntry( Data2011HadronTOS, "2011 Hadron TOS", "F" );
  leg->AddEntry( Data2011GlobalTIS, "2011 Global TIS", "F" );
  leg->AddEntry( Data2012HadronTOS, "2012 Hadron TOS", "F" );
  leg->AddEntry( Data2012GlobalTIS, "2012 Global TIS", "F" );
  leg->AddEntry( Data2011, "2011" , "L" );
  leg->AddEntry( DataYears, "2012" , "L" );
  leg->AddEntry( Data, "All", "L" );

  TCanvas *c = new TCanvas();
  hsData->Draw("HISTF");
  DataYears->Draw("HISTsame");
  Data2011->Draw("HISTsame");
  Data->Draw("HISTsame");
  leg->Draw("same");

  hsData->GetHistogram()->GetXaxis()->SetTitle("m(K^{+}#pi^{-}K^{-}#pi^{+}) [MeV]");
  hsData->GetHistogram()->GetYaxis()->SetTitle("Events / ( 8 MeV )");
  hsData->GetHistogram()->GetXaxis()->SetTitleOffset(0.8);
  hsData->GetHistogram()->GetYaxis()->SetTitleOffset(0.9);
  hsData->GetHistogram()->GetXaxis()->SetTitleSize(0.08);
  hsData->GetHistogram()->GetYaxis()->SetTitleSize(0.08);

  c->Update();
  c->Modified();
  c->Print("plots/DataSets.pdf");
}

void defineParamSet( RooWorkspace *w,  TString pdf ) {

  if ( ! w->pdf(pdf) ) {
    cout << "ERROR -- No valid pdf named " << pdf << " in workspace" << endl;
    exit(1);
  }
  RooArgSet *argset = w->pdf(pdf)->getParameters(*w->set("observables"));
  w->defineSet(Form("%s_params",pdf.Data()),*argset);
  delete argset;
}

/// ----------------------------------------------------------------- ///
/// ----------------------------------------------------------------- ///
/// ---------------------------- PDFS ------------------------------- ///
/// ----------------------------------------------------------------- ///
/// ----------------------------------------------------------------- ///

// Bs2KstKst MC
void makeBs2KstKstPdf( RooWorkspace *w ) {

  w->factory("bs2kstkst_l[-5,-20,-1]" );
  w->factory("bs2kstkst_zeta[0.]" );
  w->factory("bs2kstkst_fb[0.]" );
  w->factory("bs2kstkst_sigma[15,10,100]" );
  w->factory("bs2kstkst_mu[5350,5400]" );
  w->factory("bs2kstkst_a[2.5,0,10]" );
  w->factory("bs2kstkst_n[2.5,0,10]" );
  w->factory("bs2kstkst_a2[2.5,0,10]" );
  w->factory("bs2kstkst_n2[2.5,0,10]" );

  w->var("bs2kstkst_zeta")->setConstant(true);
  w->var("bs2kstkst_fb")->setConstant(true);

  RooIpatia2 *pdf = new RooIpatia2("bs2kstkst_mc_pdf","bs2kstkst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bs2kstkst_l"),*w->var("bs2kstkst_zeta"),*w->var("bs2kstkst_fb"),*w->var("bs2kstkst_sigma"),*w->var("bs2kstkst_mu"),*w->var("bs2kstkst_a"),*w->var("bs2kstkst_n"),*w->var("bs2kstkst_a2"),*w->var("bs2kstkst_n2"));

  w->import(*pdf);
  delete pdf;

  defineParamSet( w, "bs2kstkst_mc_pdf" );

}

// Bd2KstKst MC
void makeBd2KstKstPdf( RooWorkspace *w ) {

  w->factory("bd2kstkst_l[-5,-20,-1]" );
  w->factory("bd2kstkst_zeta[0.]" );
  w->factory("bd2kstkst_fb[0.]" );
  w->factory("bd2kstkst_sigma[15,10,100]" );
  w->factory("bd2kstkst_mu[5250,5300]" );
  w->factory("bd2kstkst_a[2.5,0,10]" );
  w->factory("bd2kstkst_n[2.5,0,10]" );
  w->factory("bd2kstkst_a2[2.5,0,10]" );
  w->factory("bd2kstkst_n2[2.5,0,10]" );

  w->var("bd2kstkst_zeta")->setConstant(true);
  w->var("bd2kstkst_fb")->setConstant(true);

  RooIpatia2 *pdf = new RooIpatia2("bd2kstkst_mc_pdf","bd2kstkst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bd2kstkst_l"),*w->var("bd2kstkst_zeta"),*w->var("bd2kstkst_fb"),*w->var("bd2kstkst_sigma"),*w->var("bd2kstkst_mu"),*w->var("bd2kstkst_a"),*w->var("bd2kstkst_n"),*w->var("bd2kstkst_a2"),*w->var("bd2kstkst_n2"));

  w->import(*pdf);
  delete pdf;

  defineParamSet( w, "bd2kstkst_mc_pdf" );

}

// Bd2PhiKst MC
void makeBd2PhiKstPdf( RooWorkspace *w ) {

  w->factory("bd2phikst_l[-5,-20,-1]" );
  w->factory("bd2phikst_zeta[0.]" );
  w->factory("bd2phikst_fb[0.]" );
  w->factory("bd2phikst_sigma[18,5,25]" );
  w->factory("bd2phikst_mu[5200,5270]" );
  w->factory("bd2phikst_a[2.5,0,10]" );
  w->factory("bd2phikst_n[2.5,0,10]" );
  w->factory("bd2phikst_a2[2.5,0,10]" );
  w->factory("bd2phikst_n2[2.5,0,10]" );

  w->var("bd2phikst_zeta")->setConstant(true);
  w->var("bd2phikst_fb")->setConstant(true);

  RooIpatia2 *pdf = new RooIpatia2("bd2phikst_mc_pdf","bd2phikst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bd2phikst_l"),*w->var("bd2phikst_zeta"),*w->var("bd2phikst_fb"),*w->var("bd2phikst_sigma"),*w->var("bd2phikst_mu"),*w->var("bd2phikst_a"),*w->var("bd2phikst_n"),*w->var("bd2phikst_a2"),*w->var("bd2phikst_n2"));

  w->import(*pdf);
  delete pdf;

  defineParamSet( w, "bd2phikst_mc_pdf" );

}

// Bs2PhiKst MC
void makeBs2PhiKstPdf( RooWorkspace *w ) {

  w->factory("bs2phikst_l[-5,-20,-1]" );
  w->factory("bs2phikst_zeta[0.]" );
  w->factory("bs2phikst_fb[0.]" );
  w->factory("bs2phikst_sigma[15,10,100]" );
  w->factory("bs2phikst_mu[5300,5400]" );
  w->factory("bs2phikst_a[2.5,0,10]" );
  w->factory("bs2phikst_n[2.5,0,10]" );
  w->factory("bs2phikst_a2[2.5,0,10]" );
  w->factory("bs2phikst_n2[2.5,0,10]" );

  w->var("bs2phikst_zeta")->setConstant(true);
  w->var("bs2phikst_fb")->setConstant(true);

  RooIpatia2 *pdf = new RooIpatia2("bs2phikst_mc_pdf","bs2phikst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bs2phikst_l"),*w->var("bs2phikst_zeta"),*w->var("bs2phikst_fb"),*w->var("bs2phikst_sigma"),*w->var("bs2phikst_mu"),*w->var("bs2phikst_a"),*w->var("bs2phikst_n"),*w->var("bs2phikst_a2"),*w->var("bs2phikst_n2"));

  w->import(*pdf);
  delete pdf;

  defineParamSet( w, "bs2phikst_mc_pdf" );

}

// Bd2RhoKst MC
void makeBd2RhoKstPdf( RooWorkspace *w ) {

  w->factory("bd2rhokst_l[-5,-20,-1]" );
  w->factory("bd2rhokst_zeta[0.]" );
  w->factory("bd2rhokst_fb[0.]" );
  w->factory("bd2rhokst_sigma[15,10,100]" );
  w->factory("bd2rhokst_mu[5300,5400]" );
  w->factory("bd2rhokst_a[2.5,0,10]" );
  w->factory("bd2rhokst_n[2.5,0,10]" );
  w->factory("bd2rhokst_a2[2.5,0,10]" );
  w->factory("bd2rhokst_n2[2.5,0,10]" );

  w->var("bd2rhokst_zeta")->setConstant(true);
  w->var("bd2rhokst_fb")->setConstant(true);

  RooIpatia2 *pdf = new RooIpatia2("bd2rhokst_mc_pdf","bd2rhokst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bd2rhokst_l"),*w->var("bd2rhokst_zeta"),*w->var("bd2rhokst_fb"),*w->var("bd2rhokst_sigma"),*w->var("bd2rhokst_mu"),*w->var("bd2rhokst_a"),*w->var("bd2rhokst_n"),*w->var("bd2rhokst_a2"),*w->var("bd2rhokst_n2"));

  w->import(*pdf);
  delete pdf;

  defineParamSet( w, "bd2rhokst_mc_pdf" );
}

// Lb2pKpipi MC
void makeLb2pKpipiPdf( RooWorkspace *w ) {
  w->factory("lb2pkpipi_mean[5450,5550]");
  w->factory("CBShape::lb2pkpipi_mc_cb1( B_s0_DTF_B_s0_M, lb2pkpipi_mean, lb2pkpipi_sigma1[27,1,40], lb2pkpipi_alpha1[0.04,0.01,5.], lb2pkpipi_n1[3.])");
  w->factory("CBShape::lb2pkpipi_mc_cb2( B_s0_DTF_B_s0_M, lb2pkpipi_mean, lb2pkpipi_sigma2[117,1,200], lb2pkpipi_alpha2[-0.04,-10.,-0.01], lb2pkpipi_n2[3.])");
  w->factory("SUM::lb2pkpipi_mc_pdf( lb2pkpipi_f1[0.4,0.,1.]*lb2pkpipi_mc_cb1, lb2pkpipi_mc_cb2 )");
  defineParamSet( w, "lb2pkpipi_mc_pdf");
}

// Lb2ppipipi MC
void makeLb2ppipipiPdf( RooWorkspace *w ) {
  w->factory("lb2ppipipi_mean[5450,5600]");
  w->factory("CBShape::lb2ppipipi_mc_pdf( B_s0_DTF_B_s0_M, lb2ppipipi_mean, lb2ppipipi_sigma[10,1,200], lb2ppipipi_alpha[0.04,0.01,10.], lb2ppipipi_n[3.])");
  defineParamSet( w, "lb2ppipipi_mc_pdf");
}

// Part Reco PDFs
void makePartRecoPdf( RooWorkspace *w ) {
  // one in each cat
  //RooCategory *cat = (RooCategory*)w->cat("DataCat");
  //for ( int i=0; i < cat->numTypes(); i++ ) {
    //cat->setIndex(i);
    //w->factory( Form("ArgusBG::part_reco_pdf_%s( B_s0_DTF_B_s0_M, part_reco_m0_%s[5226], part_reco_c_%s[-10.0,-50.,-10.], part_reco_p_%s[0.4,0.,1.] )",cat->getLabel(),cat->getLabel(),cat->getLabel(),cat->getLabel()) );
    //defineParamSet( w, Form("part_reco_pdf_%s",cat->getLabel()) );
  //}
  // same shape for each cat
  w->factory( "ArgusBG::part_reco_pdf( B_s0_DTF_B_s0_M, part_reco_m0[5226], part_reco_c[-10.0,-50.,-2.], part_reco_p[0.4,0.,1.] )");
  defineParamSet( w, "part_reco_pdf" );
  // and one to constrain it before hand
  w->factory( "ArgusBG::part_reco_self_pdf( B_s0_DTF_B_s0_M_forPartReco, part_reco_m0, part_reco_c, part_reco_p )");
}

// Combinatorial PDFs
void makeCombinatorialPdf( RooWorkspace *w ) {
  // one in each cat
  //RooCategory *cat = (RooCategory*)w->cat("DataCat");
  //for ( int i=0; i < cat->numTypes(); i++ ) {
    //cat->setIndex(i);
    //w->factory( Form("Exponential::bkg_pdf_%s( B_s0_DTF_B_s0_M, bkg_exp_p1_%s[-0.002,-0.005,0.] )", cat->getLabel(), cat->getLabel() ) );
    //defineParamSet( w, Form("bkg_pdf_%s",cat->getLabel()) );
  //}
  // same shape in each cat
  w->factory( "Exponential::bkg_pdf( B_s0_DTF_B_s0_M, bkg_exp_p1[-0.002,-0.005,0.] )" );
  defineParamSet( w, "bkg_pdf");
  // and one to constrain it before hand
  w->factory( "Exponential::bkg_self_pdf( B_s0_DTF_B_s0_M_forCombinatorial, bkg_exp_p1 )" );
  // and the shape which is both part-reco and combinatorial together
  w->factory( "Exponential::bkg_pr_pdf( B_s0_DTF_B_s0_M_forPartReco, bkg_exp_p1 )" );
  w->factory( "SUM::part_reco_bkg_self_pdf( part_reco_self_frac[0.8,0.,1.]*part_reco_self_pdf, bkg_pr_pdf )" );
}

// total PDF
void makeTotalPdf( RooWorkspace *w ) {
  
  // setup some yield ratios (as want these constant across categories)
  // 1.) constrain the bs->phikst / bd->phikst ratio (this is well known)
  // the BR ratio is (0.113 +/- 0.0287) and also include fs/fd = (0.259 +/- 0.015): TOTAL = (0.029 +/- 0.008)
  w->factory( "yield_ratio_bs2phikst_o_bd2phikst[0.029,0.,0.5]" );
  w->factory( "Gaussian::yield_ratio_bs2phikst_o_bd2phikst_constraint( yield_ratio_bs2phikst_o_bd2phikst, 0.029, 0.008 )" );
  // 2a.) have a ratio between rhokst and phikst component but no constraint
  // 2b.) constrain the bd->rhokst / bd->phikst ratio (0.39 +/- 0.13) and also include the relative efficiencies (0.24 +/- 0.02): TOTAL = (0.094,0.032)
  w->factory("yield_ratio_bd2rhokst_o_bd2phikst[0.094,0.,0.6]" );
  //w->factory( "Gaussian::yield_ratio_bd2rhokst_o_bd2phikst_constraint( yield_ratio_bd2rhokst_o_bd2phikst, 0.39, 0.13 )" );
  //w->factory( "Gaussian::yield_ratio_bd2rhokst_o_bd2phikst_constraint( yield_ratio_bd2rhokst_o_bd2phikst, 0.094, 0.032 )" );
  // 3.) have a ratio between lambda and phikst component but no constraint
  w->factory("yield_ratio_lb2pkpipi_o_bd2phikst[0.9,0.,2.]" );

  // make a yield for each category
  RooCategory *cat = (RooCategory*)w->cat("DataCat");
  for ( int i=0; i < cat->numTypes(); i++ ) {
    cat->setIndex(i);
    w->factory( Form("bkg_y_%s[50,2000]",       cat->getLabel()));  // bkg floats in each category
    w->factory( Form("part_reco_y_%s[50,1000]", cat->getLabel()));  // part-reco floats in each category
    w->factory( Form("bs2kstkst_y_%s[500,5000]", cat->getLabel())); // bs floats in each category
    w->factory( Form("bd2kstkst_y_%s[10,1000]", cat->getLabel()));  // bd floats in each category
    w->factory( Form("bd2phikst_y_%s[1,400]", cat->getLabel()));    // bd2phi floats in each category
    // do the rest as ratios
    w->factory( Form("prod::bs2phikst_y_%s( yield_ratio_bs2phikst_o_bd2phikst, bd2phikst_y_%s )", cat->getLabel(), cat->getLabel()) );
    w->factory( Form("prod::bd2rhokst_y_%s( yield_ratio_bd2rhokst_o_bd2phikst, bd2phikst_y_%s )", cat->getLabel(), cat->getLabel()) );
    w->factory( Form("prod::lb2pkpipi_y_%s( yield_ratio_lb2pkpipi_o_bd2phikst, bd2phikst_y_%s )", cat->getLabel(), cat->getLabel()) );
  }

  // construct the pdf for each category
  for ( int i=0; i < cat->numTypes(); i++ ) {
    cat->setIndex(i);

    RooArgList *yields = new RooArgList();
    yields->add(*w->var( Form("bkg_y_%s"      , cat->getLabel()) ));
    yields->add(*w->var( Form("part_reco_y_%s", cat->getLabel()) ));
    yields->add(*w->var( Form("bs2kstkst_y_%s", cat->getLabel()) ));
    yields->add(*w->var( Form("bd2kstkst_y_%s", cat->getLabel()) ));
    yields->add(*w->var( Form("bd2phikst_y_%s", cat->getLabel()) ));
    yields->add(*w->function( Form("bs2phikst_y_%s", cat->getLabel()) ));
    yields->add(*w->function( Form("bd2rhokst_y_%s", cat->getLabel()) ));
    yields->add(*w->function( Form("lb2pkpipi_y_%s", cat->getLabel()) ));

    RooArgList *pdfs   = new RooArgList();
    pdfs->add(*w->pdf("bkg_pdf" ));
    pdfs->add(*w->pdf("part_reco_pdf" ));
    pdfs->add(*w->pdf("bs2kstkst_mc_pdf" ));
    pdfs->add(*w->pdf("bd2kstkst_mc_pdf" ));
    pdfs->add(*w->pdf("bd2phikst_mc_pdf" ));
    pdfs->add(*w->pdf("bs2phikst_mc_pdf" ));
    pdfs->add(*w->pdf("bd2rhokst_mc_pdf" ));
    pdfs->add(*w->pdf("lb2pkpipi_mc_pdf" ));

    RooAddPdf *pdf = new RooAddPdf( Form("pdf_%s",cat->getLabel()), "pdf" , *pdfs, *yields);
    w->import(*pdf);
    delete pdf;

    // then make the constrained pdf
    RooArgSet *prodpdfs = new RooArgSet();
    prodpdfs->add( *w->pdf(Form("pdf_%s",cat->getLabel())) );
    prodpdfs->add( *w->pdf("yield_ratio_bs2phikst_o_bd2phikst_constraint") );
    //prodpdfs->add( *w->pdf("yield_ratio_bd2rhokst_o_bd2phikst_constraint") );
    RooProdPdf *cpdf = new RooProdPdf( Form("constrained_pdf_%s",cat->getLabel()), "constrained_pdf", *prodpdfs );
    w->import(*cpdf);
    delete cpdf;

    w->defineSet(Form("pdf_%s_yield_params",cat->getLabel()), *yields);
    w->defineSet(Form("constrained_pdf_%s_yield_params",cat->getLabel()), *yields);
  }

  // now make simultaneous pdf
  RooSimultaneous *cpdf = new RooSimultaneous( "constrained_pdf", "constrained_pdf", *w->cat("DataCat") );
  RooSimultaneous *pdf = new RooSimultaneous( "pdf", "pdf", *w->cat("DataCat") );
  for ( int i=0; i < cat->numTypes(); i++ ) {
    cat->setIndex(i);
    cpdf->addPdf( *w->pdf( Form("constrained_pdf_%s", cat->getLabel() )), cat->getLabel() );
    pdf->addPdf( *w->pdf( Form("pdf_%s", cat->getLabel() )), cat->getLabel() );
  }
  w->import(*cpdf);
  w->import(*pdf);

  delete pdf;
  delete cpdf;

}

// PDFS
void constructPdfs( RooWorkspace *w, TString outfname ) {
  makeBs2KstKstPdf( w );
  makeBd2KstKstPdf( w );
  makeBd2PhiKstPdf( w );
  makeBs2PhiKstPdf( w );
  makeBd2RhoKstPdf( w );
  makeLb2pKpipiPdf( w );
  makeLb2ppipipiPdf( w );
  makePartRecoPdf( w );
  makeCombinatorialPdf( w );
  makeTotalPdf( w );
  w->writeToFile( outfname );
}

int main() {

  bool applyMassVeto = false;
  
  gROOT->ProcessLine(".x ~/Scratch/lhcb/lhcbStyle.C");
  gROOT->ProcessLine(".x ~/lhcbStyle.C");
  system("mkdir -p root/MassFit");

  TString wsFileName   = "root/MassFit/MassFitWorkspace";
  TString pdfsFileName =  wsFileName + "WithPDFs";
  if ( applyMassVeto ) {
    wsFileName   += "_withmassveto";
    pdfsFileName += "_withmassveto";
  }
  wsFileName   += ".root";
  pdfsFileName += ".root";

  flagMultCands( "root/AnalysisOut.root", "AnalysisTree" );
  fillDatasets( "root/AnalysisOut.root", "AnalysisTree", wsFileName, applyMassVeto );

  TFile *tf = TFile::Open(wsFileName);
  RooWorkspace *w = (RooWorkspace*)tf->Get("w");
  drawData( w );

  constructPdfs( w, pdfsFileName );

  w->Print("v");
  return 0;
}
