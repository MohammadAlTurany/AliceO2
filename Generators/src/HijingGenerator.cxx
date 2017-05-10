#include "Generators/HijingGenerator.h"

ClassImp(o2::eventgen::HijingGenerator)

namespace o2 {
namespace eventgen {

Bool_t HijingGenerator::ReadEvent(FairPrimaryGenerator* primGen) {
  return kTRUE;
}

} // end namespace
} // end namespace o2
