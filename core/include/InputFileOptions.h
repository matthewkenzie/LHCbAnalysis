#ifndef InputFileOptions_h
#define InputFileOptions_h

#include <vector>
#include "TString.h"

class InputFileOptions {

  public:

    InputFileOptions();
    InputFileOptions( TString _name, int _itype );
    InputFileOptions( TString _name, int _itype, int _sqrts, TString _year);

    TString   name;
    int       itype;
    int       sqrts;
    TString   year;
    std::vector<TString>   filenames;
    std::vector<TString>   treenames;

    inline void addFile( const TString &filename ) { filenames.push_back(filename); }
    inline void addTree( const TString &treename ) { treenames.push_back(treename); }

};

#endif
