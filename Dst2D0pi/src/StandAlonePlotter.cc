#include "StandAlonePlotter.h"

using namespace std;
using namespace RooFit;
using namespace TMath;

Dst2D0pi::StandAlonePlotter::StandAlonePlotter( RooWorkspace *_w, TString _pName ):
  w(_w),
  pName(_pName),
  pResidType(0),
  pTitle(""),
  pTitleX(0.24),
  pBoxX(0.22),
  pDrawLog(false),
  pRedPull(-1)
{
  system(Form("mkdir -p plots/%s/pdf",pName.Data()));
  system(Form("mkdir -p plots/%s/png",pName.Data()));
  system(Form("mkdir -p plots/%s/C",pName.Data()));

  gROOT->ProcessLine(".x ~/Scratch/lhcb/lhcbStyle.C");
}

Dst2D0pi::StandAlonePlotter::~StandAlonePlotter(){}

TCanvas* Dst2D0pi::StandAlonePlotter::createCanvas(int w, int h){

  int top_x = canvs.size()*20;
  int top_y = canvs.size()*20;
  TString canvName = Form("c%d",int(canvs.size()));

  TCanvas *c = new TCanvas(canvName,canvName,top_x,top_y,w,h);
  canvs.push_back(c);
  return c;
}

void Dst2D0pi::StandAlonePlotter::drawBox( double xmin, double xmax, int color ) {
  boxes.push_back( DrawBox( xmin, xmax, color ) );
}

void Dst2D0pi::StandAlonePlotter::plot(TString var, vector<PlotComponent> plotComps, TString fname, const RooArgList *params, double xlow, double xhigh) {

  if ( xlow < 0 ) xlow = w->var(var)->getMin();
  if ( xhigh < 0 ) xhigh = w->var(var)->getMax();

  //print("Plotting following components in variable: "+var);
  //for (unsigned int i=0; i<plotComps.size(); i++){
    //print("\t "+plotComps[i].name,true);
  //}

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
  RooPlot *plot = w->var(var)->frame(Range(xlow,xhigh));
  TString xtitle = w->var(var)->GetTitle();
  if (TString(w->var(var)->getUnit())!=TString("")) xtitle = Form("%s (%s)",w->var(var)->GetTitle(),w->var(var)->getUnit());
  plot->GetXaxis()->SetTitle(xtitle);
  plot->GetXaxis()->SetTitleOffset(0.8);
  plot->GetYaxis()->SetTitleOffset(1.0);

  TLegend *leg;
  if ( plotComps.size()<7 ) {
    double yleg = pResidType ? 0.5 : 0.6 ;
    leg = new TLegend(0.65,yleg,0.9,0.9);
  }
  else {
    double yleg = pResidType ? 0.4 : 0.5 ;
    leg = new TLegend(0.65,yleg,0.9,0.9);
  }
  leg->SetFillColor(0);

  for (unsigned int i=0; i<plotComps.size(); i++){
    plotComps[i].plotOn(w,plot,leg);
  }

  // set specifics for residuals
  RooHist *underHist;
  RooHist *redPull;
  if ( pResidType>0 ) {
    if (pResidType==1) underHist = plot->residHist();
    if (pResidType==2) underHist = plot->pullHist();
    underHist->GetXaxis()->SetTitle(plot->GetXaxis()->GetTitle());
    plot->GetXaxis()->SetTitle("");
    upperPad->SetBottomMargin(0.1);
  }

  upperPad->cd();
  plot->Draw();

  // draw boxes if any
  for ( unsigned int i=0; i<boxes.size(); i++ ) {
    TBox *box = new TBox();
    TColor *color = gROOT->GetColor( boxes[i].color );
    color->SetAlpha(0.6);
    box->SetLineColor( color->GetNumber() );
    box->SetFillColor( color->GetNumber() );
    box->DrawBox( boxes[i].xmin, plot->GetMinimum(), boxes[i].xmax, plot->GetMaximum() );
  }

  if ( leg->GetNRows() > 0 ) {
    leg->Draw("same");
  }

  TPaveText *text = new TPaveText(pTitleX,0.8,pTitleX+0.23,0.9,"ndc");
  // if requested plot title
  if ( pTitle != "" ) {
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
  TH1F *dummyUnderHist;
  if ( pResidType>0 ) {
    if (pResidType==1) underHist->GetYaxis()->SetTitle("Residual");
    if (pResidType==2) underHist->GetYaxis()->SetTitle("Pull");
    lowerPad->SetTopMargin(0.05);
    lowerPad->SetBottomMargin(0.35);
    lowerPad->cd();
    underHist->Draw("AP");
    // draw dummy histogram as the range is ruined when the PDF range is narrow
    dummyUnderHist = new TH1F("dummy","",1,plot->GetXaxis()->GetXmin(), plot->GetXaxis()->GetXmax());
    dummyUnderHist->GetYaxis()->SetRangeUser( underHist->GetYaxis()->GetXmin(), underHist->GetYaxis()->GetXmax() );
    dummyUnderHist->Draw("AXIS");
    dummyUnderHist->GetXaxis()->SetLabelSize(0.12);
    dummyUnderHist->GetYaxis()->SetLabelSize(0.12);
    dummyUnderHist->GetXaxis()->SetTitleSize(0.2);
    dummyUnderHist->GetXaxis()->SetTitleOffset(0.7);
    dummyUnderHist->GetYaxis()->SetTitleSize(0.18);
    dummyUnderHist->GetYaxis()->SetTitleOffset(0.38);
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
      redPull = new RooHist();
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
  canv->Print(Form("plots/%s/pdf/%s.pdf",pName.Data(),fname.Data()));
  canv->Print(Form("plots/%s/png/%s.png",pName.Data(),fname.Data()));
  canv->Print(Form("plots/%s/C/%s.C",    pName.Data(),fname.Data()));

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
    if ( leg->GetNRows() > 0 ) {
      leg->Draw("same");
    }
    if ( pTitle != "" ) {
      text->Draw("same");
    }
    upperPadLog->SetLogy();
    if ( pResidType>0 ) {
      upperPadLog->SetBottomMargin(0.1);
      lowerPadLog->SetTopMargin(0.05);
      lowerPadLog->SetBottomMargin(0.35);
      lowerPadLog->cd();
      //underHist->Draw("AP");
      dummyUnderHist->Draw("AXIS");
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
      if ( pRedPull>0 ) {
        redPull->Draw("Psame");
      }
    }
    canvLog->Update();
    canvLog->Modified();
    canvLog->Print(Form("plots/%s/pdf/%s_log.pdf",pName.Data(),fname.Data()));
    canvLog->Print(Form("plots/%s/png/%s_log.png",pName.Data(),fname.Data()));
    canvLog->Print(Form("plots/%s/C/%s_log.C",    pName.Data(),fname.Data()));

    TCanvas *totCanv = createCanvas(1600,ycanv);
    totCanv->Divide(2,1);
    totCanv->cd(1);
    canv->DrawClonePad();
    totCanv->cd(2);
    canvLog->DrawClonePad();
    totCanv->Update();
    totCanv->Modified();
    totCanv->Print(Form("plots/%s/pdf/%s_tot.pdf",pName.Data(),fname.Data()));
    totCanv->Print(Form("plots/%s/png/%s_tot.png",pName.Data(),fname.Data()));
    totCanv->Print(Form("plots/%s/C/%s_tot.C",pName.Data(),fname.Data()));
  }
  boxes.clear();
}
