#ifndef cTrackerFine_h
#define cTrackerFine_h

#include <list>
#include <utility>
#include <Rtypes.h>
#include <TH2.h>
#include <TVector3.h>
#include "cFittedLine.h"

template <class T>
class cTrackerFine
{
public:
  cTrackerFine();
  virtual ~cTrackerFine();

  // Set the number of step in the accumulator used in the 2D Hough transform
  void setAngularSteps(UInt_t);
  void setDistanceSteps(UInt_t);

  // Set maximum distance from the origin for the Hough transform
  void setMaxDistance(Float_t);

  // Set the minimum energy and number of points to be considered a line
  void setMinimumEnergy(Float_t);
  void setMinPoints(UInt_t);

  void setMultFactor(std::vector<Float_t>);

  // Set the distance of the points from the line to be considered part of it
  void setPointDistance(Float_t);

  void setLinesFittable(bool);

  // Adds a point to the collection
  void addPoint(const T &);

  // Returns histogram of the current projectiveLineSpace
  TH2 *convertSpaceToHist() const;

  typedef std::list<T> pointList;

  enum class direction
  {
    x = 0,
    y = 1,
    z = 2
  };

  // private:
  UInt_t nAngularSteps;
  UInt_t nDistanceSteps;
  Float_t maxDistance;
  Float_t minEnergy;
  Float_t pointDistance;
  UInt_t minPoints;

  bool isLineFittable;

  // bool to use or not the 'oneSided' trick
  bool oneSidedTracks = true;

  Float_t **projectiveLineSpace;

  std::vector<Float_t> multFactor = {1., 1., 1.};

  // List of points, accumulator and a list with the lines found
  pointList points;
  pointList accumulator;
  //std::list<pointList> lines;
  std::list<cFittedLine<T>> fittedLines;

  // Theta and distance of the best 2D line found after the first Hough transform
  Float_t bestTheta;
  Float_t bestDist;

  // Functions used to allocate and deallocate the memory for the projectiveLineSpace
  void makeSpace();
  void clearSpace();

  // Fill projectiveLineSpace with zeroes
  void zeroSpace();

  // Find max index in the projectiveLineSpace
  std::pair<UInt_t, UInt_t> findMaximumPoint() const;

  // DEPRECATED Find vector of three max indices in the projectiveLineSpace
  std::vector<std::pair<UInt_t, UInt_t>> findMaxima() const;

  // Try to find maxima in Hough space that are related to the BEAM tracks;
  std::pair<UInt_t, UInt_t> findMaximumPoint(bool beam) const;

  // Get theta angle from the index of the projectiveLineSpace array
  Float_t getTheta(UInt_t) const;

  // Get distance from the index of the projectiveLineSpace array
  Float_t getDistance(UInt_t) const;

  // Run a 2D Hough transform as with direction a as x and b as y
  void runInitialHoughTransform(direction a, direction b);

  // Load points in the maximum in the accumulator, returns true if the maximum point has sufficient energy
  bool loadPointsInMaximum(direction a, direction b);

  // Returns a pair containing the new x and y vectors based on bestTheta
  std::pair<TVector3, TVector3> getNewHoughAxis(direction, direction);

  // Run the second hough transform using the points in the accumulator
  void runSecondHoughTransform(direction, direction);

  // Load points in the maximum as a line, returns true if a line is found
  bool loadPointsInLine(direction a, direction b);

  // Fits each line contained in lines
  void fitLines();

  // Function for the user to start the tracking
  void track(direction, direction);
};

#include "cTrackerFine.hpp"
#endif
