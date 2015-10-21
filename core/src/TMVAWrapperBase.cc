#include "TMVAWrapperBase.h"
#include "TDirectory.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TLegend.h"
#include "TPad.h"
#include "TLine.h"

using namespace std;
using namespace TMVA;
using namespace TMath;

TMVAWrapperBase::TMVAWrapperBase(TString _name, const Variables *_v, mode _rMode):
  Analyser(_name),
  v(_v),
  rMode(_rMode),
  outfilename(Form("root/%sOut.root",_name.Data())),
  numberOfBDTs(-1),
  doBDTCycling(false),
  factoryOptions(""),
  trainingOptions("")
{
  if (!doBDTCycling) numberOfBDTs=1;
  TH1::SetDefaultSumw2();
  gROOT->ProcessLine(".x ~/bin/lhcbStyle.C");
  system(Form("mkdir -p plots/%s/pdf",_name.Data()));
  system(Form("mkdir -p plots/%s/png",_name.Data()));
  system(Form("mkdir -p plots/%s/C",_name.Data()));
  types[0] = "_sig_train";
  types[1] = "_bkg_train";
  types[2] = "_sig_test";
  types[3] = "_bkg_test";
}

TMVAWrapperBase::~TMVAWrapperBase(){}

void TMVAWrapperBase::Init(){
  cout << Form("%-30s","TMVAWrapperBase::Init()") << " " << "Initialising Analyser (" << name << ")." << endl;
  setCategories();
  setFactoryOptions();
  setTrainingOptions();
  setNumberOfBDTs();
  setBDTCycling();
  setInputVariables();
  setSpectatorVariables();
  initMVAFactories();
}

void TMVAWrapperBase::Term(){

  if ( rMode==kTrain ) {
    prepare();
    train();
    test();
    evaluate();

    // print ev counts
    cout << Form("%-30s","TMVAWrapperBase::Term()") << "Training Statistics" << endl;
    cout << Form("%-30s","") << "\t" << Form("%-10s      %2s %10s %10s","Cat","b","Test","Train") << endl;
    for (vector<TString>::iterator cat=categories.begin(); cat!=categories.end(); cat++){
      // sig
      for (int b=0; b<numberOfBDTs; b++) {
        cout << Form("%-30s","TMVAWrapperBase::Term()") << "\t" << Form("%-10s (S): %2d %10d %10d",cat->Data(),b,sigEvCounts[*cat][b].first, sigEvCounts[*cat][b].second) << endl;
      }
      // bkg
      for (int b=0; b<numberOfBDTs; b++) {
        cout << Form("%-30s","TMVAWrapperBase::Term()") << "\t" << Form("%-10s (B): %2d %10d %10d",cat->Data(),b,bkgEvCounts[*cat][b].first, bkgEvCounts[*cat][b].second) << endl;
      }
    }
    cout << Form("%-30s","TMVAWrapperBase::Term()") << " " << "Saving training tree and making histograms" << endl;
    createTrainingHistograms();
    outFile->cd();
    trainingTree->Write();
    cout << Form("%-30s","TMVAWrapperBase::Term()") << " " << "Training output written to: " << outFile->GetName() << endl;
    outFile->Close();
    delete outFile;

  }

  cout << Form("%-30s","TMVAWrapperBase::Term()") << " " << "Terminating Analyser (" << name << ")." << endl;
}

bool TMVAWrapperBase::AnalyseEvent(){
  return setEventValuesAndEvaluate();
}

void TMVAWrapperBase::checkInit() {

  if (numberOfBDTs<0) {
    cerr << "ERROR -- TMVAWrapperBase::checkInit() -- no number of BDTs set" << endl;
    exit(1);
  }
  if (factoryOptions=="" && rMode==kTrain) {
    cerr << "ERROR -- TMVAWrapperBase::checkInit() -- no factory options set" << endl;
    exit(1);
  }
  if (trainingOptions=="" && rMode==kTrain) {
    cerr << "ERROR -- TMVAWrapperBase::checkInit() -- no training options set" << endl;
    exit(1);
  }
  if (varNames.size()==0) {
    cerr << "ERROR -- TMVAWrapperBase::checkInit() -- no input variables defined" << endl;
    exit(1);
  }
}

