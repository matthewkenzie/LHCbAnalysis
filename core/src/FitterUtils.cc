#include "FitterUtils.h"

using namespace std;
using namespace RooFit;
using namespace TMath;

// constructors
DataSet::DataSet(TString _name, TString _title, RooWorkspace* _w):
  name(_name),
  title(_title),
  w(_w)
{}

DataSet::DataSet(TString _name, TString _title, RooWorkspace* _w, int typ1):
  name(_name),
  title(_title),
  w(_w)
{
  addType(typ1);
}

DataSet::DataSet(TString _name, TString _title, RooWorkspace* _w, int typ1, int typ2):
  name(_name),
  title(_title),
  w(_w)
{
  addType(typ1);
  addType(typ2);
}

DataSet::DataSet(TString _name, TString _title, RooWorkspace* _w, int typ1, int typ2, int typ3):
  name(_name),
  title(_title),
  w(_w)
{
  addType(typ1);
  addType(typ2);
  addType(typ3);
}

DataSet::DataSet(TString _name, TString _title, RooWorkspace* _w, int typ1, int typ2, int typ3, int typ4):
  name(_name),
  title(_title),
  w(_w)
{
  addType(typ1);
  addType(typ2);
  addType(typ3);
  addType(typ4);
}

PlotComponent::PlotComponent(TString _name, TString _title):
  name(_name),
  title(_title),
  lcolor(1),
  lstyle(1),
  lwidth(2),
  mcolor(1),
  mstyle(0),
  fcolor(0),
  fstyle(0),
  binning(-1),
  doption("L"),
  invisible(false),
  noleg(false)
{}

PlotComponent::PlotComponent(TString _name, TString _title, int _lcolor):
  name(_name),
  title(_title),
  lcolor(_lcolor),
  lstyle(1),
  lwidth(2),
  mcolor(1),
  mstyle(0),
  fcolor(0),
  fstyle(0),
  binning(-1),
  doption("L"),
  invisible(false),
  noleg(false)
{}

PlotComponent::PlotComponent(TString _name, TString _title, int _lcolor, int _lstyle):
  name(_name),
  title(_title),
  lcolor(_lcolor),
  lstyle(_lstyle),
  lwidth(2),
  mcolor(1),
  mstyle(0),
  fcolor(0),
  fstyle(0),
  binning(-1),
  doption("L"),
  invisible(false),
  noleg(false)
{}

void PlotComponent::plotOn(RooWorkspace *w, RooPlot *plot, TLegend *leg){

  // check if component is requested (by passing of ":" in name)
  TString main = name;
  TString comp = name;

  if (name.Contains(":")) {
    main.Remove(main.First(":"),main.Length());
    comp.Remove(0,comp.First(":")+1);
  }

  // set binning to default if not requested
  if ( binning == -1 ) {
    binning = plot->getPlotVar()->getBins();
  }

  // check if dset
  if (w->data(name)) {
    if (mstyle > 0) {
      if ( invisible ) w->data(name)->plotOn(plot,Binning(binning),LineColor(lcolor),MarkerColor(mcolor),MarkerStyle(mstyle),Invisible());
      else             w->data(name)->plotOn(plot,Binning(binning),LineColor(lcolor),MarkerColor(mcolor),MarkerStyle(mstyle));
    }
    else {
      if ( invisible ) w->data(name)->plotOn(plot,Binning(binning),LineColor(lcolor),MarkerColor(mcolor),Invisible());
      else             w->data(name)->plotOn(plot,Binning(binning),LineColor(lcolor),MarkerColor(mcolor));
    }
  }

  // check if pdf
  else if (w->pdf(main)) {
    if (name.Contains(":")) {
      if ( invisible ) w->pdf(main)->plotOn(plot,Components(comp),LineColor(lcolor),LineStyle(lstyle),LineWidth(lwidth),FillColor(fcolor),FillStyle(fstyle),DrawOption(doption),Invisible());
      else             w->pdf(main)->plotOn(plot,Components(comp),LineColor(lcolor),LineStyle(lstyle),LineWidth(lwidth),FillColor(fcolor),FillStyle(fstyle),DrawOption(doption));
    }
    else {
      if ( invisible ) w->pdf(main)->plotOn(plot,LineColor(lcolor),LineStyle(lstyle),LineWidth(lwidth),FillColor(fcolor),FillStyle(fstyle),DrawOption(doption),Invisible());
      else             w->pdf(main)->plotOn(plot,LineColor(lcolor),LineStyle(lstyle),LineWidth(lwidth),FillColor(fcolor),FillStyle(fstyle),DrawOption(doption));
    }
  }

  // add to legend if necessary
  if (leg && !invisible && !noleg && (w->data(name) || w->pdf(main))) {
    TObject *legObj = (TObject*)plot->getObject(plot->numItems()-1);
    leg->AddEntry(legObj, title, doption);
  }
}

void PlotComponent::setDefaultDataStyle(){
  lcolor = kBlack;
  mcolor = kBlack;
  doption = "LEP";
  // the rest are already defaults
}

void PlotComponent::setDashedLine(int color){
  lcolor = color;
  lwidth = 3;
  lstyle = kDashed;
  doption = "L";
  // the rest are already defaults
}

void PlotComponent::setSolidLine(int color){
  lcolor = color;
  lwidth = 3;
  lstyle = 1;
  doption = "L";
  // the rest are already defaults
}

void PlotComponent::setSolidFill(int color){
  lcolor = color;
  lwidth = 3;
  lstyle = 1;
  fcolor = color;
  fstyle = 1001;
  doption = "LF";
}
