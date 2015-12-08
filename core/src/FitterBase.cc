#include <iostream>
#include <list>

// ROOT stuff
#include "TFile.h"
#include "TTree.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TLine.h"
#include "TGraphAsymmErrors.h"
#include "TMath.h"
#include "TPaveText.h"
#include "TColor.h"
#include "TBox.h"

// roofit var types
#include "RooRealVar.h"
#include "RooConstVar.h"
#include "RooArgSet.h"
#include "RooArgList.h"

// roofit data types
#include "RooAbsData.h"
#include "RooDataSet.h"
#include "RooDataHist.h"

// roofit pdf types
#include "RooAddPdf.h"
#include "RooHistPdf.h"
#include "RooGaussian.h"
#include "RooCBShape.h"

// roofit utils
#include "RooPlot.h"
#include "RooMsgService.h"
#include "RooStats/SPlot.h"

// this package
#include "FitterBase.h"
#include "Utils.h"

using namespace std;
using namespace RooFit;
using namespace TMath;
using namespace Utils;

FitterBase::FitterBase(TString wsname, TString name, bool _verbose, bool _debug):
  fitterName(name),
  verbose(_verbose),
  debug(_debug),
  pBoxX(0.22),
  pDrawLog(false),
  pTitle(""),
  pResidType(0),
  pRedPull(-1)
{
  if ( !debug ) {
    RooMsgService::instance().setGlobalKillBelow(FATAL);
    RooMsgService::instance().setSilentMode(true);
  }
  gROOT->ProcessLine(".x ~/bin/lhcbStyle.C");
  gStyle->SetPalette(1);
  // make ws
	w = new RooWorkspace(wsname);
  // add itype and eventNumber (saved by default)
  w->factory( "itype[-9999,9999]" );
  w->factory(" eventNumber[0,10e10]" );
  // make observables
	RooArgSet *observables = new RooArgSet();
  observables->add( *w->var("itype") );
  observables->add( *w->var("eventNumber") );
	w->defineSet("observables",*observables);
	delete observables;
  // make constraints
  RooArgSet *constraints = new RooArgSet();
  w->defineSet("constraints",*constraints);
  delete constraints;
  colors.push_back(kRed);
  colors.push_back(kBlue);
  colors.push_back(kGreen+1);
  system(Form("mkdir -p plots/%s/png",fitterName.Data()));
  system(Form("mkdir -p plots/%s/pdf",fitterName.Data()));
  system(Form("mkdir -p plots/%s/C",fitterName.Data()));
}

FitterBase::~FitterBase(){
  delete w;
}

void FitterBase::loadCachedWorkspace(TString fname){

  if ( verbose || debug ) {
    print( "Loading cached workspace from file: "+fname );
  }
  // get name of and then delete old workspace
  TString wsname = w->GetName();
  delete w;
  // now load the cached version in its place
  TFile *cacheFile = TFile::Open(fname);
  w = (RooWorkspace*)cacheFile->Get(wsname);
}

void FitterBase::loadCachedData(TString fname) {

  if ( verbose || debug ) {
    print( "", "Loading cached data from file: "+fname );
  }

  TString wsname = w->GetName();
  TFile *cacheFile = TFile::Open(fname);
  RooWorkspace *cacheWS = (RooWorkspace*)cacheFile->Get(wsname);

  if ( ! cacheWS) error( "Can\'t fint workspace: " + wsname + " in file " + fname );

  list<RooAbsData*> dlist = cacheWS->allData();

  for ( list<RooAbsData*>::iterator ds = dlist.begin(); ds != dlist.end(); ds++ ) {

    RooAbsData *dset = *ds;
    if ( !dset ) error("meeh");
    w->import( *dset );
  }
}

void FitterBase::addObsVar(TString name, double min, double max){
  addObsVar(name,name,"",min,max);
}

void FitterBase::addObsVar(TString name, float min, float max){
  addObsVar(name,name,"",min,max);
}

void FitterBase::addObsVar(TString name, int min, int max){
  addObsVar(name,name,"",min,max);
}

void FitterBase::addObsVar(TString name, short int min, short int max) {
  addObsVar(name,name,"",min,max);
}

void FitterBase::addObsVar(TString name, long int min, long int max) {
  addObsVar(name,name,"",min,max);
}

void FitterBase::addObsVar(TString name, bool start_val) {
  addObsVar(name,name,"",start_val);
}

void FitterBase::addObsVar(TString name, TString title, TString unit, double min, double max){
	w->factory(Form("%s[%f,%f]",name.Data(),min,max));
	((RooArgSet*)w->set("observables"))->add(*w->var(name));
  w->var(name)->SetTitle(title);
  w->var(name)->setUnit(unit);
  obs_values_d[name] = -999.;
  if ( verbose || debug ) {
    print("Added observable "+name);
  }
}

void FitterBase::addObsVar(TString name, TString title, TString unit, float min, float max){
	w->factory(Form("%s[%f,%f]",name.Data(),min,max));
	((RooArgSet*)w->set("observables"))->add(*w->var(name));
  w->var(name)->SetTitle(title);
  w->var(name)->setUnit(unit);
  obs_values_f[name] = -999.;
  if ( verbose || debug ) {
    print("Added observable "+name);
  }
}

void FitterBase::addObsVar(TString name, TString title, TString unit, int min, int max){
	if ( name == "itype" ) {
    error("itype is already read you cannot add it. It will get saved with the output by default");
  }
  w->factory(Form("%s[%d,%d]",name.Data(),min,max));
	((RooArgSet*)w->set("observables"))->add(*w->var(name));
  w->var(name)->SetTitle(title);
  w->var(name)->setUnit(unit);
  obs_values_i[name] = -999;
  if ( verbose || debug ) {
    print("Added observable "+name);
  }
}

void FitterBase::addObsVar(TString name, TString title, TString unit, short int min, short int max) {
	w->factory(Form("%s[%d,%d]",name.Data(),min,max));
	((RooArgSet*)w->set("observables"))->add(*w->var(name));
  w->var(name)->SetTitle(title);
  w->var(name)->setUnit(unit);
  obs_values_s[name] = -99;
  if ( verbose || debug ) {
    print("Added observable "+name);
  }
}

void FitterBase::addObsVar(TString name, TString title, TString unit, long int min, long int max) {
	w->factory(Form("%s[%ld,%ld]",name.Data(),min,max));
	((RooArgSet*)w->set("observables"))->add(*w->var(name));
  w->var(name)->SetTitle(title);
  w->var(name)->setUnit(unit);
  obs_values_l[name] = -99;
  if ( verbose || debug ) {
    print("Added observable "+name);
  }
}

void FitterBase::addObsVar(TString name, TString title, TString unit, bool start_val) {
	w->factory(Form("%s[%d,%d]",name.Data(),0,1));
	((RooArgSet*)w->set("observables"))->add(*w->var(name));
  w->var(name)->SetTitle(title);
  w->var(name)->setUnit(unit);
  obs_values_b[name] = start_val;
  if ( verbose || debug ) {
    print("Added observable "+name);
  }
}

void FitterBase::setUnit(TString var, TString unit){
  if (!w->var(var)) cerr << "WARNING -- FitterBase::setUnit() -- no variable named " << var << " in workspace" << endl;
  w->var(var)->setUnit(unit);
}

void FitterBase::setBins(TString var, int bins){
  if (!w->var(var)) cerr << "WARNING -- FitterBase::setBins() -- no variable named " << var << " in workspace" << endl;
  w->var(var)->setBins(bins);
}

void FitterBase::addDataset(TString name, TString title, int itype) {
  dataSets.push_back(DataSet(name,title,w,itype));
  if ( verbose || debug ) print("Added dataset "+name);
}

void FitterBase::addDataset(TString name, TString title, int itype1, int itype2) {
  dataSets.push_back(DataSet(name,title,w,itype1,itype2));
  if ( verbose || debug ) print("Added dataset "+name);
}

void FitterBase::addDataset(TString name, TString title, int itype1, int itype2, int itype3) {
  dataSets.push_back(DataSet(name,title,w,itype1,itype2,itype3));
  if ( verbose || debug ) print("Added dataset "+name);
}

void FitterBase::addDataset(TString name, TString title, int itype1, int itype2, int itype3, int itype4) {
  dataSets.push_back(DataSet(name,title,w,itype1,itype2,itype3,itype4));
  if ( verbose || debug ) print("Added dataset "+name);
}

void FitterBase::addCut(TString name, double low, double high){
  cut_value_d[name] = make_pair(low,high);
  values_d[name] = -999.;
  if ( verbose || debug ) print(Form("Added cut %s in range (%f,%f)",name.Data(),low,high));
}

void FitterBase::addCut(TString name, float low, float high){
  cut_value_f[name] = make_pair(low,high);
  values_f[name] = -999.;
  if ( verbose || debug ) print(Form("Added cut %s in range (%f,%f)",name.Data(),low,high));
}

void FitterBase::addCut(TString name, int low, int high){
  cut_value_i[name] = make_pair(low,high);
  values_i[name] = -999;
  if ( verbose || debug ) print(Form("Added cut %s in range (%d,%d)",name.Data(),low,high));
}

void FitterBase::addCut(TString name, bool val){
  cut_value_b[name] = val;
  values_b[name] = false;
  if ( verbose || debug ) val ? print("Added cut "+name+" is true") : print("Added cut "+name+" is false");
}

void FitterBase::addRequirement(TString dset, TString name, double low, double high){
  dataSets[dataSets.size()-1].addRequirement(name, low, high);
  values_d[name] = -999.;
  if ( verbose || debug ) print(Form("Added requirement %s in range (%f,%f) for dataset %s",name.Data(),low,high,dset.Data()));
}

void FitterBase::addRequirement(TString dset, TString name, float low, float high){
  dataSets[dataSets.size()-1].addRequirement(name, low, high);
  values_f[name] = -999.;
  if ( verbose || debug ) print(Form("Added requirement %s in range (%f,%f) for dataset %s",name.Data(),low,high,dset.Data()));
}

