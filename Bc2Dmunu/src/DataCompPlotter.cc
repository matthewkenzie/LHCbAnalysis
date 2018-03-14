#include "DataCompPlotter.h"
#include "TMath.h"

using namespace std;
using namespace TMath;

Bc2Dmunu::DataCompPlotter::DataCompPlotter(TString _name, const Variables_Analysis *_v):
  SkimmingPlotter(_name,_v)
{}

Bc2Dmunu::DataCompPlotter::~DataCompPlotter(){}

void Bc2Dmunu::DataCompPlotter::defineDrawingConfig(){

  // add these drawing options
  // -------------------------------------------- //
  // -------------------------------------------- //
  addDrawOpt("data2011",   "Data 2011", 11);
  setDrawOptDefaultLine(kBlue);

  addDrawOpt("data2012",   "Data 2012", 12);
  setDrawOptDefaultLine(kRed);

  addDrawOpt("data2015",   "Data 2015", 15);
  setDrawOptDefaultLine(kMagenta);

  addDrawOpt("data2016",   "Data 2016", 16);
  setDrawOptDefaultLine(kGreen+2);

  addDrawOpt("data2017",   "Data 2017", 17);
  setDrawOptDefaultLine(kGray+2);

}

