from ROOT import gROOT
#include "TROOT.h"

print("sono un cane")
gROOT.ProcessLine(".x ../callRootlogon.C")
print("rootlogon FINISHED")


