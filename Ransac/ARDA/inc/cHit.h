#include <TObject.h>
#include <vector>

#ifndef cHit_h
#define cHit_h

/**
\brief Class to store each channel's hit
*/

class cHit: public TObject {
  private:
    Double_t peakheight;
    Double_t peaktime;
    std::vector<Double_t> psaValues;  ///< Container for other values the PSA outputs

    /**
    \brief Unique id for each channel
    It is computed as follows:
    globalchannelid = channel + 100 * agetnumber +  1000 * asadnumber + 10000 * cobonumber
    */
    Int_t globalchannelid;
  public:
    cHit();
    cHit(Int_t globu, Double_t uheight, Double_t utime);
	cHit(const cHit &);
    virtual ~cHit();

    Double_t getPeakHeight() const {return peakheight;}
    Double_t getPeakTime  () const {return peaktime;  }

    Int_t getGlobalChannelId() const   {return globalchannelid;}
    void  setGlobalChannelId(Int_t cu) {globalchannelid = cu;}

    void setPeakHeight(Double_t uh) {peakheight = uh;}
    void setPeakTime  (Double_t ut) {peaktime   = ut;}

    std::vector<Double_t>&       getpsaValues()       {return psaValues;}
    const std::vector<Double_t>& getpsaValues() const {return psaValues;}

    ClassDef(cHit, 1);
};

#endif
