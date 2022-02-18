// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__dict__DSP
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
#include "pDSP.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_pDSP(void *p = 0);
   static void *newArray_pDSP(Long_t size, void *p);
   static void delete_pDSP(void *p);
   static void deleteArray_pDSP(void *p);
   static void destruct_pDSP(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::pDSP*)
   {
      ::pDSP *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::pDSP >(0);
      static ::ROOT::TGenericClassInfo 
         instance("pDSP", ::pDSP::Class_Version(), "pDSP.h", 41,
                  typeid(::pDSP), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::pDSP::Dictionary, isa_proxy, 4,
                  sizeof(::pDSP) );
      instance.SetNew(&new_pDSP);
      instance.SetNewArray(&newArray_pDSP);
      instance.SetDelete(&delete_pDSP);
      instance.SetDeleteArray(&deleteArray_pDSP);
      instance.SetDestructor(&destruct_pDSP);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::pDSP*)
   {
      return GenerateInitInstanceLocal((::pDSP*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::pDSP*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr pDSP::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *pDSP::Class_Name()
{
   return "pDSP";
}

//______________________________________________________________________________
const char *pDSP::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::pDSP*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int pDSP::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::pDSP*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *pDSP::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::pDSP*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *pDSP::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::pDSP*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void pDSP::Streamer(TBuffer &R__b)
{
   // Stream an object of class pDSP.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(pDSP::Class(),this);
   } else {
      R__b.WriteClassBuffer(pDSP::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_pDSP(void *p) {
      return  p ? new(p) ::pDSP : new ::pDSP;
   }
   static void *newArray_pDSP(Long_t nElements, void *p) {
      return p ? new(p) ::pDSP[nElements] : new ::pDSP[nElements];
   }
   // Wrapper around operator delete
   static void delete_pDSP(void *p) {
      delete ((::pDSP*)p);
   }
   static void deleteArray_pDSP(void *p) {
      delete [] ((::pDSP*)p);
   }
   static void destruct_pDSP(void *p) {
      typedef ::pDSP current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::pDSP

namespace {
  void TriggerDictionaryInitialization_libdict__DSP_Impl() {
    static const char* headers[] = {
"pDSP.h",
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
#line 1 "libdict__DSP dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$pDSP.h")))  pDSP;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libdict__DSP dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "pDSP.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"pDSP", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libdict__DSP",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libdict__DSP_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libdict__DSP_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libdict__DSP() {
  TriggerDictionaryInitialization_libdict__DSP_Impl();
}
