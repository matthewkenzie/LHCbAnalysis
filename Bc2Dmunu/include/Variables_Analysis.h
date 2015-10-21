#ifndef Bc2Dmunu_Variables_Analysis_h
#define Bc2Dmunu_Variables_Analysis_h

#include <iostream>
#include "TROOT.h"
#include "TTree.h"

#include "Variables.h"

namespace Bc2Dmunu {

  class Variables_Analysis : public Variables {

    public:

      Variables_Analysis();
      ~Variables_Analysis();

      void setInputBranches(TTree *tree);
      void setOutputBranches(TTree *tree);

      Double_t        Bplus_IPCHI2_OWNPV;
      Double_t        Bplus_IP_OWNPV;
      Double_t        Bplus_MINIP;
      Double_t        Bplus_MINIPCHI2;
      Double_t        Bplus_P;
      Double_t        Bplus_PT;
      Double_t        Bplus_PE;
      Double_t        Bplus_PX;
      Double_t        Bplus_PY;
      Double_t        Bplus_PZ;
      Double_t        Bplus_MM;
      Double_t        Bplus_MMERR;
      Double_t        Bplus_M;
      Double_t        Bplus_DIRA_OWNPV;
      Double_t        Bplus_DTF_CHI2;
      Double_t        Bplus_ENDVERTEX_CHI2;
      Double_t        Bplus_FD_CHI2;
      Double_t        Bplus_Mcorr;
      Double_t        Bplus_TAU;
      Bool_t          Bplus_L0Global_Dec;
      Bool_t          Bplus_L0Global_TIS;
      Bool_t          Bplus_L0Global_TOS;
      Bool_t          Bplus_Hlt1Global_Dec;
      Bool_t          Bplus_Hlt1Global_TIS;
      Bool_t          Bplus_Hlt1Global_TOS;
      Bool_t          Bplus_Hlt1Phys_Dec;
      Bool_t          Bplus_Hlt1Phys_TIS;
      Bool_t          Bplus_Hlt1Phys_TOS;
      Bool_t          Bplus_Hlt2Global_Dec;
      Bool_t          Bplus_Hlt2Global_TIS;
      Bool_t          Bplus_Hlt2Global_TOS;
      Bool_t          Bplus_Hlt2Phys_Dec;
      Bool_t          Bplus_Hlt2Phys_TIS;
      Bool_t          Bplus_Hlt2Phys_TOS;
      Bool_t          Bplus_L0MuonDecision_Dec;
      Bool_t          Bplus_L0MuonDecision_TIS;
      Bool_t          Bplus_L0MuonDecision_TOS;
      Bool_t          Bplus_Hlt1TrackMuonDecision_Dec;
      Bool_t          Bplus_Hlt1TrackMuonDecision_TIS;
      Bool_t          Bplus_Hlt1TrackMuonDecision_TOS;
      Bool_t          Bplus_Hlt2TopoMu2BodyBBDTDecision_Dec;
      Bool_t          Bplus_Hlt2TopoMu2BodyBBDTDecision_TIS;
      Bool_t          Bplus_Hlt2TopoMu2BodyBBDTDecision_TOS;
      Bool_t          Bplus_Hlt2TopoMu3BodyBBDTDecision_Dec;
      Bool_t          Bplus_Hlt2TopoMu3BodyBBDTDecision_TIS;
      Bool_t          Bplus_Hlt2TopoMu3BodyBBDTDecision_TOS;
      Bool_t          Bplus_Hlt2SingleMuonDecision_Dec;
      Bool_t          Bplus_Hlt2SingleMuonDecision_TIS;
      Bool_t          Bplus_Hlt2SingleMuonDecision_TOS;
      Double_t        D0_IPCHI2_OWNPV;
      Double_t        D0_IP_OWNPV;
      Double_t        D0_MINIP;
      Double_t        D0_MINIPCHI2;
      Double_t        D0_P;
      Double_t        D0_PT;
      Double_t        D0_PE;
      Double_t        D0_PX;
      Double_t        D0_PY;
      Double_t        D0_PZ;
      Double_t        D0_MM;
      Double_t        D0_MMERR;
      Double_t        D0_M;
      Double_t        D0_DIRA_OWNPV;
      Double_t        D0_DTF_CHI2;
      Double_t        D0_ENDVERTEX_CHI2;
      Double_t        D0_FD_CHI2;
      Double_t        D0_ID;
      Double_t        D0_TAU;
      Double_t        Kminus_IPCHI2_OWNPV;
      Double_t        Kminus_IP_OWNPV;
      Double_t        Kminus_MINIP;
      Double_t        Kminus_MINIPCHI2;
      Double_t        Kminus_P;
      Double_t        Kminus_PT;
      Double_t        Kminus_PE;
      Double_t        Kminus_PX;
      Double_t        Kminus_PY;
      Double_t        Kminus_PZ;
      Double_t        Kminus_M;
      Double_t        Kminus_NNK;
      Double_t        Kminus_NNKmu;
      Double_t        Kminus_NNKpi;
      Double_t        Kminus_NNmu;
      Double_t        Kminus_NNpi;
      Double_t        Kminus_PIDK;
      Double_t        Kminus_PIDmu;
      Double_t        Kminus_TRACK_CHI2;
      Double_t        Kminus_ghost;
      Double_t        Kminus_inMuon;
      Double_t        Kminus_isMuon;
      Double_t        Kminus_isMuonLoose;
      Double_t        piplus_IPCHI2_OWNPV;
      Double_t        piplus_IP_OWNPV;
      Double_t        piplus_MINIP;
      Double_t        piplus_MINIPCHI2;
      Double_t        piplus_P;
      Double_t        piplus_PT;
      Double_t        piplus_PE;
      Double_t        piplus_PX;
      Double_t        piplus_PY;
      Double_t        piplus_PZ;
      Double_t        piplus_M;
      Double_t        piplus_NNK;
      Double_t        piplus_NNKmu;
      Double_t        piplus_NNKpi;
      Double_t        piplus_NNmu;
      Double_t        piplus_NNpi;
      Double_t        piplus_PIDK;
      Double_t        piplus_PIDmu;
      Double_t        piplus_TRACK_CHI2;
      Double_t        piplus_ghost;
      Double_t        piplus_inMuon;
      Double_t        piplus_isMuon;
      Double_t        piplus_isMuonLoose;
      Double_t        muplus_IPCHI2_OWNPV;
      Double_t        muplus_IP_OWNPV;
      Double_t        muplus_MINIP;
      Double_t        muplus_MINIPCHI2;
      Double_t        muplus_P;
      Double_t        muplus_PT;
      Double_t        muplus_PE;
      Double_t        muplus_PX;
      Double_t        muplus_PY;
      Double_t        muplus_PZ;
      Double_t        muplus_M;
      Double_t        muplus_NNK;
      Double_t        muplus_NNKmu;
      Double_t        muplus_NNKpi;
      Double_t        muplus_NNmu;
      Double_t        muplus_NNpi;
      Double_t        muplus_PIDK;
      Double_t        muplus_PIDmu;
      Double_t        muplus_TRACK_CHI2;
      Double_t        muplus_ghost;
      Double_t        muplus_inMuon;
      Double_t        muplus_isMuon;
      Double_t        muplus_isMuonLoose;
      UInt_t          nCandidate;
      ULong64_t       totCandidates;
      ULong64_t       EventInSequence;
      UInt_t          runNumber;
      //ULong64_t       eventNumber;
      UInt_t          BCID;
      Int_t           BCType;
      UInt_t          OdinTCK;
      UInt_t          L0DUTCK;
      UInt_t          HLT1TCK;
      UInt_t          HLT2TCK;
      ULong64_t       GpsTime;
      Short_t         Polarity;
      Int_t           nPVs;
      Int_t           nTracks;
      Int_t           nLongTracks;
      Int_t           nDownstreamTracks;
      Int_t           nUpstreamTracks;
      Int_t           nVeloTracks;
      Int_t           nTTracks;
      Int_t           nBackTracks;
      Int_t           nRich1Hits;
      Int_t           nRich2Hits;
      Int_t           nVeloClusters;
      Int_t           nITClusters;
      Int_t           nTTClusters;
      Int_t           nOTClusters;
      Int_t           nSPDHits;
      Int_t           nMuonCoordsS0;
      Int_t           nMuonCoordsS1;
      Int_t           nMuonCoordsS2;
      Int_t           nMuonCoordsS3;
      Int_t           nMuonCoordsS4;
      Int_t           nMuonTracks;
  };

}
#endif

