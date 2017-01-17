#include "TROOT.h"
#include "TFile.h"
#include "TColor.h"
#include "TMath.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLorentzVector.h"
#include "TPaveStats.h"

#include "PlotterBase.h"

using namespace std;
using namespace TMath;

PlotterBase::PlotterBase(TString _name, const Variables *_v):
	Analyser(_name),
  v(_v),
  normalise(true),
  normalisation(1.),
  outfilename("root/PlotterBaseOut.root"),
  residType(1)
{
  TH1F::SetDefaultSumw2();
  gROOT->ProcessLine(".x ~/bin/lhcbStyle.C");
  system(Form("mkdir -p plots/%s/pdf",name.Data()));
  system(Form("mkdir -p plots/%s/png",name.Data()));
  system(Form("mkdir -p plots/%s/C",name.Data()));
}

PlotterBase::~PlotterBase(){}

void PlotterBase::Init(){
	cout << Form("%-30s","PlotterBase::Init()") << " " << "Initialising Analyser (" << name << ")." << endl;
  defineHistograms();
  defineDrawingConfig();
  initHistograms();
}

void PlotterBase::Term(){
	cout << Form("%-30s","PlotterBase::Term()") << " " << "Terminating Analyser (" << name << ")." << endl;
  drawHistograms();
  saveHistograms();
  deleteHistograms();
}

bool PlotterBase::AnalyseEvent(){
  return fillHistograms();
}

void PlotterBase::addHist(TString name, TString title, int nbins, float xlow, float xhigh, TString legPos) {
  HistContainer hist;
  hist.name = name;
  hist.title = title;
  hist.nbins = nbins;
  hist.xlow = xlow;
  hist.xhigh = xhigh;
  hist.legPos = legPos;
  histList.push_back(hist);
}

void PlotterBase::addDrawOpt(TString name, TString title, int itype){
  drawOpts.push_back(DrawingConfig(name,title,itype));
}

void PlotterBase::addDrawOpt(TString name, TString title, int itype1, int itype2){
  drawOpts.push_back(DrawingConfig(name,title,itype1,itype2));
}

void PlotterBase::addDrawOpt(TString name, TString title, int itype1, int itype2, int itype3){
  drawOpts.push_back(DrawingConfig(name,title,itype1,itype2,itype3));
}

void PlotterBase::addDrawOpt(TString name, TString title, int itype1, int itype2, int itype3, int itype4){
  drawOpts.push_back(DrawingConfig(name,title,itype1,itype2,itype3,itype4));
}

void PlotterBase::setDrawOptDefaultFill(int color){
  drawOpts[drawOpts.size()-1].setDefaultFill(color);
}

void PlotterBase::setDrawOptDefaultLine(int color){
  drawOpts[drawOpts.size()-1].setDefaultLine(color);
}

void PlotterBase::setDrawOptDefaultPoint(int color){
  drawOpts[drawOpts.size()-1].setDefaultPoint(color);
}

void PlotterBase::setDrawOptFill(int color, int style){
  drawOpts[drawOpts.size()-1].setFill(color,style);
}

void PlotterBase::setDrawOptLine(int color, int style){
  drawOpts[drawOpts.size()-1].setLine(color,style);
}

void PlotterBase::setDrawOptPoint(int color, int style){
  drawOpts[drawOpts.size()-1].setPoint(color,style);
}

void PlotterBase::addResidOpt(pair<int,int> opt) {
  residOpts.push_back(opt);
}

void PlotterBase::initHistograms(){

  // loop over each drawing opt
  for (vector<DrawingConfig>::iterator opts = drawOpts.begin(); opts != drawOpts.end(); opts++){

    histMap.push_back(map<TString,TH1F*>());
    // loop over each histogram
    for (vector<HistContainer>::iterator hist = histList.begin(); hist != histList.end(); hist++){

      TString histName = opts->name + "_v" + hist->name;
      histMap[histMap.size()-1][hist->name] = new TH1F(histName,opts->name,hist->nbins,hist->xlow,hist->xhigh);
      histMap[histMap.size()-1][hist->name]->SetDirectory(0);
    }
  }
}

void PlotterBase::fillHist(TString name, double val){

  // loop over each drawing opt
  for (unsigned int i=0; i<drawOpts.size(); i++){
    // loop over the types in the drawopt
    for (vector<int>::iterator typ = drawOpts[i].itypes.begin(); typ != drawOpts[i].itypes.end(); typ++){
      if ( v->itype == *typ) {
        histMap[i][name]->Fill(val);
      }
    }
  }
}

