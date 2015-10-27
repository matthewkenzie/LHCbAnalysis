#include <iostream>
#include <cstdlib>

#include "Utils.h"

using namespace std;

void Utils::print(TString info, TString message, bool isFlush) {

  cout << Form("%-30s",info.Data()) << message ;

  if ( isFlush ) cout << "\r" << flush;
  else cout << endl;

}

void Utils::print(TString cl_name, TString func_name, TString message, bool isFlush) {
  print(cl_name+":"+func_name+"()", message, isFlush);
}

void Utils::error(TString message) {
  print("ERROR:", message);
  exit(1);
}
