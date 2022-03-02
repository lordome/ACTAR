#ifndef cPhysicalEvent_h
#define cPhysicalEvent_h

#include "TObject.h"
#include <list>
#include "cPhysicalHit.h"

class cPhysicalEvent : public TObject {
  private:
    int eventnum = -1;
    int runnum   = -1;
    unsigned long int timestamp = 0;

    std::list<cPhysicalHit> hits;
  public:
    cPhysicalEvent();
    virtual ~cPhysicalEvent();

    void setEventNumber(int);
    int getEventNumber() const;

    void setRunNumber(int);
    int getRunNumber() const;

    void setTimestamp(unsigned long int);
    unsigned long int getTimestamp() const;

    std::list<cPhysicalHit>& getHits();
    const std::list<cPhysicalHit>& getHits() const;

    ClassDef(cPhysicalEvent, 1);
};

#endif
