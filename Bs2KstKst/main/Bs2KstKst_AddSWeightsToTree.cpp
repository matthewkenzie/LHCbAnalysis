#include <iostream>

#include "TROOT.h"
#include "TStyle.h"
#include "TColor.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"

#include "RooWorkspace.h"
#include "RooDataSet.h"
#include "RooArgSet.h"
#include "RooRealVar.h"

#include "boost/lexical_cast.hpp"

using namespace std;
using namespace RooFit;

map< ULong64_t, pair<double,double> > getMapFromDataSet( RooDataSet *data ) {

  map< ULong64_t, pair<double,double> > resMap;

  for ( int ev=0; ev<data->numEntries(); ev++ ) {

    RooArgSet *obs = (RooArgSet*)data->get(ev);

    double      mass          = obs->getRealValue("B_s0_DTF_B_s0_M");
    ULong64_t   eventNumber   = boost::lexical_cast<ULong64_t>( obs->getRealValue("eventNumber") );
    double      weight        = data->weight();

    assert( resMap.find( eventNumber )== resMap.end() ); // shouldn't be already in there

    resMap[eventNumber] = make_pair( mass, weight );

  }

  return resMap;
}

double getSWeight( map< ULong64_t, pair<double,double> > &resMap, ULong64_t eventNumber, double mass ) {

  if ( resMap.find(eventNumber)==resMap.end() ) {
    cout << "Couldn't find " << eventNumber << " in map" << endl;
    return -999.;
  }

  if ( (resMap[eventNumber].first - mass) > 0.01 ) {
    cout << "Event found " << eventNumber << " has incosistent mass " << resMap[eventNumber].first << " and " << mass << endl;
    return -999.;
  }
  double sweight = resMap[eventNumber].second;
  //cout << "Got sweight " << resMap[eventNumber].second << " for event " << eventNumber << endl;
  return sweight;

}

void addSWeightToTree() {

  TFile *tf = TFile::Open("root/MassFitResultWSWeights.root");
  RooWorkspace *w = (RooWorkspace*)tf->Get("w");

  RooDataSet *Data2011HadronTOS = (RooDataSet*)w->data("Data2011HadronTOS_sw");
  RooDataSet *Data2011GlobalTIS = (RooDataSet*)w->data("Data2011GlobalTIS_sw");
  RooDataSet *Data2012HadronTOS = (RooDataSet*)w->data("Data2012HadronTOS_sw");
  RooDataSet *Data2012GlobalTIS = (RooDataSet*)w->data("Data2012GlobalTIS_sw");

  map< ULong64_t, pair<double,double> > res2011HadronTOS = getMapFromDataSet( Data2011HadronTOS );
  map< ULong64_t, pair<double,double> > res2011GlobalTIS = getMapFromDataSet( Data2011GlobalTIS );
  map< ULong64_t, pair<double,double> > res2012HadronTOS = getMapFromDataSet( Data2012HadronTOS );
  map< ULong64_t, pair<double,double> > res2012GlobalTIS = getMapFromDataSet( Data2012GlobalTIS );

  tf->Close();

  // copy tree
  system("cp root/AnalysisOut.root root/AnalysisOutWSWeights.root");

  // update the file
  TFile *tFile = new TFile("root/AnalysisOutWSWeights.root","UPDATE");
  TTree *tree = (TTree*)tFile->Get("AnalysisTree");

  ULong64_t  eventNumber;
  double     B_s0_DTF_B_s0_M;
  int        itype;
  bool       pass_bdt;
  bool       pass_pid;
  bool       pass_rhokst;
  bool       pass_massveto;
  bool       pass_multcand;
  bool       B_s0_L0HadronDecision_TOS;
  bool       B_s0_L0Global_TIS;

  double     sweight;

  // declare new branch
  TBranch *bpt = tree->Branch( "sweight",  &sweight, "sweight/D" );

  // set address of old branches
  tree->SetBranchAddress(  "eventNumber"                 , &eventNumber                 );
  tree->SetBranchAddress(  "B_s0_DTF_B_s0_M"             , &B_s0_DTF_B_s0_M             );
  tree->SetBranchAddress(  "itype"                       , &itype                       );
  tree->SetBranchAddress(  "pass_bdt"                    , &pass_bdt                    );
  tree->SetBranchAddress(  "pass_pid"                    , &pass_pid                    );
  tree->SetBranchAddress(  "pass_rhokst"                 , &pass_rhokst                 );
  tree->SetBranchAddress(  "pass_massveto"               , &pass_massveto               );
  tree->SetBranchAddress(  "pass_multcand"               , &pass_multcand               );
  tree->SetBranchAddress(  "B_s0_L0HadronDecision_TOS"   , &B_s0_L0HadronDecision_TOS   );
  tree->SetBranchAddress(  "B_s0_L0Global_TIS"           , &B_s0_L0Global_TIS           );

  // loop tree
  for ( int ev=0; ev<tree->GetEntries(); ev++) {

    tree->GetEntry(ev);

    sweight = -999.;

    if ( (itype==71 || itype==81) && pass_bdt && pass_pid && (!pass_rhokst) && (!pass_massveto) && pass_multcand ) {

      if ( itype==71 ) {
        if ( B_s0_L0HadronDecision_TOS )                        {
          sweight = getSWeight( res2011HadronTOS, eventNumber, B_s0_DTF_B_s0_M );
        }
        if ( B_s0_L0Global_TIS && !B_s0_L0HadronDecision_TOS)   {
          sweight = getSWeight( res2011GlobalTIS, eventNumber, B_s0_DTF_B_s0_M );
        }

      }
      if ( itype==81 ) {
        if ( B_s0_L0HadronDecision_TOS )                        {
          sweight = getSWeight( res2012HadronTOS, eventNumber, B_s0_DTF_B_s0_M );
        }
        if ( B_s0_L0Global_TIS && !B_s0_L0HadronDecision_TOS)   {
          sweight = getSWeight( res2012GlobalTIS, eventNumber, B_s0_DTF_B_s0_M );
        }
      }
    }

    if ( ev%10000==0 ) cout << ev << " / " << tree->GetEntries() << endl;

    // notice NOT tree->Fill()
    bpt->Fill();

  }
  //tree->Print();
  tree->Write();
  delete tFile;

}

