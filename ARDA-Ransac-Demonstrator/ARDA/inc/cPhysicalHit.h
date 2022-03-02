#ifndef cPhysicalHit_h
#define cPhysicalHit_h

#include "TObject.h"

class cPhysicalHit : public TObject {
  private:
    Double_t x = -1.;
    Double_t y = -1.;
    Double_t z = -1.;
    Double_t energy = -1.;
    Double_t time = -1.;
    Int_t globalchannelid = 0;

    bool trackable = false;

  public:
    cPhysicalHit();
    virtual ~cPhysicalHit();

    Double_t& operator[](int);
    const Double_t& operator[](int) const;

    Double_t getX() const;
    void setX(Double_t);
    Double_t getY() const;
    void setY(Double_t);
    Double_t getZ() const;
    void setZ(Double_t);
    Double_t getEnergy() const;
    Double_t GetEnergy() const;
    void setEnergy(Double_t);
    Double_t getTime() const;
    void setTime(Double_t);
    Int_t getGlobalChannelId() const;
    void setGlobalChannelId(Int_t);

    bool isTrackable() const;
    void setTrackable(bool);

    ClassDef(cPhysicalHit, 1);
};

#endif
