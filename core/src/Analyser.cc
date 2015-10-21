#include "TLorentzVector.h"

#include "Utils.h"
#include "Analyser.h"

using namespace Utils;

Analyser::Analyser(TString _name):
  name(_name),
  processedEvents(0),
  passedEvents(0)
{}

Analyser::~Analyser(){}

void Analyser::Init() {
  print(name+"::Init()","Initialising Analyser: "+name);
}

void Analyser::Term() {
  print(name+"::Term()","Terminating Analyser: "+name);
}
