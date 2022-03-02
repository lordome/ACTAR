#include <TMath.h>
#include <TVector3.h>
#include "cLine3DFCN.h"
#include <Math/Functor.h>
#include <Fit/Fitter.h>

#include "TCanvas.h"

#include <cmath>

template <class T>
cTrackerFine<T>::cTrackerFine()
{
  projectiveLineSpace = nullptr;

  nAngularSteps = 0;
  nDistanceSteps = 0;
  maxDistance = 0;
  minPoints = 0;
}

template <class T>
cTrackerFine<T>::~cTrackerFine()
{
  clearSpace();
}

template <class T>
void cTrackerFine<T>::setAngularSteps(UInt_t v)
{
  nAngularSteps = v;
}

template <class T>
void cTrackerFine<T>::setDistanceSteps(UInt_t v)
{
  nDistanceSteps = v;
}

template <class T>
void cTrackerFine<T>::setMaxDistance(Float_t v)
{
  maxDistance = v;
}

template <class T>
void cTrackerFine<T>::setMinimumEnergy(Float_t v)
{
  minEnergy = v;
}

template <class T>
void cTrackerFine<T>::setMinPoints(UInt_t v)
{
  minPoints = v;
}

template <class T>
void cTrackerFine<T>::setPointDistance(Float_t v)
{
  pointDistance = v;
}

template <class T>
void cTrackerFine<T>::makeSpace()
{
  if (nAngularSteps == 0 || nDistanceSteps == 0)
    return;

  if (projectiveLineSpace != nullptr)
    clearSpace();

  projectiveLineSpace = new Float_t *[nAngularSteps];
  for (UInt_t i = 0; i < nAngularSteps; i++)
  {
    projectiveLineSpace[i] = new Float_t[nDistanceSteps];
  }
}

template <class T>
void cTrackerFine<T>::clearSpace()
{
  if (projectiveLineSpace == nullptr)
    return;

  for (UInt_t i = 0; i < nAngularSteps; i++)
  {
    delete[] projectiveLineSpace[i];
  }

  delete[] projectiveLineSpace;
  projectiveLineSpace = nullptr;
}

template <class T>
void cTrackerFine<T>::zeroSpace()
{
  for (UInt_t i = 0; i < nAngularSteps; i++)
  {
    for (UInt_t j = 0; j < nDistanceSteps; j++)
    {
      projectiveLineSpace[i][j] = 0.;
    }
  }
}

template <class T>
void cTrackerFine<T>::setLinesFittable(bool v)
{
  isLineFittable = v;
};

template <class T>
void cTrackerFine<T>::setMultFactor(std::vector<Float_t> v)
{
  multFactor = v;
};

template <class T>
std::pair<UInt_t, UInt_t> cTrackerFine<T>::findMaximumPoint() const
{
  Float_t val = 0.;
  UInt_t mAng = 0;
  UInt_t mDis = 0;

  for (UInt_t i = 0; i < nAngularSteps; i++)
  {
    for (UInt_t j = 0; j < nDistanceSteps; j++)
    {
      if (projectiveLineSpace[i][j] > val)
      {
        val = projectiveLineSpace[i][j];
        mAng = i;
        mDis = j;
      }
    }
  }

  return std::make_pair(mAng, mDis);
}

template <class T>
std::pair<UInt_t, UInt_t> cTrackerFine<T>::findMaximumPoint(bool isLineFittable) const
{
  Float_t val = 0.;
  UInt_t mAng = 0;
  UInt_t mDis = 0;

  if (!isLineFittable)
  {
    for (UInt_t i = 0; getTheta(i) < 0.01; i++)
    {
      for (UInt_t j = 0; j < nDistanceSteps; j++)
      {
        if (projectiveLineSpace[i][j] > val)
        {
          val = projectiveLineSpace[i][j];
          mAng = i;
          mDis = j;
        }
      }
    }
  }
  else
  {
    for (UInt_t i = 0; i < nAngularSteps; i++)
    {
      for (UInt_t j = 0; j < nDistanceSteps; j++)
      {
        if (projectiveLineSpace[i][j] > val)
        {
          val = projectiveLineSpace[i][j];
          mAng = i;
          mDis = j;
        }
      }
    }
  }

  return std::make_pair(mAng, mDis);
}

