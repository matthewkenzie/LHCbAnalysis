#include <iostream>
#include <fstream>

#include "boost/filesystem.hpp"

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"

#include "Utils.h"
#include "CreateBatchJob.h"

using namespace std;
using namespace Utils;

CreateBatchJob::CreateBatchJob( const OptParser &_rOpt, const InputFileOptions &_fOpt ):
  rOpt(_rOpt),
  fOpt(_fOpt),
  indep_filenames(fOpt.filenames),
  indep_treenames(fOpt.treenames)
{}

void CreateBatchJob::writeDatFile( vector<TString> indep_fnames, vector<TString> indep_tnames, int job)
{
  system(Form("mkdir -p %s",rOpt.batchdir.c_str()));

  string datfname = Form("%s/%s",rOpt.batchdir.c_str(),fOpt.name.Data());
  if (job > -1 ) datfname += string( Form("_j%d",job) ) ;
  datfname += string( ".dat" ) ;

  ofstream of(datfname.c_str());

  of << Form("# %s",fOpt.name.Data()) << endl;
  of << endl;

  for ( unsigned int f=0; f<indep_fnames.size(); f++) {

    of << Form("itype=%-5d  sqrts=%-2d  year=%-5s  name=%-25s   fname=%-100s   tname=%-100s", fOpt.itype, fOpt.sqrts, fOpt.year.Data(), fOpt.name.Data(), indep_fnames[f].Data(), indep_tnames[f].Data()) << endl;
  }

  of.close();

  print("CreateBatchJob::write()","Written datfile: "+datfname);

  //system(Form("cat %s",datfname.c_str()));

}

void CreateBatchJob::writeSubFile(Long64_t fEntry, Long64_t lEntry, int job , int subj) {

  system(Form("mkdir -p %s",rOpt.batchdir.c_str()));

  string basename = fOpt.name.Data();
  if ( job > -1 )  basename += string(Form("_j%d",job));
  string datfname = basename +".dat";
  if ( subj > -1 ) basename += string(Form("_sj%d",subj));
  string subfname = basename + ".sh";

  ofstream of((rOpt.batchdir+"/"+subfname).c_str());
  of << "#!/bin/bash" << endl;

  string cwd  = boost::filesystem::current_path().string();
  boost::filesystem::path prog( rOpt.prog_name );

  string prog_name = prog.stem().string();

  of << Form("touch %s/%s/%s.run",cwd.c_str(),rOpt.batchdir.c_str(),basename.c_str()) << endl;
  of << Form("rm -f %s/%s/%s.done",cwd.c_str(),rOpt.batchdir.c_str(),basename.c_str()) << endl;
  of << Form("rm -f %s/%s/%s.fail",cwd.c_str(),rOpt.batchdir.c_str(),basename.c_str()) << endl;
  of << Form("rm -f %s/%s/%s.log",cwd.c_str(),rOpt.batchdir.c_str(),basename.c_str()) << endl;

  of << "mkdir -p scratch" << endl;
  of << "cd scratch" << endl;
  of << Form("source %s/setup_lxplus.sh",cwd.c_str()) << endl;
  of << Form("cp %s/bin/%s .",cwd.c_str(),prog_name.c_str()) << endl;
  of << Form("mkdir -p lib") << endl;
  of << Form("cp %s/lib/lib%sLib.so lib/",cwd.c_str(),prog_name.c_str()) << endl;
  of << Form("cp %s/%s/%s .",cwd.c_str(), rOpt.batchdir.c_str(), datfname.c_str()) << endl;

  string exec_line;
  exec_line += Form("./%s -c %s -o %s.root -b", prog_name.c_str(), datfname.c_str(), basename.c_str());
  if ( fEntry > -1 ) exec_line += Form(" -f %lld", fEntry ) ;
  if ( lEntry > -1 ) exec_line += Form(" -l %lld", lEntry ) ;

  of << Form ( "if ( %s ) then ",exec_line.c_str() ) << endl;
  of << Form( "\t cp %s.root %s/%s/", basename.c_str(), cwd.c_str(), rOpt.batchdir.c_str() ) << endl;
  of << Form("\t touch %s/%s/%s.done",cwd.c_str(),rOpt.batchdir.c_str(),basename.c_str()) << endl;
  of << "else" << endl;
  of << Form("\t touch %s/%s/%s.fail",cwd.c_str(),rOpt.batchdir.c_str(),basename.c_str()) << endl;
  of << "fi" << endl;
  of << Form("rm -f %s/%s/%s.run",cwd.c_str(),rOpt.batchdir.c_str(),basename.c_str()) << endl;

  of.close();

  system(Form("chmod +x %s/%s",rOpt.batchdir.c_str(),subfname.c_str()));

  writtenSubScripts.push_back(subfname);

}

void CreateBatchJob::writeJobFiles() {

  cout << rOpt.jobSplitting << endl;
  if ( rOpt.jobSplitting > -1 ) {

    // job splitting on

    // need to check for multiple files

    assert( indep_filenames.size() == indep_treenames.size() );

    for ( unsigned int f=0; f<indep_filenames.size(); f++ ) {

      // update fOpt
      vector<TString> tmp_filenames;
      vector<TString> tmp_treenames;
      tmp_filenames.push_back(indep_filenames[f]);
      tmp_treenames.push_back(indep_treenames[f]);

      assert( tmp_filenames.size() == 1 );
      assert( tmp_treenames.size() == 1 );

      // write dat file for this file
      writeDatFile( tmp_filenames, tmp_treenames, f);

      // now find nevents in file
      TFile *tfile = TFile::Open(tmp_filenames[0]);
      TTree *ttree = (TTree*)tfile->Get(tmp_treenames[0]);

      Long64_t nEntries = ttree->GetEntries();

      // use it to calculate job splitting
      int required_jobs = int(ceil(double(nEntries)/rOpt.jobSplitting)+0.5);

      for ( int j=0; j<required_jobs; j++ ) {
        Long64_t low = j*(rOpt.jobSplitting);
        Long64_t high = (j+1)*(rOpt.jobSplitting) > nEntries ? nEntries : (j+1)*(rOpt.jobSplitting);
        print("",string(Form("  Job/Subjob %-3d %-3d : [ %-7lld , %-7lld ]", f, j, low, high)));
        writeSubFile( low, high, f, j);

      }
    }

  }
  else {
    writeDatFile(fOpt.filenames, fOpt.treenames);
    writeSubFile();
  }
}

void CreateBatchJob::submitJob() {

  for ( unsigned int i=0; i<writtenSubScripts.size(); i++) {

      print("CreateBatchJob::submitJob()",string(Form("Will submit: %s.sh to queue: %s",writtenSubScripts[i].c_str(),rOpt.queue.c_str())));


    string cwd  = boost::filesystem::current_path().string();
    string ename = boost::filesystem::path( writtenSubScripts[i] ).stem().string();
    string path = cwd + "/" + rOpt.batchdir + "/" + ename;

    string submit_line;
    if ( rOpt.runLocal ) {
      submit_line = string(Form( "%s.sh > %s.log", path.c_str(),path.c_str()));
      system( submit_line.c_str() );
    }
    if ( rOpt.queue != "" && !rOpt.runLocal ) {
      submit_line = string(Form( "bsub -q %s -o %s.log %s.sh", rOpt.queue.c_str(),path.c_str(), path.c_str()));
      system( submit_line.c_str() );
    }
  }
}

void CreateBatchJob::WriteAndSubmit() {
  writeJobFiles();
  submitJob();
}
