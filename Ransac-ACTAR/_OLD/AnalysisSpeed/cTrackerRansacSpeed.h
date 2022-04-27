#ifndef cTrackerRansacSpeed_h
#define cTrackerRansacSpeed_h

#include <list>
#include <utility>
#include <Rtypes.h>
#include <TH2.h>
#include <TVector3.h>
#include "cFittedLine.h"
#include <vector>

#ifdef __CLING__
#pragma link C++ class std::list < cPhysicalHit> + ;
#pragma link C++ class cFittedLine < cPhysicalHit> + ;
#pragma link C++ class std::list < cFittedLine < cPhysicalHit>> + ;
#pragma link C++ class std::list < TVector3> + ;
#pragma link C++ class cFittedEvent < cPhysicalHit> + ;
#pragma link C++ class cVertex < cPhysicalHit> + ;
#pragma link C++ class std::list < cVertex < cPhysicalHit>> + ;
#pragma link C++ class cFittedEvent < cPhysicalHit> + ;
#pragma link C++ class cDrawEvents < cPhysicalHit> + ;
#pragma link C++ class cTrackerRansacSpeed < cPhysicalHit> + ;
#endif

template <class T>
class cTrackerRansacSpeed
{
public:
    cTrackerRansacSpeed(){};
    virtual ~cTrackerRansacSpeed(){};

    // Adds a point to the collection
    void addPoint(const T &);

    void setTracksEnergyThreshold(double);
    void setBeamEnergyThreshold(double);
    void setTracksWidth(double);
    void setBeamWidth(double);
    void setLoopsNumber(double);
    void setTrackMinSize(double);
    void setBeamMinSize(double);

    // Calculation of 3D line from two vectors
    std::array<TVector3, 2> GetParamWithSample(std::array<Double_t, 4> sample1, std::array<Double_t, 4> sample2);
    // SQUARE! of the distance between the point and the line
    Double_t GetError_noscale(std::array<TVector3, 2> model, std::array<Double_t, 4> p);
    // SQUARE! of the distance between the point and the line
    Double_t GetError(std::array<TVector3, 2> model, std::array<Double_t, 4>  p, double zScale);
    // function used to test the clusters.
    Double_t ClusterTest(double &sumvalue, double &totalenergy, std::vector<T> &inliers);

    // double getTracksEnergyThreshold() const;
    // double getBeamEnergyThreshold  () const;
    // double getTracksWidth          () const;
    // double getBeamWidth            () const;
    // double getLoopsNumber          () const;
    // double getTrackMinSize         () const;
    // double getBeamMinSize          () const;

    enum class direction
    {
        x = 0,
        y = 1,
        z = 2
    };

    // private:

    double tracksEnergyThreshold;
    double beamEnergyThreshold;
    double tracksWidth;
    double beamWidth;
    double loopsNumber;
    double trackMinSize;
    double beamMinSize;
    double zRescaling = 1.;

    std::list<T> points;
    // std::list<T> unfittedPoints;
    std::list<cFittedLine<T>> lines;
    std::list<cFittedLine<T>> fittedLines;

    std::vector<std::vector<double>> iterSize;
    std::vector<std::vector<double>> iterEnergy;

    // Function for running Ransac
    void Ransac(double &minEnergy, double &minSize, double &widthTrack, bool&  beamTracks, bool &oneSidedTracks, bool &isFittable);

    // Load points in the maximum as a line, returns true if a line is found
    // bool loadPointsInLine(direction, direction);

    // Fits each line contained in lines
    void fitLines();

    // Function for the user to start the tracking
    void track(direction, direction);
};

#include "cTrackerRansacSpeed.hpp"
#endif
