#ifndef cFittedLine_h
#define cFittedLine_h

#include "Rtypes.h"
#include "TObject.h"
#include "TVector3.h"
#include "TPolyLine3D.h"
#include <list>

// This class contains the information of a fitted line alongdide the points
// used in the fit

template<class T>
class cFittedLine : public TObject {
  private:
    TVector3 direction;
    TVector3 basepoint;
    std::list<T> points;
    bool fittable = false;
    bool gone = false;

    //TPolyLine3D *drawObj;


  public:
    cFittedLine();
    cFittedLine(TVector3 n, TVector3 b);
    virtual ~cFittedLine();

    TVector3 getDirection() const;
    TVector3 getBasepoint() const;
    void setDirection(const TVector3&);
    void setBasepoint(const TVector3&);
    void setPoints(const std::list<T>);
    std::list<T>& getPoints();
    void setFittable(bool);
    bool isFittable();
    void setGone(bool);
    bool isGone();

    //void Draw(Option_t* option = "");

    ClassDef(cFittedLine, 3);
};

#include "cFittedLine.hpp"

#endif
