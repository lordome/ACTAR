#define createPSAFactoryName(pf, po) pf ## po
#define PSAFactoryName createPSAFactoryName(psaClassName, Factory)

#include "cAnalysisController.h"

class PSAFactoryName : public cAnalysisController::abstractFactory {
  public:
    PSAFactoryName() : abstractFactory(psaIdName) {}
    ~PSAFactoryName() {}

    cPSA* createPSA() {return new psaClassName;}
};

static PSAFactoryName createPSAFactoryName(psaClassName, FacObj);
