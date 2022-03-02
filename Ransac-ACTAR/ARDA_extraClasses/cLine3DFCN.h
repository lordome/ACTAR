#ifndef cLine3DFCN_h
#define cLine3DFCN_h

#include <cmath>
#include "TVector3.h"

template <class T>
class cLine3DFCN {
  public:
    cLine3DFCN() {}
    virtual ~cLine3DFCN() {}

    void setPoints(std::list<T>&);
    void setPoints(std::list<T>*);

    double operator() (const double*);

    TVector3 getDirection(const std::vector<double>& ) const;
    TVector3 getBasepoint(const std::vector<double>& ) const;

  private:
    std::list<T> *points;
};


template <class T>
void cLine3DFCN<T>::setPoints(std::list<T> &p) {
  points = &p;
}

template <class T>
void cLine3DFCN<T>::setPoints(std::list<T> *p) {
  points = p;
}

template<class T>
double cLine3DFCN<T>::operator() (const double *par) {
  // par[0] angle from x axis (theta)
  // par[1] angle from y axis (phi)

  // Unit vector with the line direction and for the corresponding base
  TVector3 n(cos(par[0]), sin(par[0]) * cos(par[1]), sin(par[0]) * sin(par[1]));
  TVector3 xtil(-sin(par[0]), cos(par[0]) * cos(par[1]), cos(par[0]) * sin(par[1]));
  TVector3 ytil(0, -sin(par[1]), cos(par[1]));

  double res = 0.;

  for (auto &p: *points) {
    TVector3 pos(p[0], p[1], p[2]);

    // Project the point on the abstract plane
    double xx = pos * xtil;
    double yy = pos * ytil;

    // Calculate the distance
    double distSq = (xx - par[2])*(xx - par[2]) + (yy - par[3])*(yy - par[3]);

    res += distSq * p.GetEnergy();
  }

  return res;
}

template<class T>
TVector3 cLine3DFCN<T>::getDirection(const std::vector<double>& par) const {
  TVector3 n(cos(par[0]), sin(par[0]) * cos(par[1]), sin(par[0]) * sin(par[1]));
  return n;
}

template<class T>
TVector3 cLine3DFCN<T>::getBasepoint(const std::vector<double>& par) const {
  TVector3 xtil(-sin(par[0]), cos(par[0]) * cos(par[1]), cos(par[0]) * sin(par[1]));
  TVector3 ytil(0, -sin(par[1]), cos(par[1]));

  return xtil*par[2] + ytil*par[3];
}

#endif