void TMVAWrapperBase::initMVAFactories() {

  checkInit();

  // TRAINING MODE
  if ( rMode == kTrain ) {
    outFile = TFile::Open(outfilename,"RECREATE");

    // Set branches of training tree
    trainingTree = new TTree(Form("%s_TrainingTree",name.Data()),"TrainingTree");
    trainingTree->Branch("relBDT",     &relBDT);
    trainingTree->Branch("isSigEvent", &isSigEvent);
    trainingTree->Branch("eventCat",   &eventCat);
    for (vector<TString>::iterator var=varNames.begin(); var!=varNames.end(); var++) {
      trainingTree->Branch(*var, &readVarMap[*var]);
    }

    // loop over BDT categories (e.g. 2011/2012)
    for (vector<TString>::iterator cat = categories.begin(); cat != categories.end(); cat++) {

      factoryContainer[*cat] = vector<TMVA::Factory*>();
      sigEvCounts[*cat] = vector<pair<int,int> >();
      bkgEvCounts[*cat] = vector<pair<int,int> >();

      // loop over number of BDTs
      for (int b=0; b<numberOfBDTs; b++){
        factoryContainer[*cat].push_back(new Factory(Form("%sFactory",name.Data()),outFile,factoryOptions));
        // set counters
        sigEvCounts[*cat].push_back(make_pair(0,0));
        bkgEvCounts[*cat].push_back(make_pair(0,0));

        // add variables to BDT
        for (vector<TString>::iterator var=varNames.begin(); var!=varNames.end(); var++){
          factoryContainer[*cat][b]->AddVariable(*var);
        }
      }
    }
  }
  // EVALUATION MODE -- ALSO USED FOR TRAINING MODE
  // loop over BDT categories (e.g. 2011/2012)
  for (vector<TString>::iterator cat = categories.begin(); cat != categories.end(); cat++) {

    readerContainer[*cat]  = vector<TMVA::Reader*>();

    // loop over number of BDTs
    for (int b=0; b<numberOfBDTs; b++){
      readerContainer[*cat].push_back(new Reader( "!Color:!Silent" ));

      // add variables to BDT
      for (vector<TString>::iterator var=varNames.begin(); var!=varNames.end(); var++){
        readerContainer[*cat][b]->AddVariable(*var, &readVarMap[*var]);
      }

      // only do booking here if in eval mode
      if ( rMode == kEval ) {
        // weights files
        TString weightsFile = Form("weights/%sFactory_%s_BDT_%d.weights.xml",name.Data(),cat->Data(),b);
        TString methodName  = Form("BDT%dmethod",b);
        readerContainer[*cat][b]->BookMVA( methodName, weightsFile );
      }
    }
  }
}

void TMVAWrapperBase::prepare() {

  // prepare trees
  for (vector<TString>::iterator cat=categories.begin(); cat!=categories.end(); cat++){
    for (int b=0; b<numberOfBDTs; b++){
      factoryContainer[*cat][b]->PrepareTrainingAndTestTree("","!V");
    }
  }
  // book methods
  for (vector<TString>::iterator cat=categories.begin(); cat!=categories.end(); cat++){
    for (int b=0; b<numberOfBDTs; b++){
      factoryContainer[*cat][b]->BookMethod( Types::kBDT, Form("%s_BDT_%d",cat->Data(),b), trainingOptions );
    }
  }
}

void TMVAWrapperBase::train() {
  for (vector<TString>::iterator cat=categories.begin(); cat!=categories.end(); cat++){
    for (int b=0; b<numberOfBDTs; b++){
      factoryContainer[*cat][b]->TrainAllMethods();
    }
  }
}

void TMVAWrapperBase::test() {
  for (vector<TString>::iterator cat=categories.begin(); cat!=categories.end(); cat++){
    for (int b=0; b<numberOfBDTs; b++){
      factoryContainer[*cat][b]->TestAllMethods();
    }
  }
}

void TMVAWrapperBase::evaluate() {
  for (vector<TString>::iterator cat=categories.begin(); cat!=categories.end(); cat++){
    for (int b=0; b<numberOfBDTs; b++){
      factoryContainer[*cat][b]->EvaluateAllMethods();
    }
  }
}