void FitterBase::addRequirement(TString dset, TString name, int low, int high){
  dataSets[dataSets.size()-1].addRequirement(name, low, high);
  values_i[name] = -999;
  if ( verbose || debug ) print(Form("Added requirement %s in range (%d,%d) for dataset %s",name.Data(),low,high,dset.Data()));
}

void FitterBase::addRequirement(TString dset, TString name, int val){
  dataSets[dataSets.size()-1].addRequirement(name, val, val);
  values_i[name] = -999;
  if ( verbose || debug ) print(Form("Added requirement %s == %d for dataset %s",name.Data(),val,dset.Data()));
}

void FitterBase::addRequirement(TString dset, TString name, bool val){
  dataSets[dataSets.size()-1].addRequirement(name, val);
  values_b[name] = false;
  if ( verbose || debug ) val ? print("Added requirement "+name+" is true for dataset "+dset) : print("Added cut "+name+" is false for dataset "+dset);
}

void FitterBase::makeDatasets() {

  for (vector<DataSet>::iterator dataIt=dataSets.begin(); dataIt!=dataSets.end(); dataIt++){
    RooDataSet *data = new RooDataSet(dataIt->name,dataIt->title,*w->set("observables"));
    w->import(*data);
    delete data;
  }
}

void FitterBase::fillDatasets(TString fname, TString tname){

  print("Filling datasets....");

	TFile *tf = TFile::Open(fname);
	TTree *tree = (TTree*)tf->Get(tname);

  RooArgSet *observables = (RooArgSet*)w->set("observables");

  // default variables to read
	int itype;
  ULong64_t eventNumber;
	tree->SetBranchAddress("itype",&itype);
  tree->SetBranchAddress("eventNumber",&eventNumber);

  // set branch addresses from tree into container for observables
  //RooRealVar *parg;
	//TIterator *iter = observables->createIterator();
	//while ((parg = (RooRealVar*)iter->Next())){
		//obs_values[parg->GetName()] = -999.;
		//tree->SetBranchAddress(parg->GetName(),&obs_values[parg->GetName()]);
	//}
  //delete iter;
  for (map<TString,double>::iterator it = obs_values_d.begin(); it != obs_values_d.end(); it++) {
    tree->SetBranchAddress(it->first, &obs_values_d[it->first]);
  }
  for (map<TString,float>::iterator it = obs_values_f.begin(); it != obs_values_f.end(); it++) {
    tree->SetBranchAddress(it->first, &obs_values_f[it->first]);
  }
  for (map<TString,int>::iterator it = obs_values_i.begin(); it != obs_values_i.end(); it++) {
    tree->SetBranchAddress(it->first, &obs_values_i[it->first]);
  }
  for (map<TString,short int>::iterator it = obs_values_s.begin(); it != obs_values_s.end(); it++) {
    tree->SetBranchAddress(it->first, &obs_values_s[it->first]);
  }
  for (map<TString,long int>::iterator it = obs_values_l.begin(); it != obs_values_l.end(); it++) {
    tree->SetBranchAddress(it->first, &obs_values_l[it->first]);
  }
  for (map<TString,bool>::iterator it = obs_values_b.begin(); it != obs_values_b.end(); it++) {
    tree->SetBranchAddress(it->first, &obs_values_b[it->first]);
  }

  // set branch address for cut values and requirements
  setBranchAddresses(tree);

  // now loop entries
	for (int e=0; e<tree->GetEntries(); e++){
		tree->GetEntry(e);

    // set itype and eventNumber
    w->var("itype")->setVal( itype );
    w->var("eventNumber")->setVal( eventNumber );

    // print occasional entries in debug mode
    if ( debug && e%50000==0 ) {
      printEntry(e);
    }

    bool passes = true;

    // the variable cuts
    if ( !passesCuts() ) {
      continue;
    }

    // the observable range cuts
    RooRealVar *parg;
		TIterator *iter = observables->createIterator();
		while ((parg = (RooRealVar*)iter->Next())) {

      // have to skip itype and eventNumber
      if ( parg->GetName()==TString("itype") || parg->GetName()==TString("eventNumber") ) {
        passes=true;
      }
      // doubles
      else if ( obs_values_d.find(parg->GetName()) != obs_values_d.end() ) {
        if (obs_values_d[parg->GetName()] < w->var(parg->GetName())->getMin() || obs_values_d[parg->GetName()] > w->var(parg->GetName())->getMax()) {
          passes=false;
        }
        w->var(parg->GetName())->setVal(obs_values_d[parg->GetName()]);
      }
      // floats
      else if ( obs_values_f.find(parg->GetName()) != obs_values_f.end() ) {
        if (obs_values_f[parg->GetName()] < w->var(parg->GetName())->getMin() || obs_values_f[parg->GetName()] > w->var(parg->GetName())->getMax()) {
          passes=false;
        }
        w->var(parg->GetName())->setVal(obs_values_f[parg->GetName()]);
      }
      // ints
      else if ( obs_values_i.find(parg->GetName()) != obs_values_i.end() ) {
        if (obs_values_i[parg->GetName()] < w->var(parg->GetName())->getMin() || obs_values_i[parg->GetName()] > w->var(parg->GetName())->getMax()) {
          passes=false;
        }
        w->var(parg->GetName())->setVal(obs_values_i[parg->GetName()]);
      }
      // shorts
      else if ( obs_values_s.find(parg->GetName()) != obs_values_s.end() ) {
        if (obs_values_s[parg->GetName()] < w->var(parg->GetName())->getMin() || obs_values_s[parg->GetName()] > w->var(parg->GetName())->getMax()) {
          passes=false;
        }
        w->var(parg->GetName())->setVal(obs_values_s[parg->GetName()]);
      }
      // longs
      else if ( obs_values_l.find(parg->GetName()) != obs_values_l.end() ) {
        if (obs_values_l[parg->GetName()] < w->var(parg->GetName())->getMin() || obs_values_l[parg->GetName()] > w->var(parg->GetName())->getMax()) {
          passes=false;
        }
        w->var(parg->GetName())->setVal(obs_values_l[parg->GetName()]);
      }
      // bools
      else if ( obs_values_b.find(parg->GetName()) != obs_values_b.end() ) {
        w->var(parg->GetName())->setVal(obs_values_b[parg->GetName()]);
      }
      else {
        cerr << "ERROR -- FitterBase::fillDatasets() -- observable with name " << parg->GetName() << " has not be found in any of the observable maps" << endl;
        exit(1);
      }
		}

		if (!passes) continue;

    // dataset filling
    for (vector<DataSet>::iterator dataIt=dataSets.begin(); dataIt!=dataSets.end(); dataIt++){
      // check requirements
      if ( !passesRequirements(*dataIt) ) continue;
      // check itype
      for (vector<int>::iterator typIt=dataIt->itypes.begin(); typIt!=dataIt->itypes.end(); typIt++){
        if (itype==*typIt){
          w->data(dataIt->name)->add(*(w->set("observables")));
        }
      }
    }
  }
  tf->Close();
}

void FitterBase::fillOutputTrees( TFile *outf ) {

  TDirectory *dir = outf->mkdir("trees");
  dir->cd();

  list<RooAbsData*> dlist = w->allData();

  for ( list<RooAbsData*>::iterator ds = dlist.begin(); ds != dlist.end(); ds++ ) {

    // make a tree for each
    RooAbsData *dset = *ds;
    TTree *tree = new TTree( dset->GetName(), dset->GetName() );
    cout << Form("Filling output tree: %s",tree->GetName()) << endl;

    // default variables (itype, eventNumber)
    int itype;
    ULong64_t eventNumber;

    // make map container for variables
    map<TString, double> out_vars;

    // initialise map
    const RooArgSet *dset_args = dset->get();
    // iterate these and add them to obs and sw values as appropriate
    RooRealVar *var;
    TIterator *iter = dset_args->createIterator();
    while ( (var = (RooRealVar*)iter->Next()) ) {

      TString br_name = var->GetName();
      if ( br_name == "itype" || br_name == "eventNumber" ) continue;
      out_vars[br_name] = -999.;
    }

    // set tree branches
    tree->Branch( "itype",       &itype,       "itype/I" );
    tree->Branch( "eventNumber", &eventNumber, "eventNumber/l" );
    for ( map<TString,double>::iterator val = out_vars.begin(); val != out_vars.end(); val++ ) {
      tree->Branch( val->first ,  &val->second,  val->first+"/D" );
    }
    // fill tree
    for (int i=0; i<dset->numEntries(); i++) {
      const RooArgSet *dset_args = dset->get(i);
      // defaults
      itype = int( dset_args->getRealValue( "itype" ) );
      eventNumber = long( dset_args->getRealValue( "eventNumber" ) );
      // iterate these and add them to obs and sw values as appropriate
      RooRealVar *var;
      TIterator *iter = dset_args->createIterator();
      while ( (var = (RooRealVar*)iter->Next()) ) {
        TString br_name = var->GetName();
        out_vars[br_name] = var->getVal();
      }
      tree->Fill();
    }
    cout << "\t " << tree->GetEntries() << " entries" << endl;
    tree->Write();
  }
}

void FitterBase::save(TString fname){

  TFile *outf = new TFile(fname,"RECREATE");
  outf->cd();
  fillOutputTrees( outf );
  outf->cd();

  w->Write();
  if ( verbose || debug ) {
    cout << "Printout of workspace: " << endl;
    w->Print();
  }
  if ( saveObjsStore.size() > 0 ) {
    TDirectory *dir = outf->mkdir("residuals");
    dir->cd();
    for (vector<TObject*>::iterator obj=saveObjsStore.begin(); obj!=saveObjsStore.end(); obj++){
      (*obj)->Write();
    }
  }
  outf->Close();
  cout << "Written output to file: " << fname << endl;
}

