#ifndef EDGE_DETECTOR_H
#define EDGE_DETECTOR_H

class EdgeDetector {
  private:
    double _kappa; //Threshold (default value is 2);
    int _N;        //Number of abnormal events in sequence before triggering (default is 10)
    int _edgeBin;
    double _edgeBase;
    int _skip;     //Skip the first '_skip' points in the trace.
    int _init;     //Use further '_init' points to initialise the algorithm.

  public:
    EdgeDetector(double kappa = 2., int N = 5);
    ~EdgeDetector();

    //bool FindEdge(double *signal, double *baseline, int length = 512);
    bool FindEdge(short int *signal, short int *baseline, int length = 512);

    void SetKappa(double kappa);
    void SetN(int N);
    void SetSkip(int skip);
    void SetInit(int init);

    double GetKappa();
    int GetN();
    int GetSkip();
    int GetInit();
    int GetEdgeBin();
    double GetEdgeBase();
};

#endif //EDGE_DETECTOR_H
