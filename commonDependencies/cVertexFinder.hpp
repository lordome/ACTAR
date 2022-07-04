

#include <cmath>
#include <Math/Functor.h>
#include <Fit/Fitter.h>
#include <algorithm>

#include "cVertex.h"

template <class T>
void cVertexFinder<T>::findVertex(cFittedEvent<T> *event)
{
  event->getVertex().clear();
  std::list<cFittedLine<T>> maybeTracks;
  bestTracks.clear();

  std::list<cFittedLine<T>> linesList = event->getLines();

  int it = 0;
  for (auto i = linesList.begin(); i != linesList.end(); i++)
  {
    it++;
    if (!i->isFittable())
      continue;

    TVector3 modelB = i->getBasepoint();
    TVector3 modelD = i->getDirection();

    // maybeTracks.clear();
    // maybeTracks.push_back(*i);

    for (auto j = event->getLines().begin(); j != event->getLines().end(); j++)
    {
      // Avoid crossing the line with itself
      if (i->getBasepoint() == j->getBasepoint() || !j->isFittable())
        continue;

      bool pBack = false;

      for (auto &h : j->getPoints())
      {
        TVector3 v(h[0], h[1], h[2]);
        if (getError(modelB, modelD, v) < (maxDist * maxDist))
        {
          if (getMinDistance(i->getPoints(), j->getPoints(), 20))
          {
            pBack = true;
            maybeTracks.push_back(*i);
            i = linesList.erase(i);
            i--;
            break;
          }
        }
      }
      if (pBack)
        break;
    }

    if (maybeTracks.size() > bestTracks.size() && maybeTracks.size() > 1)
    {
      bestTracks = maybeTracks;
    }
  }

  if (bestTracks.size() < 2)
    return;

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
  const std::vector<double> &par = r.Parameters();

  TVector3 ver(par[0], par[1], par[2]);

  cVertex<T> v(ver);
  v.getTracks() = bestTracks;

  event->getVertex().push_back(v);

  cout << "number of tracks_" << bestTracks.size() << endl;
  cout << "verPosition: " << ver[0] << "  " << ver[1] << "   " << ver[2] << endl;
}

template <class T>
double cVertexFinder<T>::getMaxZ() const
{
  return maxZ;
}

template <class T>
void cVertexFinder<T>::setMaxZ(double v)
{
  maxZ = v;
}

template <class T>
std::vector<double> cVertexFinder<T>::getParStart() const
{
  return parStart;
}

template <class T>
void cVertexFinder<T>::setParStart(std::vector<double> v)
{
  parStart = v;
}

template <class T>
double cVertexFinder<T>::getMinZ() const
{
  return minZ;
}

template <class T>
void cVertexFinder<T>::setMinZ(double v)
{
  minZ = v;
}

template <class T>
void cVertexFinder<T>::setMaxDist(double v)
{
  maxDist = v;
}

template <class T>
double cVertexFinder<T>::getMaxDist() const
{
  return maxDist;
}

template <class T>
double cVertexFinder<T>::getError(const cFittedLine<T> &model, const T &hit) const
{
  return getError(model.getBasepoint(), model.getDirection(), hit);
}

template <class T>
double cVertexFinder<T>::getError(TVector3 base, TVector3 dir, const T &hit) const
{
  TVector3 h = {hit[0], hit[1], hit[2]};

  return getError(base, dir, h);
}

// template <class T>
// double cVertexFinder<T>::getError(TVector3 base, TVector3 dir, TVector3 hit) const
// {
//   TVector3 a = hit;
//   a -= base;

//   TVector3 H = base + a.Dot(dir) * dir;

//   TVector3 d = {hit[0], hit[1], hit[2]};
//   d -= H;

//   return d.Mag2();
// }

template <class T>
double cVertexFinder<T>::getError(TVector3 base, TVector3 dir, TVector3 x0) const
{
  TVector3 x1 = base;
  TVector3 x2 = base + dir;

  double num = ((x2 - x1).Cross(x1 - x0)).Mag2();
  double den = (x2 - x1).Mag2();

  return num / den;
}

template <class T>
double cVertexFinder<T>::operator()(const double *par)
{
  double chisq = 0.;

  TVector3 p = {par[0], par[1], par[2]};

  for (auto &i : bestTracks)
  {
    double err = getError(i.getBasepoint(), i.getDirection(), p);

    chisq += err;
  }

  return chisq;
}

template <class T>
bool cVertexFinder<T>::getMinDistance(std::list<T> l1, std::list<T> l2, const double target)
{

  double minDist = DBL_MAX;
  for (auto &it_l1 : l1)
  {
    for (auto &it_l2 : l2)
    {
      double actDist = sqrt(pow((it_l1[0] - it_l2[0]), 2) + pow((it_l1[1] - it_l2[1]), 2) + pow((it_l1[2] - it_l2[2]), 2));
      if (actDist < minDist)
      {
        // cout << "points: " << it_l1[0] << "  " << it_l1[1] << "  " << it_l1[2] << "  " << it_l2[0] << "  " << it_l2[1] << "  " << it_l2[2] << "  " << endl;
        minDist = actDist;
      }
    }
    if (minDist < target)
    {
      return true;
    }
  }
  return false;
}
