#ifndef CreateBatchJob_h
#define CreateBatchJob_h

#include <iostream>
#include <vector>
#include <string>

#include "TString.h"

#include "OptParser.h"
#include "InputFileOptions.h"

class CreateBatchJob {

  public :

    CreateBatchJob(const OptParser &_rOpt, const InputFileOptions &_fOpt);
    ~CreateBatchJob(){}

    void WriteAndSubmit();

    void writeJobFiles();
    void submitJob();

    const OptParser& rOpt;
    const InputFileOptions& fOpt;

  private:

    void writeDatFile(std::vector<TString> indep_fnames, std::vector<TString> indep_tnames, int job=-1);
    void writeSubFile(Long64_t fEntry=-1, Long64_t lEntry=-1, int job=-1, int subj=-1);
    void writeCondorFile(int job=-1, int subj=-1);

    std::vector<TString> indep_filenames;
    std::vector<TString> indep_treenames;

    std::vector<std::string> writtenSubScripts;
};

#endif
