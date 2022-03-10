#include <vector>
#include <array>
#include <cmath>

#include "TMath.h"
#include "TVector3.h"
#include "TRandom3.h"
#include <Math/Functor.h>
#include <Fit/Fitter.h>

#include "cTrackerRansac.h"
#include "cLine3DFCN.h"

TVector3 zero(0, 0, 0);

typedef std::array<Int_t, 2> arrayI2;
typedef std::array<Double_t, 3> arrayD3;
typedef std::array<Double_t, 4> arrayD4;
typedef std::array<TVector3, 2> arrayV2;

// template <class T>
// cTrackerRansac<T>::cTrackerRansac(){};

// template <class T>
// cTrackerRansac<T>::~cTrackerRansac(){};

template <class T>
void cTrackerRansac<T>::setTracksEnergyThreshold(double v)
{
  tracksEnergyThreshold = v;
};
template <class T>
void cTrackerRansac<T>::setBeamEnergyThreshold(double v)
{
  beamEnergyThreshold = v;
};
template <class T>
void cTrackerRansac<T>::setTracksWidth(double v)
{
  tracksWidth = v;
};
template <class T>
void cTrackerRansac<T>::setBeamWidth(double v) { beamWidth = v; };
template <class T>
void cTrackerRansac<T>::setLoopsNumber(double v) { loopsNumber = v; };
template <class T>
void cTrackerRansac<T>::setTrackMinSize(double v) { trackMinSize = v; };
template <class T>
void cTrackerRansac<T>::setBeamMinSize(double v) { beamMinSize = v; };

template <class T>
void cTrackerRansac<T>::addPoint(const T &v)
{
  points.push_back(v);
}
// Calculation of 3D line from two vectors
template <class T>
std::array<TVector3, 2> cTrackerRansac<T>::GetParamWithSample(std::array<Double_t, 4> sample1, std::array<Double_t, 4> sample2)
{
  TVector3 p1(sample1[0], sample1[1], sample1[2]);
  TVector3 p2(sample2[0], sample2[1], sample2[2]);
  TVector3 u = p2 - p1;
  std::array<TVector3, 2> r = {p1, u.Unit()};
  return r;
};

