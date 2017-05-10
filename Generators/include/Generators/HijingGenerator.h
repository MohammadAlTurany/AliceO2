/// \file HijingGenerator.h
/// \brief Definition of the Hijing event generator wrapper
/// \author S. Wenzel - Mai 2017

#ifndef ALICEO2_GEN_HIJING_H_
#define ALICEO2_GEN_HIJING_H_

#include "FairGenerator.h"

namespace o2 {
namespace eventgen {

/// This class wraps the Hijing event generator into
/// a FairGenerator
class HijingGenerator : public FairGenerator
{
  public:
   HijingGenerator() = default;

   // the FairGenerator interface methods

   /** Generates (or reads) one event and adds the tracks to the
    ** injected primary generator instance.
    ** @param primGen  pointer to the primary FairPrimaryGenerator
    **/
   bool ReadEvent(FairPrimaryGenerator* primGen) override;
   
   ClassDefOverride(HijingGenerator, 1)
};

} // end namespace eventgen
} // end namespace o2

#endif
