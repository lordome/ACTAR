// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__dict__FT
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
#include "pFT.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void delete_pFT(void *p);
   static void deleteArray_pFT(void *p);
   static void destruct_pFT(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::pFT*)
   {
      ::pFT *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::pFT >(0);
      static ::ROOT::TGenericClassInfo 
         instance("pFT", ::pFT::Class_Version(), "pFT.h", 28,
                  typeid(::pFT), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::pFT::Dictionary, isa_proxy, 4,
                  sizeof(::pFT) );
      instance.SetDelete(&delete_pFT);
      instance.SetDeleteArray(&deleteArray_pFT);
      instance.SetDestructor(&destruct_pFT);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::pFT*)
   {
      return GenerateInitInstanceLocal((::pFT*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::pFT*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr pFT::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *pFT::Class_Name()
{
   return "pFT";
}

//______________________________________________________________________________
const char *pFT::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::pFT*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int pFT::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::pFT*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *pFT::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::pFT*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *pFT::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::pFT*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void pFT::Streamer(TBuffer &R__b)
{
   // Stream an object of class pFT.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(pFT::Class(),this);
   } else {
      R__b.WriteClassBuffer(pFT::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_pFT(void *p) {
      delete ((::pFT*)p);
   }
   static void deleteArray_pFT(void *p) {
      delete [] ((::pFT*)p);
   }
   static void destruct_pFT(void *p) {
      typedef ::pFT current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::pFT

namespace {
  void TriggerDictionaryInitialization_libdict__FT_Impl() {
    static const char* headers[] = {
"pFT.h",
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
#line 1 "libdict__FT dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$pFT.h")))  pFT;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libdict__FT dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "pFT.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"pFT", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libdict__FT",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libdict__FT_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libdict__FT_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libdict__FT() {
  TriggerDictionaryInitialization_libdict__FT_Impl();
}
