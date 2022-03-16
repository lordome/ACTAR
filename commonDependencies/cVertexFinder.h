#ifndef cVertexFinder_h
#define cVertexFinder_h

#include <list>
#include <TVector3.h>
#include "cFittedEvent.h"

template <class T>
class cVertexFinder
{
public:
  void findVertex(cFittedEvent<T> *);
  double getMaxZ() const;
  void setMaxZ(double);
  double getMinZ() const;
  void setMinZ(double);
  void setMaxDist(double);
  double getMaxDist() const;

  void setParStart(std::vector<double>);
  std::vector<double> getParStart() const;

  double getError(const cFittedLine<T> &, const T &) const;
  double getError(TVector3, TVector3, const T &) const;
  double getError(TVector3, TVector3, TVector3) const;

  double operator()(const double *);

  bool getMinDistance(std::list<T> l1, std::list<T> l2, const double target);

private:
  double minZ = 0.;
  double maxZ = 0.;
  double maxDist = 0.;

  std::vector<double> parStart = {0., 0., 0.};

  std::list<cFittedLine<T>> bestTracks;
};

#include "cVertexFinder.hpp"

#endif
