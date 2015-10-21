#ifndef Bs2KstKst_MultiDimCutOpt_h
#define Bs2KstKst_MultiDimCutOpt_h

#include <iostream>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "RooWorkspace.h"
#include "RooDataHist.h"
#include "RooAbsData.h"
#include "RooAbsPdf.h"

namespace Bs2KstKst {

  class Scan1DResult {
    public:
      Scan1DResult(TGraph *g, double X, double Y):
        gr(g), maxX(X), maxY(Y) {}
      ~Scan1DResult(){}
      TGraph *gr;
      double maxX;
      double maxY;
  };

  class Scan2DResult {
    public:
      Scan2DResult(TH2D *h, double X1, double X2, double Y):
        hist(h), maxX1(X1), maxX2(X2), maxY(Y) {}
      ~Scan2DResult(){}
      TH2D *hist;
      double maxX1;
      double maxX2;
      double maxY;
  };

  class MultiDimCutOpt {

    public:

      MultiDimCutOpt();
      ~MultiDimCutOpt();

      void setup(TString inf, TString outf, TString cachef);

      void makeInitialDatasets();
      void makePDFs();
      void loadWorkspace();
      void loadDatasetsFromFile( TString fname );
      void plotShapes();

      void run();
      void runSimple(int type=0);

      void save();

      void drawAtCut(TString name, double bdt_cut, double kaon_cut, double pion_cut);

      void setScanPoints1D(int val) { scanpoints1d = val; }
      void setScanPoints2D(int val) { scanpoints2d = val; }

      void setRunAll(bool val) { runAll = val; }
      void setRun2011(bool val) { run2011 = val; }
      void setRun2012(bool val) { run2012 = val; }

      void setMisIdType(int val) { misIdType = val; }

    private:

      bool runAll;
      bool run2011;
      bool run2012;

      TString infilename;
      TString outfilename;
      TString cachefilename;

      RooWorkspace *w;
      TTree *tree;
      TFile *outFile;

      RooPlot *getSimplePlot(TString var, TString data, TString pdf="");

      void plot(RooAbsData *data, RooAbsPdf *pdf, TString ext="", double *cutvals=0);
      double fitAndReduce(const double *cutvs);
      double fitAndReduce(TString cut);
      RooDataHist *reduce(TString cut_expr);

      void fitAndSetConst(TString pdf, TString data);

      RooDataSet* getDatasetFromWS( RooWorkspace *ws, TString name2011, TString name2012, TString newname );

      // ----- //
      // misid yield type:
      // // 1 = rho_mc_y + phi_mc_y + lb_mc_y (DEFAULT)
      // // 2 = rho_mc_y
      // // 3 = phi_mc_y
      // // 4 = lb_mc_y
      // // 5 = rho_mc_y + phi_mc_y
      int misIdType;

      // ----- //
      // figure of merit type:
      // // 0 = S/sqrt(S+B) (DEFAULT)
      // // 1 = S/sqrt(S+B+M)
      // // 2 = S/sqrt(S+B+sigma*M)
      // // 3 = S/sqrt(S+B+2sigma*M)
      // // 4 = S/sqrt(S+B+M) * purity
      int fomType;
      void getEventEstimates(TTree *tree);

      TCut getSigSel();
      TCut getBkgSel();
      TCut getMisSel();
      TCut getCut(double *cutvals);

      Scan1DResult scan1D( int varInd, double lowVal, double highVal, int npoints, double *cutvals);
      Scan2DResult scan2D( int varInd1, int varInd2, double lowVal1, double highVal1, int npoints1, double lowVal2, double highVal2, int npoints2, double *cutvals );

      double getFOM(double *cutvals);

      int n_reducs;

      double sigNorm;
      double bkgNorm;
      double misNorm;

      void setAxes(TH1 *obj, TString xtitle, TString ytitle);
      void setAxes(TH1 *obj, TString xtitle, TString ytitle, TString ztitle);
      void setAxes(TGraph *obj, TString xtitle, TString ytitle);

      int scanpoints1d;
      int scanpoints2d;
  };
}
#endif
