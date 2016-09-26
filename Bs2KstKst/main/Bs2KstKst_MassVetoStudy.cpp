#include <iostream>
#include <vector>
#include <map>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TH1F.h"
#include "TLegend.h"

using namespace std;

bool sortFunc( pair<TString,double> A, pair<TString,double> B ) {
  if ( A.second > B.second ) return true;
  else return false;
}

int main(){

  system("mkdir -p plots/MassVetoStudy/pdf");
  gROOT->ProcessLine(".x ~/Scratch/lhcb/lhcbStyle.C");

  TFile *tf = TFile::Open("root/AnalysisOutMassVarPlots.root");
  TTree *tree = (TTree*)tf->Get("AnalysisTree");

  double sigEnts = tree->GetEntries("itype==-70 || itype==-80");
  double bkgEnts = tree->GetEntries("itype==-77 || itype==-87");

  TGraph *roc = new TGraph();
  TGraph *sEff = new TGraph();
  TGraph *bEff = new TGraph();

  // scan ROC
  for ( int i=0; i<20 ; i++ ) {

    double window = 0. + (i*5);

    double sigPass = tree->GetEntries( Form("(itype==-70 || itype==-80) && TMath::Abs(B_s0_M_PipPimKmPip-5279.53)>%5.1f && TMath::Abs(B_s0_M_KpPimPimPip-5279.53)>%5.1f",window,window) );
    double bkgPass = tree->GetEntries( Form("(itype==-77 || itype==-87) && TMath::Abs(B_s0_M_PipPimKmPip-5279.53)>%5.1f && TMath::Abs(B_s0_M_KpPimPimPip-5279.53)>%5.1f",window,window) );

    double sigEff = double(sigPass)/double(sigEnts);
    double bkgRej = 1. - double(bkgPass)/double(bkgEnts);

    sEff->SetPoint(i, window, sigEff);
    bEff->SetPoint(i, window, 1.-bkgRej);
    roc->SetPoint(i, sigEff, bkgRej );
  }

  TCanvas *c = new TCanvas();
  roc->GetXaxis()->SetTitleOffset(0.8);
  roc->GetYaxis()->SetTitleOffset(0.8);
  roc->GetXaxis()->SetTitle("K^{*0}K^{*0} efficiency");
  roc->GetYaxis()->SetTitle("#rho^{0}K^{*0} rejection");
  roc->Draw("ALP");

  c->Update();
  c->Modified();
  c->Print("plots/MassVetoStudy/pdf/roc.pdf");

  TCanvas *c2 = new TCanvas();
  sEff->GetXaxis()->SetTitleOffset(0.8);
  sEff->GetYaxis()->SetTitleOffset(0.8);
  sEff->GetXaxis()->SetTitle("Cut window size (#pm MeV)");
  sEff->GetYaxis()->SetTitle("Efficiency");
  sEff->SetMarkerColor(kBlue);
  sEff->SetLineColor(kBlue);
  bEff->SetMarkerColor(kRed);
  bEff->SetLineColor(kRed);
  sEff->Draw("ALP");
  bEff->Draw("LPsame");
  c2->Update();
  c2->Modified();
  c2->Print("plots/MassVetoStudy/pdf/eff.pdf");

  // Study veto with PID cuts
  double Kplus_ProbNNk;
  double Kminus_ProbNNk;
  double Piplus_ProbNNk;
  double Piminus_ProbNNk;
  double Kplus_ProbNNpi;
  double Kminus_ProbNNpi;
  double Piplus_ProbNNpi;
  double Piminus_ProbNNpi;
  double B_s0_M_KpPimPimPip;
  double B_s0_M_PipPimKmPip;
  double B_s0_DTF_B_s0_M;
  double M_KpKm;
  double M_PipPim;
  int itype;

  tree->SetBranchAddress("itype", &itype);
  tree->SetBranchAddress("Kplus_ProbNNk"                 , &Kplus_ProbNNk                   );
  tree->SetBranchAddress("Kplus_ProbNNpi"                , &Kplus_ProbNNpi                  );
  tree->SetBranchAddress("Kminus_ProbNNk"                , &Kminus_ProbNNk                  );
  tree->SetBranchAddress("Kminus_ProbNNpi"               , &Kminus_ProbNNpi                 );
  tree->SetBranchAddress("Piplus_ProbNNk"                , &Piplus_ProbNNk                  );
  tree->SetBranchAddress("Piplus_ProbNNpi"               , &Piplus_ProbNNpi                 );
  tree->SetBranchAddress("Piminus_ProbNNk"               , &Piminus_ProbNNk                 );
  tree->SetBranchAddress("Piminus_ProbNNpi"              , &Piminus_ProbNNpi                );
  tree->SetBranchAddress("B_s0_M_KpPimPimPip"            , &B_s0_M_KpPimPimPip              );
  tree->SetBranchAddress("B_s0_M_PipPimKmPip"            , &B_s0_M_PipPimKmPip              );
  tree->SetBranchAddress("B_s0_DTF_B_s0_M"               , &B_s0_DTF_B_s0_M                 );
  tree->SetBranchAddress("M_KpKm"                        , &M_KpKm                          );
  tree->SetBranchAddress("M_PipPim"                      , &M_KpKm                          );

  TH1F *sigB = new TH1F("sigB","sig",100,5300,5500);
  TH1F *bkgB = new TH1F("bkgB","bkg",100,5300,5500);
  TH1F *sigA = new TH1F("sigA","sig",100,5300,5500);
  TH1F *bkgA = new TH1F("bkgA","bkg",100,5300,5500);
  TH1F *sigP = new TH1F("sigP","sig",100,5300,5500);
  TH1F *bkgP = new TH1F("bkgP","bkg",100,5300,5500);


  for (int ev=0; ev<tree->GetEntries(); ev++) {
  //for (int ev=0; ev<10; ev++) {
    tree->GetEntry(ev);

    //cout << ev << endl;

    if ( itype==-70 || itype==-80 ) sigB->Fill( B_s0_DTF_B_s0_M );
    if ( itype==-77 || itype==-87 ) bkgB->Fill( B_s0_DTF_B_s0_M );

    // my PID cuts?
    if ( (Kplus_ProbNNk>0.2 && Kplus_ProbNNk*(1.-Kplus_ProbNNpi)>0.3) && (Kminus_ProbNNk>0.2 && Kminus_ProbNNk*(1.-Kminus_ProbNNpi)>0.3) &&
         (Piplus_ProbNNpi>0.2 && Piplus_ProbNNpi*(1.-Piplus_ProbNNk)>0.3) && (Piminus_ProbNNpi>0.2 && Piminus_ProbNNpi*(1.-Piminus_ProbNNk)>0.3) ) {
      if ( itype==-70 || itype==-80 ) sigP->Fill( B_s0_DTF_B_s0_M );
      if ( itype==-77 || itype==-87 ) bkgP->Fill( B_s0_DTF_B_s0_M );
    }

    vector< pair<TString,double> > ProbNNk;
    ProbNNk.push_back( make_pair("Kplus", Kplus_ProbNNk) );
    ProbNNk.push_back( make_pair("Kminus", Kminus_ProbNNk) );
    ProbNNk.push_back( make_pair("Piplus", Piplus_ProbNNk) );
    ProbNNk.push_back( make_pair("Piminus", Piminus_ProbNNk) );

    map<TString,double> ProbNNpi;
    ProbNNpi["Kplus"]   = Kplus_ProbNNpi;
    ProbNNpi["Kminus"]  = Kminus_ProbNNpi;
    ProbNNpi["Piplus"]  = Piplus_ProbNNpi;
    ProbNNpi["Piminus"] = Piminus_ProbNNpi;

    sort( ProbNNk.begin(), ProbNNk.end(), &sortFunc );

    //cout << ProbNNk[0].first << " " << ProbNNk[0].second << " " << ProbNNpi[ProbNNk[0].first] << endl;
    //cout << ProbNNk[1].first << " " << ProbNNk[1].second << " " << ProbNNpi[ProbNNk[1].first] << endl;
    //cout << ProbNNk[2].first << " " << ProbNNk[2].second << " " << ProbNNpi[ProbNNk[2].first] << endl;
    //cout << ProbNNk[3].first << " " << ProbNNk[3].second << " " << ProbNNpi[ProbNNk[3].first] << endl;

    bool passKaon = false;
    bool passPion = false;

    // kaon - i.e. one with the highest ProbNNk
    TString kaonName = ProbNNk[0].first;
    double kaonVal = ProbNNk[0].second;
    if ( kaonVal>0.2 && kaonVal*(1-ProbNNpi[kaonName])>0.3 ) {
      //cout << "pass kaon pid" << endl;
      passKaon = true;
    }

    // this is the others
    passPion=true;
    for (int i=1; i<4; i++ ) { // these are now the "pions"
      TString name = ProbNNk[i].first;
      if ( ProbNNpi[name]< 0.2 || ProbNNpi[name]*(1.-ProbNNk[i].second)<0.3 ) {
        passPion = false;
      }
    }
    //if (passPion) cout << "pass pion pid" << endl;

    bool passMassVeto = false;
    if ( TMath::Abs( B_s0_M_PipPimKmPip-5279.53 )<30. || TMath::Abs( B_s0_M_KpPimPimPip-5279.53 )<30. ) {
      //cout << "passMassVeto" << endl;
      passMassVeto = true;
    }

    if ( passKaon && passPion && passMassVeto ) continue;
    if ( itype==-70 || itype==-80 ) sigA->Fill( B_s0_DTF_B_s0_M );
    if ( itype==-77 || itype==-87 ) bkgA->Fill( B_s0_DTF_B_s0_M );
  }

  //
  sigB->SetLineColor(kBlue);
  bkgB->SetLineColor(kRed);
  sigA->SetFillColorAlpha(kBlue-7, 0.8);
  bkgA->SetFillColorAlpha(kRed-7, 0.8);
  sigP->SetFillColorAlpha(kBlue-7, 0.8);
  bkgP->SetFillColorAlpha(kRed-7, 0.8);

  double scaleFact = bkgB->Integral() / sigB->Integral();
  sigB->Scale(scaleFact);
  sigA->Scale(scaleFact);
  sigP->Scale(scaleFact);

  sigB->GetXaxis()->SetTitle("m(B_{s}^{0}) [MeV]");
  sigB->GetXaxis()->SetTitleOffset(0.8);
  sigB->GetXaxis()->SetTitleSize(0.08);

  TLegend *leg = new TLegend(0.6,0.6,0.9,0.9);
  leg->SetFillColor(0);
  leg->AddEntry(sigB, "B_{s}^{0}#rightarrowK^{*0}K^{*0} MC Before Cuts", "L");
  leg->AddEntry(sigA, "B_{s}^{0}#rightarrowK^{*0}K^{*0} MC After Cuts", "LF");
  leg->AddEntry(bkgB, "B_{d}^{0}#rightarrow#rho^{0}K^{*0} MC Before Cuts", "L");
  leg->AddEntry(bkgA, "B_{d}^{0}#rightarrow#rho^{0}K^{*0} MC After Cuts", "LF");

  TCanvas *c3 = new TCanvas();
  sigB->Draw("HIST");
  sigA->Draw("HISTFsame");
  bkgB->Draw("HISTsame");
  bkgA->Draw("HISTFsame");
  leg->Draw("same");
  c3->Update();
  c3->Modified();
  c3->Print("plots/MassVetoStudy/pdf/mass.pdf");

  TCanvas *c4 = new TCanvas();
  sigB->Draw("HIST");
  sigP->Draw("HISTFsame");
  bkgB->Draw("HISTsame");
  bkgP->Draw("HISTFsame");
  leg->Draw("same");
  c4->Update();
  c4->Modified();
  c4->Print("plots/MassVetoStudy/pdf/pid.pdf");

  cout << "Signal B:           " << sigB->GetEntries() << endl;
  cout << "Signal A (!rho)     " << sigA->GetEntries() << " " << double(sigA->GetEntries())/double(sigB->GetEntries()) << endl;
  cout << "Signal A (pid)      " << sigP->GetEntries() << " " << double(sigP->GetEntries())/double(sigB->GetEntries()) << endl;
  cout << "Background B:       " << bkgB->GetEntries() << endl;
  cout << "Background A (!rho) " << bkgA->GetEntries() << " " << double(bkgA->GetEntries())/double(bkgB->GetEntries()) << endl;
  cout << "Background A (pid)  " << bkgP->GetEntries() << " " << double(bkgP->GetEntries())/double(bkgB->GetEntries()) << endl;

  return 0;
}
