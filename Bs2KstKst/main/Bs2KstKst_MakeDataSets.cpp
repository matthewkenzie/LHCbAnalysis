#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TString.h"

#include "RooPlot.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooWorkspace.h"

using namespace std;
using namespace RooFit;

int main (int argc, char **argv) {

  TFile *tf = TFile::Open("root/AnalysisOut.root");
  TTree *tree = (TTree*)tf->Get("AnalysisTree");

  RooWorkspace *w = new RooWorkspace("w","w");

  // tree variables
  Double_t  t_mass;
  Double_t  t_KST1_m;
  Double_t  t_KST2_m;
  int       t_itype;
  ULong64_t t_eventNumber;
  Bool_t    t_pass_bdt;
  Bool_t    t_pass_pid;
  TString   *t_evname = 0;

  // tree branches
  tree->SetBranchAddress("B_s0_DTF_B_s0_M",      &t_mass        );
  tree->SetBranchAddress("B_s0_DTF_KST1_M",      &t_KST1_m   );
  tree->SetBranchAddress("B_s0_DTF_KST2_M",      &t_KST2_m   );
  tree->SetBranchAddress("itype",                &t_itype       );
  tree->SetBranchAddress("eventNumber",          &t_eventNumber );
  tree->SetBranchAddress("pass_bdt",             &t_pass_bdt    );
  tree->SetBranchAddress("pass_pid",             &t_pass_pid    );
  tree->SetBranchAddress("evname",               &t_evname      );

  // observables
  RooRealVar *mass        = new RooRealVar("B_s0_DTF_B_s0_M", "m(K#piK#pi)", 5000., 5800.);
  RooRealVar *KST1_mass   = new RooRealVar("B_s0_DTF_KST1_M", "m(K#pi)"    , 750, 1700 );
  RooRealVar *KST2_mass   = new RooRealVar("B_s0_DTF_KST2_M", "m(K#pi)"    , 750, 1700 );
  RooRealVar *itype       = new RooRealVar("itype","itype",-10,10); itype->removeRange();
  RooRealVar *eventNumber = new RooRealVar("eventNumber","eventNumber",0,10e10);


  // map to store datasets
  map<int, RooDataSet*> dsets;

  for ( int ev=0; ev<tree->GetEntries(); ev++ ) {
    tree->GetEntry(ev);

    if ( t_itype >= 0 && !t_pass_bdt ) continue;
    if ( t_itype >= 0 && !t_pass_pid ) continue;

    // if entry not in map then add it
    if ( dsets.find( t_itype ) == dsets.end() ) {
      dsets[t_itype] = new RooDataSet( *t_evname, *t_evname, RooArgSet(*mass,*KST1_mass,*KST2_mass,*itype,*eventNumber) );
    }

    // add to dataset
    mass->setVal( t_mass );
    KST1_mass->setVal( t_KST1_m );
    KST2_mass->setVal( t_KST2_m );
    itype->setVal( t_itype );
    eventNumber->setVal( t_eventNumber );
    dsets[t_itype]->add( RooArgSet(*mass,*KST1_mass,*KST2_mass,*itype,*eventNumber) );

    if ( ev%10000==0 ) {
      cout << ev << "/" << tree->GetEntries() << endl;
      cout << "\t" << t_itype << " " << *t_evname << " " << t_eventNumber << " " << t_mass << endl;
    }
  }

  for ( map<int, RooDataSet*>::iterator it = dsets.begin(); it != dsets.end(); it++ ) {

    cout << "\t" << it->first << " " << it->second->GetName() << " " << it->second->numEntries() << endl;
    w->import(*it->second);

  }

  tf->Close();

  delete t_evname;

  w->writeToFile("tmp/DataSets.root");
  return 0;
}