void FitterBase::print(TString line, bool blank){
  TString lead = TString(Form("%-30s ","FitterBase()"));
  if (blank) lead = TString(Form("%-30s "," "));
  line = lead + line;
  cout << line << endl;
}

void FitterBase::defineParamSet(TString pdf){
  if ( ! w->pdf(pdf) ) {
    cout << "ERROR -- No valid pdf named " << pdf << " in workspace" << endl;
    exit(1);
  }
  RooArgSet *argset = w->pdf(pdf)->getParameters(*w->set("observables"));
  w->defineSet(Form("%s_params",pdf.Data()),*argset);
  delete argset;
}

void FitterBase::defineYieldSet(TString pdf_name){

  RooAbsPdf* pdf = w->pdf(pdf_name);
  RooArgSet *comps = pdf->getComponents();
  RooArgSet *allNonYieldParams = new RooArgSet();
  RooArgSet *nonYieldParams = new RooArgSet();
  // loop components
  RooAbsPdf *parg;
  TIterator *iter = comps->createIterator();
  while ( (parg = (RooAbsPdf*)iter->Next()) ) {
    if ( parg == w->pdf(pdf_name) ) continue;
    // also catch the yield constrained to another yield case (i.e. that it's not really a pdf)
    if ( ! w->pdf(parg->GetName()) ) continue;
    parg->Print();
    RooArgSet *argset = parg->getParameters(*w->set("observables"));
    nonYieldParams->add(*argset);
    allNonYieldParams->add(*argset);
    delete argset;
  }
  delete iter;
  allNonYieldParams->add(*w->set("observables"));
  RooArgSet *yieldParams = w->pdf(pdf_name)->getParameters(*allNonYieldParams);
  w->defineSet(Form("%s_yield_params",pdf_name.Data()),*yieldParams);
  w->defineSet(Form("%s_nonyield_params",pdf_name.Data()),*nonYieldParams);
  delete nonYieldParams;
  delete allNonYieldParams;
  delete yieldParams;
}

void FitterBase::saveSnapshot(TString name, TString pdf){

  RooArgSet *observables = (RooArgSet*)w->set("observables");
  RooArgSet *parameters = w->pdf(pdf)->getParameters(observables);
  w->saveSnapshot(name,*parameters);
  if ( verbose || debug ){
    print("Saved snapshot of pdf: "+pdf+" fit parameters to "+name);
  }
}

void FitterBase::loadSnapshot(TString name){
  w->loadSnapshot(name);
}

TCanvas* FitterBase::createCanvas(int canv_w, int canv_h){

  int top_x = canvs.size()*20;
  int top_y = canvs.size()*20;
  TString canvName = Form("c%d",int(canvs.size()));

  TCanvas *c = new TCanvas(canvName,canvName,top_x,top_y,canv_w,canv_h);
  canvs.push_back(c);
  return c;
}

// make multiple canvas plot
void FitterBase::plotMultiCanv( int n, TString name, int canv_w, int canv_h ) {

  int ncanvs = canvs.size();
  if ( ncanvs < n ) {
    cerr << "ERROR -- FitterBase::plotMultiCanv() -- there aren't enough canvases to multiplot " << n << " of them" << endl;
    exit(1);
  }

  TCanvas *canv = createCanvas(canv_w, canv_h);
  canv->DivideSquare(n);

  int iCanv=1;
  for (int i=ncanvs-n; i<ncanvs; i++) {
    canv->cd(iCanv);
    canvs[i]->DrawClonePad();
    iCanv++;
  }
  canv->Update();
  canv->Modified();

  canv->Print(Form("plots/%s/pdf/%s.pdf",fitterName.Data(),name.Data()));
  canv->Print(Form("plots/%s/png/%s.png",fitterName.Data(),name.Data()));
  canv->Print(Form("plots/%s/C/%s.C",    fitterName.Data(),name.Data()));
}

void FitterBase::plot(TString var, TString data, TString pdf, int resid, TString title, bool drawLeg){

  w->pdf(pdf) ?
    print("Plotting data: "+data+" and pdf: "+pdf+" in variable: "+var) :
    print("Plotting data: "+data+" in variable: "+var) ;

  RooPlot *plot = w->var(var)->frame(Title(data));
  plot->GetXaxis()->SetTitleOffset(0.8);
  plot->GetYaxis()->SetTitleOffset(0.75);

  TPaveText *text = new TPaveText(0.15,0.8,0.4,0.9,"ndc");
  text->SetFillColor(0);
  text->SetShadowColor(0);
  text->SetLineColor(0);
  text->AddText(title);

  TLegend *leg = new TLegend(0.65,0.7,0.89,0.89);
  leg->SetFillColor(0);

  if (w->data(data)) {
    w->data(data)->plotOn(plot);
    TObject *obj = plot->getObject(plot->numItems()-1);
    leg->AddEntry(obj,w->data(data)->GetTitle(),"LEP");
  }
  if (w->pdf(pdf)) {
    w->pdf(pdf)->plotOn(plot);
    TObject *obj = plot->getObject(plot->numItems()-1);
    leg->AddEntry(obj,w->pdf(pdf)->GetTitle(),"L");
  }
  RooHist *underHist;
  if (w->pdf(pdf) && w->data(data) && resid>0) {
    if (resid==1) underHist = plot->residHist();
    if (resid==2) underHist = plot->pullHist();
  }
  TCanvas *canv;
  if (resid==0) {
    canv = createCanvas();
    canv->SetBottomMargin(0.2);
    plot->Draw();
    if (drawLeg) leg->Draw("same");
    if ( title != "" ) text->Draw("same");
  }
  else {
    canv = createCanvas(800,800);
    TPad *upperPad = new TPad(Form("%s_upper",canv->GetName()),"",0.,0.33,1.,1.);
    TPad *lowerPad = new TPad(Form("%s_lower",canv->GetName()),"",0.,0.,1.,0.33);
    canv->cd();
    upperPad->Draw();
    lowerPad->Draw();
    // under hist style
    underHist->GetXaxis()->SetRangeUser(plot->GetXaxis()->GetXmin(), plot->GetXaxis()->GetXmax());
    underHist->GetXaxis()->SetTitle(plot->GetXaxis()->GetTitle());
    if (resid==1) underHist->GetYaxis()->SetTitle("Residual");
    if (resid==2) underHist->GetYaxis()->SetTitle("Pull");
    underHist->GetXaxis()->SetLabelSize(0.12);
    underHist->GetYaxis()->SetLabelSize(0.12);
    underHist->GetXaxis()->SetTitleSize(0.2);
    underHist->GetXaxis()->SetTitleOffset(0.7);
    underHist->GetYaxis()->SetTitleSize(0.18);
    underHist->GetYaxis()->SetTitleOffset(0.38);
    // canv style and plot
    plot->GetXaxis()->SetTitle("");
    upperPad->SetBottomMargin(0.1);
    upperPad->cd();
    plot->Draw();
    if (drawLeg) leg->Draw("same");
    if ( title != "" ) text->Draw("same");
    canv->cd();
    lowerPad->SetTopMargin(0.05);
    lowerPad->SetBottomMargin(0.35);
    lowerPad->cd();
    underHist->Draw("AP");
    if ( resid==2 ) { // pull hist
      double ymin = underHist->GetYaxis()->GetXmin();
      double ymax = underHist->GetYaxis()->GetXmax();
      double yrange = Max( Abs( ymin ), Abs( ymax ) );
      underHist->GetYaxis()->SetRangeUser( -1.*yrange, 1.*yrange );

      double xmin = plot->GetXaxis()->GetXmin();
      double xmax = plot->GetXaxis()->GetXmax();

      TColor *mycol3sig = gROOT->GetColor( kGray );
      mycol3sig->SetAlpha(0.5);
      TColor *mycol2sig = gROOT->GetColor( kGray+1 );
      mycol2sig->SetAlpha(0.5);
      TColor *mycol1sig = gROOT->GetColor( kGray+2 );
      mycol1sig->SetAlpha(0.5);

      TBox box3sig;
      box3sig.SetFillColor( mycol3sig->GetNumber() );
      //box3sig.SetFillColorAlpha( kGray, 0.5 );
      box3sig.SetFillStyle(1001);
      box3sig.DrawBox( xmin, -3., xmax, 3.);
      TBox box2sig;
      box2sig.SetFillColor( mycol2sig->GetNumber() );
      //box2sig.SetFillColorAlpha( kGray+1, 0.5 );
      box2sig.SetFillStyle(1001);
      box2sig.DrawBox( xmin, -2., xmax, 2.);
      TBox box1sig;
      box1sig.SetFillColor( mycol1sig->GetNumber() );
      //box1sig.SetFillColorAlpha( kGray+2, 0.5 );
      box1sig.SetFillStyle(1001);
      box1sig.DrawBox( xmin, -1., xmax, 1.);

      TLine lineErr;
      lineErr.SetLineWidth(1);
      lineErr.SetLineColor(kBlue-9);
      lineErr.SetLineStyle(2);
      lineErr.DrawLine(plot->GetXaxis()->GetXmin(),1.,plot->GetXaxis()->GetXmax(),1.);
      lineErr.DrawLine(plot->GetXaxis()->GetXmin(),-1.,plot->GetXaxis()->GetXmax(),-1.);
      lineErr.DrawLine(plot->GetXaxis()->GetXmin(),2.,plot->GetXaxis()->GetXmax(),2.);
      lineErr.DrawLine(plot->GetXaxis()->GetXmin(),-2.,plot->GetXaxis()->GetXmax(),-2.);
      lineErr.DrawLine(plot->GetXaxis()->GetXmin(),3.,plot->GetXaxis()->GetXmax(),3.);
      lineErr.DrawLine(plot->GetXaxis()->GetXmin(),-3.,plot->GetXaxis()->GetXmax(),-3.);
    }
    TLine line;
    line.SetLineWidth(3);
    line.SetLineColor(kBlue);
    line.DrawLine(plot->GetXaxis()->GetXmin(),0.,plot->GetXaxis()->GetXmax(),0.);
    underHist->Draw("Psame");
    // draw red pull points
    if (pRedPull>-1) {
      RooHist *redPull = new RooHist();
      int newp=0;
      for (int p=0; p<underHist->GetN(); p++) {
        double x,y;
        underHist->GetPoint(p,x,y);
        if ( TMath::Abs(y)>pRedPull ) {
          redPull->SetPoint(newp,x,y);
          redPull->SetPointError(newp,0.,0.,underHist->GetErrorYlow(p),underHist->GetErrorYhigh(p));
          newp++;
        }
      }
      redPull->SetLineWidth(underHist->GetLineWidth());
      redPull->SetMarkerStyle(underHist->GetMarkerStyle());
      redPull->SetMarkerSize(underHist->GetMarkerSize());
      redPull->SetLineColor(kRed);
      redPull->SetMarkerColor(kRed);
      redPull->Draw("Psame");
    }
  }
  canv->Update();
  canv->Modified();
  canv->Print(Form("plots/%s/pdf/v%s_d%s_p%s.pdf",fitterName.Data(),var.Data(),data.Data(),pdf.Data()));
  canv->Print(Form("plots/%s/png/v%s_d%s_p%s.png",fitterName.Data(),var.Data(),data.Data(),pdf.Data()));
  canv->Print(Form("plots/%s/C/v%s_d%s_p%s.C",    fitterName.Data(),var.Data(),data.Data(),pdf.Data()));

  delete text;
  delete leg;
}

