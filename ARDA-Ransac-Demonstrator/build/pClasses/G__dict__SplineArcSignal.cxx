// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__dict__SplineArcSignal
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
#include "pSplineArcSignal.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_pSplineArcSignal(void *p = 0);
   static void *newArray_pSplineArcSignal(Long_t size, void *p);
   static void delete_pSplineArcSignal(void *p);
   static void deleteArray_pSplineArcSignal(void *p);
   static void destruct_pSplineArcSignal(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::pSplineArcSignal*)
   {
      ::pSplineArcSignal *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::pSplineArcSignal >(0);
      static ::ROOT::TGenericClassInfo 
         instance("pSplineArcSignal", ::pSplineArcSignal::Class_Version(), "pSplineArcSignal.h", 24,
                  typeid(::pSplineArcSignal), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::pSplineArcSignal::Dictionary, isa_proxy, 4,
                  sizeof(::pSplineArcSignal) );
      instance.SetNew(&new_pSplineArcSignal);
      instance.SetNewArray(&newArray_pSplineArcSignal);
      instance.SetDelete(&delete_pSplineArcSignal);
      instance.SetDeleteArray(&deleteArray_pSplineArcSignal);
      instance.SetDestructor(&destruct_pSplineArcSignal);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::pSplineArcSignal*)
   {
      return GenerateInitInstanceLocal((::pSplineArcSignal*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::pSplineArcSignal*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr pSplineArcSignal::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *pSplineArcSignal::Class_Name()
{
   return "pSplineArcSignal";
}

//______________________________________________________________________________
const char *pSplineArcSignal::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::pSplineArcSignal*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int pSplineArcSignal::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::pSplineArcSignal*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *pSplineArcSignal::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::pSplineArcSignal*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *pSplineArcSignal::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::pSplineArcSignal*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void pSplineArcSignal::Streamer(TBuffer &R__b)
{
   // Stream an object of class pSplineArcSignal.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(pSplineArcSignal::Class(),this);
   } else {
      R__b.WriteClassBuffer(pSplineArcSignal::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_pSplineArcSignal(void *p) {
      return  p ? new(p) ::pSplineArcSignal : new ::pSplineArcSignal;
   }
   static void *newArray_pSplineArcSignal(Long_t nElements, void *p) {
      return p ? new(p) ::pSplineArcSignal[nElements] : new ::pSplineArcSignal[nElements];
   }
   // Wrapper around operator delete
   static void delete_pSplineArcSignal(void *p) {
      delete ((::pSplineArcSignal*)p);
   }
   static void deleteArray_pSplineArcSignal(void *p) {
      delete [] ((::pSplineArcSignal*)p);
   }
   static void destruct_pSplineArcSignal(void *p) {
      typedef ::pSplineArcSignal current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::pSplineArcSignal

namespace {
  void TriggerDictionaryInitialization_libdict__SplineArcSignal_Impl() {
    static const char* headers[] = {
"pSplineArcSignal.h",
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
#line 1 "libdict__SplineArcSignal dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$pSplineArcSignal.h")))  pSplineArcSignal;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libdict__SplineArcSignal dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "pSplineArcSignal.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"pSplineArcSignal", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libdict__SplineArcSignal",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libdict__SplineArcSignal_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libdict__SplineArcSignal_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libdict__SplineArcSignal() {
  TriggerDictionaryInitialization_libdict__SplineArcSignal_Impl();
}
