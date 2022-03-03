#ifndef cTrackLines_h
#define cTrackLines_h

#include <TMath.h>
#include <TVector3.h>
#include <list>
#include "cFittedLine.h"
#include "cPhysicalEvent.h"
#include "cLine3DFCN.h"
#include <Math/Functor.h>
#include <Fit/Fitter.h>

#include <cmath>

void trackLines(std::list<cFittedLine<cPhysicalHit>>& fittedLines)
{

 for (auto &i : fittedLines)
  {

    if (i.isFittable())
    {
      // Create  the "chi square" function, and load the points of the line in it
      cLine3DFCN<cPhysicalHit> fcnFunctor;
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

      double phistart = 0.;
      if (n[1] == 0.)
      {
        phistart = 0.;
      }
      else
      {
        phistart = TMath::ATan(n[2] / n[1]);
      }

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

    return;
}

#endif