void FitterBase::plot(TString var, vector<PlotComponent> plotComps, TString fname, const RooArgList *params) {

  print("Plotting following components in variable: "+var);
  for (unsigned int i=0; i<plotComps.size(); i++){
    print("\t "+plotComps[i].name,true);
  }

  int ycanv         = pResidType ? 800 : 600 ;
  double ypadchange = pResidType ? 0.33 : 0. ;
  TCanvas *canv = createCanvas(800,ycanv);
  TPad *upperPad = new TPad(Form("%s_upper",canv->GetName()),"",0.,ypadchange,1.,1.);
  TPad *lowerPad = new TPad(Form("%s_lower",canv->GetName()),"",0.,0.,1.,ypadchange);
  canv->cd();
  upperPad->Draw();
  lowerPad->Draw();
  upperPad->SetLeftMargin(0.18);
  lowerPad->SetLeftMargin(0.18);
  RooPlot *plot = w->var(var)->frame();
  TString xtitle = w->var(var)->GetTitle();
  if (TString(w->var(var)->getUnit())!=TString("")) xtitle = Form("%s (%s)",w->var(var)->GetTitle(),w->var(var)->getUnit());
  plot->GetXaxis()->SetTitle(xtitle);
  plot->GetXaxis()->SetTitleOffset(0.8);
  plot->GetYaxis()->SetTitleOffset(0.8);

  TLegend *leg;
  if ( plotComps.size()<7 ) {
    double yleg = pResidType ? 0.5 : 0.6 ;
    leg = new TLegend(0.6,yleg,0.9,0.9);
  }
  else {
    double yleg = pResidType ? 0.4 : 0.5 ;
    leg = new TLegend(0.6,yleg,0.9,0.9);
  }
  leg->SetFillColor(0);

  for (unsigned int i=0; i<plotComps.size(); i++){
    plotComps[i].plotOn(w,plot,leg);
  }

  // set specifics for residuals
  RooHist *underHist;
  if ( pResidType>0 ) {
    if (pResidType==1) underHist = plot->residHist();
    if (pResidType==2) underHist = plot->pullHist();
    underHist->GetXaxis()->SetTitle(plot->GetXaxis()->GetTitle());
    plot->GetXaxis()->SetTitle("");
    upperPad->SetBottomMargin(0.1);
  }

  upperPad->cd();
  plot->Draw();
  if ( leg->GetNRows() > 0 ) {
    leg->Draw("same");
  }

  // if requested plot title
  if ( pTitle != "" ) {
    TPaveText *text = new TPaveText(0.24,0.8,0.47,0.9,"ndc");
    text->SetFillColor(0);
    text->SetShadowColor(0);
    text->SetLineColor(0);
    text->AddText(pTitle);
    text->Draw("same");
  }

  // if requested also plot parameter values
  if (params) {
    double top = 0.9;
    if ( pTitle != "" ) {
      top = 0.8;
    }
    double bottom = top-(0.05*params->getSize());
    TPaveText *pNames = new TPaveText(pBoxX,bottom,pBoxX+0.10,top,"ndc");
    TPaveText *pVals = new TPaveText(pBoxX+0.10,bottom,pBoxX+0.19,top,"ndc");
    TPaveText *pErrs = new TPaveText(pBoxX+0.19,bottom,pBoxX+0.23,top,"ndc");
    pNames->SetFillColor(0);
    pNames->SetShadowColor(0);
    pNames->SetLineColor(0);
    pNames->SetTextSize(0.03);
    pNames->SetTextAlign(11);
    pVals->SetFillColor(0);
    pVals->SetShadowColor(0);
    pVals->SetLineColor(0);
    pVals->SetTextSize(0.03);
    pVals->SetTextAlign(11);
    pErrs->SetFillColor(0);
    pErrs->SetShadowColor(0);
    pErrs->SetLineColor(0);
    pErrs->SetTextSize(0.03);
    pErrs->SetTextAlign(11);
    RooRealVar *parg;
    TIterator *iter = params->createIterator();
    while ((parg = (RooRealVar*)iter->Next())){
	    pNames->AddText(Form("%-10s",parg->GetTitle()));
      pVals->AddText(Form("= %7.1f ",parg->getVal()));
      pErrs->AddText(Form("#pm %-5.1f",parg->getError()));
    }
    pNames->Draw("same");
    pVals->Draw("same");
    pErrs->Draw("same");
  }

  // plot resid if required
  if ( pResidType>0 ) {
    underHist->GetXaxis()->SetRangeUser(plot->GetXaxis()->GetXmin(), plot->GetXaxis()->GetXmax());
    if (pResidType==1) underHist->GetYaxis()->SetTitle("Residual");
    if (pResidType==2) underHist->GetYaxis()->SetTitle("Pull");
    underHist->GetXaxis()->SetLabelSize(0.12);
    underHist->GetYaxis()->SetLabelSize(0.12);
    underHist->GetXaxis()->SetTitleSize(0.2);
    underHist->GetXaxis()->SetTitleOffset(0.7);
    underHist->GetYaxis()->SetTitleSize(0.18);
    underHist->GetYaxis()->SetTitleOffset(0.38);
    lowerPad->SetTopMargin(0.05);
    lowerPad->SetBottomMargin(0.35);
    lowerPad->cd();
    underHist->Draw("AP");
    // if pull plot
    if (pResidType==2) {
      double ymin = underHist->GetYaxis()->GetXmin();
      double ymax = underHist->GetYaxis()->GetXmax();
      double yrange = Max( Abs( ymin ), Abs( ymax ) );
      underHist->GetYaxis()->SetRangeUser( -1.*yrange, 1.*yrange );

      double xmin = plot->GetXaxis()->GetXmin();
      double xmax = plot->GetXaxis()->GetXmax();

      TColor *mycol3sig = gROOT->GetColor( kGray );
      mycol3sig->SetAlpha(0.5);
      TColor *mycol2sig = gROOT->GetColor( kGray+1 );
      mycol2sig->SetAlpha(0.5);
      TColor *mycol1sig = gROOT->GetColor( kGray+2 );
      mycol1sig->SetAlpha(0.5);

      TBox box3sig;
      box3sig.SetFillColor( mycol3sig->GetNumber() );
      //box3sig.SetFillColorAlpha( kGray, 0.5 );
      box3sig.SetFillStyle(1001);
      box3sig.DrawBox( xmin, -3., xmax, 3.);
      TBox box2sig;
      box2sig.SetFillColor( mycol2sig->GetNumber() );
      //box2sig.SetFillColorAlpha( kGray+1, 0.5 );
      box2sig.SetFillStyle(1001);
      box2sig.DrawBox( xmin, -2., xmax, 2.);
      TBox box1sig;
      box1sig.SetFillColor( mycol1sig->GetNumber() );
      //box1sig.SetFillColorAlpha( kGray+2, 0.5 );
      box1sig.SetFillStyle(1001);
      box1sig.DrawBox( xmin, -1., xmax, 1.);

      TLine lineErr;
      lineErr.SetLineWidth(1);
      lineErr.SetLineColor(kBlue-9);
      lineErr.SetLineStyle(2);
      lineErr.DrawLine(plot->GetXaxis()->GetXmin(),1.,plot->GetXaxis()->GetXmax(),1.);
      lineErr.DrawLine(plot->GetXaxis()->GetXmin(),-1.,plot->GetXaxis()->GetXmax(),-1.);
      lineErr.DrawLine(plot->GetXaxis()->GetXmin(),2.,plot->GetXaxis()->GetXmax(),2.);
      lineErr.DrawLine(plot->GetXaxis()->GetXmin(),-2.,plot->GetXaxis()->GetXmax(),-2.);
      lineErr.DrawLine(plot->GetXaxis()->GetXmin(),3.,plot->GetXaxis()->GetXmax(),3.);
      lineErr.DrawLine(plot->GetXaxis()->GetXmin(),-3.,plot->GetXaxis()->GetXmax(),-3.);
    }
    // line at zero
    TLine line;
    line.SetLineWidth(3);
    line.SetLineColor(kBlue);
    line.DrawLine(plot->GetXaxis()->GetXmin(),0.,plot->GetXaxis()->GetXmax(),0.);
    underHist->Draw("Psame");
    // draw red pull points
    if (pRedPull>-1) {
      RooHist *redPull = new RooHist();
      int newp=0;
      for (int p=0; p<underHist->GetN(); p++) {
        double x,y;
        underHist->GetPoint(p,x,y);
        if ( TMath::Abs(y)>pRedPull ) {
          redPull->SetPoint(newp,x,y);
          redPull->SetPointError(newp,0.,0.,underHist->GetErrorYlow(p),underHist->GetErrorYhigh(p));
          newp++;
        }
      }
      redPull->SetLineWidth(underHist->GetLineWidth());
      redPull->SetMarkerStyle(underHist->GetMarkerStyle());
      redPull->SetMarkerSize(underHist->GetMarkerSize());
      redPull->SetLineColor(kRed);
      redPull->SetMarkerColor(kRed);
      redPull->Draw("Psame");
    }
  }

  canv->Update();
  canv->Modified();
  canv->Print(Form("plots/%s/pdf/%s.pdf",fitterName.Data(),fname.Data()));
  canv->Print(Form("plots/%s/png/%s.png",fitterName.Data(),fname.Data()));
  canv->Print(Form("plots/%s/C/%s.C",    fitterName.Data(),fname.Data()));

  if ( pDrawLog ) {
    TCanvas *canvLog = createCanvas(800,ycanv);
    TPad *upperPadLog = new TPad(Form("%s_upper",canv->GetName()),"",0.,ypadchange,1.,1.);
    TPad *lowerPadLog = new TPad(Form("%s_lower",canv->GetName()),"",0.,0.,1.,ypadchange);
    canvLog->cd();
    upperPadLog->Draw();
    lowerPadLog->Draw();
    upperPadLog->SetLeftMargin(0.18);
    lowerPadLog->SetLeftMargin(0.18);
    RooPlot *logplot = (RooPlot*)plot->Clone();
    logplot->GetYaxis()->SetRangeUser(1.,plot->GetMaximum()*2);
    upperPadLog->cd();
    logplot->Draw();
    upperPadLog->SetLogy();
    if ( pResidType>0 ) {
      upperPadLog->SetBottomMargin(0.1);
      lowerPadLog->SetTopMargin(0.05);
      lowerPadLog->SetBottomMargin(0.35);
      lowerPadLog->cd();
      underHist->Draw("AP");
      // if pull
      if (pResidType==2) {
        double ymin = underHist->GetYaxis()->GetXmin();
        double ymax = underHist->GetYaxis()->GetXmax();
        double yrange = Max( Abs( ymin ), Abs( ymax ) );
        underHist->GetYaxis()->SetRangeUser( -1.*yrange, 1.*yrange );

        double xmin = plot->GetXaxis()->GetXmin();
        double xmax = plot->GetXaxis()->GetXmax();

        TColor *mycol3sig = gROOT->GetColor( kGray );
        mycol3sig->SetAlpha(0.5);
        TColor *mycol2sig = gROOT->GetColor( kGray+1 );
        mycol2sig->SetAlpha(0.5);
        TColor *mycol1sig = gROOT->GetColor( kGray+2 );
        mycol1sig->SetAlpha(0.5);

        TBox box3sig;
        box3sig.SetFillColor( mycol3sig->GetNumber() );
        //box3sig.SetFillColorAlpha( kGray, 0.5 );
        box3sig.SetFillStyle(1001);
        box3sig.DrawBox( xmin, -3., xmax, 3.);
        TBox box2sig;
        box2sig.SetFillColor( mycol2sig->GetNumber() );
        //box2sig.SetFillColorAlpha( kGray+1, 0.5 );
        box2sig.SetFillStyle(1001);
        box2sig.DrawBox( xmin, -2., xmax, 2.);
        TBox box1sig;
        box1sig.SetFillColor( mycol1sig->GetNumber() );
        //box1sig.SetFillColorAlpha( kGray+2, 0.5 );
        box1sig.SetFillStyle(1001);
        box1sig.DrawBox( xmin, -1., xmax, 1.);

        TLine lineErr;
        lineErr.SetLineWidth(1);
        lineErr.SetLineColor(kBlue-9);
        lineErr.SetLineStyle(2);
        lineErr.DrawLine(plot->GetXaxis()->GetXmin(),1.,plot->GetXaxis()->GetXmax(),1.);
        lineErr.DrawLine(plot->GetXaxis()->GetXmin(),-1.,plot->GetXaxis()->GetXmax(),-1.);
        lineErr.DrawLine(plot->GetXaxis()->GetXmin(),2.,plot->GetXaxis()->GetXmax(),2.);
        lineErr.DrawLine(plot->GetXaxis()->GetXmin(),-2.,plot->GetXaxis()->GetXmax(),-2.);
        lineErr.DrawLine(plot->GetXaxis()->GetXmin(),3.,plot->GetXaxis()->GetXmax(),3.);
        lineErr.DrawLine(plot->GetXaxis()->GetXmin(),-3.,plot->GetXaxis()->GetXmax(),-3.);
      }
      // line at zero
      TLine line;
      line.SetLineWidth(3);
      line.SetLineColor(kBlue);
      line.DrawLine(plot->GetXaxis()->GetXmin(),0.,plot->GetXaxis()->GetXmax(),0.);
      underHist->Draw("Psame");
    }
    canvLog->Update();
    canvLog->Modified();
    canvLog->Print(Form("plots/%s/pdf/%s_log.pdf",fitterName.Data(),fname.Data()));
    canvLog->Print(Form("plots/%s/png/%s_log.png",fitterName.Data(),fname.Data()));
    canvLog->Print(Form("plots/%s/C/%s_log.C",    fitterName.Data(),fname.Data()));

    TCanvas *totCanv = createCanvas(1600,ycanv);
    totCanv->Divide(2,1);
    totCanv->cd(1);
    canv->DrawClonePad();
    totCanv->cd(2);
    canvLog->DrawClonePad();
    totCanv->Update();
    totCanv->Modified();
    totCanv->Print(Form("plots/%s/pdf/%s_tot.pdf",fitterName.Data(),fname.Data()));
    totCanv->Print(Form("plots/%s/png/%s_tot.png",fitterName.Data(),fname.Data()));
    totCanv->Print(Form("plots/%s/C/%s_tot.C",fitterName.Data(),fname.Data()));
  }
}