void TMVAWrapperBase::addEvent(TString cat, bool isSig) {
  vector<double> values;
  // check map has been filled
  for (vector<TString>::iterator var=varNames.begin(); var!=varNames.end(); var++){
    if (varMap.find(*var)==varMap.end()) {
      cerr << "ERROR -- TMVAWrapperBase::addEvent() -- variable not in map " << *var << endl;
      exit(1);
    }
    values.push_back(varMap[*var]);
  }
  // check category exists
  if ( find(categories.begin(), categories.end(), cat)==categories.end() )
  {
    cerr << "ERROR -- TMVAWrapperBase::addEvent() -- category " << cat << " does not exist" << endl;
    exit(1);
  }

  relBDT = v->eventNumber%numberOfBDTs;
  isSigEvent = isSig;
  eventCat = cat;

  if (doBDTCycling) {
    for (int b=0; b<numberOfBDTs; b++) {
      if (b==relBDT) {
        if (isSig) {
          factoryContainer[cat][b]->AddSignalTestEvent(values);
          sigEvCounts[cat][b].first += 1;
        }
        else {
          factoryContainer[cat][b]->AddBackgroundTestEvent(values);
          bkgEvCounts[cat][b].first += 1;
        }
      }
      else {
        if (isSig) {
          factoryContainer[cat][b]->AddSignalTrainingEvent(values);
          sigEvCounts[cat][b].second += 1;
        }
        else {
          factoryContainer[cat][b]->AddBackgroundTrainingEvent(values);
          bkgEvCounts[cat][b].second += 1;
        }
      }
    }
  }
  else { // no BDT cycling
    if (v->eventNumber%2==0) {
      if (isSig) {
        factoryContainer[cat][0]->AddSignalTestEvent(values);
        sigEvCounts[cat][0].first += 1;
      }
      else {
        factoryContainer[cat][0]->AddBackgroundTestEvent(values);
        bkgEvCounts[cat][0].first += 1;
      }
    }
    else {
      if (isSig) {
        factoryContainer[cat][0]->AddSignalTrainingEvent(values);
        sigEvCounts[cat][0].second += 1;
      }
      else {
        factoryContainer[cat][0]->AddBackgroundTrainingEvent(values);
        bkgEvCounts[cat][0].second += 1;
      }
    }
  }
  values.clear();
  trainingTree->Fill();
}

void TMVAWrapperBase::addSignalEvent(TString cat){
  addEvent(cat,true);
}

void TMVAWrapperBase::addBackgroundEvent(TString cat){
  addEvent(cat,false);
}

void TMVAWrapperBase::addVar(TString var){
 varNames.push_back(var);
 cout << Form("%-30s","TMVAWrapperBase::addVar()") << " Variable " << var << " added" << endl;
 // initilise reading map
 readVarMap[var] = -99999.;
}

void TMVAWrapperBase::setVal(TString var, double value){
  if ( find(varNames.begin(), varNames.end(), var)==varNames.end() ) {
    cerr << "ERROR -- TMVAWrapperBase::setVal() -- variable " << var << " not in varNames" << endl;
    exit(1);
  }
  varMap[var] = value;
  readVarMap[var] = float(value);
}

float TMVAWrapperBase::evaluateMVAValue(TString cat) {

  // check category exists
  if ( find(categories.begin(), categories.end(), cat)==categories.end() )
  {
    cerr << "ERROR -- TMVAWrapperBase::addEvent() -- category " << cat << " does not exist" << endl;
    exit(1);
  }

  relBDT = v->eventNumber%numberOfBDTs;
  return readerContainer[cat][relBDT]->EvaluateMVA( Form("BDT%dmethod",relBDT) );
}

TString TMVAWrapperBase::getHistName(TString cat, int b, TString var, TString ext) {
  return TString(Form("%s_%d_%s%s",cat.Data(),b,var.Data(),ext.Data()));
}

void TMVAWrapperBase::setHistStyle(TH1F *sigTrain, TH1F *bkgTrain, TH1F *sigTest, TH1F *bkgTest){
  TColor *clearBlue = gROOT->GetColor(kBlue-4);
  clearBlue->SetAlpha(0.4);
  TColor *clearRed = gROOT->GetColor(kRed-4);
  clearRed->SetAlpha(0.4);

  sigTrain->SetLineColor(clearBlue->GetNumber());
  sigTrain->SetFillColor(clearBlue->GetNumber());
  bkgTrain->SetLineColor(clearRed->GetNumber());
  bkgTrain->SetFillColor(clearRed->GetNumber());

  sigTest->SetLineColor(kBlue);
  sigTest->SetMarkerColor(kBlue);
  bkgTest->SetLineColor(kRed);
  bkgTest->SetMarkerColor(kRed);

  sigTrain->GetXaxis()->SetTitleSize(0.07);
  sigTrain->GetYaxis()->SetTitleSize(0.07);
  bkgTrain->GetXaxis()->SetTitleSize(0.07);
  bkgTrain->GetYaxis()->SetTitleSize(0.07);
  sigTest->GetXaxis()->SetTitleSize(0.07);
  sigTest->GetYaxis()->SetTitleSize(0.07);
  bkgTest->GetXaxis()->SetTitleSize(0.07);
  bkgTest->GetYaxis()->SetTitleSize(0.07);

  sigTrain->GetXaxis()->SetTitleOffset(0.8);
  sigTrain->GetYaxis()->SetTitleOffset(0.85);
  bkgTrain->GetXaxis()->SetTitleOffset(0.8);
  bkgTrain->GetYaxis()->SetTitleOffset(0.85);
  sigTest->GetXaxis()->SetTitleOffset(0.8);
  sigTest->GetYaxis()->SetTitleOffset(0.85);
  bkgTest->GetXaxis()->SetTitleOffset(0.8);
  bkgTest->GetYaxis()->SetTitleOffset(0.85);

}

