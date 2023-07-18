
#include "InttEvent.h"
#include "InttCluster.h"

#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include <TFile.h>
#include <TTree.h>
#include <TNtuple.h>
#include <TH1.h>
#include <TDirectory.h>

using namespace std;


InttEvent* inttEvt = nullptr;

TFile* tree_file = nullptr;
TTree* tree = nullptr;

TFile* outtree_file = nullptr;

TH1F* h_adc;
TH1F* h_adc1;
TH1F* h_adc2;
TH1F* h_nclus;
TH1F* h_nbcofull;
TH1F* h_clussize;
TH1F* h_bcodiff;
TH1F* h_bcodiff2;
TNtuple* h_clustree;
TTree* h_evttree;


// variable for evttree
int pid, evt, nclus[14];
uint64_t bco_full;


//////////////////////////////////////////////////
//
int init_done=0;

int InitAnalysis(const char* outRootfile)
{
  if (init_done) return 1;
  init_done = 1;

  TDirectory* gDir = gDirectory;

  outtree_file = TFile::Open(outRootfile, "recreate");

  h_adc  = new TH1F("h_adc", "adc", 100, 0, 300);
  h_adc1 = new TH1F("h_adc1", "adc1", 100, 0, 300);
  h_adc2 = new TH1F("h_adc2", "adc2", 100, 0, 300);
  h_nclus = new TH1F("h_nclus", "nclus", 100, 0, 2000);
  h_nbcofull = new TH1F("h_nbcofull", "nbcofull", 100, 0, 100);
  h_clussize = new TH1F("h_clussize", "cluster size", 100, 0, 100);
  h_bcodiff  = new TH1F("h_bcodiff", "bcodiff", 1000, -10000000, 10000000);
  h_bcodiff2 = new TH1F("h_bcodiff2", "bcodiff2", 1000, -500, 500);

  h_clustree = new TNtuple("h_clustree", "cluster tree", "nclus:size:adc:module:chip_id:chan_id:bco_full:roc:barrel:layer:ladder:arm");

  h_evttree = new TTree("h_evttree", "Event tree");

  h_evttree->Branch("pid",      &pid, "pid/I");
  h_evttree->Branch("bco_full", &bco_full, "bco_full/l");
  h_evttree->Branch("event",    &evt,   "event/I");
  h_evttree->Branch("nclus",     nclus, "nclus[14]/I");


  

  gDirectory = gDir;

  return 0;
}

int Process_event (InttEvent* inttEvent)
{

  InttClusterList* inttClusterList = new InttClusterList;

  Clustering(inttEvent, inttClusterList);

  ////////////////////////////////////
  // analysis
  //
  int nHits = inttEvent->getNHits();
  map<Long64_t, int> vbcocount;
  for(int ihit = 0; ihit<nHits; ihit++)
  {
      Long64_t bco_f = inttEvent->getHit(ihit)->bco_full;
      auto itrBC = vbcocount.find(bco_f);
      if(itrBC==vbcocount.end()){
        vbcocount.insert(make_pair(bco_f, 0));
      }
      else {
        itrBC->second++;
      }
  }
  cout<<"nbco : "<<vbcocount.size()<<endl;
  h_nbcofull->Fill(vbcocount.size());
  Long64_t bco_main=0;
  int ncount_max=0;
  for(auto itrBC = vbcocount.begin(); itrBC!=vbcocount.end(); ++itrBC)
  {
      if(itrBC->second >= ncount_max) {
        ncount_max=itrBC->second;
        bco_main = itrBC->first;
      }
      cout<<"    "<<itrBC->first<<" "<<itrBC->second<<endl;
  }
  for(auto itrBC = vbcocount.begin(); itrBC!=vbcocount.end(); ++itrBC)
  {
     Long64_t bco_diff = itrBC->first - bco_main;
     cout<<"    diff= "<<bco_diff<<endl;
     h_bcodiff->Fill(bco_diff, itrBC->second);
     h_bcodiff2->Fill(bco_diff, itrBC->second);
  }

  


  // cluster analysis
  int nClusters = inttClusterList->getNhits();
  h_nclus->Fill(nClusters);

  cout<<"Nclus: "<<nClusters<<endl;
  float buf[12];
  for(int ilad=0; ilad<14; ilad++){ nclus[ilad]=0;}
  pid=0;

  for(int iclus = 0; iclus<nClusters; iclus++)
  {
      InttCluster* clus = inttClusterList->getCluster(iclus);
      if(clus==nullptr) {cout<<"Null cluster : "<<iclus<<endl; continue; }

      h_adc->Fill(clus->adc);
      if(clus->nhits==1) h_adc1->Fill(clus->adc);
      if(clus->nhits>=2) h_adc2->Fill(clus->adc);

      h_clussize->Fill(clus->nhits);

      // "nclus:size:adc:module:chip_id:chan_id:bco_full:roc:barrel:layer:ladder:arm");
      buf[ 0] = nClusters;
      buf[ 1] = clus->nhits;
      buf[ 2] = clus->adc;
      buf[ 3] = clus->module;
      buf[ 4] = clus->chip_id;
      buf[ 5] = clus->ch;
      buf[ 6] = clus->bco_full;
      buf[ 7] = clus->roc;
      buf[ 8] = clus->barrel;
      buf[ 9] = clus->layer;
      buf[10] = clus->ladder;
      buf[11] = clus->arm;

      h_clustree->Fill(buf);

      if(iclus==0){
          pid = clus->pid;
          bco_full = clus->bco_full;
      }

      nclus[clus->module]++;
  }
  //
  ////////////////////////////////////
  //
  h_evttree->Fill();

  

  delete inttClusterList;

  return 0;
}

int RunAnalysis(const char *rootFile)
{

  cout<<"file : "<<rootFile<<endl;
  TDirectory* gDir = gDirectory;

  tree_file = TFile::Open(rootFile);

  gDirectory = gDir;

  inttEvt = nullptr;

  tree = (TTree*)tree_file->Get("tree");
  tree->SetBranchAddress("event", &inttEvt);

  //read the Tree
  for (Int_t ev = 0; ev < tree->GetEntries(); ev++) {
    tree->GetEntry(ev);

    cout<<"Event : "<<ev<<" "<<inttEvt->evtSeq<<endl;
    inttEvt->sort();
    //inttEvt->show();
    
    Process_event(inttEvt);
  }
  //delete hfile;
  tree_file->Close();

  h_adc->Print();

  //  tree->Write();
  outtree_file->cd();
  outtree_file->Write();
  outtree_file->Close();


  return 0;
}