void FitterBase::plot2D(TString xvar, TString yvar, TString obj) {

  // create histogram
  TH2F* th2 = w->var(xvar)->createHistogram(Form("th2_%s_vs_%s_%s",xvar.Data(),yvar.Data(),obj.Data()),*w->var(yvar));

  // style
  TColor *mycol = gROOT->GetColor( kGray+3 );
  mycol->SetAlpha(0.4);
  th2->SetLineColor( mycol->GetNumber() );
  th2->SetLineWidth(1);
  th2->GetYaxis()->SetNdivisions( th2->GetXaxis()->GetNdivisions() );

  TCanvas *canv = createCanvas(1600,600);
  canv->Divide(2,1);

  canv->cd(1);
  gPad->SetPhi(40);
  gPad->SetTheta(30);
  gPad->SetTopMargin(0.01);
  gPad->SetRightMargin(0.05);
  gPad->SetBottomMargin(0.2);

  //
  // Draw lego style
  // check for data first
  //
  if (w->data(obj)) {
    w->data(obj)->fillHistogram(th2,RooArgSet(*w->var(xvar),*w->var(yvar)));
    th2->Draw("lego2fbbb");
  }
  if (w->pdf(obj)) {
    w->pdf(obj)->fillHistogram(th2,RooArgSet(*w->var(xvar),*w->var(yvar)));
    th2->Draw("SURF2");
  }

  canv->cd(2);
  gPad->SetRightMargin(0.15);
  gPad->SetLeftMargin(0.18);

  TH2F* th2_flat = (TH2F*)th2->Clone(Form("%s_flat",th2->GetName()));
  th2_flat->GetXaxis()->SetTitleOffset(0.8);
  th2_flat->GetYaxis()->SetTitleOffset(1.);
  th2_flat->Draw("colz");

  canv->Update();
  canv->Modified();
  //
  TString fname = Form("th2_%s_vs_%s_%s",xvar.Data(),yvar.Data(),obj.Data());
  canv->Print(Form("plots/%s/C/%s.C",    fitterName.Data(),fname.Data()));
  canv->Print(Form("plots/%s/pdf/%s.pdf",    fitterName.Data(),fname.Data()));
  canv->Print(Form("plots/%s/png/%s.png",    fitterName.Data(),fname.Data()));

}

void FitterBase::fit(TString pdf, TString data, bool constrained, double rangeLow, double rangeHigh) {

  bool range = false;
  if ( rangeLow > -999 && rangeHigh > -999 && rangeHigh>rangeLow ) range = true;

  print("Fitting pdf: "+pdf+" to dataset: "+data);

  if ( ! w->pdf(pdf) || ! w->data(data) ) {
    if ( verbose || debug ) w->Print();
    if ( ! w->pdf(pdf) ) cerr << "ERROR -- FitterBase::fit() -- pdf " << pdf.Data() << " not found in workspace" << endl;
    if ( ! w->data(data) ) cerr << "ERROR -- FitterBase::fit() -- data " << data.Data() << " not found in workspace" << endl;
  }

  if (w->pdf(pdf) && w->data(data)) {
    if ( constrained ){
      if ( range )
        w->pdf(pdf)->fitTo(*w->data(data),Constrained(),Range(rangeLow,rangeHigh));
      else
        w->pdf(pdf)->fitTo(*w->data(data),Constrained());
    }
    else {
      if ( range )
        w->pdf(pdf)->fitTo(*w->data(data),Range(rangeLow,rangeHigh));
      else
        w->pdf(pdf)->fitTo(*w->data(data));
    }
  }
  saveSnapshot(Form("%s_fit",pdf.Data()),pdf);
  if ( verbose && w->set(Form("%s_params",pdf.Data())) ) {
    print("Fitted values: ");
    w->set(Form("%s_params",pdf.Data()))->Print("v");
  }
}