void PlotterBase::fillHist(TString name, float val){
  fillHist(name,double(val));
}

void PlotterBase::fillHist(TString name, int val){
  fillHist(name,double(val));
}

void PlotterBase::fillHist(TString name, bool val){
  fillHist(name,double(val));
}

void PlotterBase::drawHistograms(){

	cout << Form("%-30s","PlotterBase::drawHistograms()") << " " << "Draw Histograms...." << endl;
  for (vector<HistContainer>::iterator hist = histList.begin(); hist != histList.end(); hist++){

    //cout << hist->name << endl;

    vector<TH1F*> histContainer;
    TLegend *legend;
    if (hist->legPos=="L") {
      legend = new TLegend(0.2,0.7,0.4,0.89);
    }
    else if (hist->legPos=="R") {
      legend = new TLegend(0.7,0.7,0.9,0.89);
    }
    else {
      cerr << "ERROR -- VariablesPlotter::drawHistograms() -- Unrecognised legPos option: " << hist->legPos << endl;
      exit(1);
    }
    legend->SetFillColor(0);
    double max = 0.;

    // loop over draw opts to draw them
    //
    for (unsigned int i=0; i<drawOpts.size(); i++){

      TH1F *theHist = histMap[i][hist->name];

      //cout << hist->name << " " << drawOpts[i].name << " " << theHist->GetName() << " " << theHist->GetEntries() << endl;
      // normalise
      if (normalise) {
        theHist->Scale(normalisation/theHist->Integral());
      }
      theHist->GetXaxis()->SetTitle(hist->title);

      // set drawing options
      drawOpts[i].SetStyle(theHist);

      // update max
      max = Max(max, theHist->GetMaximum());

      // add to legend
      legend->AddEntry(theHist, drawOpts[i].title, drawOpts[i].legOpt);

      // add to tempory container
      histContainer.push_back(theHist);
    }

    // resid hists here
    vector<TH1F*> residContainer;
    for (vector<pair<int,int> >::iterator opt=residOpts.begin(); opt!=residOpts.end(); opt++){

      // make residual histogram
      TH1F *numerator = histContainer[opt->first];
      TH1F *denominator = histContainer[opt->second];
      TString name = Form("resid_%s_over_%s",numerator->GetName(),denominator->GetName());
      TH1F *th1f = (TH1F*)numerator->Clone(name);
      if (residType==1) {
        th1f->Add(denominator,-1.);
      }
      else if (residType==2) {
        th1f->Divide(denominator);
      }
      else {
        cerr << "ERROR -- PlotterBase::drawHistograms() -- no such residType " << residType << endl;
        exit(1);
      }

      // set style to the same as numerator
      drawOpts[opt->first].SetStyle(th1f);

      // set max
      double residMax = Max(th1f->GetMaximum(),Abs(th1f->GetMinimum()));
      th1f->GetYaxis()->SetRangeUser(-1.4*residMax,1.4*residMax);

      // add to container
      residContainer.push_back(th1f);

    }
    //
    gStyle->SetOptStat(0);
    histContainer[0]->GetYaxis()->SetRangeUser(0,max*1.3);
    // get bin width
    TString ext="";
    if ( histContainer[0]->GetBinWidth(1) == histContainer[0]->GetBinWidth(2) ) {
      ext += Form(" / %3.1f",histContainer[0]->GetBinWidth(1));
    }
    histContainer[0]->GetYaxis()->SetTitle("Events"+ext);
    if (normalise) histContainer[0]->GetYaxis()->SetTitle("Probability"+ext);
    histContainer[0]->GetXaxis()->SetTitleOffset(0.8);
    histContainer[0]->GetYaxis()->SetTitleOffset(0.9);

    int canvWidth = 800;
    int canvHeight = 600;
    if (residContainer.size()>0) canvHeight = 900;
    TCanvas *canv = new TCanvas("c","c",canvWidth,canvHeight);

    // nominal plot first
    double nomPadLow = 0.;
    if (residContainer.size()>0) nomPadLow = 0.4;
    TPad *pad = new TPad("p_nom","p_nom",0.,nomPadLow,1.,1.);
    pad->SetLeftMargin(0.18);
    pad->cd();
    histContainer[0]->Draw(drawOpts[0].plotOpt);
    for (unsigned int i=1; i<histContainer.size(); i++){
      histContainer[i]->Draw(drawOpts[i].plotOpt+"same");
    }
    legend->Draw("same");
    canv->cd();
    pad->Draw();
		canv->Update();
		canv->Modified();

    // resid plot second
    if (residContainer.size()>0) {
      TPad *resPad = new TPad("p_res","p_res",0.,0.0,1.,nomPadLow);
      resPad->SetLeftMargin(0.18);
      resPad->SetTopMargin(0.01);
      resPad->SetBottomMargin(0.25);
      resPad->cd();
      // get relative height of resid pad to nominal pad
      double scale = pad->GetHNDC()/resPad->GetHNDC();
      double relscale = resPad->GetHNDC()/(pad->GetHNDC()+resPad->GetHNDC());
      // scale axis variables appropriately
      residContainer[0]->GetXaxis()->SetTitleSize(residContainer[0]->GetXaxis()->GetTitleSize()*scale);
      residContainer[0]->GetXaxis()->SetLabelSize(residContainer[0]->GetXaxis()->GetLabelSize()*scale);
      residContainer[0]->GetXaxis()->SetTitleOffset(residContainer[0]->GetXaxis()->GetTitleOffset()*relscale*1.5);
      residContainer[0]->GetXaxis()->SetLabelOffset(residContainer[0]->GetXaxis()->GetLabelOffset()*relscale);
      residContainer[0]->GetYaxis()->SetTitleSize(residContainer[0]->GetYaxis()->GetTitleSize()*scale);
      residContainer[0]->GetYaxis()->SetLabelSize(residContainer[0]->GetYaxis()->GetLabelSize()*scale);
      residContainer[0]->GetYaxis()->SetTitleOffset(residContainer[0]->GetYaxis()->GetTitleOffset()*relscale);
      residContainer[0]->GetYaxis()->SetLabelOffset(residContainer[0]->GetYaxis()->GetLabelOffset()*relscale);
      residContainer[0]->GetYaxis()->SetTitle("Residual");
      // draw them
      residContainer[0]->Draw(drawOpts[residOpts[0].first].plotOpt);
      // draw a line
      TLine line;
      line.SetLineWidth(3);
      line.SetLineColor(kBlack);
      line.SetLineStyle(kDashed);
      double yline = residType==1 ? 0. : 1. ;
      line.DrawLine(residContainer[0]->GetXaxis()->GetXmin(),yline,residContainer[0]->GetXaxis()->GetXmax(),yline);
      // redraw
      for (unsigned int i=0; i<residContainer.size(); i++){
        residContainer[i]->Draw(drawOpts[residOpts[i].first].plotOpt+"same");
      }

      // update the canvas
      canv->cd();
      resPad->Draw();
      pad->Draw();
      canv->Update();
      canv->Modified();
    }
    canv->Print(Form("plots/%s/pdf/%s.pdf",name.Data(),hist->name.Data()));
    canv->Print(Form("plots/%s/png/%s.png",name.Data(),hist->name.Data()));
    canv->Print(Form("plots/%s/C/%s.C",name.Data(),hist->name.Data()));
    delete canv;
    delete legend;

    histContainer.clear();
    for (unsigned int i=0; i<residContainer.size(); i++){
      delete residContainer[i];
    }
    residContainer.clear();
  }
}

void PlotterBase::saveHistograms(){

	cout << Form("%-30s","PlotterBase::saveHistograms()") << " " << "Saving Histograms to file " << outfilename << endl;

  TFile *outFile = new TFile(outfilename,"RECREATE");
  outFile->cd();
  for (unsigned int i=0; i<histMap.size(); i++){
    for (map<TString, TH1F*>::iterator it = histMap[i].begin(); it != histMap[i].end(); it++){
      it->second->Write();
    }
  }
  outFile->Close();
}

void PlotterBase::deleteHistograms(){

	cout << Form("%-30s","PlotterBase::deleteHistograms()") << " " << "Deleting Histograms...." << endl;
  for (unsigned int i=0; i<histMap.size(); i++){
    for (map<TString, TH1F*>::iterator it = histMap[i].begin(); it != histMap[i].end(); it++){
      delete it->second;
    }
  }
}
