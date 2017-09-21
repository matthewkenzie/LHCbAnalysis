#ifndef Bs2KstKst_RunCutOptimisation_h
#define Bs2KstKst_RunCutOptimisation_h

#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TGraph.h"

namespace Bs2KstKst {

  class RunCutOptimisation {

    public:

      RunCutOptimisation(TString fname, TString tname);
      ~RunCutOptimisation();

      inline void setExpNSignalEvents( float nevs ) { nExpSigEvs = nevs; };
      inline void setExpNBackgroundEvents( float nevs ) { nExpBkgEvs = nevs; };
      inline void setExpNMisIDEvents( float nevs ) { nExpMisEvs = nevs; };

      inline void setSignalSelString( TString cut ) { sigSel = cut; };
      inline void setBackgroundSelString( TString cut ) { bkgSel = cut; };
      inline void setMisIDSelString( TString cut ) {misSel = cut; };

      void optimise( TString var, std::vector<int> fomTypes, int nsteps, float min, float max, TString plotext="", TString operation=">" );

      TString getFOMName( int type );
      float getFOM( int fomType, float nSig, float nBkg, float nMis );

      void plotOptCurves( std::vector<TGraph*> curves, std::vector<int> fomTypes, TString var, float xmin, float xmax, TString plotext="" );
      void printOptResult( std::vector<TGraph*> curves, std::vector<int> fomTypes, TString var );

    private:

      float nExpSigEvs;
      float nExpBkgEvs;
      float nExpMisEvs;

      TString sigSel;
      TString bkgSel;
      TString misSel;

      TFile *inf;
      TTree *tree;
  };
}

#endif
