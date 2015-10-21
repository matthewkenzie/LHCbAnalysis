#ifndef FitterUtils_h
#define FitterUtils_h

#include <iostream>
#include <vector>
#include "TString.h"
#include "TLegend.h"
#include "RooWorkspace.h"
#include "RooArgSet.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"
#include "RooAbsArg.h"
#include "RooPlot.h"
#include "RooAbsData.h"
#include "RooAbsPdf.h"

// mock class to trick cint
class FitterUtils {
  public:
    FitterUtils(){ std::cout << "FitterUtils::FitterUtils() is not a real class" << std::endl;}
    ~FitterUtils(){}
};

class DataSet {

  public:

    DataSet(TString _name, TString _title, RooWorkspace* _w);
    ~DataSet(){}

    // overloaded constructors
    DataSet(TString _name, TString _title, RooWorkspace* _w, int typ1);
    DataSet(TString _name, TString _title, RooWorkspace* _w, int typ1, int typ2);
    DataSet(TString _name, TString _title, RooWorkspace* _w, int typ1, int typ2, int typ3);
    DataSet(TString _name, TString _title, RooWorkspace* _w, int typ1, int typ2, int typ3, int typ4);

    TString name;
    TString title;
    std::vector<int> itypes;
    RooWorkspace *w;

    void addType(int type) { itypes.push_back(type); }

    // specific cuts for this dataset
    void addRequirement(TString name, double low, double high) {
      requirements_d[name] = std::make_pair(low,high);
    }
    void addRequirement(TString name, float low, float high) {
      requirements_f[name] = std::make_pair(low,high);
    }
    void addRequirement(TString name, int low, int high) {
      requirements_i[name] = std::make_pair(low,high);
    }
    void addRequirement(TString name, int val) {
      requirements_i[name] = std::make_pair(val,val);
    }
    void addRequirement(TString name, bool val) {
      requirements_b[name] = val;
    }
    //
    std::map<TString,std::pair<double, double> > requirements_d;
    std::map<TString,std::pair<float, float> > requirements_f;
    std::map<TString,std::pair<int, int> > requirements_i;
    std::map<TString,bool> requirements_b;
};

class PlotComponent {

  public:
    PlotComponent(TString _name, TString _title);
    PlotComponent(TString _name, TString _title, int _lcolor);
    PlotComponent(TString _name, TString _title, int _lcolor, int _lstyle);

    void plotOn(RooWorkspace *w, RooPlot *plot, TLegend *leg=NULL);

    TString name;
    TString title;
    int lcolor;
    int lstyle;
    int lwidth;
    int mcolor;
    int mstyle;
    int fcolor;
    int fstyle;
    int binning;
    TString doption;
    bool invisible;
    bool noleg;

    void setDefaultDataStyle();
    void setDashedLine(int color);
    void setSolidLine(int color);
    void setSolidFill(int color);

};

#endif