void FitterBase::sfit(TString pdf_name, TString data_name, TString yields_name, TString nonyields_name) {

  // check pdf, data and yields exist
  //
  if ( !w->loadSnapshot(Form("%s_fit",pdf_name.Data())) ) {
    error( Form("FitterBase::sfit() -- No snapshot %s_fit found. You must fit the pdf to the data before sweighting",pdf_name.Data()) );
  }
  w->loadSnapshot(Form("%s_fit",pdf_name.Data()));
  // pdf
  RooAbsPdf *pdf    = w->pdf(pdf_name);
  if ( !pdf ) {
    error( Form("FitterBase::sfit() -- pdf name \'%s\' is NULL",pdf_name.Data()) );
  }
  // data
  RooDataSet *data  = (RooDataSet*)w->data(data_name);
  if ( !data ) {
    error( Form("FitterBase::sfit() -- data name \'%s\' is NULL",data_name.Data()) );
  }
  // yields
  if ( yields_name=="" ) {
    yields_name = Form("%s_yield_params",pdf_name.Data());
  }
  RooArgSet *yields = (RooArgSet*)w->set(yields_name);
  if ( !yields ) {
    error( Form("FitterBase::sfit() -- yields name \'%s\' is NULL",yields_name.Data()) );
  }
  // nonyields
  if ( nonyields_name=="" ) {
    nonyields_name = Form("%s_nonyield_params",pdf_name.Data());
  }
  RooArgSet *nonyields = (RooArgSet*)w->set(nonyields_name);
  if ( !nonyields ) {
    error( Form("FitterBase::sfit() -- nonyields name \'%s\' is NULL",nonyields_name.Data()) );
  }

  print("Performing sfit of pdf: "+pdf_name+" to data: "+data_name);

  pdf->fitTo(*data, Extended()); // shouldn't need another fit as should have loaded snapshot but it doesn't work without it
  nonyields->setAttribAll("Constant");

  if ( verbose || debug ) {
    print("Will use these yields in splot:");
    debug ?
      yields->Print("v") :
      yields->Print()    ;
    print("Will not float these in splot:");
    debug ?
      nonyields->Print("v") :
      nonyields->Print()    ;
  }

  //print("What's happening?");
  //TString a;
  //cin >> a;

  RooStats::SPlot *sData = new RooStats::SPlot(Form("%s_sfit",data->GetName()),Form("%s sfit",data->GetTitle()), *data, pdf, RooArgList(*yields));
  w->import(*sData);
  w->import(*data,Rename(Form("%s_wsweights",data->GetName())));
  print("Created SPlot with name "+TString(sData->GetName()));
  print("Renamed data with sweights to: "+data_name+"_wsweights");
  delete sData;
}

void FitterBase::sproject(TString data_name, TString var_name) {

  data_name = data_name+"_wsweights";
  RooDataSet *data = (RooDataSet*)w->data(data_name);
  if ( !data ) {
    cerr << "ERROR -- FitterBase::sproject() -- data name \'" << data_name << "\' is NULL" << endl;
    exit(1);
  }

  print("Projecting sweighted data with weight: "+var_name);

  RooDataSet *swdata = new RooDataSet(Form("%s_proj_%s",data->GetName(),var_name.Data()),Form("%s sweight proj %s",data->GetTitle(),var_name.Data()), data, *data->get(),0,var_name+"_sw");
  w->import(*swdata);
  delete swdata;
}

void FitterBase::freeze(TString pdf){
	if ( ! w->set(Form("%s_params",pdf.Data())) ) {
    cout << "ERROR -- No valid set named " << pdf << "_params in workspace" << endl;
    exit(1);
  }
  ((RooArgSet*)w->set(Form("%s_params",pdf.Data())))->setAttribAll("Constant");
  if ( verbose || debug ) {
    print("Frozen parameter values of pdf: "+pdf);
    if ( debug ) {
      w->set(Form("%s_params",pdf.Data()))->Print("v");
    }
  }
}

double FitterBase::integral(TString pdf, TString var, TString scale, double low, double high){

  double scaleValue = 1.;
  if ( w->var(scale) ) {
    scaleValue = w->var(scale)->getVal();
  }
  else if ( w->data(scale) ) {
    scaleValue = w->data(scale)->sumEntries();
  }
  else if ( scale=="") {
    scaleValue = 1.;
  }
  else {
    cout << "ERROR -- FitterBase::integral() -- nothing called " << scaleValue << " in workspace" << endl;
    exit(1);
  }

  bool hasRange = (low<-998 && high<-998) ? false : true ;
  if (hasRange){
    w->var(var)->setRange("IntegralRange",low,high);
  }

  RooAbsReal *integral;
  if (hasRange) {
    integral = w->pdf(pdf)->createIntegral(RooArgSet(*w->var(var)),NormSet(RooArgSet(*w->var(var))),Range("IntegralRange"));
  }
  else {
    integral = w->pdf(pdf)->createIntegral(RooArgSet(*w->var(var)),NormSet(RooArgSet(*w->var(var))));
  }
  double value = scaleValue*integral->getVal();
  delete integral;
  return value;
}

void FitterBase::splot(TString var, TString data, TString title, int bins){
  vector<TString> temp;
  splot(var, data, temp, title, bins);
}

void FitterBase::splot(TString var, TString data, vector<TString> compDsets, TString title, int bins) {

  if (!w->data(data)) {
    cerr << "ERROR -- FitterBase::splot() -- no sweighted data exists" << endl;
    exit(1);
  }
  if ( bins==-1 ){
    bins = w->var(var)->getBins();
  }
  TCanvas *canv = createCanvas();

  TPaveText *text = new TPaveText(0.15,0.8,0.4,0.9,"ndc");
  text->SetFillColor(0);
  text->SetShadowColor(0);
  text->SetLineColor(0);
  text->AddText(title);

  TLegend *leg = new TLegend(0.6,0.6,0.9,0.9);
  leg->SetFillColor(0);
  RooPlot *splot = w->var(var)->frame();
  splot->SetTitle(Form("%s projection",w->var(var)->GetTitle()));
  splot->GetXaxis()->SetTitleOffset(0.8);
  splot->GetYaxis()->SetTitleOffset(0.7);
  TString xtitle = w->var(var)->GetTitle();
  if (TString(w->var(var)->getUnit())!=TString("")) xtitle = Form("%s (%s)",w->var(var)->GetTitle(),w->var(var)->getUnit());
  splot->GetXaxis()->SetTitle(w->var(var)->GetTitle());

  w->data(data)->plotOn(splot,Binning(bins));
  RooHist *dh = (RooHist*)splot->getObject(splot->numItems()-1);
  leg->AddEntry(dh,w->data(data)->GetTitle(),"LEP");
  storeSPlotProjection(dh,Form("%s_v%s",data.Data(),var.Data()));

  vector<RooHist*> compHists;
  for (unsigned int i=0; i<compDsets.size(); i++){
    TString dset = compDsets[i];
    w->data(dset)->plotOn(splot,Binning(bins),MarkerColor(colors[i]),LineColor(colors[i]),Rescale(w->data(data)->sumEntries()/w->data(dset)->sumEntries()));
    RooHist *sh = (RooHist*)splot->getObject(splot->numItems()-1);
    leg->AddEntry(sh,w->data(dset)->GetTitle(),"LEP");
    compHists.push_back(sh);
    storeSPlotProjection(sh,Form("%s_v%s",dset.Data(),var.Data()));
    storeSPlotRatio(dh,sh,Form("resid_%s_o_%s_v%s",data.Data(),dset.Data(),var.Data()));
  }

  splot->Draw();
  leg->Draw("same");
  text->Draw("same");
  canv->Update();
  canv->Modified();
  canv->Print(Form("plots/%s/pdf/splot_v%s.pdf",fitterName.Data(),var.Data()));
  canv->Print(Form("plots/%s/png/splot_v%s.png",fitterName.Data(),var.Data()));
  canv->Print(Form("plots/%s/C/splot_v%s.C",    fitterName.Data(),var.Data()));

  if (compHists.size()>0) {
    TCanvas *canvResid = createCanvas();
    for (unsigned int i=0; i<compHists.size(); i++){
      RooHist *residHist = new RooHist(*dh,*compHists[i],1.,-1.,RooAbsData::SumW2);
      residHist->SetTitle("Residual");
      residHist->GetYaxis()->SetTitle("Data-MC");
      residHist->GetXaxis()->SetTitle(xtitle);
      residHist->GetXaxis()->SetTitleOffset(0.8);
      residHist->GetYaxis()->SetTitleOffset(0.7);
      residHist->SetLineColor(colors[i]);
      residHist->SetMarkerColor(colors[i]);
      if (i==0) {
        residHist->Draw();
      }
      else {
        residHist->Draw("same");
      }
      residHist->SetDrawOption("AP");
      canvResid->Update();
      canvResid->Modified();
    }
    TLine *l = new TLine();
    l->SetLineColor(kBlack);
    l->SetLineStyle(kDashed);
    l->SetLineWidth(3);
    l->DrawLine(w->var(var)->getMin(),0.,w->var(var)->getMax(),0.);
    canvResid->Update();
    canvResid->Modified();
    canvResid->Print(Form("plots/%s/pdf/splot_resid_v%s.pdf",fitterName.Data(),var.Data()));
    canvResid->Print(Form("plots/%s/png/splot_resid_v%s.png",fitterName.Data(),var.Data()));
    canvResid->Print(Form("plots/%s/C/splot_resid_v%s.C",    fitterName.Data(),var.Data()));
  }

}

