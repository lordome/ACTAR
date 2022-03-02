#include <TMath.h>
#include <EdgeDetector.h>

using namespace std;

EdgeDetector::EdgeDetector(double kappa, int N)  : _kappa(kappa), _N(N)
{
  _skip = 10; //Skip the first part of the trace completely.
  _init = 10; //Use a few points to find initial estimates of mu, sigma.
}

EdgeDetector::~EdgeDetector() {}

//bool EdgeDetector::FindEdge(double *signal, double *baseline, int length = 512)
bool EdgeDetector::FindEdge(short int *signal, short int *baseline, int length)
{
  _edgeBin = -1;
  _edgeBase = 0.;
  bool detection = false;
  double mu = signal[_skip-1] - baseline[_skip-1];//The average of preceding signals.
  double sigma2 = 0.;    //Average of squared std. deviation.
  //Initialisation...
  for(int i=_skip; i<_skip+_init; i++){
    int k = i + 1;
    double yk = signal[i] - baseline[i];
    mu = (k-1.)/k * mu + 1./k * yk;
    sigma2 = (k-1.)/k * sigma2 + 1./k * TMath::Power(yk-mu,2);
  }
  //Here we start looking for changes in the signal.
  int edge = _skip + _init;
  double level = mu;
  for(int i=_skip+_init; i<length; i++){
    int k = i + 1;
    double yk = signal[i] - baseline[i];
    double gk = (yk - mu)/TMath::Sqrt(sigma2);
    bool normal = TMath::Abs(gk) > _kappa ? false : true;
    if(normal){
      edge = i;
      level = mu;
    }
    if(i - edge > _N){  //We identified a change in the signal!
      detection = true;
      _edgeBin = edge;  //'edge' denotes the last 'normal' bin.
      _edgeBase = level;
      break;
    }
    mu = (k-1.)/k * mu + 1./k * yk;
    sigma2 = (k-1.)/k * sigma2 + 1./k * TMath::Power(yk-mu,2);
  }

  return detection;
}

void EdgeDetector::SetKappa(double kappa)
{
  _kappa = kappa;
}

void EdgeDetector::SetN(int N)
{
  _N = N;
}

void EdgeDetector::SetSkip(int skip)
{
  _skip = skip;
}

void EdgeDetector::SetInit(int init)
{
  _init = init;
}

double EdgeDetector::GetKappa()
{
  return _kappa;
}

int EdgeDetector::GetN()
{
  return _N;
}

int EdgeDetector::GetSkip()
{
  return _skip;
}

int EdgeDetector::GetInit()
{
  return _init;
}

int EdgeDetector::GetEdgeBin()
{
  return _edgeBin;
}

double EdgeDetector::GetEdgeBase()
{
  return _edgeBase;
}