double TMVAWrapperBase::getMaximum(TH1F *sigTrain, TH1F *bkgTrain, TH1F *sigTest, TH1F* bkgTest) {
  return TMath::Max( TMath::Max(sigTrain->GetMaximum(),bkgTrain->GetMaximum()), TMath::Max(sigTest->GetMaximum(),bkgTest->GetMaximum()) );
}

void TMVAWrapperBase::createTrainingHistograms() {

  histStore.clear();
  // setup histograms
  // loop categories
  for (vector<TString>::iterator cat=categories.begin(); cat!=categories.end(); cat++) {
    // loop BDTs
    for (int b=0; b<numberOfBDTs; b++) {
      // loop variables
      for (vector<TString>::iterator var=varNames.begin(); var!=varNames.end(); var++) {
        for (int i=0; i<4; i++) {
          TString histName = getHistName(*cat, b, *var, types[i]);
          histStore[histName] = new TH1F(histName,histName,50,trainingTree->GetMinimum(var->Data()),trainingTree->GetMaximum(var->Data()));
        }
      }
      // do bdtoutput
      // want to read bdt output for BDTs that have just been trained
      TString weightsFile = Form("weights/%sFactory_%s_BDT_%d.weights.xml",name.Data(),cat->Data(),b);
      TString methodName  = Form("BDT%dmethod",b);
      readerContainer[*cat][b]->BookMVA( methodName, weightsFile );
      for (int i=0; i<4; i++) {
        TString histName = getHistName(*cat, b, "bdtoutput", types[i]);
        histStore[histName] = new TH1F(histName,histName,100,-1.,1.);
      }
    }
  }
  // add bdtoutput branches to training tree
  for (int b=0; b<numberOfBDTs; b++) {
    bdtoutBranches.push_back( trainingTree->Branch( Form("bdtoutput%d",b), &bdtval) );
  }

  // fill histograms
  for (int ev=0; ev<trainingTree->GetEntries(); ev++){
    trainingTree->GetEntry(ev);
    // loop bdts
    for (int b=0; b<numberOfBDTs; b++) {
      bool isTest = ( b==relBDT );
      bool isTrain = !isTest;
      TString type = "";
      if ( isSigEvent ) {
        if ( isTest )  type = types[2];
        if ( isTrain ) type = types[0];
      }
      else {
        if ( isTest )  type = types[3];
        if ( isTrain ) type = types[1];
      }
      for (vector<TString>::iterator var=varNames.begin(); var!=varNames.end(); var++){
        TString histName = getHistName(eventCat, b, *var, type);
        histStore[histName]->Fill(readVarMap[*var]);
      }
      TString histName = getHistName(eventCat, b, "bdtoutput", type);
      bdtval = readerContainer[eventCat][b]->EvaluateMVA( Form("BDT%dmethod",b) );
      histStore[histName]->Fill(bdtval);
      bdtoutBranches[b]->Fill();
    }
  }
  // new directory in outfile for histograms
  TDirectory *dir = outFile->mkdir(Form("%s_TrainingHists",name.Data()));
  dir->cd();
  for (map<TString,TH1F*>::iterator it=histStore.begin(); it!=histStore.end(); it++){
    it->second->Write();
  }
  // make plots
  for (vector<TString>::iterator cat=categories.begin(); cat!=categories.end(); cat++){
    for (int b=0; b<numberOfBDTs; b++) {
      for (vector<TString>::iterator var=varNames.begin(); var!=varNames.end(); var++){
        makePlot(*cat,b,*var);
      }
      makePlot(*cat,b,"bdtoutput");
    }
  }
  histStore.clear();
}

