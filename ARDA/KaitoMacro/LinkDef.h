#ifndef LINKDEF_H
#define LINKDEF_H 1

#include <stdio.h>
#include <fstream>
#include <vector>

#include "TVector3.h"
#include "TString.h"

using namespace std;

#pragma link C++ class vector<TVector3> +;
#pragma link C++ class vector<TString> +;


#endif
