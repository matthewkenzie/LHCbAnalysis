#ifndef PlotterUtils_h
#define PlotterUtils_h

#include <iostream>
#include <vector>
#include "TROOT.h"
#include "TH1F.h"
#include "TColor.h"
#include "TString.h"

// mock class to trick cint
class PlotterUtils {
  public:
    PlotterUtils(){ std::cout << "PlotterUtils::PlotterUtils() is not a real class" << std::endl;}
    ~PlotterUtils(){}
};

struct HistContainer{
  TString name;
  TString title;
  int nbins;
  float xlow;
  float xhigh;
  TString legPos;
};

class DrawingConfig {
  public:
    DrawingConfig(TString _name, TString _title)
    {
      init(_name,_title);
    }
    DrawingConfig(TString _name, TString _title, int itype1)
    {
      init(_name,_title);
      itypes.push_back(itype1);
    }
    DrawingConfig(TString _name, TString _title, int itype1, int itype2)
    {
      init(_name,_title);
      itypes.push_back(itype1);
      itypes.push_back(itype2);
    }
    DrawingConfig(TString _name, TString _title, int itype1, int itype2, int itype3)
    {
      init(_name,_title);
      itypes.push_back(itype1);
      itypes.push_back(itype2);
      itypes.push_back(itype3);
    }
    DrawingConfig(TString _name, TString _title, int itype1, int itype2, int itype3, int itype4)
    {
      init(_name,_title);
      itypes.push_back(itype1);
      itypes.push_back(itype2);
      itypes.push_back(itype3);
      itypes.push_back(itype4);
    }
    DrawingConfig(TString _name, TString _title, std::vector<int> _itypes)
    {
      init(_name,_title);
      itypes = _itypes;
    }

    void init(TString _name, TString _title){
      name    = _name;
      title   = _title;
      xaxis   = "";
      yaxis   = "";
      lcolor  = -1;
      lstyle  = -1;
      fcolor  = -1;
      fstyle  = -1;
      mcolor  = -1;
      mstyle  = -1;
      msize   = -1;
      lwidth  = -1;
      plotOpt = "HIST";
      legOpt  = "L";
    }

    void SetStyle(TH1F *th1f) {
      if (xaxis  != "") th1f->GetXaxis()->SetTitle(xaxis);
      if (yaxis  != "") th1f->GetYaxis()->SetTitle(yaxis);
      if (lcolor != -1) th1f->SetLineColor(lcolor);
      if (lstyle != -1) th1f->SetLineStyle(lstyle);
      if (fcolor != -1) th1f->SetFillColor(fcolor);
      if (fstyle != -1) th1f->SetFillStyle(fstyle);
      if (mcolor != -1) th1f->SetMarkerColor(mcolor);
      if (mstyle != -1) th1f->SetMarkerStyle(mstyle);
      if (msize  != -1) th1f->SetMarkerSize(msize);
      if (lwidth != -1) th1f->SetLineWidth(lwidth);
    }

    void addType(int itype){ itypes.push_back(itype); }

    void setDefaultFill(int color) {
      TColor* mycolor = gROOT->GetColor(color);
      mycolor->SetAlpha(0.5);
      fcolor = mycolor->GetNumber();
      lcolor = mycolor->GetNumber();
      plotOpt = "HIST";
      legOpt = "LF";
    }

    void setDefaultLine(int color) {
      lcolor = color;
      lwidth = 3;
      plotOpt = "HIST";
      legOpt = "LF";
    }

    void setDefaultPoint(int color) {
      lcolor = color;
      mcolor = color;
      mstyle = kFullCircle;
      lwidth = 3;
      plotOpt = "LEP";
      legOpt = "LEP";
    }

    void setFill(int color, int style){
      fstyle = style;
      setDefaultFill(color);
    }

    void setLine(int color, int style){
      lstyle = style;
      setDefaultLine(color);
    }

    void setPoint(int color, int style){
      mstyle = style;
      setDefaultPoint(color);
    }

    void print(){
      std::cout << "Variables Plotter Info: " << std::endl;
      std::cout << "\t" << "Name:    "  << name    << std::endl;
      std::cout << "\t" << "Title:   "  << title   << std::endl;
      std::cout << "\t" << "lcolor:  "  << lcolor  << std::endl;
      std::cout << "\t" << "lstyle:  "  << lstyle  << std::endl;
      std::cout << "\t" << "fcolor:  "  << fcolor  << std::endl;
      std::cout << "\t" << "fstyle:  "  << fstyle  << std::endl;
      std::cout << "\t" << "mcolor:  "  << mcolor  << std::endl;
      std::cout << "\t" << "mstyle:  "  << mstyle  << std::endl;
      std::cout << "\t" << "msize:   "  << msize   << std::endl;
      std::cout << "\t" << "lwidth:  "  << lwidth  << std::endl;
      std::cout << "\t" << "plotopt: "  << plotOpt << std::endl;
    }

    TString name;
    TString title;
    std::vector<int> itypes;

    TString xaxis;
    TString yaxis;
    int lcolor;
    int lstyle;
    int fcolor;
    int fstyle;
    int mcolor;
    int mstyle;
    int msize;
    int lwidth;
    TString plotOpt;
    TString legOpt;
};

#endif
