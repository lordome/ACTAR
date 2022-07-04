// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__dict__CFD
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "pCFD.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_pCFD(void *p = 0);
   static void *newArray_pCFD(Long_t size, void *p);
   static void delete_pCFD(void *p);
   static void deleteArray_pCFD(void *p);
   static void destruct_pCFD(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::pCFD*)
   {
      ::pCFD *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::pCFD >(0);
      static ::ROOT::TGenericClassInfo 
         instance("pCFD", ::pCFD::Class_Version(), "pCFD.h", 54,
                  typeid(::pCFD), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::pCFD::Dictionary, isa_proxy, 4,
                  sizeof(::pCFD) );
      instance.SetNew(&new_pCFD);
      instance.SetNewArray(&newArray_pCFD);
      instance.SetDelete(&delete_pCFD);
      instance.SetDeleteArray(&deleteArray_pCFD);
      instance.SetDestructor(&destruct_pCFD);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::pCFD*)
   {
      return GenerateInitInstanceLocal((::pCFD*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::pCFD*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr pCFD::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *pCFD::Class_Name()
{
   return "pCFD";
}

//______________________________________________________________________________
const char *pCFD::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::pCFD*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int pCFD::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::pCFD*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *pCFD::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::pCFD*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *pCFD::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::pCFD*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void pCFD::Streamer(TBuffer &R__b)
{
   // Stream an object of class pCFD.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(pCFD::Class(),this);
   } else {
      R__b.WriteClassBuffer(pCFD::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_pCFD(void *p) {
      return  p ? new(p) ::pCFD : new ::pCFD;
   }
   static void *newArray_pCFD(Long_t nElements, void *p) {
      return p ? new(p) ::pCFD[nElements] : new ::pCFD[nElements];
   }
   // Wrapper around operator delete
   static void delete_pCFD(void *p) {
      delete ((::pCFD*)p);
   }
   static void deleteArray_pCFD(void *p) {
      delete [] ((::pCFD*)p);
   }
   static void destruct_pCFD(void *p) {
      typedef ::pCFD current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::pCFD

namespace {
  void TriggerDictionaryInitialization_libdict__CFD_Impl() {
    static const char* headers[] = {
"pCFD.h",
0
    };
    static const char* includePaths[] = {
"/home/lorenzo/root/include",
"/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/inc",
"/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/libMFM",
"/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/pClasses/inc",
"/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/leuvenPSA/inc",
"/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/pClasses",
"/home/lorenzo/root/include/",
"/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/pClasses/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libdict__CFD dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$pCFD.h")))  pCFD;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libdict__CFD dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "pCFD.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"pCFD", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libdict__CFD",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libdict__CFD_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libdict__CFD_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libdict__CFD() {
  TriggerDictionaryInitialization_libdict__CFD_Impl();
}
