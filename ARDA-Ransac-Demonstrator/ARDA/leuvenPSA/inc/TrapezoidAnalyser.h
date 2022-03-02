#ifndef TRAPEZOID_ANALYSER_H
#define TRAPEZOID_ANALYSER_H
#include <vector>
#include "SignalAnalyser.h"
#include "EdgeDetector.h"

class TrapezoidAnalyser : public SignalAnalyser {
  protected:
    EdgeDetector _detector;
    int _decayTime;
    int _riseTime;
    int _topWidth;
    std::vector<double> shapedPulse;
    std::vector<short int> baseLine;

  public:

    /**
    * This class analyses the pulse height by measuring the maximum height of
    * the trapezoidally shaped pulse.
    */
    TrapezoidAnalyser(int decayTime, int riseTime, int topWidth);
    ~TrapezoidAnalyser();

    //bool Analyse(double *signal, double *baseline, int length = 512);
    using SignalAnalyser::Analyse;
    virtual bool Analyse(short int *signal, short int *baseline, int length = 512);

    void SetDecayTime(int time);
    void SetRiseTime(int time);
    void SetTopWidth(int width);
    //void SetAvgWidth(int width);

    int GetDecayTime();
    int GetRiseTime();
    int GetTopWidh();
    //int GetAvgWidth();
    int GetEdgeBin();
    double GetEdgeBase();
    EdgeDetector & GetDetector();
    std::vector<double> & GetShapedPulse();
};

#endif //TRAPEZOID_ANALYSER_H
