#include <iostream>
#include <functional>
#include <TMath.h>
#include <TrapezoidAnalyser.h>

using namespace std;

/*
TrapezoidAnalyser::TrapezoidAnalyser(int decayTime, int riseTime, int topWidth, int avgWidth)
: _decayTime(decayTime), _riseTime(riseTime), _topWidth(topWidth), _avgWidth(avgWidth) {}
*/

TrapezoidAnalyser::TrapezoidAnalyser(int decayTime, int riseTime, int topWidth)
: _decayTime(decayTime), _riseTime(riseTime), _topWidth(topWidth) {}

TrapezoidAnalyser::~TrapezoidAnalyser() {}

bool TrapezoidAnalyser::Analyse(short int *signal, short int *baseline, int length)
{
  _signal = 0.;
  shapedPulse.assign(length,0.);

  bool detection = _detector.FindEdge(signal,baseline,length);

  //If we had a detection, estimate the signal strength.
  if(detection){
    //Extract starting point of signal and baseline level.
    int edgeBin = _detector.GetEdgeBin();
    double level = _detector.GetEdgeBase();

    //The signal, corrected for fixed pattern noise and DC level.
    auto nu = [&](int bin) {
      if(bin <= edgeBin || bin >= length) return 0.;
      else return signal[bin] - baseline[bin] - level;
    };

    auto d = [&](int k, int l, int j){
      return nu(j) - nu(j-k) - nu(j-l) + nu(j-k-l);
    };

    /*
    //Trapezoidal pulse of unit amplitude.
    auto f = [&](int bin, int k, int l){
      int i = bin - (edgeBin+1);
      if(0 <= i && i < k){return (i + 1.)/k;}
      else if(k <= i && i < l){return 1.;}
      else if(l <= i && i < l + k){return 1. - (i - (l-1.))/k;}
      else {return 0.;}
    };
    */

    //Ok, we now have edge and baseline, which we use for the trapezoidal filter.
    int M = _decayTime;
    int m = _topWidth;
    int k = _riseTime;
    int l = m + k;
    //int avgStop = edgeBin + l;
    //int avgStart = avgStop - _avgWidth;
    double pp = 0.;
    double s = 0.;

    //Check if signal is too close to the end of the sampling window.
    if(edgeBin+l+k >= length) return false;

    for(int i=edgeBin+1; i<length; i++){
      pp += d(k,l,i);
      s += (pp + M * d(k,l,i))/((double)k*M);
      if(TMath::Abs(s) > TMath::Abs(_signal)) _signal = s; //Simple peak sensing.
      shapedPulse.at(i) = s;
      //if(i < l+k-2 + (edgeBin+1)) _signal += s * f(i,k,l); //Auto-correlation
      //if(i > avgStart){
      //  _signal += s;
        //cout << "s = " << s << ",  i = " << i << ",  s/width = " <<  s/_avgWidth << "  signal = " << _signal << endl;
      //}
      //if(i == avgStop) break;
    }
    //_signal /= ((3.*l-k)/3.); //dividing by square-norm (for auto-correlation method).
    //_signal /= _avgWidth;  //dividing by window width (for averaging method).
  }

  return detection;
}

int TrapezoidAnalyser::GetEdgeBin()
{
  return _detector.GetEdgeBin();
}

double TrapezoidAnalyser::GetEdgeBase()
{
  return _detector.GetEdgeBase();
}

void TrapezoidAnalyser::SetDecayTime(int time)
{
  _decayTime = time;
}

void TrapezoidAnalyser::SetRiseTime(int time)
{
  _riseTime = time;
}

void TrapezoidAnalyser::SetTopWidth(int width)
{
  _topWidth = width;
}

/*
void TrapezoidAnalyser::SetAvgWidth(int width)
{
  _avgWidth = width;
}
*/

int TrapezoidAnalyser::GetDecayTime()
{
  return _decayTime;
}

int TrapezoidAnalyser::GetRiseTime()
{
  return _riseTime;
}

int TrapezoidAnalyser::GetTopWidh()
{
  return _topWidth;
}

/*
int TrapezoidAnalyser::GetAvgWidth()
{
  return _avgWidth;
}
*/

EdgeDetector & TrapezoidAnalyser::GetDetector()
{
  return _detector;
}

vector<double> & TrapezoidAnalyser::GetShapedPulse()
{
  return shapedPulse;
}
