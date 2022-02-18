#ifndef cSignalPreprocessorManager_h
#define cSignalPreprocessorManager_h

#include "cSignalPreprocessor.h"
#include <list>

class cSignalPreprocessorManager {
  public:
    cSignalPreprocessorManager() {}
    virtual ~cSignalPreprocessorManager() {}

    class abstractFactory {
      public:
        abstractFactory(std::string nameU) : name(nameU) {registerPreprocessor(this);}
        virtual ~abstractFactory() {}

        std::string getName() const {return name;}

        virtual cSignalPreprocessor* createPreprocessor() = 0;
      private:
        std::string name;
    };

    static void registerPreprocessor(abstractFactory*);

    static std::list<abstractFactory*> getPreprocessorFactories();

  private:
    static std::list<abstractFactory*>& getPreprocList();
};

#endif
