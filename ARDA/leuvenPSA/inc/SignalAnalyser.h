#ifndef SIGNAL_ANALYSER_H
#define SIGNAL_ANALYSER_H
#include <vector>

class SignalAnalyser {
  protected:
    double _signal;
    std::vector<short int> baseLine;

  public:
    SignalAnalyser() {};
    virtual ~SignalAnalyser() {};

    //virtual bool Analyse(double *signal, double *baseline, int length = 512) = 0;

    virtual bool Analyse(short int *signal, short int *baseline, int length = 512) = 0;

    virtual bool Analyse(short int *signal, int length = 512);
    inline double GetSignal() {return _signal;}
};

#endif //SIGNAL_ANALYSER_H
