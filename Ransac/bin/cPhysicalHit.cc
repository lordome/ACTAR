#include "cPhysicalHit.h"
#include <stdexcept>

ClassImp(cPhysicalHit);

cPhysicalHit::cPhysicalHit() {}

cPhysicalHit::~cPhysicalHit() {}

Double_t& cPhysicalHit::operator[](int i) {
       if (i == 0) return x;
  else if (i == 1) return y;
  else if (i == 2) return z;
  else throw std::out_of_range("cPhysical hit has only three dimensions");
}

const Double_t& cPhysicalHit::operator[](int i) const {
  return this->operator[](i);
}

Double_t cPhysicalHit::getX() const {
  return x;
}

void cPhysicalHit::setX(Double_t v) {
  x = v;
}

Double_t cPhysicalHit::getY() const {
  return y;
}

void cPhysicalHit::setY(Double_t v) {
  y = v;
}

Double_t cPhysicalHit::getZ() const {
  return z;
}

void cPhysicalHit::setZ(Double_t v) {
  z = v;
}

Double_t cPhysicalHit::getEnergy() const {
  return energy;
}

Double_t cPhysicalHit::GetEnergy() const {
  return energy;
}

void cPhysicalHit::setEnergy(Double_t v) {
  energy = v;
}

Double_t cPhysicalHit::getTime() const {
  return time;
}

void cPhysicalHit::setTime(Double_t v) {
  time = v;
}

Int_t cPhysicalHit::getGlobalChannelId() const {
  return globalchannelid;
}

void cPhysicalHit::setGlobalChannelId(Int_t v) {
  globalchannelid = v;
}

bool cPhysicalHit::isTrackable() const {
  return trackable;
}

void cPhysicalHit::setTrackable(bool d = true) {
  trackable = d;
}
