#ifndef FitterBase_h
#define FitterBase_h

#include <vector>

#include "TObject.h"
#include "TString.h"
#include "TCanvas.h"
#include "TApplication.h"

#include "RooWorkspace.h"
#include "RooHist.h"
#include "RooArgSet.h"

#include "FitterUtils.h"

class DataSet;
class CombDataSet;

class FitterBase {

  public:

    FitterBase(TString wsname="w", TString name="Fitter", bool _verbose=false, bool _debug=false);
    virtual ~FitterBase() = 0;

    void loadCachedWorkspace(TString fname);
    void loadCachedData(TString fname);

    void addObsVar(TString name, double min, double max);
    void addObsVar(TString name, float min, float max);
    void addObsVar(TString name, int min, int max);
    void addObsVar(TString name, short int min, short int max);
    void addObsVar(TString name, long int min, long int max);
    void addObsVar(TString name, bool start_val);
    void addObsVar(TString name, TString title, TString unit, double min, double max);
    void addObsVar(TString name, TString title, TString unit, float min, float max);
    void addObsVar(TString name, TString title, TString unit, int min, int max);
    void addObsVar(TString name, TString title, TString unit, short int min, short int max);
    void addObsVar(TString name, TString title, TString unit, long int min, long int max);
    void addObsVar(TString name, TString title, TString unit, bool start_val);

    void addCategory(TString name, std::vector<TString> cat_values);
    void addCategory(TString name, TString cat1);
    void addCategory(TString name, TString cat1, TString cat2);
    void addCategory(TString name, TString cat1, TString cat2, TString cat3);
    void addCategory(TString name, TString cat1, TString cat2, TString cat3, TString cat4);

    void setUnit(TString var, TString unit);
    void setBins(TString var, int bins);

    void addDataset(TString name, TString title, int itype);
    void addDataset(TString name, TString title, int itype1, int itype2);
    void addDataset(TString name, TString title, int itype1, int itype2, int itype3);
    void addDataset(TString name, TString title, int itype1, int itype2, int itype3, int itype4);
    void addCombDataset(TString name, TString title, TString cat, std::map<TString,TString> catDataMap);

    void addCut(TString name, double low, double high);
    void addCut(TString name, float low, float high);
    void addCut(TString name, int low, int high);
    void addCut(TString name, bool val);

    void addRequirement(TString dset, TString name, double low, double high);
    void addRequirement(TString dset, TString name, float low, float high);
    void addRequirement(TString dset, TString name, int low, int high);
    void addRequirement(TString dset, TString name, int val);
    void addRequirement(TString dset, TString name, bool val);

    void addParameter(TString name, double val);
    void addParameter(TString name, double low, double high);
    void addParameter(TString name, double val, double low, double high);
    void addConstraint(TString var);
    void addConstraint(TString var, double mean, double sigma);

    void makeGaussConstrainedParameter(TString name, TString par, double mean, double sigma);
    void makeGaussConstraintFromParameter(TString name, TString par);

    void makeGaussianPDF(TString name, TString par);
    void makeGaussianSumPDF(TString name, TString par, int n);
    void makeDisjointGaussianSumPDF(TString name, TString par, int n);
    void makeCBPDF(TString name, TString par);
    void makeDoubleCBPDF(TString name, TString par);
    void makeDisjointDoubleCBPDF(TString name, TString par);
    void makeTemplatePDF(TString name, TString par, TString data);
    void makeSumPDF(TString name, TString pdf1, TString pdf2);
    void makeSumPDF(TString name, TString pdf1, TString pdf2, TString pdf3);
    void makeSumPDF(TString name, TString pdf1, TString pdf2, TString pdf3, TString pdf4);
    void makeSumPDF(TString name, TString pdf1, TString pdf2, TString pdf3, TString pdf4, TString pdf5);
    void makeSumPDF(TString name, std::vector<TString> pdfList);

