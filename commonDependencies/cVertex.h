#ifndef cVertex_h
#define cVertex_h

#include "cFittedLine.h"
#include "TObject.h"
#include "TVector3.h"
#include <list>

template <class T>
class cVertex : public TObject
{
public:
  cVertex() : x(0.), y(0.), z(0.) {}
  cVertex(double xu, double yu, double zu) : x(xu), y(yu), z(zu) {}
  cVertex(TVector3 &v) : x(v[0]), y(v[1]), z(v[2]) {}
  virtual ~cVertex() {}

  std::list<cFittedLine<T>> &getTracks() { return tracks; }
  const std::list<cFittedLine<T>> &getTracks() const { return tracks; }

  operator TVector3() const { return TVector3(x, y, z); }

  double getX() const { return x; }
  double getY() const { return y; }
  double getZ() const { return z; }

private:
  std::list<cFittedLine<T>> tracks;
  double x, y, z;

  ClassDef(cVertex, 2);
};

#endif
