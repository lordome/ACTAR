#include <vector>
#include "TObject.h"
#include "cHit.h"
#include "cRawSignal.h"

#ifndef cRawEvent_h
#define cRawEvent_h

/**
\brief Class that conitains the array of hits for each pad together with the raw signal
*/

class cRawEvent : public TObject {
  private:
    int eventnum;
    unsigned long int timestamp;
    std::vector<cHit>       hits;

    std::vector<cRawSignal> signal;

  public:
    cRawEvent();
    virtual ~cRawEvent();

    int getEventNumber() const {return eventnum; }
    unsigned long int getTimestamp()   const {return timestamp;}

    void setEventNumber(int eu) {eventnum  = eu;}
    void setTimestamp  (unsigned long int tu) {timestamp = tu;}

          std::vector<cHit>& getHits()       {return hits;}
    const std::vector<cHit>& getHits() const {return hits;}

    std::vector<cRawSignal>& getSignal() {return signal;}

    void clear();                          ///< Clears the object
    void append(const std::vector<cHit>&); ///< Appends hits to the vector

    /// Appends hits and signals (when present) of another event
    void append(const cRawEvent&);

    ClassDef(cRawEvent, 2);
};

#endif