void draw( TTree *tree, TString var, int bins, double xmin, double xmax, TString xtitle="", TString unit="" ) {

  TH1F *h = new TH1F( var, var, bins, xmin, xmax );

  tree->Draw( var+">>"+var, "sweight*( (itype==71 || itype==81) && pass_bdt && pass_pid && (!pass_rhokst) && (!pass_massveto) && pass_multcand)", "goff" );

  TString title = h->GetTitle();
  if ( xtitle!="" ) title = xtitle;
  if ( unit!="" ) title += " ("+unit+")";
  h->GetXaxis()->SetTitle( title );

  h->GetYaxis()->SetTitle( "sWeighted Events" );
  h->GetXaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetTitleSize(0.06);
  h->GetXaxis()->SetTitleOffset(0.8);
  h->GetYaxis()->SetTitleOffset(0.8);

  TCanvas *c = new TCanvas();
  h->Draw("LEP");
  h->GetYaxis()->SetRangeUser(0., h->GetMaximum()*1.1 );
  c->Update();
  c->Modified();

  c->Print(Form("plots/MassFit/pdf/%s_sw.pdf",var.Data()));
  c->Print(Form("plots/MassFit/png/%s_sw.png",var.Data()));

}

void draw2D( TTree *tree, TString var1, TString var2, int xbins, double xmin, double xmax, int ybins, double ymin, double ymax, TString xtitle="", TString ytitle="", TString xunit="", TString yunit="" ) {

  TH2F *h = new TH2F( var1+"_"+var2, "", xbins, xmin, xmax, ybins, ymin, ymax );

  tree->Draw( var2+":"+var1+">>"+var1+"_"+var2, "sweight*( (itype==71 || itype==81) && pass_bdt && pass_pid && (!pass_rhokst) && (!pass_massveto) && pass_multcand)", "goff" );

  TString title = h->GetTitle();
  if ( xtitle!="" ) title = xtitle;
  if ( xunit!="") title += " ("+xunit+")";
  h->GetXaxis()->SetTitle( title );

  if ( ytitle!="" ) title = ytitle;
  if ( yunit!="" ) title += " ("+yunit+")";
  h->GetYaxis()->SetTitle( title );

  h->GetZaxis()->SetTitle( "sWeighted Events" );

  h->GetXaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetTitleSize(0.06);
  h->GetZaxis()->SetTitleSize(0.06);
  h->GetXaxis()->SetTitleOffset(1.3);
  h->GetYaxis()->SetTitleOffset(1.3);
  h->GetZaxis()->SetTitleOffset(1.);

  TColor *mycol = gROOT->GetColor( kGray+3 );
  mycol->SetAlpha(0.4);
  h->SetLineColor( mycol->GetNumber() );
  h->SetLineWidth(1);
  h->GetYaxis()->SetNdivisions( h->GetXaxis()->GetNdivisions() );

  TCanvas *c = new TCanvas();
  c->cd();
  gPad->SetPhi(40);
  gPad->SetTheta(30);
  gPad->SetTopMargin(0.);
  gPad->SetBottomMargin(0.08);

  h->Draw("LEGO2FBBB");
  c->Update();
  c->Modified();

  c->Print(Form("plots/MassFit/pdf/%s_%s_sw.pdf",var1.Data(),var2.Data()));
  c->Print(Form("plots/MassFit/png/%s_%s_sw.png",var1.Data(),var2.Data()));

  delete c;

  TCanvas *c2 = new TCanvas();
  c2->cd();
  c2->SetLeftMargin(0.15);
  c2->SetRightMargin(0.18);
  h->Draw("COLZ");
  c2->Update();
  c2->Modified();
  c2->Print(Form("plots/MassFit/pdf/%s_%s_sw_flat.pdf",var1.Data(),var2.Data()));
  c2->Print(Form("plots/MassFit/png/%s_%s_sw_flat.png",var1.Data(),var2.Data()));

  delete c2;

}

