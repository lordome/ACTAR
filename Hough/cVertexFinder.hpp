#include <cmath>
#include <Math/Functor.h>
#include <Fit/Fitter.h>

#include "cVertex.h"

template<class T>
void cVertexFinder<T>::findVertex(cFittedEvent<T>* event) {
  event->getVertex().clear();

  std::list<cFittedLine<T>> maybeTracks;
  
  bestTracks.clear();

  for (auto i = event->getLines().begin(); i != event->getLines().end(); i++) {

    if (!i->isFittable()) continue;

    TVector3 modelB = i->getBasepoint();
    TVector3 modelD = i->getDirection();

    maybeTracks.push_back(*i);

    for (auto j = event->getLines().begin(); j != event->getLines().end(); j++) {
      // Avoid crossing the line with itself
      if (i == j || !j->isFittable()) continue;

      for (auto &h: j->getPoints()) {
		TVector3 v(h[0], h[1], h[2]);  
        if (getError(modelB, modelD, v) < maxDist*maxDist) {
          maybeTracks.push_back(*j);
          break;
        }
      }

      if (maybeTracks.size() > bestTracks.size() && maybeTracks.size() > 1) {
        bestTracks = maybeTracks;
      }
    }
  }

  if (bestTracks.size() < 2) return;

  // Find vertex position
  ROOT::Fit::Fitter fitter;
  ROOT::Math::Functor fcn(*this, 3);

  // Settare parametri iniziali
  double pStart[3] = {parStart[0], parStart[1], parStart[2]};

  double delta = 100.;

  fitter.SetFCN(fcn, pStart);

  fitter.Config().ParamsSettings()[0].SetValue(pStart[0]);
  fitter.Config().ParamsSettings()[0].SetLimits(-delta, 128. + delta);

  fitter.Config().ParamsSettings()[0].SetValue(pStart[1]);
  fitter.Config().ParamsSettings()[0].SetLimits(-delta, 128. + delta);

  fitter.Config().ParamsSettings()[0].SetValue(pStart[2]);
  fitter.Config().ParamsSettings()[0].SetLimits(-delta, maxZ + delta);

  fitter.FitFCN();

  // Get the fit result
  auto &r = fitter.Result();
  const std::vector<double>& par = r.Parameters();

  TVector3 ver(par[0], par[1], par[2]);

  cVertex<T> v(ver);
  v.getTracks() = bestTracks;
  
  event->getVertex().push_back(v);
}

template<class T>
double cVertexFinder<T>::getMaxZ() const {
  return maxZ;
}

template<class T>
void cVertexFinder<T>::setMaxZ(double v) {
  maxZ = v;
}


template<class T>
std::vector<double> cVertexFinder<T>::getParStart() const {
  return parStart;
}

template<class T>
void cVertexFinder<T>::setParStart(std::vector<double> v) {
  parStart = v;
}


template<class T>
double cVertexFinder<T>::getMinZ() const {
  return minZ;
}

template<class T>
void cVertexFinder<T>::setMinZ(double v) {
  minZ = v;
}

template<class T>
void cVertexFinder<T>::setMaxDist(double v) {
  maxDist = v;
}

template<class T>
double cVertexFinder<T>::getMaxDist() const {
  return maxDist;
}

template<class T>
double cVertexFinder<T>::getError(const cFittedLine<T>& model, const T& hit) const {
  return getError(model.getBasepoint(), model.getDirection(), hit);
}

template<class T>
double cVertexFinder<T>::getError(TVector3 base, TVector3 dir, const T& hit) const {
  TVector3 h = {hit[0], hit[1], hit[2]};

  return getError(base, dir, h);
}

template<class T>
double cVertexFinder<T>::getError(TVector3 base, TVector3 dir, TVector3 hit) const {
  TVector3 a = hit;
  a -= base;

  TVector3 H = base + a.Dot(dir) * dir;

  TVector3 d = {hit[0], hit[1], hit[2]};
  d -= H;

  return d.Mag2();
}

template<class T>
double cVertexFinder<T>::operator() (const double* par) {
  double chisq = 0.;

  TVector3 p = {par[0], par[1], par[2]};

  for (auto &i: bestTracks) {
    double err = getError(i.getBasepoint(), i.getDirection(), p);

    chisq += err;
  }

  return chisq;
}
