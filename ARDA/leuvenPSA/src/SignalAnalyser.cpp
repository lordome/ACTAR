#include <vector>
#include <SignalAnalyser.h>

using namespace std;

bool SignalAnalyser::Analyse(short int *signal, int length)
{
  baseLine.assign(length,0);
  return Analyse(signal,baseLine.data(),length);
}

/*
bool SignalAnalyser::Analyse(short int *signal, short int *baseline, int length)
{
  vector<double> d_signal (length);
  vector<double> d_baseline (length);
  for(int i=0; i<length; i++){
    d_signal.at(i) = (double) (signal[i]);
    d_baseline.at(i) = (double) (baseline[i]);
  }
  return Analyse(d_signal.data(),d_baseline.data(),length);
}
*/
