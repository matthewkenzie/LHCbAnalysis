#include "InputFileOptions.h"

using namespace std;

InputFileOptions::InputFileOptions(){}

InputFileOptions::InputFileOptions( TString _name, int _itype, int _sqrts, TString _year):
  name(_name),
  itype(_itype),
  sqrts(_sqrts),
  year(_year)
{}

InputFileOptions::InputFileOptions( TString _name, int _itype):
  name(_name),
  itype(_itype),
  sqrts(-1),
  year("")
{}
