#include "Parameters.h"
#include "TObject.h"

#ifndef cRawSignal_h
#define cRawSignal_h

class pSignal;

/**
\brief This class describes the raw signal
This class is used to store the raw signal from each channel
*/

class cRawSignal : public TObject {
  private:
    Int_t channel;
    Int_t aget;
    Int_t asad;
    Int_t cobo;
    Int_t event;

    Float_t samples[NSAMPLES];
  public:
    cRawSignal() : channel(-1),
                   aget   (-1),
                   asad   (-1),
                   cobo   (-1),
                   event  (-1) {
                     for (int i = 0; i < NSAMPLES; i++) samples[i] = 0;
                   }

    virtual ~cRawSignal() {}

    void setChannel(Int_t v) {channel = v;}
    void setAget   (Int_t v) {aget    = v;}
    void setAsad   (Int_t v) {asad    = v;}
    void setCobo   (Int_t v) {cobo    = v;}
    void setEvent  (Int_t v) {event   = v;}

    Int_t getChannel() const {return channel;}
    Int_t getAget   () const {return aget;}
    Int_t getAsad   () const {return asad;}
    Int_t getCobo   () const {return cobo;}
    Int_t getEvent  () const {return event;}

    Int_t getGlobalChannelId() const;

    Float_t* getSamples() {return samples;}

    operator pSignal() const;

    ClassDef(cRawSignal, 1);
};

#endif