void FitterBase::storeSPlotProjection(RooHist *rh, TString name){

  TGraphAsymmErrors *gr = new TGraphAsymmErrors();
  gr->SetName(name);
  double x,y;
  for (int p=0; p<rh->GetN(); p++){
    rh->GetPoint(p,x,y);
    gr->SetPoint(p,x,y);
    gr->SetPointError(p,rh->GetErrorXlow(p),rh->GetErrorXhigh(p),rh->GetErrorYlow(p),rh->GetErrorYhigh(p));
  }
  saveObjsStore.push_back(gr);

}

void FitterBase::storeSPlotRatio(RooHist *h1, RooHist *h2, TString name){

  if (h1->GetN()!=h2->GetN()){
    cerr << "ERROR -- FitterBase::storeSPlotRatio() -- two histograms do not have same number of points" << endl;
    exit(1);
  }

  TGraphAsymmErrors *gr = new TGraphAsymmErrors();
  gr->SetName(name);
  double x1, y1, x2, y2;
  for (int p=0; p<h1->GetN(); p++){
    h1->GetPoint(p,x1,y1);
    h2->GetPoint(p,x2,y2);
    assert(Abs(x1-x2)<0.001);
    double val = y1/y2;
    double y1errL = h1->GetErrorYlow(p);
    double y1errH = h1->GetErrorYhigh(p);
    double y2errL = h2->GetErrorYlow(p);
    double y2errH = h2->GetErrorYhigh(p);
    double errL = val * Sqrt( (y1errL/y1)*(y1errL/y1) + (y2errL/y2)*(y2errL/y2)  );
    double errH = val * Sqrt( (y1errH/y1)*(y1errH/y1) + (y2errH/y2)*(y2errH/y2)  );
    if (Abs(y1)<1.e-3 || Abs(y2)<1.e-3) {
      val = 1.;
      errL = 10.;
      errH = 10.;
    }
    gr->SetPoint(p,x1,val);
    gr->SetPointError(p,h1->GetErrorXlow(p),h1->GetErrorXhigh(p),errL,errH);
  }
  saveObjsStore.push_back(gr);
}

void FitterBase::setBranchAddresses(TTree *tree) {
  // doubles
  for (map<TString,double>::iterator it = values_d.begin(); it != values_d.end(); it++){
    //it->second = -999.;
    tree->SetBranchAddress(it->first, &it->second);
    if ( verbose || debug ) print("Added cut branch "+it->first);
  }
  // floats
  for (map<TString,float>::iterator it = values_f.begin(); it != values_f.end(); it++){
    //it->second = -999.;
    tree->SetBranchAddress(it->first, &it->second);
    if ( verbose || debug ) print("Added cut branch "+it->first);
  }
  // ints
  for (map<TString,int>::iterator it = values_i.begin(); it != values_i.end(); it++){
    //it->second = -999;
    tree->SetBranchAddress(it->first, &it->second);
    if ( verbose || debug ) print("Added cut branch "+it->first);
  }
  // bools
  for (map<TString,bool>::iterator it = values_b.begin(); it != values_b.end(); it++){
    //it->second = 0;
    tree->SetBranchAddress(it->first, &it->second);
    if ( verbose || debug ) print("Added cut branch "+it->first);
  }
}

bool FitterBase::passesCuts() {
  // doubles
  for (map<TString,pair<double,double> >::iterator it = cut_value_d.begin(); it != cut_value_d.end(); it++){
    if ( values_d[it->first] < it->second.first)  return false;
    if ( values_d[it->first] > it->second.second) return false;
  }
  // floats
  for (map<TString,pair<float,float> >::iterator it = cut_value_f.begin(); it != cut_value_f.end(); it++){
    if ( values_f[it->first] < it->second.first)  return false;
    if ( values_f[it->first] > it->second.second) return false;
  }
  // ints
  for (map<TString,pair<int,int> >::iterator it = cut_value_i.begin(); it != cut_value_i.end(); it++){
    if ( values_i[it->first] < it->second.first)  return false;
    if ( values_i[it->first] > it->second.second) return false;
  }
  // bools
  for (map<TString,bool>::iterator it = cut_value_b.begin(); it != cut_value_b.end(); it++){
    if ( values_b[it->first] != it->second)  return false;
  }
  return true;
}

bool FitterBase::passesRequirements(DataSet &dset){
  // doubles
  for (map<TString,pair<double,double> >::iterator it = dset.requirements_d.begin(); it != dset.requirements_d.end(); it++){
    if ( values_d[it->first] < it->second.first)  return false;
    if ( values_d[it->first] > it->second.second) return false;
  }
  // floats
  for (map<TString,pair<float,float> >::iterator it = dset.requirements_f.begin(); it != dset.requirements_f.end(); it++){
    if ( values_f[it->first] < it->second.first)  return false;
    if ( values_f[it->first] > it->second.second) return false;
  }
  // ints
  for (map<TString,pair<int,int> >::iterator it = dset.requirements_i.begin(); it != dset.requirements_i.end(); it++){
    if ( values_i[it->first] < it->second.first)  return false;
    if ( values_i[it->first] > it->second.second) return false;
  }
  // bools
  for (map<TString,bool>::iterator it = dset.requirements_b.begin(); it != dset.requirements_b.end(); it++){
    if ( values_b[it->first] != it->second)  return false;
  }
  return true;
}

void FitterBase::printEntry(int entry){

  print(Form("----- Printing contents of entry %-6d: -----",entry));
  print("   DEFAULT:", true);
  print(Form(" \t %-20s %d", "itype", int(w->var("itype")->getVal())),true);
  print(Form(" \t %-20s %ld", "eventNumber", long(w->var("eventNumber")->getVal())),true);
  print("   OBSERVABLES:",true);
  for (map<TString,double>::iterator val = obs_values_d.begin(); val != obs_values_d.end(); val++){
    print(Form(" \t %-20s %f",val->first.Data(),val->second),true);
  }
  for (map<TString,float>::iterator val = obs_values_f.begin(); val != obs_values_f.end(); val++){
    print(Form(" \t %-20s %f",val->first.Data(),val->second),true);
  }
  for (map<TString,int>::iterator val = obs_values_i.begin(); val != obs_values_i.end(); val++){
    print(Form(" \t %-20s %d",val->first.Data(),val->second),true);
  }
  for (map<TString,short int>::iterator val = obs_values_s.begin(); val != obs_values_s.end(); val++){
    print(Form(" \t %-20s %d",val->first.Data(),val->second),true);
  }
  for (map<TString,long int>::iterator val = obs_values_l.begin(); val != obs_values_l.end(); val++){
    print(Form(" \t %-20s %ld",val->first.Data(),val->second),true);
  }
  for (map<TString,bool>::iterator val = obs_values_b.begin(); val != obs_values_b.end(); val++){
    print(Form(" \t %-20s %d",val->first.Data(),val->second),true);
  }
  print("   CUTS/REQUIREMENTS:",true);
  for (map<TString,double>::iterator val = values_d.begin(); val != values_d.end(); val++){
    print(Form(" \t %-20s %f",val->first.Data(),val->second),true);
  }
  for (map<TString,float>::iterator val = values_f.begin(); val != values_f.end(); val++){
    print(Form(" \t %-20s %f",val->first.Data(),val->second),true);
  }
  for (map<TString,int>::iterator val = values_i.begin(); val != values_i.end(); val++){
    print(Form(" \t %-20s %d",val->first.Data(),val->second),true);
  }
  for (map<TString,bool>::iterator val = values_b.begin(); val != values_b.end(); val++){
    print(Form(" \t %-20s %d",val->first.Data(),val->second),true);
  }
  print("----------------------------------------------",true);

}

void FitterBase::addParameter(TString name, double val) {
  RooRealVar *var = new RooRealVar(name, name, val);
  var->setConstant(false);
  var->removeRange();
  w->import(*var);
  if ( debug ) {
    print("Added variable with name "+name);
  }
  delete var;
}

void FitterBase::addParameter(TString name, double val, double low, double high) {
  addParameter(name, val);
  w->var(name)->setRange(low,high);
}

void FitterBase::addParameter(TString name, double low, double high) {
  double val = low + (high-low)/2.;
  addParameter(name, val, low, high);
}

void FitterBase::addConstraint(TString var) {
  if (!w->var(var)) {
    cerr << "WARNING -- FitterBase::addConstraint() -- variable " << var << " not in workspace. Ignoring." << endl;
    return;
  }
  addConstraint(var, w->var(var)->getVal(), w->var(var)->getError());
}

void FitterBase::addConstraint(TString var, double mean, double sigma) {
  TString name = var + "_constraint";
  if (w->pdf(name)) {
    cerr << "WARNING -- FitterBase::addConstraint() -- constraint pdf " << name << " already in workspace. Ignoring." << endl;
    return;
  }
  if (!w->var(var)) {
    cerr << "WARNING -- FitterBase::addConstraint() -- variable " << var << " not in workspace. Ignoring." << endl;
    return;
  }
  RooGaussian *constraint = new RooGaussian(name,name,*w->var(var),RooConst(mean),RooConst(sigma));
  w->import(*constraint);
  delete constraint;
  ((RooArgSet*)w->set("constraints"))->add(*w->pdf(name));
}

void FitterBase::makeGaussConstrainedParameter(TString name, TString par, double mean, double sigma) {
  if (w->pdf(name)) {
    cerr << "WARNING -- FitterBase::makeGaussConstrainedParameter() -- gaussian " << name << "already in workspace. Ignoring." << endl;
    return;
  }
  if (!w->var(par)) {
    cerr << "WARNING -- FitterBase::makeGaussConstrainedParameter() -- parameter " << par << " not found in workspace. Ignoring." << endl;
    return;
  }
  w->factory( Form("Gaussian::%s(%s,%f,%f)",name.Data(),par.Data(),mean,sigma) );
}