template <class T>
std::vector<std::pair<UInt_t, UInt_t>> cTrackerFine<T>::findMaxima() const
{
  Float_t val = 0.;
  UInt_t mAng = 0;
  UInt_t mDis = 0;

  UInt_t secAng = 0;
  UInt_t secDis = 0;

  UInt_t thiAng = 0;
  UInt_t thiDis = 0;

  for (UInt_t i = 0; i < nAngularSteps; i++)
  {
    for (UInt_t j = 0; j < nDistanceSteps; j++)
    {
      if (projectiveLineSpace[i][j] > val)
      {
        val = projectiveLineSpace[i][j];
        thiAng = secAng;
        thiDis = secDis;
        secAng = mAng;
        secDis = mDis;
        mAng = i;
        mDis = j;
      }
    }
  }
  std::vector<std::pair<UInt_t, UInt_t>> vecPairs = {
      std::make_pair(mAng, mDis),
      std::make_pair(secAng, secDis),
      std::make_pair(thiAng, thiDis)};

  cout << "First max " << mAng << "  " << mDis << endl;
  cout << "Secon max " << secAng << "  " << secDis << endl;
  cout << "Third max " << thiAng << "  " << thiDis << endl;

  return vecPairs;
};

template <class T>
void cTrackerFine<T>::addPoint(const T &v)
{
  points.push_back(v);
}

template <class T>
TH2 *cTrackerFine<T>::convertSpaceToHist() const
{
  TH2 *h = new TH2F("plsHist", "", nAngularSteps, 0., TMath::Pi(), nDistanceSteps, -maxDistance, maxDistance);

  for (UInt_t i = 0; i < nAngularSteps; i++)
  {
    for (UInt_t j = 0; j < nDistanceSteps; j++)
    {
      h->SetBinContent(i, j, projectiveLineSpace[i][j]);
    }
  }

  h->GetXaxis()->SetTitle("Angle");
  h->GetYaxis()->SetTitle("Distance");

  return h;
}

template <class T>
Float_t cTrackerFine<T>::getTheta(UInt_t i) const
{
  return TMath::Pi() / nAngularSteps * (0.5 + Float_t(i));
}

template <class T>
Float_t cTrackerFine<T>::getDistance(UInt_t i) const
{
  return Float_t(i) * (2. * maxDistance / nDistanceSteps) - maxDistance;
}

template <class T>
void cTrackerFine<T>::runInitialHoughTransform(direction a, direction b)
{
  // Check if the space is ready, if not make it
  if (projectiveLineSpace == nullptr)
    makeSpace();

  // Fill the space
  for (UInt_t i = 0; i < nAngularSteps; i++)
  {
    Float_t theta = getTheta(i);

    Float_t cos = TMath::Cos(theta);
    Float_t sin = TMath::Sin(theta);

    // Make every point vote
    for (auto &p : points)
    {
      Float_t xtil = -p[UInt_t(a)] * multFactor[UInt_t(a)] * sin + p[UInt_t(b)] * multFactor[UInt_t(b)] * cos;

      if (TMath::Abs(xtil) < maxDistance)
      {
        // rint rounds a float to the nearest integer value. This is done because
        // simple typecasting would truncate the number
        UInt_t index = rint((xtil + maxDistance) / (2. * maxDistance / nDistanceSteps));

        // Find the lines within pointDistance of the point in order to vote in
        // multiple bins. In this way the histogram filling and the point selection
        // are placed in the same way
        Int_t pointDistanceInBins = rint(pointDistance / (2. * maxDistance / nDistanceSteps));

        for (Int_t j = -pointDistanceInBins; j <= pointDistanceInBins; j++)
        {
          if (j + index >= nDistanceSteps || j + index < 0)
            continue;

          projectiveLineSpace[i][j + index] += p.GetEnergy();
        }
      }
    }
  }
}

