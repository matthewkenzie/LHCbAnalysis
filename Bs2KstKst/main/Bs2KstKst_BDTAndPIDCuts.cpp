#include <vector>

#include "RunCutOptimisation.h"

using namespace std;
using namespace Bs2KstKst;

int main(int argc, char **argv) {

  TString fname = "root/AnalysisOut.root";
  TString tname = "AnalysisTree";

  // need to do the prelim fit to get the idea of numbers
  //
  RunCutOptimisation optimiser( fname, tname );

  // fom types to run
  vector<int> fomTypes;
  fomTypes.push_back(0);
  fomTypes.push_back(1);
  fomTypes.push_back(2);

  // numbers from fit guess
  optimiser.setExpNSignalEvents( 11034 );
  optimiser.setExpNBackgroundEvents( 163994 );
  optimiser.setExpNMisIDEvents( 12368 );

  optimiser.setSignalSelString( "(itype==-70 || itype==-80)" );
  optimiser.setBackgroundSelString( "(itype==71 || itype==81) && (B_s0_DTF_B_s0_M>5600 && B_s0_DTF_B_s0_M<5800)" );
  optimiser.setMisIDSelString( "(itype==-77 | itype==-87)" );

  // BDT cut optimisation
  optimiser.optimise( "bdtoutput", fomTypes, 100, -1., 1. );

  // PID cut optimisation
  optimiser.optimise( "min_kaon_V3ProbNNk_corr", fomTypes, 100, 0, 1 );
  optimiser.optimise( "min_kaon_V3ProbNNKpi_corr", fomTypes, 100, 0, 1 );
  optimiser.optimise( "min_pion_V3ProbNNpi_corr", fomTypes, 100, 0, 1 );
  optimiser.optimise( "min_pion_V3ProbNNpiK_corr", fomTypes, 100, 0, 1 );

}
