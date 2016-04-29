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

CombDataSet::CombDataSet(TString _name, TString _title, TString _cat_name):
  name(_name),
  title(_title),
  cat_name(_cat_name)
{}

CombDataSet::CombDataSet(TString _name, TString _title, TString _cat_name, map<TString,TString> _catDataMap):
  name(_name),
  title(_title),
  cat_name(_cat_name),
  catDataMap(_catDataMap)
{}

void CombDataSet::addCatDataset( TString cat, TString data ){
  catDataMap[cat] = data;
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
  noleg(false),
  slice(""),
  project("")
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
  noleg(false),
  slice(""),
  project("")
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
  noleg(false),
  slice(""),
  project("")
{}

void PlotComponent::plotOn(RooWorkspace *w, RooPlot *plot, TLegend *leg){

  // check if component is requested (by passing of ":" in name)
  TString main = name;
  TString comp = name;

  if (name.Contains(":")) {
    main.Remove(main.First(":"),main.Length());
    comp.Remove(0,comp.First(":")+1);
  }

  TString slice_name = slice;
  TString slice_value = slice;
  if (slice.Contains(":")) {
    slice_name.Remove(slice_name.First(":"),slice_name.Length());
    slice_value.Remove(0,slice_value.First(":")+2);
  }

  TString project_name = project;
  TString project_value = project;
  if (project.Contains(":")) {
    project_name.Remove( project_name.First(":"), project_name.Length() );
    project_value.Remove( 0, project_value.First(":")+2 );
  }

  // set binning to default if not requested
  if ( binning == -1 ) {
    binning = plot->getPlotVar()->getBins();
  }

  // check if dset
  if (w->data(name)) {

    // plot options
    RooLinkedList l;
    RooCmdArg cmd1 = Binning(binning)    ; l.Add( &cmd1 );
    RooCmdArg cmd2 = LineColor(lcolor)   ; l.Add( &cmd2 );
    RooCmdArg cmd3 = MarkerColor(mcolor) ; l.Add( &cmd3 );
    RooCmdArg cmd4;
    RooCmdArg cmd5;
    RooCmdArg cmd6;
    if ( mstyle > 0 ) {
      cmd4 = MarkerStyle(mstyle);
      l.Add( &cmd4 );
    }
    if ( invisible )  {
      cmd5 = Invisible();
      l.Add( &cmd5 );
    }
    if ( slice!="" )  {
      cmd6 = Cut( slice_name+"=="+slice_name+"::"+slice_value );
      l.Add( &cmd6 );
    }

    // plot
    w->data(name)->plotOn(plot,l);

  }

  // check if pdf
  else if (w->pdf(main)) {

    // plot options
    RooLinkedList l;
    RooCmdArg cmd1 = LineColor(lcolor)  ; l.Add( &cmd1 );
    RooCmdArg cmd2 = LineColor(lcolor)  ; l.Add( &cmd2 );
    RooCmdArg cmd3 = LineStyle(lstyle)  ; l.Add( &cmd3 );
    RooCmdArg cmd4 = LineWidth(lwidth)  ; l.Add( &cmd4 );
    RooCmdArg cmd5 = FillColor(fcolor)  ; l.Add( &cmd5 );
    RooCmdArg cmd6 = FillStyle(fstyle)  ; l.Add( &cmd6 );
    RooCmdArg cmd7 = DrawOption(doption); l.Add( &cmd7 );
    RooCmdArg cmd8;
    RooCmdArg cmd9;
    RooCmdArg cmd10;
    RooCmdArg cmd11;
    if (name.Contains(":")) {
      cmd8 = Components(comp);
      l.Add( &cmd8 );
    }
    if (invisible) {
      cmd9 = Invisible();
      l.Add( &cmd9 );
    }
    if ( slice!="" )  {
      cmd10 = Slice( *w->cat(slice_name), slice_value );
      l.Add( &cmd10 );
    }
    if ( project!="") {
      cmd11 = ProjWData( *w->cat(project_name), *w->data(project_value) );
      l.Add( &cmd11 );
    }

    // plot
    w->pdf(main)->plotOn( plot, l );

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

DrawBox::DrawBox( double _xmin, double _xmax, int _color ):
  xmin(_xmin),
  xmax(_xmax),
  color(_color)
{}

DrawBox::~DrawBox(){}
