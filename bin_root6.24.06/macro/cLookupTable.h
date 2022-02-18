#include <string>
#include <map>
#include <list>
#include "TObject.h"

#ifndef cLookupTable_h
#define cLookupTable_h

/**
\brief Stores the lookup table
and gives useful function  to navigate through it
*/
class cLookupTable : public TObject {
  public:
    cLookupTable() {} /**< Default constructor */

    cLookupTable(std::string);
    cLookupTable(std::string, std::string);

    /**
    \brief Channel data class
    This class is designed to contain the important informations
    necessary to each channel
    */
    class chanData {
      private:
        Int_t row;         /**< Row number */
        Int_t col;         /**< Column number */
        Int_t det;         /**< Detector number */
        std::string psaID; /**< Id of the PSA to apply */
      public:
        chanData();
        chanData(Int_t, Int_t, Int_t, std::string);
        virtual ~chanData() {}

        Int_t       getRow()   const {return row;}   /**< Row number */
        Int_t       getCol()   const {return col;}   /**< Column number */
        Int_t       getDet()   const {return det;}   /**< Detector number */
        std::string getPsaID() const {return psaID;} /**< Id of the PSA to apply */

        void setPsaID(std::string v) {psaID = v;}
    };

    static Int_t getGlobalChannelId(Int_t channel, Int_t aget, Int_t asad, Int_t cobo);

    virtual ~cLookupTable() {}

    std::map<Int_t, chanData>& getTable() {return table;}

    std::string getPSAIdFromGlobalChannelId(Int_t) const;

    std::string getPSAIdFromNumbers(Int_t channel, Int_t aget, Int_t asad, Int_t cobo) const;

    static Int_t getCoboFromGlobalChannelId(Int_t);
    static Int_t getAsadFromGlobalChannelId(Int_t);
    static Int_t getAgetFromGlobalChannelId(Int_t);
    static Int_t getChanFromGlobalChannelId(Int_t);

    void loadPreprocessorTable(std::string);
    const std::list<std::string>& getPreprocessorRequested();

  private:
    /**
    Table to associate each channel's globalchannelid to the detector
    informations.
    */
    std::map<Int_t, chanData> table;

    /**
    List containing the preprocessor to use
    */
    std::list<std::string> prepNames;

  ClassDef(cLookupTable, 1);
};

typedef cLookupTable::chanData chanData;

#endif
