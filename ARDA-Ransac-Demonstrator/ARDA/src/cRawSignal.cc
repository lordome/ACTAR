#include "cRawSignal.h"
#include "cLookupTable.h"
#include "pSignal.h"

ClassImp(cRawSignal);

Int_t cRawSignal::getGlobalChannelId() const {
  return cLookupTable::getGlobalChannelId(channel, aget, asad, cobo);
}

cRawSignal::operator pSignal() const {
  pSignal *p = new pSignal(NSAMPLES);

  for (Int_t i = 0; i < NSAMPLES; i++) p->SetAt(samples[i], i);

  return *p;
}
