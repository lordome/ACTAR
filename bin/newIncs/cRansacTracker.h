#ifndef cRansacTracker_h
#define cRansacTracker_h

#include "cFittedEvent.h"
#include "cPhysicalHit.h"
#include "TObject.h"

template <class T>
class cRansacTracker : public TObject {

public:

  cRansacTracker();
  ~cRansacTracker();

  double getThreshold()& const     { return threshold; };
  double getfEnergy()&   const     { return fenergy;   };
  double getWidth()&     const     { return width;     };
  double getfWidth()&    const     { return fwidth;    };
  double getLoops()&     const     { return loops;     };
  double getTrsize()&    const     { return trsize;    };
  double getBesize()&    const     { return besize;    };
  double getSqVertexWidth()& const { return SqVertex_Width; };

  double setThreshold (double& th)            { threshold = th;            };
  double setfEnergy (double& fEn)             { fenergy = fEn;             };
  double setWidth(double& wid)                { width = wi;                };
  double setfWidth (double& fwid)             { fwidth = fwid;             };
  double setSqVertexWidth (double& sqVerwid)  { SqVertex_Width = sqVerwid; };
  double setLoops (double& lop)               { loops = lop;               };
  double setTrsize (double& trsi)             { trsize = trsi;             };
  double setBesize (double& besi)             { besize = besi;             };


  void Cluster( cFittedEvent<T> * fitEvt, list<cPhysicalHit>& hitslist, double& trackcount );
  void Track  ( cFittedEvent<T> * fitEvt );
  void Vertex ( cFittedEvent<T> * fitEvt, vector<int>& besttracks; );

private:

  double threshold      = 3500; // minimum energy requested for a single cluster.
  double fenergy        = 40000; //energy threshold for a track in order to be considered a beam track.
  double width          = 4; // maximum distance from model accepted in clustering
  double fwidth         = 3.5; // maximum distance from model accepted in clustering for beam tracks
  double SqVertex_Width = 55; //maximum distance accepted between two different cluster
  double loops          = 1000; // number of loops, i.e. number of random couples chosen.
  double trsize         = 16; //min number of pads required in order to consider a cluster a real track
  double besize         = 60;   //min number of pads required in order to consider a cluster a real track FOR BEAM

  TVector3 zero = (0.,0.,0.);
}

#include "cRansacTracker.hpp"

#endif
