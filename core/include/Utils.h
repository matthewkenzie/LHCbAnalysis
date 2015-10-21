#ifndef Utils_h
#define Utils_h

#include "TString.h"

namespace Utils {

  void print(TString info, TString message, bool flush=false);
  void print(TString cl_name, TString func_name, TString message, bool flush=false);
  void error(TString message);

}

#endif
