// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__dict__DigitalSignal
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
#include "pDigitalSignal.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_pDigitalSignal(void *p = 0);
   static void *newArray_pDigitalSignal(Long_t size, void *p);
   static void delete_pDigitalSignal(void *p);
   static void deleteArray_pDigitalSignal(void *p);
   static void destruct_pDigitalSignal(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::pDigitalSignal*)
   {
      ::pDigitalSignal *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::pDigitalSignal >(0);
      static ::ROOT::TGenericClassInfo 
         instance("pDigitalSignal", ::pDigitalSignal::Class_Version(), "pDigitalSignal.h", 45,
                  typeid(::pDigitalSignal), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::pDigitalSignal::Dictionary, isa_proxy, 4,
                  sizeof(::pDigitalSignal) );
      instance.SetNew(&new_pDigitalSignal);
      instance.SetNewArray(&newArray_pDigitalSignal);
      instance.SetDelete(&delete_pDigitalSignal);
      instance.SetDeleteArray(&deleteArray_pDigitalSignal);
      instance.SetDestructor(&destruct_pDigitalSignal);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::pDigitalSignal*)
   {
      return GenerateInitInstanceLocal((::pDigitalSignal*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::pDigitalSignal*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr pDigitalSignal::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *pDigitalSignal::Class_Name()
{
   return "pDigitalSignal";
}

//______________________________________________________________________________
const char *pDigitalSignal::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::pDigitalSignal*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int pDigitalSignal::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::pDigitalSignal*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *pDigitalSignal::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::pDigitalSignal*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *pDigitalSignal::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::pDigitalSignal*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void pDigitalSignal::Streamer(TBuffer &R__b)
{
   // Stream an object of class pDigitalSignal.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(pDigitalSignal::Class(),this);
   } else {
      R__b.WriteClassBuffer(pDigitalSignal::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_pDigitalSignal(void *p) {
      return  p ? new(p) ::pDigitalSignal : new ::pDigitalSignal;
   }
   static void *newArray_pDigitalSignal(Long_t nElements, void *p) {
      return p ? new(p) ::pDigitalSignal[nElements] : new ::pDigitalSignal[nElements];
   }
   // Wrapper around operator delete
   static void delete_pDigitalSignal(void *p) {
      delete ((::pDigitalSignal*)p);
   }
   static void deleteArray_pDigitalSignal(void *p) {
      delete [] ((::pDigitalSignal*)p);
   }
   static void destruct_pDigitalSignal(void *p) {
      typedef ::pDigitalSignal current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::pDigitalSignal

namespace {
  void TriggerDictionaryInitialization_libdict__DigitalSignal_Impl() {
    static const char* headers[] = {
"pDigitalSignal.h",
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
#line 1 "libdict__DigitalSignal dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$pDigitalSignal.h")))  pDigitalSignal;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libdict__DigitalSignal dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "pDigitalSignal.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"pDigitalSignal", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libdict__DigitalSignal",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libdict__DigitalSignal_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libdict__DigitalSignal_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libdict__DigitalSignal() {
  TriggerDictionaryInitialization_libdict__DigitalSignal_Impl();
}