void TMVAWrapperBase::makePlot(TString cat, int b, TString var) {

  // main histograms
  TH1F *sigTrain = histStore[getHistName(cat,b,var,types[0])];
  TH1F *bkgTrain = histStore[getHistName(cat,b,var,types[1])];
  TH1F *sigTest = histStore[getHistName(cat,b,var,types[2])];
  TH1F *bkgTest = histStore[getHistName(cat,b,var,types[3])];
  sigTrain->Scale(1./sigTrain->Integral());
  bkgTrain->Scale(1./bkgTrain->Integral());
  sigTest->Scale(1./sigTest->Integral());
  bkgTest->Scale(1./bkgTest->Integral());
  bkgTrain->GetYaxis()->SetTitle("Probability");
  setHistStyle(sigTrain,bkgTrain,sigTest,bkgTest);
  // residual histograms
  TH1F *sigResid = (TH1F*)sigTest->Clone(getHistName(cat,b,var,"_sig_resid"));
  sigResid->Add(sigTrain,-1.);
  TH1F *bkgResid = (TH1F*)bkgTest->Clone(getHistName(cat,b,var,"_bkg_resid"));
  bkgResid->Add(bkgTrain,-1.);
  double max = getMaximum(sigTrain,bkgTrain,sigTest,bkgTest)*1.2;
  // canvas styles
  TCanvas *canv = new TCanvas("c","c",800,800);
  TPad *upperPad = new TPad("upper","",0.,0.33,1.,1.);
  TPad *lowerPad = new TPad("lower","",0.,0.,1.,0.33);
  upperPad->cd();
  // legend
  upperPad->SetTopMargin(0.2);
  upperPad->SetBottomMargin(0.07);
  TLegend *leg1 = new TLegend(0.1,0.85,0.4,0.98);
  leg1->SetFillColor(0);
  leg1->AddEntry(bkgTrain,"Background (train)","LF");
  leg1->AddEntry(sigTrain,"Signal (train)","LF");
  TLegend *leg2 = new TLegend(0.4,0.85,0.7,0.98);
  leg2->SetFillColor(0);
  leg2->AddEntry(bkgTest, "Background (test)","LEP");
  leg2->AddEntry(sigTest, "Signal (test)","LEP");
  TLegend *leg3 = new TLegend(0.7,0.85,1.0,0.98);
  leg3->SetFillColor(0);
  leg3->AddEntry("", Form("KS-Test=%4.2f",bkgTrain->KolmogorovTest(bkgTest)), "");
  leg3->AddEntry("", Form("KS-Test=%4.2f",sigTrain->KolmogorovTest(sigTest)), "");

  // upper plot
  bkgTrain->GetYaxis()->SetRangeUser(0.,max);
  bkgTrain->Draw("HISTF");
  sigTrain->Draw("HISTFSAME");
  bkgTest->Draw("LEPsame");
  sigTest->Draw("LEPsame");
  leg1->Draw();
  leg2->Draw();
  leg3->Draw();
  canv->cd();
  upperPad->Draw();

  // lower plot
  lowerPad->cd();
  lowerPad->SetBottomMargin(0.28);
  double residMax = 1.1*Max( Max(sigResid->GetMaximum(), bkgResid->GetMaximum()), Max(Abs(sigResid->GetMinimum()),Abs(bkgResid->GetMinimum())) );
  bkgResid->GetYaxis()->SetRangeUser(-1*residMax,residMax);
  bkgResid->GetXaxis()->SetTitle(var);
  bkgResid->GetYaxis()->SetTitle("Residual");
  bkgResid->GetXaxis()->SetLabelSize(0.12);
  bkgResid->GetYaxis()->SetLabelSize(0.12);
  bkgResid->GetXaxis()->SetTitleSize(0.14);
  bkgResid->GetYaxis()->SetTitleSize(0.14);
  bkgResid->GetXaxis()->SetTitleOffset(0.85);
  bkgResid->GetYaxis()->SetTitleOffset(0.45);
  bkgResid->Draw("LEP");
  TLine *line = new TLine();
  line->SetLineWidth(2);
  line->SetLineStyle(kDashed);
  line->DrawLine(bkgResid->GetBinLowEdge(1),0.,bkgResid->GetBinLowEdge(sigResid->GetNbinsX()+1),0.);
  bkgResid->Draw("LEPsame");
  sigResid->Draw("LEPsame");
  canv->cd();
  lowerPad->Draw();

  canv->Update();
  canv->Modified();
  TString canvName = Form("%s_%d_%s",cat.Data(),b,var.Data());
  canv->Print(Form("plots/%s/pdf/%s.pdf",name.Data(),canvName.Data()));
  canv->Print(Form("plots/%s/png/%s.png",name.Data(),canvName.Data()));
  canv->Print(Form("plots/%s/C/%s.C",name.Data(),canvName.Data()));
  delete canv;

}
