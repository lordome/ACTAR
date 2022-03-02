// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__dict__Utility
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
#include "pUtility.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_pUtility(void *p = 0);
   static void *newArray_pUtility(Long_t size, void *p);
   static void delete_pUtility(void *p);
   static void deleteArray_pUtility(void *p);
   static void destruct_pUtility(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::pUtility*)
   {
      ::pUtility *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::pUtility >(0);
      static ::ROOT::TGenericClassInfo 
         instance("pUtility", ::pUtility::Class_Version(), "pUtility.h", 16,
                  typeid(::pUtility), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::pUtility::Dictionary, isa_proxy, 4,
                  sizeof(::pUtility) );
      instance.SetNew(&new_pUtility);
      instance.SetNewArray(&newArray_pUtility);
      instance.SetDelete(&delete_pUtility);
      instance.SetDeleteArray(&deleteArray_pUtility);
      instance.SetDestructor(&destruct_pUtility);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::pUtility*)
   {
      return GenerateInitInstanceLocal((::pUtility*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::pUtility*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr pUtility::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *pUtility::Class_Name()
{
   return "pUtility";
}

//______________________________________________________________________________
const char *pUtility::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::pUtility*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int pUtility::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::pUtility*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *pUtility::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::pUtility*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *pUtility::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::pUtility*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void pUtility::Streamer(TBuffer &R__b)
{
   // Stream an object of class pUtility.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(pUtility::Class(),this);
   } else {
      R__b.WriteClassBuffer(pUtility::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_pUtility(void *p) {
      return  p ? new(p) ::pUtility : new ::pUtility;
   }
   static void *newArray_pUtility(Long_t nElements, void *p) {
      return p ? new(p) ::pUtility[nElements] : new ::pUtility[nElements];
   }
   // Wrapper around operator delete
   static void delete_pUtility(void *p) {
      delete ((::pUtility*)p);
   }
   static void deleteArray_pUtility(void *p) {
      delete [] ((::pUtility*)p);
   }
   static void destruct_pUtility(void *p) {
      typedef ::pUtility current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::pUtility

namespace {
  void TriggerDictionaryInitialization_libdict__Utility_Impl() {
    static const char* headers[] = {
"pUtility.h",
0
    };
    static const char* includePaths[] = {
"/home/lorenzo/root/include",
"/home/lorenzo/Desktop/ARDAC/ARDA/inc",
"/home/lorenzo/Desktop/ARDAC/ARDA/libMFM",
"/home/lorenzo/Desktop/ARDAC/ARDA/pClasses/inc",
"/home/lorenzo/Desktop/ARDAC/ARDA/leuvenPSA/inc",
"/home/lorenzo/Desktop/ARDAC/ARDA/pClasses",
"/home/lorenzo/root/include/",
"/home/lorenzo/Desktop/ARDAC/build/pClasses/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libdict__Utility dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$pUtility.h")))  pUtility;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libdict__Utility dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "pUtility.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"pUtility", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libdict__Utility",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libdict__Utility_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libdict__Utility_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libdict__Utility() {
  TriggerDictionaryInitialization_libdict__Utility_Impl();
}
