/// \author S. Wenzel - Mai 2017

#ifndef ALICEO2_GEN_FROMKINEMATICS_H_
#define ALICEO2_GEN_FROMKINEMATICS_H_

#include "FairGenerator.h"

class TBranch;
class TFile;

namespace o2 {
namespace eventgen {

/// This class implements a generic FairGenerator which
/// reads the particles from a kinemenatics file produced by AliRoot 
class FromTreeKinematicsGenerator : public FairGenerator
{
  public:
   FromTreeKinematicsGenerator() = default;
   FromTreeKinematicsGenerator(const char *name);
   
   // the FairGenerator interface methods

   /** Generates (or reads) one event and adds the tracks to the
    ** injected primary generator instance.
    ** @param primGen  pointer to the primary FairPrimaryGenerator
    **/
   bool ReadEvent(FairPrimaryGenerator* primGen) override;
   
  private:
   TFile*      mEventFile = nullptr; //! the file containing the persistent events
   int         mEventCounter = 0;
   int         mEventsAvailable = 0;
   
   ClassDefOverride(FromTreeKinematicsGenerator, 1);
};

} // end namespace eventgen
} // end namespace o2

#endif