    void makeDatasets();
    void fillDatasets(TString fname, TString tname);
    void makeCombinedDatasets();
    void makeDataHist(TString dsname, TString dhname);

    void save(TString fname);
    void print(TString line, bool blank=false);

    void setVerbose(bool val=true){ verbose=val; }
    void setDebug(bool val=true){ debug=val; }

    RooWorkspace *w;
    TString fitterName;

    // these should be implemented by the inherited fitter
    // can of course be implemened and do nothing
    virtual void addObsVars() = 0;
    virtual void addCuts() = 0;
    virtual void addDatasets() = 0;
    virtual void constructPdfs() = 0;
    virtual void run() = 0;
    virtual void makePlots() = 0;

    bool verbose;
    bool debug;

  protected:

    std::vector<DataSet> dataSets;
    std::vector<CombDataSet> combDataSets;
    std::vector<int> colors;
    std::vector<TObject*> saveObjsStore;
    std::vector<TCanvas*> canvs;

    void defineParamSet(TString pdf);
    void defineYieldSet(TString pdf_name);
    void saveSnapshot(TString name, TString pdf);
    void loadSnapshot(TString name);

		void fit(TString pdf, TString data, bool constrained=false, double rangeLow=-999, double rangeHigh=-999);
    void freeze(TString pdf);

    double integral(TString pdf, TString var, TString scale="", double low=-999, double high=-999);

    void sfit(TString pdf_name, TString data_name, TString yields_name="", TString nonyields_name="", bool refit=true, bool loadSnapshot=true);
    void sproject(TString data_name, TString var_name);

    TCanvas* createCanvas(int canv_w=800, int canv_h=600);

    void plotMultiCanv( int n, TString name, int canv_w=800, int canv_h=600 );

    void plot(TString var, TString data, TString pdf="", int resid=0, TString title="", bool drawLeg=true, double xlow=-1, double xhigh=-1); // resid==0 (no resid), ==1 (resid hist), ==2 (pull hist)
    void plot(TString var, std::vector<PlotComponent> plotComps, TString fname, const RooArgList *params=NULL, double xlow=-1, double xhigh=-1);
    void plot2D(TString xvar, TString yvar, TString obj);

    void splot(TString var, TString data, TString title="", int bins=-1);
    void splot(TString var, TString data, std::vector<TString> compDsets, TString title="", int bins=-1);

    void storeSPlotProjection(RooHist *rh, TString name);
    void storeSPlotRatio(RooHist *dh, RooHist *sh, TString name);

    // plot opts
    double getPBoxX() { return pBoxX; }
    void   setPBoxX(double val) { pBoxX = val; }
    void   setDrawLog(bool val=true) { pDrawLog = val; }
    void   setTitle(TString title) { pTitle = title; }
    void   setResidType(int type)  { pResidType = type; }
    void   setRedPull(int type) { pRedPull = type; }

    void   fillOutputTrees( TFile *outf );

  private:

    std::map<TString,double> obs_values_d;
    std::map<TString,float> obs_values_f;
    std::map<TString,int> obs_values_i;
    std::map<TString,short int> obs_values_s;
    std::map<TString,long int> obs_values_l;
    std::map<TString,bool> obs_values_b;

    std::map<TString,std::pair<double,double> > cut_value_d;
    std::map<TString,std::pair<float,float> >  cut_value_f;
    std::map<TString,std::pair<int, int> >    cut_value_i;
    std::map<TString,bool>   cut_value_b;

    std::map<TString,double> values_d;
    std::map<TString,float>  values_f;
    std::map<TString,int>    values_i;
    std::map<TString,bool>   values_b;

    void setBranchAddresses(TTree* tree);
    bool passesCuts();
    bool passesRequirements(DataSet &dset);

    void printEntry(int entry);

    // plot opts
    double  pBoxX;
    bool    pDrawLog;
    TString pTitle;
    int     pResidType;
    int     pRedPull; // plot red pull points
};

#endif
