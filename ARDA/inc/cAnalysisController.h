#include "cPSA.h"
#include <string>
#include <map>
#include <list>

#ifndef cAnalysisController_h
#define cAnalysisController_h

/**
\brief This class admministers the assignment of raw data frame to the correct pulse shape analysis
To add a new PSA take cBasicPSA as an example
*/

class cAnalysisController {
  public:
    cPSA* getPSA(std::string);

    class abstractFactory {
      public:
        abstractFactory(std::string name) {registerPSA(name, this);}
        virtual ~abstractFactory() {}

        virtual cPSA* createPSA() = 0;
      private:
        abstractFactory() {}
    };

    cAnalysisController (); ///< Constructor
    ~cAnalysisController(); ///< Destructor

    std::list<std::string> getPSANames() const;
  private:
    static void registerPSA(std::string, abstractFactory*);

    /**
    This map contains the factory that wille create the PSA object corresponding
    to every PSA id string
    */
    static std::map<std::string, abstractFactory*>& getPSAlist();

    /**
    \brief To ensure thread safety and fast access every copy has its own
    PSA list, copied by the constructor when the object is created
    */
    std::map<std::string, abstractFactory*> properPSAList;
};

#endif
