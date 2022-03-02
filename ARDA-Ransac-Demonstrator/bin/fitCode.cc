// i deve essere una lista di punti

// Create  the "chi square" function, and load the points of the line in it
cLine3DFCN<T> fcnFunctor;
fcnFunctor.setPoints(*i);

// Create the fitter object
ROOT::Fit::Fitter fitter;

// Create a functor that wraps the "chi square" function in order for the Fitter
// to like it
ROOT::Math::Functor fcn(fcnFunctor, 4);

// Look for the point that is further from the first in the array. Then use
// the line passing the two as starting parameter for the fit
TVector3 firstp(i->front()[0], i->front()[1], i->front()[2]);
TVector3 maxp;
double curMaxDist = 0.;
for (auto &p: *i) {
    TVector3 currentPoint(p[0], p[1], p[2]);
    double curDist = (currentPoint - firstp).Mag();
    if (curMaxDist < curDist) {
    curMaxDist = curDist;
    maxp = currentPoint;
    }
}

TVector3 n = (firstp - maxp).Unit();
double thstart = TMath::ACos(n[0]);
double phistart = TMath::ATan(n[2]/n[1]);

TVector3 xtil(-sin(thstart), cos(thstart) * cos(phistart), cos(thstart) * sin(phistart));
TVector3 ytil(0, -sin(phistart), cos(phistart));

// Parameter starting point
double pStart[4] = {thstart, phistart, firstp * xtil, firstp * ytil};

// Set up the fit and run it
fitter.SetFCN(fcn, pStart);
fitter.Config().ParamsSettings()[0].SetValue(thstart);
fitter.Config().ParamsSettings()[1].SetValue(phistart);
fitter.Config().ParamsSettings()[2].SetValue(firstp * xtil);
fitter.Config().ParamsSettings()[3].SetValue(firstp * ytil);
fitter.FitFCN();

// Get the fit result
auto &r = fitter.Result();
const std::vector<double>& par = r.Parameters();

// Create the fitted line and store it
cFittedLine<T> fl(fcnFunctor.getDirection(par), fcnFunctor.getBasepoint(par));
fl.setPoints(*i);
fittedLines.push_back(fl);