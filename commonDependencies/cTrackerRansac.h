#ifndef cTrackerRansac_h
#define cTrackerRansac_h

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
#pragma link C++ class cTrackerRansac < cPhysicalHit> + ;
#endif

template <class T>
class cTrackerRansac
{
public:
    cTrackerRansac(){};
    virtual ~cTrackerRansac(){};

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
    Double_t GetError(std::array<TVector3, 2> model, std::array<Double_t, 4> p, double zScale);
    // function used to test the clusters.
    Double_t ClusterTest(double &sumvalue, double &totalenergy, std::vector<T> &inliers);

    // GET functions - as copies
    double getTracksEnergyThreshold() const { return tracksEnergyThreshold; };
    double getBeamEnergyThreshold() const { return beamEnergyThreshold; };
    double getTracksWidth() const { return tracksWidth; };
    double getBeamWidth() const { return beamWidth; };
    double getLoopsNumber() const { return loopsNumber; };
    double getTrackMinSize() const { return trackMinSize; };
    double getBeamMinSize() const { return beamMinSize; };

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

    bool longTracksEqualBeam = false;

    std::list<T> points;
    std::list<cFittedLine<T>> lines;
    std::list<cFittedLine<T>> fittedLines;

    // Function for running Ransac over the event.
    void Ransac(bool &beamTracks, bool &oneSidedTracks, bool &isFittable);

    // values used to define the boundaries of the center, and to check whether tracks on sides include points on the other sides.
    // Used only if oneSidedTracks is true!
    int lowerOneSideLimit = 61;
    int upperOneSideLimit = 68;
    int midChamberOneSide = 64; // 16 for demo

    // chamber dimensions over beam axis (for classifying beam Tracks)
    int maxX = 128;
    int beamXmin = int(maxX * 0.1);
    int beamXmax = int(maxX * 0.9);

    // Fits each line contained in lines
    void fitLines();

    // Function for the user to start the tracking
    void track(direction, direction);
};

#include "cTrackerRansac.hpp"
#endif