template <class T>
void cTrackerRansac<T>::Ransac(double &minEnergy, double &minSize, double &widthTrack, bool &beamTracks, bool &oneSidedTracks, bool &isFittable)
{
  // points index into the hits vector
  int rndpoint1 = 0;
  int rndpoint2 = 0;

  int iterations = 0;

  std::array<Double_t, 4> sample1 = {0, 0, 0, 0};
  std::array<Double_t, 4> sample2 = {0, 0, 0, 0};
  std::array<TVector3, 2> maybeModel = {zero, zero};
  std::array<TVector3, 2> bestModel = {zero, zero};
  std::vector<T> alsoInliers; // vector to be filled with the possible indexes.
  std::vector<T> bestInliers; // vector to be filled with the best indexes.

  Double_t bestErr = DBL_MAX;
  Double_t thisErr = 0.;
  Double_t bestEnergy = 0.;

  cFittedLine<T> bestTrack;

  TRandom3 *rnd = new TRandom3;

  std::vector<T> hits = std::vector<T>(points.begin(), points.end());

  while (1)
  { // There are particular conditions, the points chosen randomly must have same y & z, the energy threshold is very high, the number of pads required has to be high.

    iterations = 0;
    bestErr = DBL_MAX;
    bestTrack.getPoints().clear();
    bestInliers.clear();

    while (iterations < loopsNumber)
    {

      rndpoint1 = rnd->Uniform(0, hits.size());
      rndpoint2 = rnd->Uniform(0, hits.size());

      while (!hits[rndpoint1].isTrackable())      //|| unfittedPoints[rndpoint1] == -1)
      {                                           // isTrackable lets us select some hits that are not useful for tracking (ancillaryHit, noisy pad.)
        rndpoint1 = rnd->Uniform(0, hits.size()); // isTrackable is selected into precalibrator.cc
      };
      while (!hits[rndpoint2].isTrackable()) //|| unfittedPoints[rndpoint2] == -1)
      {
        rndpoint2 = rnd->Uniform(0, hits.size());
      };

      if (beamTracks)
      {
        if (abs(hits[rndpoint1].getZ() - hits[rndpoint2].getZ()) != 0 || abs(hits[rndpoint1].getY() - hits[rndpoint2].getY()) > 0.5)
        {
          iterations++;
          continue;
        }; // testing if y1==y2 && z1==z2
      }

      maybeModel[0].SetXYZ(0, 0, 0);
      maybeModel[1].SetXYZ(0, 0, 0);
      alsoInliers.clear();

      sample1 = {hits[rndpoint1].getX(), hits[rndpoint1].getY(), hits[rndpoint1].getZ(), hits[rndpoint1].getEnergy()};
      sample2 = {hits[rndpoint2].getX(), hits[rndpoint2].getY(), hits[rndpoint2].getZ(), hits[rndpoint2].getEnergy()};

      maybeModel = GetParamWithSample(sample1, sample2); // building the 3D line between the random points.

      double getErrorMean = 0;
      double sumValue = 0;
      double totalEnergy = 0;

      // For every point in data, if point fits maybeModel with an error(squared) smaller than width,
      // add its index to alsoInliers, compute totalEnergy and compute sumValue (used to test the inliers.)

      for (unsigned int i = 0; i < hits.size(); i++)
      {

        std::array<double_t, 4> samplex = {hits[i].getX(), hits[i].getY(), hits[i].getZ(), hits[i].getEnergy()};

        if (GetError(maybeModel, samplex, zRescaling) < pow(widthTrack, 2) && hits[i].isTrackable())
        {
          alsoInliers.push_back(hits[i]);
          sumValue += GetError(maybeModel, samplex, zRescaling) * hits[i].getEnergy();
          totalEnergy += hits[i].getEnergy();
        }
      }

      getErrorMean = ClusterTest(sumValue, totalEnergy, alsoInliers); // function that evaluates the cluster(it can be changed)

      if (alsoInliers.size() > minSize && getErrorMean < bestErr && totalEnergy > minEnergy)
      { // comparing the cluster to the previous best one.
        // if the current cluster is better than the previous best, it simply becomes the new best.
        bestModel = maybeModel;
        bestErr = getErrorMean;
        bestInliers = alsoInliers;
        bestEnergy = totalEnergy;
      }
      ++iterations;
    }

    double minX = 1000;
    double maxX = 0;
    double minZ = 1000;
    double maxZ = 0;
    if (1)
    {
      for (auto &it : bestInliers)
      {
        auto xPosition = it.getX();
        if (xPosition < minX)
          minX = xPosition;

        if (xPosition > maxX)
          maxX = xPosition;

        auto zPosition = it.getZ();
        if (zPosition < minZ)
          minZ = zPosition;

        if (zPosition > maxZ)
          maxZ = zPosition;
      }
      if (beamTracks && (minX > 10 || maxX < 110))
      {
        break;
      }
    }

    if (bestInliers.empty())
    {
      break;
    }

    if (oneSidedTracks)
    {
      double averageY = 0;
      for (unsigned int i = 0; i < bestInliers.size(); i++)
      {
        averageY += bestInliers[i].getY();
      }

      averageY /= bestInliers.size();

      std::vector<T> temporary;

      if (averageY > 68)
      {
        for (unsigned int i = 0; i < bestInliers.size(); i++)
        {
          if (bestInliers[i].getY() > 64)
            temporary.push_back(bestInliers[i]);
        }
        bestInliers = temporary;
      }

      if (averageY < 61)
      {
        for (unsigned int i = 0; i < bestInliers.size(); i++)
        {
          if (bestInliers[i].getY() < 64)
            temporary.push_back(bestInliers[i]);
        }
        bestInliers = temporary;
      }

      double tempEnergy = 0;
      for (auto &i : bestInliers)
      {
        tempEnergy += i.getEnergy();
      }
      bestEnergy = tempEnergy;
    }

    if (bestInliers.size() < minSize || bestEnergy < minEnergy)
    {
      break;
    }

    // Setting all the used points to -1, so they won't be used anymore.
    for (auto it_hits = hits.begin(); it_hits != hits.end(); it_hits++)
    {

      T p = *it_hits;
      std::array<double_t, 4> samplex = {p.getX(), p.getY(), p.getZ(), p.getEnergy()};

      if (GetError(bestModel, samplex, zRescaling) < pow(widthTrack, 2) && p.isTrackable())
      {
        // Remove the point from the point list
        it_hits = hits.erase(it_hits);
        it_hits--;
      }
    }

    std::list<T> list_i;
    for (auto &it_best : bestInliers)
    {
      list_i.push_back(it_best);
    }
    cFittedLine<T> bestTrack;
    bestTrack.getPoints().insert(bestTrack.getPoints().begin(), list_i.begin(), list_i.end()); // setting bestTrack (conversion from vector to list)
    bestTrack.setFittable(isFittable);


    if ((minX < 5 && maxX > 120) || (minZ < 30 && maxZ > 450))
      bestTrack.setFittable(false);
    else
      bestTrack.setFittable(isFittable);
    // saving bestTrack into tracker, which is going to be saved into the output tree.
    lines.push_back(bestTrack);

  } // end while (1){

  points = std::list<T>(hits.begin(), hits.end());

  return;
};

template <class T>
void cTrackerRansac<T>::fitLines()
{
  trackLines(lines);
}

template <class T>
Double_t cTrackerRansac<T>::GetError(std::array<TVector3, 2> model, std::array<Double_t, 4> p, double zScale)
{
  TVector3 a = {p[0] - model[0][0], p[1] - model[0][1], p[2] - model[0][2]};
  TVector3 H = model[0] + a.Dot(model[1]) * model[1];
  TVector3 d = {p[0] - H[0], p[1] - H[1], (p[2] - H[2]) / zScale}; // it was necessary to put this factor 10 due to measurement units (as I have set the precalibrator).

  return d.Mag2();
};

template <class T>
Double_t cTrackerRansac<T>::GetError_noscale(std::array<TVector3, 2> model, std::array<Double_t, 4> p)
{
  TVector3 a = {p[0] - model[0][0], p[1] - model[0][1], p[2] - model[0][2]};
  TVector3 H = model[0] + a.Dot(model[1]) * model[1];
  TVector3 d = {p[0] - H[0], p[1] - H[1], (p[2] - H[2])}; // it was necessary to put this factor 10 due to measurement units (as I have set the precalibrator).

  return d.Mag2();
};

template <class T>
Double_t cTrackerRansac<T>::ClusterTest(double &sumvalue, double &totalenergy, vector<T> &inliers)
{
  double test = sumvalue / (totalenergy * inliers.size());
  return test;
}