template <class T>
bool cTrackerFine<T>::loadPointsInMaximum(direction a, direction b)
{
  auto max = findMaximumPoint(isLineFittable);
  bestTheta = getTheta(max.first);
  bestDist = getDistance(max.second);

  // Check if the wanted point has sufficient energy to be considered a line
  if (projectiveLineSpace[max.first][max.second] < minEnergy)
    return false;

  Float_t cos = TMath::Cos(bestTheta);
  Float_t sin = TMath::Sin(bestTheta);

  for (typename pointList::iterator i = points.begin(); i != points.end(); i++)
  {
    T p = *i;

    Float_t xtil = -p[UInt_t(a)] * multFactor[UInt_t(a)] * sin + p[UInt_t(b)] * multFactor[UInt_t(b)] * cos;

    if (TMath::Abs(bestDist - xtil) < pointDistance)
    {
      // Remove the point from the point list
      i = points.erase(i);
      i--;

      // Add the point to the accumulator, for it to be used later
      accumulator.push_back(p);
    }
  }

  return true;
}

template <class T>
std::pair<TVector3, TVector3> cTrackerFine<T>::getNewHoughAxis(direction a, direction b)
{
  // The unit vector of the previous Hough transform will be the x axis of this one
  TVector3 av;
  TVector3 bv;

  av[UInt_t(a)] = 1.;
  bv[UInt_t(b)] = 1.;

  // Unit vector along the line on the x-y plane
  TVector3 x = av * TMath::Cos(bestTheta) + bv * TMath::Sin(bestTheta);

  // Unit vector in the direction of x, y or z not already taken
  TVector3 y(1., 1., 1.);
  y = y - av - bv;

  return std::make_pair(x, y);
}

template <class T>
void cTrackerFine<T>::runSecondHoughTransform(direction a, direction b)
{
  auto xy = getNewHoughAxis(a, b);
  TVector3 x = xy.first;
  TVector3 y = xy.second;

  // Fill the space
  for (UInt_t i = 0; i < nAngularSteps; i++)
  {
    Float_t theta = getTheta(i);

    Float_t cos = TMath::Cos(theta);
    Float_t sin = TMath::Sin(theta);

    // Make every point vote
    for (auto &p : accumulator)
    {
      // Create a vector corresponding to the point
      TVector3 pv(p[0] * multFactor[0], p[1] * multFactor[1], p[2] * multFactor[2]);

      // Project the point along the new x axis
      Float_t xproj = pv * x;

      // Project the point along the new y axis
      Float_t yproj = pv * y;

      Float_t xtil = -xproj * sin + yproj * cos;

      if (TMath::Abs(xtil) < maxDistance)
      {
        UInt_t index = rint((xtil + maxDistance) / (2. * maxDistance / nDistanceSteps));

        if (index >= nDistanceSteps)
          continue;

        projectiveLineSpace[i][index] += p.GetEnergy();
      }
    }
  }
}

template <class T>
bool cTrackerFine<T>::loadPointsInLine(direction a, direction b)
{
  auto xy = getNewHoughAxis(a, b);
  TVector3 x = xy.first;
  TVector3 y = xy.second;

  pointList lineCand;

  auto max = findMaximumPoint();
  Float_t mTheta = getTheta(max.first);
  Float_t mDist = getDistance(max.second);

  Float_t cos = TMath::Cos(mTheta);
  Float_t sin = TMath::Sin(mTheta);

  Float_t lineEnergy = 0.;
  UInt_t numberOfPoints = 0;

  for (typename pointList::iterator i = accumulator.begin(); i != accumulator.end(); i++)
  {
    T p = *i;
    // Create a vector corresponding to the point
    TVector3 pv(p[0] * multFactor[0], p[1] * multFactor[1], p[2] * multFactor[2]);

    // Project the point along the new x axis
    Float_t xproj = pv * x;

    // Project the point along the new y axis
    Float_t yproj = pv * y;

    Float_t xtil = -xproj * sin + yproj * cos;

    if (TMath::Abs(mDist - xtil) < pointDistance)
    {
      i = accumulator.erase(i);
      i--;

      // Add the point to the accumulator, for it to be used later
      lineCand.push_back(p);

      lineEnergy += p.GetEnergy();
      numberOfPoints++;
    }
  }
  // Average trick - to be used for traces directed on one side only.

  if (oneSidedTracks)
  {
    Float_t averageY = 0;

    for (auto &p : lineCand)
    {
      averageY += p.getY();
    }
    averageY /= lineCand.size();

    if (averageY > 68.)
    {
      for (auto it_hits = lineCand.begin(); it_hits != lineCand.end(); it_hits++)
      {
        T p = *it_hits;
        if (p.getY() < 64.)
        {
          it_hits = lineCand.erase(it_hits);
          it_hits--;
        }
      }
    }
    if (averageY < 61.)
    {
      for (auto it_hits = lineCand.begin(); it_hits != lineCand.end(); it_hits++)
      {
        T p = *it_hits;
        if (p.getY() > 64)
        {
          it_hits = lineCand.erase(it_hits);
          it_hits--;
        }
      }
    }
  }

  if (lineEnergy > minEnergy && minPoints < numberOfPoints)
  {
    // create a cFittedLine and push it to the list of lines
    cFittedLine<T> lineToPush;
    lineToPush.setPoints(lineCand);
    lineToPush.setFittable(isLineFittable);
    fittedLines.push_back(lineToPush);

    return true;
  }
  else
  {
    // Put all the stuff back into the accumulator
    accumulator.insert(accumulator.end(), lineCand.begin(), lineCand.end());
    return false;
  }
}

