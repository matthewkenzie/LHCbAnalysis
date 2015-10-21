#ifndef PlotterBase_h
#define PlotterBase_h

#include <iostream>

#include "TString.h"
#include "TH1F.h"

#include "Analyser.h"
#include "Variables.h"
#include "PlotterUtils.h"

class PlotterBase : public Analyser {

  public:

    PlotterBase(TString _name, const Variables *_v);
    ~PlotterBase();

    void Init();
    void Term();
    bool AnalyseEvent();

    // abstract functions that must be implemented
    virtual bool fillHistograms() = 0;
    virtual void defineDrawingConfig() = 0;
    virtual void defineHistograms() = 0;

    const Variables *v;

  protected:

    void addHist(TString name, TString title, int bins, float xlow, float xhigh, TString legPos="L");
    void addDrawOpt(TString name, TString title, int itype);
    void addDrawOpt(TString name, TString title, int itype1, int itype2);
    void addDrawOpt(TString name, TString title, int itype1, int itype2, int itype3);
    void addDrawOpt(TString name, TString title, int itype1, int itype2, int itype3, int itype4);

    void setDrawOptDefaultFill(int color);
    void setDrawOptDefaultLine(int color);
    void setDrawOptDefaultPoint(int color);

    void setDrawOptFill(int color, int style);
    void setDrawOptLine(int color, int style);
    void setDrawOptPoint(int color, int style);

    void addResidOpt(std::pair<int,int> opt);
    void setResidType(int val) { residType = val; }

    void fillHist(TString name, double val);
    void fillHist(TString name, float val);
    void fillHist(TString name, int val);
    void fillHist(TString name, bool val);

    bool normalise;
    double normalisation;
    TString outfilename;

  private:
    // functions
    void initHistograms();
    void drawHistograms();
    void saveHistograms();
    void deleteHistograms();

    // data memebers
    std::vector<HistContainer> histList;
    std::vector<std::map<TString,TH1F*> > histMap;
    std::vector<DrawingConfig> drawOpts;
    std::vector<std::pair<int,int> > residOpts;

    // resid type
    int residType;
};

#endif
