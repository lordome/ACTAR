#include "cPhysicalEvent.h"

ClassImp(cPhysicalEvent);

cPhysicalEvent::cPhysicalEvent() {}

cPhysicalEvent::~cPhysicalEvent() {}

void cPhysicalEvent::setEventNumber(int v) {
  eventnum = v;
}

int cPhysicalEvent::getEventNumber() const {
  return eventnum;
}

void cPhysicalEvent::setRunNumber(int v) {
  runnum = v;
}

int cPhysicalEvent::getRunNumber() const {
  return runnum;
}

void cPhysicalEvent::setTimestamp(unsigned long int v) {
  timestamp = v;
}

unsigned long int cPhysicalEvent::getTimestamp() const {
  return timestamp;
}

std::list<cPhysicalHit>& cPhysicalEvent::getHits() {
  return hits;
}

const std::list<cPhysicalHit>& cPhysicalEvent::getHits() const {
  return hits;
}
