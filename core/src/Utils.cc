#include <iostream>
#include <cstdlib>

#include "Utils.h"

using namespace std;

void Utils::print(TString info, TString message, bool flush) {

  cout << Form("%-30s",info.Data()) << message << "\r";

  if ( flush ) cout << "\r" << flush;
  else cout << endl;

}

void Utils::print(TString cl_name, TString func_name, TString message, bool flush) {
  print(cl_name+":"+func_name+"()", message, flush);
}

void Utils::error(TString message) {
  print("ERROR:", message);
  exit(1);
}
