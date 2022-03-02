#include "cHit.h"

ClassImp(cHit);

/// Default constructor
cHit::cHit() {
  peakheight      = -1;
  peaktime        = -1;
  globalchannelid = -1;
}

/// Constructor
cHit::cHit(Int_t globu, Double_t uheight, Double_t utime) {
  globalchannelid = globu;
  peakheight      = uheight;
  peaktime        = utime;
}

///Copy constructor
cHit::cHit(const cHit &h) {
  globalchannelid = h.globalchannelid;
  peakheight      = h.peakheight;
  peaktime        = h.peaktime;
  psaValues       = h.psaValues;
}

/// Destructor
cHit::~cHit() {}
