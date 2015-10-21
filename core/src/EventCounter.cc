#include "Utils.h"
#include "EventCounter.h"

using namespace std;
using namespace Utils;

EventCounter::EventCounter() {}

void EventCounter::addDataType( int itype, TString evName ) {

  typeToNameMap[itype]  = evName;
  nameToTypeMap[evName] = itype;

  if ( find ( itypes.begin(), itypes.end(), itype ) == itypes.end() ) {
    itypes.push_back( itype );
  }
  if ( find ( evNames.begin(), evNames.end(), evName ) == evNames.end() ) {
    evNames.push_back( evName );
  }
}

void EventCounter::addAnalyser( TString anName ) {
  analyserNames.push_back( anName );
}

void EventCounter::Init() {

  // reverse sort
  //std::vector<TString> sortedNames = sort( evNames.begin(), evNames.end() );
  //
  sortedNames = evNames;

  hPass = new TH2I("hPass","nPass",analyserNames.size(),0,analyserNames.size(), sortedNames.size(),0,sortedNames.size());
  hFail = new TH2I("hFail","nFail",analyserNames.size(),0,analyserNames.size(), sortedNames.size(),0,sortedNames.size());
  hProc = new TH2I("hProc","nTot",analyserNames.size(),0,analyserNames.size(), sortedNames.size(),0,sortedNames.size());
  hEff  = new TH2D("hEff" ,"eff",analyserNames.size(),0,analyserNames.size(), sortedNames.size(),0,sortedNames.size());
  hType = new TH1I("hType","type map", sortedNames.size(), 0, sortedNames.size());

  setStyle(hPass);
  setStyle(hFail);
  setStyle(hProc);
  setStyle(hEff);
  setStyle(hType,false);

}

void EventCounter::setStyle(TH1* h, bool is2d) {

  if (is2d) {
    for ( int xbin=1; xbin<=h->GetNbinsX(); xbin++ ){
      h->GetXaxis()->SetBinLabel(xbin, analyserNames[xbin-1].Data() );
    }
    for ( int ybin=1; ybin<=h->GetNbinsY(); ybin++ ){
      h->GetYaxis()->SetBinLabel(ybin, sortedNames[ybin-1].Data() );
    }
  }
  else {
    for ( int xbin=1; xbin<=h->GetNbinsX(); xbin++ ) {
      h->GetXaxis()->SetBinLabel(xbin, sortedNames[xbin-1].Data());
      h->SetBinContent(xbin, nameToTypeMap[sortedNames[xbin-1]]);
    }
  }
}

void EventCounter::countEvent( int itype, TString analyserName, bool isPass ) {

  TString evName = typeToNameMap[itype];

  countEvent( evName, analyserName, isPass );

}

void EventCounter::countEvent( TString evName, TString analyserName, bool isPass ) {

  //int ybin = sortedNames.index( evName );
  //int xbin = analyserNames.index( analyserName );
  //int ybin = * ( find ( sortedNames.begin(), sortedNames.end(), evName ) );
  //int xbin = * ( find ( analyserNames.begin(), analyserNames.end(), analyserName ) );
  // budget
  int xbin = -1;
  int ybin = -1;
  for ( int x=0; x<analyserNames.size(); x++) {
    if ( analyserNames[x] == analyserName ) {
      xbin = x;
      break;
    }
  }
  for (int y=0; y<sortedNames.size(); y++) {
    if ( sortedNames[y] == evName ) {
      ybin = y;
      break;
    }
  }

  hProc->Fill(xbin,ybin);
  if ( isPass ) hPass->Fill(xbin, ybin);
  else          hFail->Fill(xbin, ybin);

  int bin2d = hEff->FindBin( xbin, ybin );
  hEff->SetBinContent ( bin2d,  100.* double( hPass->GetBinContent(bin2d) ) / double( hProc->GetBinContent(bin2d) ) );

}

void EventCounter::printShortSummary( TString evName ) {

    int index=-1;
    for ( unsigned int i=0; i<sortedNames.size(); i++ ) {
      if ( sortedNames[i] == evName ) {
        index = i;
        break;
      }
    }

    int itype  = nameToTypeMap[ sortedNames[index] ];

    print("EventCounter::shortSummary()", Form("%-30s (itype=%-4d)", evName.Data(), itype) );

    int startEvs   = int(hProc->GetBinContent( hProc->FindBin(0,index) ) );
    int passEvs    = int(hPass->GetBinContent( hProc->FindBin( analyserNames.size()-1, index) ) );
    double sumEff  = 100.* double ( passEvs ) / double ( startEvs );

    for ( int x=0; x < analyserNames.size(); x++ ) {

      int bin2d = hPass->FindBin(x,index);

      print("", Form("       %-15s -- %5d / %-5d events passed = %6.2f%% efficient ", analyserNames[x].Data(), int(hPass->GetBinContent(bin2d)), int(hProc->GetBinContent(bin2d)), hEff->GetBinContent(bin2d) ) );
    }
    print("", Form("     %-15s -- %5d / %-5d events passed = %6.2f%% efficient ", "TOTAL", passEvs, startEvs, sumEff) );
}

void EventCounter::printSummary() {

  int allEventsSeen        = 0;
  int allEventsSeenPassing = 0;

  print("EventCounter::printSummary()", "Event Count Summary:");
  for ( int y=0; y < sortedNames.size(); y++ ) {

    int itype  = nameToTypeMap[ sortedNames[y] ];
    TString evname = sortedNames[y];

    int startEvs   = int(hProc->GetBinContent( hProc->FindBin(0,y) ) );
    int passEvs    = int(hPass->GetBinContent( hProc->FindBin( analyserNames.size()-1, y) ) );
    double sumEff  = 100.* double ( passEvs ) / double ( startEvs );

    allEventsSeen        += startEvs;
    allEventsSeenPassing += passEvs;

    printShortSummary( sortedNames[y] );
    //for ( int x=0; x < analyserNames.size(); x++ ) {

      //int bin2d = hPass->FindBin(x,y);

      //print("", Form("       %-15s -- %5d / %-5d events passed = %6.2f%% efficient ", analyserNames[x].Data(), int(hPass->GetBinContent(bin2d)), int(hProc->GetBinContent(bin2d)), hEff->GetBinContent(bin2d) ) );
    //}
    //print("", Form("     %-15s -- %5d / %-5d events passed = %6.2f%% efficient ", "TOTAL", passEvs, startEvs, sumEff) );
  }

  double totalEff = 100.* double( allEventsSeenPassing ) / double( allEventsSeen );

  print("", Form("  %-15s -- %5d / %-5d events passed = %6.2f%% efficient ", "GLOBAL", allEventsSeenPassing, allEventsSeen, totalEff ) );

}