void makeSWPlots() {

  gROOT->ProcessLine(".x ~/Scratch/lhcb/lhcbStyle.C");
  gStyle->SetPalette(1);
  TFile *tf = TFile::Open("root/AnalysisOutWSWeights.root");
  TTree *tree = (TTree*)tf->Get("AnalysisTree");

  draw( tree, "B_s0_DTF_B_s0_M"         , 100, 5000  , 5800 , "m(K^{+}#pi^{-}K^{-}#pi^{+})", "MeV"  );
  draw( tree, "B_s0_DTF_B_s0_MERR"      , 100, 0     , 100  , "m(K^{+}#pi^{-}K^{-}#pi^{+})", "MeV"  );
  draw( tree, "B_s0_DTF_KST1_M"         , 90 , 800   , 1700 , "m(K^{-}#pi^{+})",             "MeV"  );
  draw( tree, "B_s0_DTF_KST2_M"         , 90 , 800   , 1700 , "m(K^{-}#pi^{+})",             "MeV"  );
  draw( tree, "B_s0_DTF_B_s0_CosTheta1" , 50 , -1    , -1   , "cos(#theta_{1})",             ""     );
  draw( tree, "B_s0_DTF_B_s0_CosTheta2" , 50 , -1    , -1   , "cos(#theta_{2})",             ""     );
  draw( tree, "B_s0_DTF_B_s0_Phi1"      , 63 , -3.15 , 3.15 , "#Phi",                        "rad"  );
  draw( tree, "B_s0_DTF_TAU"            , 50 ,  0    ,  10  , "t",                           "fs"   );
  draw( tree, "B_s0_DTF_TAUERR"         , 50 ,  0    ,  0.1 , "#sigma_{t}",                  "fs"   );
  draw( tree, "B_s0_TAGDECISION_OS"     , 3  , -1    ,  2   , "TAG OS DEC",                  ""     );
  draw( tree, "B_s0_TAGOMEGA_OS"        , 50 ,  0    ,  0.5 , "TAG OS OMEGA",                ""     );
  draw( tree, "B_s0_SS_nnetKaon_DEC"    , 3  , -1    ,  2   , "TAG SS Kaon DEC",             ""     );
  draw( tree, "B_s0_SS_nnetKaon_PROB"   , 50 ,  0    ,  0.5 , "TAG SS Kaon PROB",            ""     );
  draw2D( tree, "B_s0_DTF_KST1_M", "B_s0_DTF_KST2_M", 45, 800, 1700, 45, 800, 1700, "m(K^{-}#pi^{+})", "m(K^{+}#pi^{-})", "MeV", "MeV" );

  tf->Close();

}

int main() {

  addSWeightToTree();
  makeSWPlots();

  return 0;
}
