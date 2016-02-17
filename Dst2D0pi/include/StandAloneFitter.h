#ifndef StandAloneFitter_h
#define StandAloneFitter_h

#include <iostream>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TString.h"
#include "TH1F.h"

#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooPlot.h"

#include "FitterUtils.h"
#include "StandAlonePlotter.h"

namespace Dst2D0pi {

  class StandAloneFitter {

    public:

      StandAloneFitter();
      ~StandAloneFitter();

      void run(bool makeDatasets, bool loadDatasets);

      void MakeDataSets();
      void LoadDataSets();

      void D0MassFit();
      inline void SetSignalWindow( double xmin, double xmax ) { signalWindowMin=xmin; signalWindowMax=xmax; signalWindowSet=true; }
      void GetYieldsInMassWindow();

      void D0IPChi2Fit();

      inline void SetIPCut( double xmax ) { ipCut=xmax; ipCutSet=true; }
      void GetYieldsFromIPCut();

      void D0LifetimeFit();

    private:

      TFile *dataFile;
      TTree *dataTree;

      TFile *mcFile;
      TTree *mcTree;

      RooWorkspace *w;

      TString cuts;
      TString mccuts_prompt;
      TString mccuts_sec;

      StandAlonePlotter *p;

      bool isMassFit;
      bool haveYields;
      bool isIPCHI2Fit;

      bool signalWindowSet;
      double signalWindowMin;
      double signalWindowMax;

      double nSigInWindow;
      double nBkgInWindow;

      bool ipCutSet;
      double ipCut;

      bool haveFinalYields;
      double nPrompt;
      double nSec;
      double nBkg;
  };
}

#endif
