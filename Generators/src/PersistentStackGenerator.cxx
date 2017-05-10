#include "Generators/PersistentStackGenerator.h"
#include <TTree.h>
#include <TFile.h>
#include <TBranch.h>
#include <FairLogger.h>
#include <TParticle.h>
#include <TClonesArray.h>
#include <FairPrimaryGenerator.h>

ClassImp(o2::eventgen::PersistentStackGenerator)

namespace o2 {
namespace eventgen {

  PersistentStackGenerator::PersistentStackGenerator(const char *name) {
    mEventFile = TFile::Open(name);
    if (mEventFile == nullptr) {
      LOG(ERROR) << "EventFile " << name << " not found \n"; 
      return;
    }

    // try to read the tree containing tree 
    TTree *eventTree = nullptr;
    mEventFile->GetObject("EventTree", eventTree);

    if (eventTree == nullptr){
      LOG(ERROR) << "Could not read event tree from file " << name << "\n"; 
      return;
    }

    // get the branch
    mEventBranch = eventTree->GetBranch("Events");
    if (mEventBranch == nullptr){
      LOG(ERROR) << "Could not read event branch from tree\n"; 
      return;
    }

    // connect the branch with the container
    mEventBranch->SetAddress(&mParticles);
    //
    LOG(INFO) << "PersistentStackGenerator: Loading event file succesful " << std::endl;
    LOG(INFO) << "PersistentStackGenerator: File contains " << mEventBranch->GetEntries() << " events " << std::endl;
  }
  
  Bool_t PersistentStackGenerator::ReadEvent(FairPrimaryGenerator* primGen) {
    if (mEventBranch != nullptr) {
      if (mEventCounter < mEventBranch->GetEntries()) {
        // read the event from the branch
	mEventBranch->GetEntry(mEventCounter);
        for (int i = 0; i < mParticles->GetEntries(); ++i) {
	  auto primary = static_cast<TParticle*>(mParticles->UncheckedAt(i));
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
        LOG(ERROR) << "PersistentStackGenerator: Ran out of events\n";
      }
    }
    return kFALSE;
  }

} // end namespace
} // end namespace o2
