/// \file Point.h
/// \brief Class for TPC Point
#ifndef ALICEO2_TPC_POINT_H
#define ALICEO2_TPC_POINT_H

#include "SimulationDataFormat/BaseHits.h"
#include <vector>

namespace o2 {
namespace TPC {

// a minimal and plain TPC hit class
class ElementalHit {
 public:
  Point3D<float> mPos; // cartesian position of Hit
  float mTime = -1;    // time of flight
  float mELoss = -2;   // energy loss

 public:
  ElementalHit() = default; // for ROOT IO
  ~ElementalHit() = default;

  // constructor
  ElementalHit(float x, float y, float z, float time, float e)
    :  mPos(x, y, z), mTime(time), mELoss(e) {}

  ClassDefNV(ElementalHit,1);
};

// a higher order hit class encapsulating
// a set of elemental hits belonging to the same trackid (and sector)
// construct used to do less MC truth linking and to save memory
// this hitcontainer is linkable with FairLinks,
// and can be stored as element of a TClonesArray into a branch
class LinkableHitGroup : public o2::BaseHit {
public:
  LinkableHitGroup() :
#ifdef HIT_AOS
  mHits()
#else
  mHitsX_v(),
  mHitsY_v(),
  mHitsZ_v(),
  mHitsT_v(),
  mHitsE_v(),
  mHitsX(nullptr),
  mHitsY(nullptr),
  mHitsZ(nullptr),
  mHitsT(nullptr),
  mHitsE(nullptr),
  mSize(0)
#endif
    {
    }

  LinkableHitGroup(int trackID) :
#ifdef HIT_AOS
  mHits()
#else
  mHitsX_v(),
  mHitsY_v(),
  mHitsZ_v(),
  mHitsT_v(),
  mHitsE_v(),
  mHitsX(nullptr),
  mHitsY(nullptr),
  mHitsZ(nullptr),
  mHitsT(nullptr),
  mHitsE(nullptr),
  mSize(0)
#endif
  {
    SetTrackID(trackID);
  }

  ~LinkableHitGroup() override = default;

  void addHit(float x, float y, float z, float time, short e) {
#ifdef HIT_AOS
    mHits.emplace_back(x,y,z,time,e);
#else
    //    if(e > 255){
    //  std::cerr << "charge overflow << " << e << "\n";
    //}
    unsigned char q = e;
    mHitsX_v.emplace_back(x);
    mHitsY_v.emplace_back(y);
    mHitsZ_v.emplace_back(z);
    mHitsT_v.emplace_back(time);
    mHitsE_v.emplace_back(q);
    mSize=mHitsX_v.size();
    mHitsX=&mHitsX_v[0];
    mHitsY=&mHitsY_v[0];
    mHitsZ=&mHitsZ_v[0];
    mHitsT=&mHitsT_v[0];
    mHitsE=&mHitsE_v[0];
#endif
  }

  size_t getSize() const {
#ifdef HIT_AOS
    return mHits.size();
#else
    return mSize;
#endif
  }

  ElementalHit getHit(size_t index) const {
#ifdef HIT_AOS
    // std::vector storage
    return mHits[index];
#else
    return ElementalHit(mHitsX[index],mHitsY[index],mHitsZ[index],mHitsT[index],mHitsE[index]);
#endif
  }

  void shrinkToFit() {
    // shrink all the containers to have exactly the required size
    // might improve overall memory consumption
#ifdef HIT_AOS
    // std::vector storage
    mHits.shrink_to_fit();
#else
    mHitsX_v.shrink_to_fit();
    mHitsY_v.shrink_to_fit();
    mHitsZ_v.shrink_to_fit();
    mHitsT_v.shrink_to_fit();
    mHitsE_v.shrink_to_fit();
    mHitsX=&mHitsX_v[0];
    mHitsY=&mHitsY_v[0];
    mHitsZ=&mHitsZ_v[0];
    mHitsT=&mHitsT_v[0];
    mHitsE=&mHitsE_v[0];
#endif
  }

  // in future we might want to have a method
  // FitAndCompress()
  // which does a track fit and produces a parametrized hit
  // (such as done in a similar form in AliRoot)
public:
#ifdef HIT_AOS
  std::vector<o2::TPC::ElementalHit> mHits; // the hits for this group
#else
  std::vector<float> mHitsX_v; //! do not stream this (just for memory convenience)
  std::vector<float> mHitsY_v; //! do not stream this
  std::vector<float> mHitsZ_v; //! do not stream this
  std::vector<float> mHitsT_v; //! do not stream this
  std::vector<short> mHitsE_v; //! do not stream this ( unsigned char enough for number of electrons ?? )
  // let us stream ordinary buffers for compression AND IO/speed!!
  Int_t mSize;
  float *mHitsX = nullptr; //[mSize]
  float *mHitsY = nullptr; //[mSize]
  float *mHitsZ = nullptr; //[mSize]
  float *mHitsT = nullptr; //[mSize]
  short *mHitsE = nullptr; //[mSize]
#endif
  ClassDefOverride(LinkableHitGroup, 1);
};

class Point : public o2::BasicXYZEHit<float>
{
  public:

    /// Default constructor
    Point() = default;

    /// Constructor with arguments
    /// @param trackID  Index of MCTrack
    /// @param detID    Detector ID
    /// @param pos      Ccoordinates at entrance to active volume [cm]
    /// @param mom      Momentum of track at entrance [GeV]
    /// @param tof      Time since event start [ns]
    /// @param length   Track length since creation [cm]
    /// @param eLoss    Energy deposit [GeV]
    Point(float x, float y, float z, float time, float nElectrons, float trackID, float detID);

    /// Destructor
    ~Point() override = default;

    /// Output to screen
    void Print(const Option_t* opt) const override;

  private:
    /// Copy constructor
    Point(const Point& point);
    Point operator=(const Point& point);

  ClassDefOverride(o2::TPC::Point,1)
};

inline
Point::Point(float x, float y, float z, float time, float nElectrons, float trackID, float detID)
  : BasicXYZEHit<float>(x, y, z, time, nElectrons, trackID, detID)
{}

}
}

#endif
