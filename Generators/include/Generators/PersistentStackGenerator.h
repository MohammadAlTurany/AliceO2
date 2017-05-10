/// \author S. Wenzel - Mai 2017

#ifndef ALICEO2_GEN_PERSISTENTSTACK_H_
#define ALICEO2_GEN_PERSISTENTSTACK_H_

#include "FairGenerator.h"

class TBranch;
class TFile;
class TClonesArray;

namespace o2 {
namespace eventgen {

/// This class implements a generic FairGenerator which
/// reads the particles from a persistent Stack file (essentially
/// a branch of TClonesArrays of TParticles)
/// The persistent file can be generated with any sort of generator
class PersistentStackGenerator : public FairGenerator
{
  public:
   PersistentStackGenerator() = default;
   PersistentStackGenerator(const char *name);
   
   // the FairGenerator interface methods

   /** Generates (or reads) one event and adds the tracks to the
    ** injected primary generator instance.
    ** @param primGen  pointer to the primary FairPrimaryGenerator
    **/
   bool ReadEvent(FairPrimaryGenerator* primGen) override;
   
  private:
   TFile*      mEventFile = nullptr; //! the file containing the persistent events
   TBranch*    mEventBranch = nullptr; //! the branch containing the particle collections
   TClonesArray *mParticles = nullptr; //!
   int         mEventCounter = 0;
   
   ClassDefOverride(PersistentStackGenerator, 1);
};

} // end namespace eventgen
} // end namespace o2

#endif