void FitterBase::makeGaussConstraintFromParameter(TString name, TString par) {

  if (w->pdf(name)) {
    cerr << "WARNING -- FitterBase::makeGaussConstrainedParameter() -- gaussian " << name << "already in workspace. Ignoring." << endl;
    return;
  }
  if (!w->var(par)) {
    cerr << "WARNING -- FitterBase::makeGaussConstrainedParameter() -- parameter " << par << " not found in workspace. Ignoring." << endl;
    return;
  }
  w->factory( Form("Gaussian::%s(%s,%f,%f)",name.Data(),par.Data(),w->var(par)->getVal(),w->var(par)->getError()) );

}

void FitterBase::makeGaussianPDF(TString name, TString par) {

  addParameter(name+"_mean", 5300);
  addParameter(name+"_sigma", 20);
  RooGaussian *pdf = new RooGaussian(name,name,*w->var(par),*w->var(name+"_mean"),*w->var(name+"_sigma"));
  w->import(*pdf);
  delete pdf;
  defineParamSet(name);
}

void FitterBase::makeGaussianSumPDF(TString name, TString par, int n) {

  // several gaussians with the same mean
  addParameter(name+"_mean", w->var(par)->getMin(), w->var(par)->getMax() );

  RooArgList *pdfs = new RooArgList();
  RooArgList *fracs = new RooArgList();

  for ( int i=0; i<n; i++ ) {

    TString sigma_name = Form("%s_sigma%d",name.Data(),i);
    addParameter( sigma_name, 20 );

    TString gaus_name = Form("%s_gaus%d",name.Data(),i);
    RooGaussian *gaus = new RooGaussian( gaus_name, gaus_name, *w->var(par), *w->var(name+"_mean"), *w->var(sigma_name) );
    pdfs->add(*gaus);

    if ( i < n-1 ) {
      TString frac_name = Form("%s_f%d",name.Data(),i);
      addParameter( frac_name, 1./double(n), 0., 1. );
      fracs->add(*w->var(frac_name) );
    }
  }

  RooAddPdf *pdf = new RooAddPdf(name,name,*pdfs,*fracs);
  w->import(*pdf);
  delete pdf;
  delete pdfs;
  delete fracs;
  defineParamSet(name);
}

void FitterBase::makeDisjointGaussianSumPDF(TString name, TString par, int n) {

  // several gaussians with different means

  RooArgList *pdfs = new RooArgList();
  RooArgList *fracs = new RooArgList();

  for ( int i=0; i<n; i++ ) {

    TString mean_name = Form("%s_mean%d",name.Data(),i);
    addParameter( mean_name, w->var(par)->getMin(), w->var(par)->getMax() );

    TString sigma_name = Form("%s_sigma%d",name.Data(),i);
    addParameter( sigma_name, 100, 0, 2.*(w->var(par)->getMax() - w->var(par)->getMin()) );

    TString gaus_name = Form("%s_gaus%d",name.Data(),i);
    RooGaussian *gaus = new RooGaussian( gaus_name, gaus_name, *w->var(par), *w->var(mean_name), *w->var(sigma_name) );
    pdfs->add(*gaus);

    if ( i < n-1 ) {
      TString frac_name = Form("%s_f%d",name.Data(),i);
      addParameter( frac_name, 1./double(n), 0., 1. );
      fracs->add(*w->var(frac_name) );
    }
  }

  RooAddPdf *pdf = new RooAddPdf(name,name,*pdfs,*fracs);
  w->import(*pdf);
  delete pdf;
  delete pdfs;
  delete fracs;
  defineParamSet(name);
}

void FitterBase::makeCBPDF(TString name, TString par) {

  addParameter(name+"_mean"  , 5300 );
  addParameter(name+"_sigma" , 20   );
  addParameter(name+"_alpha" , 0.5  );
  addParameter(name+"_n"     , 0.5  );
  RooCBShape *pdf = new RooCBShape(name,name,*w->var(par),*w->var(name+"_mean"),*w->var(name+"_sigma"),*w->var(name+"_alpha"),*w->var(name+"_n"));
  w->import(*pdf);
  delete pdf;
  defineParamSet(name);
}

void FitterBase::makeDoubleCBPDF(TString name, TString par) {

  addParameter(name+"_mean"   , w->var(par)->getMin(), w->var(par)->getMax() );
  addParameter(name+"_sigma"  ,  20   );
  addParameter(name+"_alpha1" ,  0.5 ,   0., 20. );
  addParameter(name+"_n1"     ,  0.5 ,   0., 20. );
  addParameter(name+"_alpha2" , -0.5 , -20., 0.  );
  addParameter(name+"_n2"     ,  0.5 ,   0., 20. );
  addParameter(name+"_f1"     ,  0.5 ,   0., 1.  );
  RooCBShape *cb1 = new RooCBShape(name+"_cb1",name+"_cb1",*w->var(par),*w->var(name+"_mean"),*w->var(name+"_sigma"),*w->var(name+"_alpha1"),*w->var(name+"_n1"));
  RooCBShape *cb2 = new RooCBShape(name+"_cb2",name+"_cb2",*w->var(par),*w->var(name+"_mean"),*w->var(name+"_sigma"),*w->var(name+"_alpha2"),*w->var(name+"_n2"));
  RooAddPdf *pdf = new RooAddPdf(name, name, RooArgList(*cb1,*cb2), RooArgList(*w->var(name+"_f1")));
  w->import(*pdf);
  delete pdf;
  delete cb1;
  delete cb2;
  defineParamSet(name);
}

void FitterBase::makeDisjointDoubleCBPDF(TString name, TString par) {

  addParameter(name+"_mean1"   , w->var(par)->getMin(), w->var(par)->getMax() );
  addParameter(name+"_mean2"   , w->var(par)->getMin(), w->var(par)->getMax() );
  addParameter(name+"_sigma1"  ,  100   );
  addParameter(name+"_sigma2"  ,  100   );
  addParameter(name+"_alpha1" ,  0.5 ,   0., 20. );
  addParameter(name+"_n1"     ,  0.5 ,   0., 20. );
  addParameter(name+"_alpha2" , -0.5 , -20., 0.  );
  addParameter(name+"_n2"     ,  0.5 ,   0., 20. );
  addParameter(name+"_f1"     ,  0.5 ,   0., 1.  );
  RooCBShape *cb1 = new RooCBShape(name+"_cb1",name+"_cb1",*w->var(par),*w->var(name+"_mean1"),*w->var(name+"_sigma1"),*w->var(name+"_alpha1"),*w->var(name+"_n1"));
  RooCBShape *cb2 = new RooCBShape(name+"_cb2",name+"_cb2",*w->var(par),*w->var(name+"_mean2"),*w->var(name+"_sigma2"),*w->var(name+"_alpha2"),*w->var(name+"_n2"));
  RooAddPdf *pdf = new RooAddPdf(name, name, RooArgList(*cb1,*cb2), RooArgList(*w->var(name+"_f1")));
  w->import(*pdf);
  delete pdf;
  delete cb1;
  delete cb2;
  defineParamSet(name);
}

void FitterBase::makeSumPDF(TString name, vector<TString> pdfList) {

  RooArgList *pdfs = new RooArgList();
  RooArgList *fracs = new RooArgList();

  // make pdf and fraction list
  for (int i=0; i<pdfList.size(); i++) {
    if ( !w->pdf(pdfList[i]) ) {
      print( "FitterBase", "ERROR - no pdf "+pdfList[i]+" found in workspace" );
    }
    pdfs->add( *w->pdf(pdfList[i]) );
    if ( i<pdfList.size()-1 ) {
      TString frac_name = Form("%s_f%d",name.Data(),i);
      addParameter(frac_name , 1./double(pdfList.size()), 0., 1.);
      fracs->add( *w->var(frac_name) );
    }
  }
  RooAddPdf *pdf = new RooAddPdf(name, name, *pdfs, *fracs );
  w->import(*pdf);
  delete pdf;
  delete pdfs;
  delete fracs;
  defineParamSet(name);

}

void FitterBase::makeSumPDF(TString name, TString pdf1, TString pdf2) {

  vector<TString> pdfs;
  pdfs.push_back(pdf1);
  pdfs.push_back(pdf2);
  makeSumPDF( name, pdfs );
}

void FitterBase::makeSumPDF(TString name, TString pdf1, TString pdf2, TString pdf3) {

  vector<TString> pdfs;
  pdfs.push_back(pdf1);
  pdfs.push_back(pdf2);
  pdfs.push_back(pdf3);
  makeSumPDF( name, pdfs );
}

void FitterBase::makeSumPDF(TString name, TString pdf1, TString pdf2, TString pdf3, TString pdf4) {

  vector<TString> pdfs;
  pdfs.push_back(pdf1);
  pdfs.push_back(pdf2);
  pdfs.push_back(pdf3);
  pdfs.push_back(pdf4);
  makeSumPDF( name, pdfs );
}

void FitterBase::makeSumPDF(TString name, TString pdf1, TString pdf2, TString pdf3, TString pdf4, TString pdf5) {

  vector<TString> pdfs;
  pdfs.push_back(pdf1);
  pdfs.push_back(pdf2);
  pdfs.push_back(pdf3);
  pdfs.push_back(pdf4);
  pdfs.push_back(pdf5);
  makeSumPDF( name, pdfs );
}

void FitterBase::makeTemplatePDF(TString name, TString par, TString data) {

  if ( !w->var(par) ) {
    print("FitterBase", "ERROR - no such parameter "+par+" in workspace" );
  }
  if ( !w->data(data) ) {
    print("FitterBase", "ERROR - no such dataset "+data+" in workspace" );
  }

  RooDataSet *dset = (RooDataSet*)w->data(data);
  RooDataHist *dhist = new RooDataHist(data+"Hist","",RooArgSet(*w->var(par)), *dset);
  RooHistPdf *pdf = new RooHistPdf(name,"",RooArgSet(*w->var(par)),*dhist);
  w->import(*pdf);
  delete pdf;
  defineParamSet(name);
}
