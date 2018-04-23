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
  string analysis_name = boost::filesystem::current_path().filename().string();

  of << Form("touch %s/%s/%s.run",cwd.c_str(),rOpt.batchdir.c_str(),basename.c_str()) << endl;
  of << Form("rm -f %s/%s/%s.done",cwd.c_str(),rOpt.batchdir.c_str(),basename.c_str()) << endl;
  of << Form("rm -f %s/%s/%s.fail",cwd.c_str(),rOpt.batchdir.c_str(),basename.c_str()) << endl;
  of << Form("rm -f %s/%s/%s.log",cwd.c_str(),rOpt.batchdir.c_str(),basename.c_str()) << endl;

  of << "mkdir -p scratch" << endl;
  of << "cd scratch" << endl;
  of << Form("source %s/../scripts/setup_cam.sh",cwd.c_str()) << endl;
  of << Form("cp %s/bin/%s .",cwd.c_str(),prog_name.c_str()) << endl;
  of << Form("mkdir -p lib") << endl;
  of << Form("cp %s/../build/libLHCbAnalysisComponents.so lib/",cwd.c_str()) << endl;
  of << Form("cp %s/../build/%s/lib%sComponents.so lib/",cwd.c_str(),analysis_name.c_str(),analysis_name.c_str()) << endl;
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

  print("CreateBatchJob::write()","Written subfile: "+subfname);
}

void CreateBatchJob::writeCondorFile(int job, int subj ){

  string basename = fOpt.name.Data();
  if ( job > -1 )  basename += string(Form("_j%d",job));
  if ( subj > -1 ) basename += string(Form("_sj%d",subj));
  string jobfname = basename + ".job";

  ofstream of((rOpt.batchdir+"/"+jobfname).c_str());

  string cwd  = boost::filesystem::current_path().string();
  string subfname = cwd + "/" + rOpt.batchdir + "/" + basename + ".sh";
  string outfname = cwd + "/" + rOpt.batchdir + "/" + basename + ".out";
  string errfname = cwd + "/" + rOpt.batchdir + "/" + basename + ".err";
  string logfname = cwd + "/" + rOpt.batchdir + "/" + basename + ".log";

  of << "# Condor environment" << endl;
  of << "Universe                = vanilla" << endl;
  of << "getenv                  = true" << endl;
  of << "copy_to_spool           = true" << endl;
  of << "should_transfer_files   = YES" << endl;
  of << "when_to_transfer_output = ON_EXIT_OR_EVICT" << endl;
  of << "environment = CONDOR_ID=$(Cluster).$(Process)" << endl;

  of << "# Requirements" << endl;
  of << "Requirements = ( Arch == \"X86_64\" && OSTYPE == \"SLC6\" && ( POOL == \"GENERAL\" || POOL == \"GEN_FARM\" ) )" << endl;
  of << "Rank                    = kflops" << endl;
  of << "request_memory          = 1000" << endl;

  of << "# Condor Output" << endl;
  of << Form("output = %s",outfname.c_str()) << endl;
  of << Form("error  = %s",errfname.c_str()) << endl;
  of << Form("Log    = %s",logfname.c_str()) << endl;

  of << "# Submit" << endl;
  of << Form("Executable  = %s",subfname.c_str()) << endl;
  of << "Arguments  = " << endl;
  of << "Queue " << endl;

  of.close();

  print("CreateBatchJob::write()","Written condor file: "+jobfname);
}

void CreateBatchJob::writeJobFiles() {

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
        if ( rOpt.queue == "CONDOR" || rOpt.queue == "CONDORDRY" ) writeCondorFile(f,j);

      }
    }

  }
  else {
    writeDatFile(fOpt.filenames, fOpt.treenames);
    writeSubFile();
    if ( rOpt.queue == "CONDOR" || rOpt.queue == "CONDORDRY" ) writeCondorFile();
  }
}

void CreateBatchJob::submitJob() {

  for ( unsigned int i=0; i<writtenSubScripts.size(); i++) {

    if ( rOpt.queue != "" && rOpt.queue != "CONDORDRY" ) {
      print("CreateBatchJob::submitJob()",string(Form("Will submit: %s to queue: %s",writtenSubScripts[i].c_str(),rOpt.queue.c_str())));
    }
    if ( rOpt.runLocal ) {
      print("CreateBatchJob::submitJob()",string(Form("Will run: %s locally",writtenSubScripts[i].c_str())));
    }


    string cwd  = boost::filesystem::current_path().string();
    string ename = boost::filesystem::path( writtenSubScripts[i] ).stem().string();
    string path = cwd + "/" + rOpt.batchdir + "/" + ename;

    string submit_line;
    // run job locally
    if ( rOpt.runLocal ) {
      submit_line = string(Form( "%s.sh > %s.log", path.c_str(),path.c_str()));
      system( submit_line.c_str() );
    }
    // run job on CERN lxbatch system
    if ( rOpt.queue != "" && rOpt.queue != "CONDOR" && rOpt.queue != "CONDORDRY" && !rOpt.runLocal ) {
      submit_line = string(Form( "bsub -q %s -o %s.log %s.sh", rOpt.queue.c_str(),path.c_str(), path.c_str()));
      system( submit_line.c_str() );
    }
    // run job on Cambridge CONDOR system
    if ( rOpt.queue == "CONDOR" && !rOpt.runLocal ) {
      submit_line = string(Form( "condor_submit %s.job", path.c_str()));
      system( submit_line.c_str() );
    }
  }
}

void CreateBatchJob::WriteAndSubmit() {
  writeJobFiles();
  submitJob();
}
