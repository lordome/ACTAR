#ifndef cFittedEvent_h
#define cFittedEvent_h

#include "cFittedLine.h"
#include "TObject.h"
#include "TVector3.h"
#include <list>
#include "cVertex.h"

template<class T>
class cFittedEvent : public TObject {
private:

  std::list<cFittedLine<T>> lines;
  std::list<T> unfittedPoints;

  std::list<T> ancillaryHit;
  int eventnum;
  int runnum;
  unsigned long int timestamp;

  std::list<cVertex<T>> vertex;

public:
  cFittedEvent();
  virtual ~cFittedEvent();

  void setEventNumber(int);
  int getEventNumber() const;

  void setTimestamp(unsigned long int);
  unsigned long int getTimestamp() const;

  void setRunNumber(int);
  int getRunNumber() const;

  std::list<cFittedLine<T>>& getLines();
  const std::list<cFittedLine<T>>& getLines() const;
  void setLines(std::list<cFittedLine<T>>&);

  std::list<T>& getUnfittedPoints();
  const std::list<T>& getUnfittedPoints() const;
  void setUnfittedPoints(std::list<T>&);

  std::list<T>& getAncillaryHit();
  const std::list<T>& getAncillaryHit() const;
  void setAncillaryHit(std::list<T>&);

  void setVertex(std::list<cVertex<T>>&);
  std::list<cVertex<T>>& getVertex();


  ClassDef(cFittedEvent, 2);
};
#include "cFittedEvent.hpp"
#endif