template <class T>
void cTrackerFine<T>::fitLines()
{
  for (auto &i : fittedLines)
  {

    if (i.isFittable())
    {
      // Create  the "chi square" function, and load the points of the line in it
      cLine3DFCN<T> fcnFunctor;
      fcnFunctor.setPoints(i.getPoints());

      // Create the fitter object
      ROOT::Fit::Fitter fitter;

      // Create a functor that wraps the "chi square" function in order for the Fitter
      // to like it
      ROOT::Math::Functor fcn(fcnFunctor, 4);

      // Look for the point that is further from the first in the array. Then use
      // the line passing the two as starting parameter for the fit
      auto pt = i.getPoints().front();
      TVector3 firstp(pt[0], pt[1], pt[2]);
      TVector3 maxp;
      double curMaxDist = 0.;
      for (auto &p : i.getPoints())
      {
        TVector3 currentPoint(p[0], p[1], p[2]);
        double curDist = (currentPoint - firstp).Mag();
        if (curMaxDist < curDist)
        {
          curMaxDist = curDist;
          maxp = currentPoint;
        }
      }

      TVector3 n = (firstp - maxp).Unit();
      double thstart = TMath::ACos(n[0]);
      double phistart = TMath::ATan(n[2] / n[1]);

      TVector3 xtil(-sin(thstart), cos(thstart) * cos(phistart), cos(thstart) * sin(phistart));
      TVector3 ytil(0, -sin(phistart), cos(phistart));

      // Parameter starting point
      double pStart[4] = {thstart, phistart, firstp * xtil, firstp * ytil};

      // Set up the fit and run it
      fitter.SetFCN(fcn, pStart);
      fitter.Config().ParamsSettings()[0].SetValue(thstart + 1e-3);
      fitter.Config().ParamsSettings()[1].SetValue(phistart + 1e-3);
      fitter.Config().ParamsSettings()[2].SetValue(firstp * xtil + 1e-3);
      fitter.Config().ParamsSettings()[3].SetValue(firstp * ytil + 1e-3);
      fitter.FitFCN();

      // Get the fit result
      auto &r = fitter.Result();
      const std::vector<double> &par = r.Parameters();

      // Create the fitted line and store it

      i.setDirection(fcnFunctor.getDirection(par));
      i.setBasepoint(fcnFunctor.getBasepoint(par));
    }
  }
}

template <class T>
void cTrackerFine<T>::track(direction a, direction b)
{
  if (projectiveLineSpace == nullptr)
    makeSpace();

  // Initialize the space
  zeroSpace();

  // Run the first Hough transform until lines are found
  bool mm;

  do
  {
    runInitialHoughTransform(a, b);
    mm = loadPointsInMaximum(a, b);

    //drawHoughSpace();

    zeroSpace();

    // Run the second Hough transform and save points to a line
    runSecondHoughTransform(a, b);
    mm = mm && loadPointsInLine(a, b);

    // Put the points left in the accumulator in the point list and then clear the accumulator
    points.insert(points.end(), accumulator.begin(), accumulator.end());
    accumulator.clear();

    zeroSpace();
  } while (mm);
}



template <class T>
void cTrackerFine<T>::drawHoughSpace()
{
  TCanvas *c = new TCanvas();
  c->cd();
  TH2 *h = convertSpaceToHist();
  h->Draw("colz");
  c->WaitPrimitive();

  delete h;
  c->Close();
  delete c;
};