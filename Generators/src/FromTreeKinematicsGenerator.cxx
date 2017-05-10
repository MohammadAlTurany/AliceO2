#include "Generators/FromTreeKinematicsGenerator.h"
#include <TTree.h>
#include <TFile.h>
#include <TBranch.h>
#include <FairLogger.h>
#include <TParticle.h>
#include <TClonesArray.h>
#include <FairPrimaryGenerator.h>
#include <cstdio>

ClassImp(o2::eventgen::FromTreeKinematicsGenerator)

namespace o2 {
namespace eventgen {

  FromTreeKinematicsGenerator::FromTreeKinematicsGenerator(const char *name) {
    mEventFile = TFile::Open(name);
    if (mEventFile == nullptr) {
      LOG(ERROR) << "EventFile " << name << " not found \n"; 
      return;
    }
    // the kinematics will be stored inside a Tree "TreeK" with branch "Particles"
    // different events are stored inside TDirectories

    // we need to probe for the number of events
    char eventstring[7];
    TObject *object = nullptr;
    do {
      sprintf(eventstring, "Event%d", mEventsAvailable);
      //std::cout << "probing for " << eventstring << "\n";
      object = mEventFile->Get(eventstring);
      //std::cout << "got " << object << "\n";
      if (object != nullptr) mEventsAvailable++;
    }
    while (object != nullptr);
    std::cout << "Found " << mEventsAvailable << " events in this file \n";
  }
  
  Bool_t FromTreeKinematicsGenerator::ReadEvent(FairPrimaryGenerator* primGen) {
    if (mEventCounter < mEventsAvailable) {
	// get the tree and the branch
	char treestring[14];
	sprintf(treestring, "Event%d/TreeK", mEventCounter);
        TTree *tree = (TTree*) mEventFile->Get(treestring);
	if (tree == nullptr) return kFALSE;

	auto branch = tree->GetBranch("Particles");
        TParticle *primary = new TParticle();
	branch->SetAddress(&primary);
        for (int i = 0; i < branch->GetEntries(); ++i) {
	  branch->GetEntry(i);// fill primary
          auto pdgid = primary->GetPdgCode();
	  auto px = primary->Px();
	  auto py = primary->Py();
	  auto pz = primary->Pz();
	  auto vx = primary->Vx();
	  auto vy = primary->Vy();
	  auto vz = primary->Vz();
	  
	  auto parent = -1;
	  bool wanttracking = true;
	  auto e = primary->Energy();
	  auto tof = primary->T();
	  auto weight = primary->GetWeight();
	  primGen->AddTrack(pdgid, px, py, pz, vx, vy, vz, parent, wanttracking, e, tof, weight);
        }
	mEventCounter++;
        return kTRUE;
      }
      else {
        LOG(ERROR) << "FromTreeKinematicsGenerator: Ran out of events\n";
      }
    
    return kFALSE;
  }

} // end namespace
} // end namespace o2
