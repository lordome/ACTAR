// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME ActarSimDict
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
#include "/home/ats/Actarsim/include/ActarSimData.hh"
#include "/home/ats/Actarsim/include/ActarSimTrack.hh"
#include "/home/ats/Actarsim/include/ActarSimSimpleTrack.hh"
#include "/home/ats/Actarsim/include/ActarSimSilHit.hh"
#include "/home/ats/Actarsim/include/ActarSimSilRingHit.hh"
#include "/home/ats/Actarsim/include/ActarSimSciHit.hh"
#include "/home/ats/Actarsim/include/ActarSimSciRingHit.hh"
#include "/home/ats/Actarsim/include/ActarSimPlaHit.hh"
#include "/home/ats/Actarsim/include/ActarSimPrimaryInfo.hh"
#include "/home/ats/Actarsim/include/ActarSimBeamInfo.hh"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_ActarSimData(void *p = nullptr);
   static void *newArray_ActarSimData(Long_t size, void *p);
   static void delete_ActarSimData(void *p);
   static void deleteArray_ActarSimData(void *p);
   static void destruct_ActarSimData(void *p);
   static void streamer_ActarSimData(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ActarSimData*)
   {
      ::ActarSimData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ActarSimData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("ActarSimData", ::ActarSimData::Class_Version(), "include/ActarSimData.hh", 16,
                  typeid(::ActarSimData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ActarSimData::Dictionary, isa_proxy, 16,
                  sizeof(::ActarSimData) );
      instance.SetNew(&new_ActarSimData);
      instance.SetNewArray(&newArray_ActarSimData);
      instance.SetDelete(&delete_ActarSimData);
      instance.SetDeleteArray(&deleteArray_ActarSimData);
      instance.SetDestructor(&destruct_ActarSimData);
      instance.SetStreamerFunc(&streamer_ActarSimData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ActarSimData*)
   {
      return GenerateInitInstanceLocal((::ActarSimData*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ActarSimData*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ActarSimTrack(void *p = nullptr);
   static void *newArray_ActarSimTrack(Long_t size, void *p);
   static void delete_ActarSimTrack(void *p);
   static void deleteArray_ActarSimTrack(void *p);
   static void destruct_ActarSimTrack(void *p);
   static void streamer_ActarSimTrack(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ActarSimTrack*)
   {
      ::ActarSimTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ActarSimTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("ActarSimTrack", ::ActarSimTrack::Class_Version(), "include/ActarSimTrack.hh", 24,
                  typeid(::ActarSimTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ActarSimTrack::Dictionary, isa_proxy, 16,
                  sizeof(::ActarSimTrack) );
      instance.SetNew(&new_ActarSimTrack);
      instance.SetNewArray(&newArray_ActarSimTrack);
      instance.SetDelete(&delete_ActarSimTrack);
      instance.SetDeleteArray(&deleteArray_ActarSimTrack);
      instance.SetDestructor(&destruct_ActarSimTrack);
      instance.SetStreamerFunc(&streamer_ActarSimTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ActarSimTrack*)
   {
      return GenerateInitInstanceLocal((::ActarSimTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ActarSimTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ActarSimSimpleTrack(void *p = nullptr);
   static void *newArray_ActarSimSimpleTrack(Long_t size, void *p);
   static void delete_ActarSimSimpleTrack(void *p);
   static void deleteArray_ActarSimSimpleTrack(void *p);
   static void destruct_ActarSimSimpleTrack(void *p);
   static void streamer_ActarSimSimpleTrack(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ActarSimSimpleTrack*)
   {
      ::ActarSimSimpleTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ActarSimSimpleTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("ActarSimSimpleTrack", ::ActarSimSimpleTrack::Class_Version(), "include/ActarSimSimpleTrack.hh", 16,
                  typeid(::ActarSimSimpleTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ActarSimSimpleTrack::Dictionary, isa_proxy, 16,
                  sizeof(::ActarSimSimpleTrack) );
      instance.SetNew(&new_ActarSimSimpleTrack);
      instance.SetNewArray(&newArray_ActarSimSimpleTrack);
      instance.SetDelete(&delete_ActarSimSimpleTrack);
      instance.SetDeleteArray(&deleteArray_ActarSimSimpleTrack);
      instance.SetDestructor(&destruct_ActarSimSimpleTrack);
      instance.SetStreamerFunc(&streamer_ActarSimSimpleTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ActarSimSimpleTrack*)
   {
      return GenerateInitInstanceLocal((::ActarSimSimpleTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ActarSimSimpleTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ActarSimSilHit(void *p = nullptr);
   static void *newArray_ActarSimSilHit(Long_t size, void *p);
   static void delete_ActarSimSilHit(void *p);
   static void deleteArray_ActarSimSilHit(void *p);
   static void destruct_ActarSimSilHit(void *p);
   static void streamer_ActarSimSilHit(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ActarSimSilHit*)
   {
      ::ActarSimSilHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ActarSimSilHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("ActarSimSilHit", ::ActarSimSilHit::Class_Version(), "include/ActarSimSilHit.hh", 17,
                  typeid(::ActarSimSilHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ActarSimSilHit::Dictionary, isa_proxy, 16,
                  sizeof(::ActarSimSilHit) );
      instance.SetNew(&new_ActarSimSilHit);
      instance.SetNewArray(&newArray_ActarSimSilHit);
      instance.SetDelete(&delete_ActarSimSilHit);
      instance.SetDeleteArray(&deleteArray_ActarSimSilHit);
      instance.SetDestructor(&destruct_ActarSimSilHit);
      instance.SetStreamerFunc(&streamer_ActarSimSilHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ActarSimSilHit*)
   {
      return GenerateInitInstanceLocal((::ActarSimSilHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ActarSimSilHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ActarSimSilRingHit(void *p = nullptr);
   static void *newArray_ActarSimSilRingHit(Long_t size, void *p);
   static void delete_ActarSimSilRingHit(void *p);
   static void deleteArray_ActarSimSilRingHit(void *p);
   static void destruct_ActarSimSilRingHit(void *p);
   static void streamer_ActarSimSilRingHit(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ActarSimSilRingHit*)
   {
      ::ActarSimSilRingHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ActarSimSilRingHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("ActarSimSilRingHit", ::ActarSimSilRingHit::Class_Version(), "include/ActarSimSilRingHit.hh", 17,
                  typeid(::ActarSimSilRingHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ActarSimSilRingHit::Dictionary, isa_proxy, 16,
                  sizeof(::ActarSimSilRingHit) );
      instance.SetNew(&new_ActarSimSilRingHit);
      instance.SetNewArray(&newArray_ActarSimSilRingHit);
      instance.SetDelete(&delete_ActarSimSilRingHit);
      instance.SetDeleteArray(&deleteArray_ActarSimSilRingHit);
      instance.SetDestructor(&destruct_ActarSimSilRingHit);
      instance.SetStreamerFunc(&streamer_ActarSimSilRingHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ActarSimSilRingHit*)
   {
      return GenerateInitInstanceLocal((::ActarSimSilRingHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ActarSimSilRingHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ActarSimSciHit(void *p = nullptr);
   static void *newArray_ActarSimSciHit(Long_t size, void *p);
   static void delete_ActarSimSciHit(void *p);
   static void deleteArray_ActarSimSciHit(void *p);
   static void destruct_ActarSimSciHit(void *p);
   static void streamer_ActarSimSciHit(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ActarSimSciHit*)
   {
      ::ActarSimSciHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ActarSimSciHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("ActarSimSciHit", ::ActarSimSciHit::Class_Version(), "include/ActarSimSciHit.hh", 17,
                  typeid(::ActarSimSciHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ActarSimSciHit::Dictionary, isa_proxy, 16,
                  sizeof(::ActarSimSciHit) );
      instance.SetNew(&new_ActarSimSciHit);
      instance.SetNewArray(&newArray_ActarSimSciHit);
      instance.SetDelete(&delete_ActarSimSciHit);
      instance.SetDeleteArray(&deleteArray_ActarSimSciHit);
      instance.SetDestructor(&destruct_ActarSimSciHit);
      instance.SetStreamerFunc(&streamer_ActarSimSciHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ActarSimSciHit*)
   {
      return GenerateInitInstanceLocal((::ActarSimSciHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ActarSimSciHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ActarSimSciRingHit(void *p = nullptr);
   static void *newArray_ActarSimSciRingHit(Long_t size, void *p);
   static void delete_ActarSimSciRingHit(void *p);
   static void deleteArray_ActarSimSciRingHit(void *p);
   static void destruct_ActarSimSciRingHit(void *p);
   static void streamer_ActarSimSciRingHit(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ActarSimSciRingHit*)
   {
      ::ActarSimSciRingHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ActarSimSciRingHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("ActarSimSciRingHit", ::ActarSimSciRingHit::Class_Version(), "include/ActarSimSciRingHit.hh", 17,
                  typeid(::ActarSimSciRingHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ActarSimSciRingHit::Dictionary, isa_proxy, 16,
                  sizeof(::ActarSimSciRingHit) );
      instance.SetNew(&new_ActarSimSciRingHit);
      instance.SetNewArray(&newArray_ActarSimSciRingHit);
      instance.SetDelete(&delete_ActarSimSciRingHit);
      instance.SetDeleteArray(&deleteArray_ActarSimSciRingHit);
      instance.SetDestructor(&destruct_ActarSimSciRingHit);
      instance.SetStreamerFunc(&streamer_ActarSimSciRingHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ActarSimSciRingHit*)
   {
      return GenerateInitInstanceLocal((::ActarSimSciRingHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ActarSimSciRingHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ActarSimPlaHit(void *p = nullptr);
   static void *newArray_ActarSimPlaHit(Long_t size, void *p);
   static void delete_ActarSimPlaHit(void *p);
   static void deleteArray_ActarSimPlaHit(void *p);
   static void destruct_ActarSimPlaHit(void *p);
   static void streamer_ActarSimPlaHit(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ActarSimPlaHit*)
   {
      ::ActarSimPlaHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ActarSimPlaHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("ActarSimPlaHit", ::ActarSimPlaHit::Class_Version(), "include/ActarSimPlaHit.hh", 17,
                  typeid(::ActarSimPlaHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ActarSimPlaHit::Dictionary, isa_proxy, 16,
                  sizeof(::ActarSimPlaHit) );
      instance.SetNew(&new_ActarSimPlaHit);
      instance.SetNewArray(&newArray_ActarSimPlaHit);
      instance.SetDelete(&delete_ActarSimPlaHit);
      instance.SetDeleteArray(&deleteArray_ActarSimPlaHit);
      instance.SetDestructor(&destruct_ActarSimPlaHit);
      instance.SetStreamerFunc(&streamer_ActarSimPlaHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ActarSimPlaHit*)
   {
      return GenerateInitInstanceLocal((::ActarSimPlaHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ActarSimPlaHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ActarSimPrimaryInfo(void *p = nullptr);
   static void *newArray_ActarSimPrimaryInfo(Long_t size, void *p);
   static void delete_ActarSimPrimaryInfo(void *p);
   static void deleteArray_ActarSimPrimaryInfo(void *p);
   static void destruct_ActarSimPrimaryInfo(void *p);
   static void streamer_ActarSimPrimaryInfo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ActarSimPrimaryInfo*)
   {
      ::ActarSimPrimaryInfo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ActarSimPrimaryInfo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("ActarSimPrimaryInfo", ::ActarSimPrimaryInfo::Class_Version(), "include/ActarSimPrimaryInfo.hh", 19,
                  typeid(::ActarSimPrimaryInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ActarSimPrimaryInfo::Dictionary, isa_proxy, 16,
                  sizeof(::ActarSimPrimaryInfo) );
      instance.SetNew(&new_ActarSimPrimaryInfo);
      instance.SetNewArray(&newArray_ActarSimPrimaryInfo);
      instance.SetDelete(&delete_ActarSimPrimaryInfo);
      instance.SetDeleteArray(&deleteArray_ActarSimPrimaryInfo);
      instance.SetDestructor(&destruct_ActarSimPrimaryInfo);
      instance.SetStreamerFunc(&streamer_ActarSimPrimaryInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ActarSimPrimaryInfo*)
   {
      return GenerateInitInstanceLocal((::ActarSimPrimaryInfo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ActarSimPrimaryInfo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ActarSimBeamInfo(void *p = nullptr);
   static void *newArray_ActarSimBeamInfo(Long_t size, void *p);
   static void delete_ActarSimBeamInfo(void *p);
   static void deleteArray_ActarSimBeamInfo(void *p);
   static void destruct_ActarSimBeamInfo(void *p);
   static void streamer_ActarSimBeamInfo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ActarSimBeamInfo*)
   {
      ::ActarSimBeamInfo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ActarSimBeamInfo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("ActarSimBeamInfo", ::ActarSimBeamInfo::Class_Version(), "include/ActarSimBeamInfo.hh", 17,
                  typeid(::ActarSimBeamInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ActarSimBeamInfo::Dictionary, isa_proxy, 16,
                  sizeof(::ActarSimBeamInfo) );
      instance.SetNew(&new_ActarSimBeamInfo);
      instance.SetNewArray(&newArray_ActarSimBeamInfo);
      instance.SetDelete(&delete_ActarSimBeamInfo);
      instance.SetDeleteArray(&deleteArray_ActarSimBeamInfo);
      instance.SetDestructor(&destruct_ActarSimBeamInfo);
      instance.SetStreamerFunc(&streamer_ActarSimBeamInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ActarSimBeamInfo*)
   {
      return GenerateInitInstanceLocal((::ActarSimBeamInfo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ActarSimBeamInfo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr ActarSimData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *ActarSimData::Class_Name()
{
   return "ActarSimData";
}

//______________________________________________________________________________
const char *ActarSimData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ActarSimData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int ActarSimData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ActarSimData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ActarSimData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ActarSimData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ActarSimData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ActarSimData*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ActarSimTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *ActarSimTrack::Class_Name()
{
   return "ActarSimTrack";
}

//______________________________________________________________________________
const char *ActarSimTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ActarSimTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int ActarSimTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ActarSimTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ActarSimTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ActarSimTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ActarSimTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ActarSimTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ActarSimSimpleTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *ActarSimSimpleTrack::Class_Name()
{
   return "ActarSimSimpleTrack";
}

//______________________________________________________________________________
const char *ActarSimSimpleTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ActarSimSimpleTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int ActarSimSimpleTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ActarSimSimpleTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ActarSimSimpleTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ActarSimSimpleTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ActarSimSimpleTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ActarSimSimpleTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ActarSimSilHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *ActarSimSilHit::Class_Name()
{
   return "ActarSimSilHit";
}

//______________________________________________________________________________
const char *ActarSimSilHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ActarSimSilHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int ActarSimSilHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ActarSimSilHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ActarSimSilHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ActarSimSilHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ActarSimSilHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ActarSimSilHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ActarSimSilRingHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *ActarSimSilRingHit::Class_Name()
{
   return "ActarSimSilRingHit";
}

//______________________________________________________________________________
const char *ActarSimSilRingHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ActarSimSilRingHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int ActarSimSilRingHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ActarSimSilRingHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ActarSimSilRingHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ActarSimSilRingHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ActarSimSilRingHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ActarSimSilRingHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ActarSimSciHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *ActarSimSciHit::Class_Name()
{
   return "ActarSimSciHit";
}

//______________________________________________________________________________
const char *ActarSimSciHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ActarSimSciHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int ActarSimSciHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ActarSimSciHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ActarSimSciHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ActarSimSciHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ActarSimSciHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ActarSimSciHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ActarSimSciRingHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *ActarSimSciRingHit::Class_Name()
{
   return "ActarSimSciRingHit";
}

//______________________________________________________________________________
const char *ActarSimSciRingHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ActarSimSciRingHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int ActarSimSciRingHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ActarSimSciRingHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ActarSimSciRingHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ActarSimSciRingHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ActarSimSciRingHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ActarSimSciRingHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ActarSimPlaHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *ActarSimPlaHit::Class_Name()
{
   return "ActarSimPlaHit";
}

//______________________________________________________________________________
const char *ActarSimPlaHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ActarSimPlaHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int ActarSimPlaHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ActarSimPlaHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ActarSimPlaHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ActarSimPlaHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ActarSimPlaHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ActarSimPlaHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ActarSimPrimaryInfo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *ActarSimPrimaryInfo::Class_Name()
{
   return "ActarSimPrimaryInfo";
}

//______________________________________________________________________________
const char *ActarSimPrimaryInfo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ActarSimPrimaryInfo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int ActarSimPrimaryInfo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ActarSimPrimaryInfo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ActarSimPrimaryInfo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ActarSimPrimaryInfo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ActarSimPrimaryInfo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ActarSimPrimaryInfo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ActarSimBeamInfo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *ActarSimBeamInfo::Class_Name()
{
   return "ActarSimBeamInfo";
}

//______________________________________________________________________________
const char *ActarSimBeamInfo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ActarSimBeamInfo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int ActarSimBeamInfo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ActarSimBeamInfo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ActarSimBeamInfo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ActarSimBeamInfo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ActarSimBeamInfo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ActarSimBeamInfo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void ActarSimData::Streamer(TBuffer &R__b)
{
   // Stream an object of class ActarSimData.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> thetaPrim1;
      R__b >> thetaPrim2;
      R__b >> phiPrim1;
      R__b >> phiPrim2;
      R__b >> energyPrim1;
      R__b >> energyPrim2;
      R__b >> energyOnGasPrim1;
      R__b >> energyOnGasPrim2;
      R__b >> stepSumLengthOnGasPrim1;
      R__b >> stepSumLengthOnGasPrim2;
      R__b >> eventID;
      R__b >> runID;
      R__b.CheckByteCount(R__s, R__c, ActarSimData::IsA());
   } else {
      R__c = R__b.WriteVersion(ActarSimData::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << thetaPrim1;
      R__b << thetaPrim2;
      R__b << phiPrim1;
      R__b << phiPrim2;
      R__b << energyPrim1;
      R__b << energyPrim2;
      R__b << energyOnGasPrim1;
      R__b << energyOnGasPrim2;
      R__b << stepSumLengthOnGasPrim1;
      R__b << stepSumLengthOnGasPrim2;
      R__b << eventID;
      R__b << runID;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ActarSimData(void *p) {
      return  p ? new(p) ::ActarSimData : new ::ActarSimData;
   }
   static void *newArray_ActarSimData(Long_t nElements, void *p) {
      return p ? new(p) ::ActarSimData[nElements] : new ::ActarSimData[nElements];
   }
   // Wrapper around operator delete
   static void delete_ActarSimData(void *p) {
      delete ((::ActarSimData*)p);
   }
   static void deleteArray_ActarSimData(void *p) {
      delete [] ((::ActarSimData*)p);
   }
   static void destruct_ActarSimData(void *p) {
      typedef ::ActarSimData current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ActarSimData(TBuffer &buf, void *obj) {
      ((::ActarSimData*)obj)->::ActarSimData::Streamer(buf);
   }
} // end of namespace ROOT for class ::ActarSimData

//______________________________________________________________________________
void ActarSimTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class ActarSimTrack.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      R__b >> xCoord;
      R__b >> yCoord;
      R__b >> zCoord;
      R__b >> xPreCoord;
      R__b >> yPreCoord;
      R__b >> zPreCoord;
      R__b >> energyStep;
      R__b >> parentTrackID;
      R__b >> trackID;
      R__b >> eventID;
      R__b >> runID;
      R__b.CheckByteCount(R__s, R__c, ActarSimTrack::IsA());
   } else {
      R__c = R__b.WriteVersion(ActarSimTrack::IsA(), kTRUE);
      R__b << xCoord;
      R__b << yCoord;
      R__b << zCoord;
      R__b << xPreCoord;
      R__b << yPreCoord;
      R__b << zPreCoord;
      R__b << energyStep;
      R__b << parentTrackID;
      R__b << trackID;
      R__b << eventID;
      R__b << runID;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ActarSimTrack(void *p) {
      return  p ? new(p) ::ActarSimTrack : new ::ActarSimTrack;
   }
   static void *newArray_ActarSimTrack(Long_t nElements, void *p) {
      return p ? new(p) ::ActarSimTrack[nElements] : new ::ActarSimTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_ActarSimTrack(void *p) {
      delete ((::ActarSimTrack*)p);
   }
   static void deleteArray_ActarSimTrack(void *p) {
      delete [] ((::ActarSimTrack*)p);
   }
   static void destruct_ActarSimTrack(void *p) {
      typedef ::ActarSimTrack current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ActarSimTrack(TBuffer &buf, void *obj) {
      ((::ActarSimTrack*)obj)->::ActarSimTrack::Streamer(buf);
   }
} // end of namespace ROOT for class ::ActarSimTrack

//______________________________________________________________________________
void ActarSimSimpleTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class ActarSimSimpleTrack.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> xPre;
      R__b >> yPre;
      R__b >> zPre;
      R__b >> xPost;
      R__b >> yPost;
      R__b >> zPost;
      R__b >> energyStride;
      R__b >> particleCharge;
      R__b >> particleMass;
      R__b >> particleID;
      R__b >> strideLength;
      R__b >> particleEnergy;
      R__b >> timePre;
      R__b >> timePost;
      R__b >> numberSteps;
      R__b >> strideOrdinal;
      R__b >> parentTrackID;
      R__b >> trackID;
      R__b >> eventID;
      R__b >> runID;
      R__b.CheckByteCount(R__s, R__c, ActarSimSimpleTrack::IsA());
   } else {
      R__c = R__b.WriteVersion(ActarSimSimpleTrack::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << xPre;
      R__b << yPre;
      R__b << zPre;
      R__b << xPost;
      R__b << yPost;
      R__b << zPost;
      R__b << energyStride;
      R__b << particleCharge;
      R__b << particleMass;
      R__b << particleID;
      R__b << strideLength;
      R__b << particleEnergy;
      R__b << timePre;
      R__b << timePost;
      R__b << numberSteps;
      R__b << strideOrdinal;
      R__b << parentTrackID;
      R__b << trackID;
      R__b << eventID;
      R__b << runID;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ActarSimSimpleTrack(void *p) {
      return  p ? new(p) ::ActarSimSimpleTrack : new ::ActarSimSimpleTrack;
   }
   static void *newArray_ActarSimSimpleTrack(Long_t nElements, void *p) {
      return p ? new(p) ::ActarSimSimpleTrack[nElements] : new ::ActarSimSimpleTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_ActarSimSimpleTrack(void *p) {
      delete ((::ActarSimSimpleTrack*)p);
   }
   static void deleteArray_ActarSimSimpleTrack(void *p) {
      delete [] ((::ActarSimSimpleTrack*)p);
   }
   static void destruct_ActarSimSimpleTrack(void *p) {
      typedef ::ActarSimSimpleTrack current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ActarSimSimpleTrack(TBuffer &buf, void *obj) {
      ((::ActarSimSimpleTrack*)obj)->::ActarSimSimpleTrack::Streamer(buf);
   }
} // end of namespace ROOT for class ::ActarSimSimpleTrack

//______________________________________________________________________________
void ActarSimSilHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class ActarSimSilHit.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> detectorID;
      R__b >> xpos;
      R__b >> ypos;
      R__b >> zpos;
      R__b >> time;
      R__b >> energy;
      R__b >> eBeforeSil;
      R__b >> eAfterSil;
      R__b >> trackID;
      R__b >> eventID;
      R__b >> runID;
      R__b >> particleID;
      R__b >> particleCharge;
      R__b >> particleMass;
      R__b >> stepsContributing;
      R__b.CheckByteCount(R__s, R__c, ActarSimSilHit::IsA());
   } else {
      R__c = R__b.WriteVersion(ActarSimSilHit::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << detectorID;
      R__b << xpos;
      R__b << ypos;
      R__b << zpos;
      R__b << time;
      R__b << energy;
      R__b << eBeforeSil;
      R__b << eAfterSil;
      R__b << trackID;
      R__b << eventID;
      R__b << runID;
      R__b << particleID;
      R__b << particleCharge;
      R__b << particleMass;
      R__b << stepsContributing;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ActarSimSilHit(void *p) {
      return  p ? new(p) ::ActarSimSilHit : new ::ActarSimSilHit;
   }
   static void *newArray_ActarSimSilHit(Long_t nElements, void *p) {
      return p ? new(p) ::ActarSimSilHit[nElements] : new ::ActarSimSilHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_ActarSimSilHit(void *p) {
      delete ((::ActarSimSilHit*)p);
   }
   static void deleteArray_ActarSimSilHit(void *p) {
      delete [] ((::ActarSimSilHit*)p);
   }
   static void destruct_ActarSimSilHit(void *p) {
      typedef ::ActarSimSilHit current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ActarSimSilHit(TBuffer &buf, void *obj) {
      ((::ActarSimSilHit*)obj)->::ActarSimSilHit::Streamer(buf);
   }
} // end of namespace ROOT for class ::ActarSimSilHit

//______________________________________________________________________________
void ActarSimSilRingHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class ActarSimSilRingHit.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> detectorID;
      R__b >> xpos;
      R__b >> ypos;
      R__b >> zpos;
      R__b >> time;
      R__b >> energy;
      R__b >> eBeforeSil;
      R__b >> eAfterSil;
      R__b >> trackID;
      R__b >> eventID;
      R__b >> runID;
      R__b >> particleID;
      R__b >> particleCharge;
      R__b >> particleMass;
      R__b >> stepsContributing;
      R__b.CheckByteCount(R__s, R__c, ActarSimSilRingHit::IsA());
   } else {
      R__c = R__b.WriteVersion(ActarSimSilRingHit::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << detectorID;
      R__b << xpos;
      R__b << ypos;
      R__b << zpos;
      R__b << time;
      R__b << energy;
      R__b << eBeforeSil;
      R__b << eAfterSil;
      R__b << trackID;
      R__b << eventID;
      R__b << runID;
      R__b << particleID;
      R__b << particleCharge;
      R__b << particleMass;
      R__b << stepsContributing;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ActarSimSilRingHit(void *p) {
      return  p ? new(p) ::ActarSimSilRingHit : new ::ActarSimSilRingHit;
   }
   static void *newArray_ActarSimSilRingHit(Long_t nElements, void *p) {
      return p ? new(p) ::ActarSimSilRingHit[nElements] : new ::ActarSimSilRingHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_ActarSimSilRingHit(void *p) {
      delete ((::ActarSimSilRingHit*)p);
   }
   static void deleteArray_ActarSimSilRingHit(void *p) {
      delete [] ((::ActarSimSilRingHit*)p);
   }
   static void destruct_ActarSimSilRingHit(void *p) {
      typedef ::ActarSimSilRingHit current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ActarSimSilRingHit(TBuffer &buf, void *obj) {
      ((::ActarSimSilRingHit*)obj)->::ActarSimSilRingHit::Streamer(buf);
   }
} // end of namespace ROOT for class ::ActarSimSilRingHit

//______________________________________________________________________________
void ActarSimSciHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class ActarSimSciHit.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> type;
      R__b >> copy;
      R__b >> time;
      R__b >> energy;
      R__b >> eventID;
      R__b >> runID;
      R__b >> particleID;
      R__b >> particleCharge;
      R__b >> particleMass;
      R__b.CheckByteCount(R__s, R__c, ActarSimSciHit::IsA());
   } else {
      R__c = R__b.WriteVersion(ActarSimSciHit::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << type;
      R__b << copy;
      R__b << time;
      R__b << energy;
      R__b << eventID;
      R__b << runID;
      R__b << particleID;
      R__b << particleCharge;
      R__b << particleMass;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ActarSimSciHit(void *p) {
      return  p ? new(p) ::ActarSimSciHit : new ::ActarSimSciHit;
   }
   static void *newArray_ActarSimSciHit(Long_t nElements, void *p) {
      return p ? new(p) ::ActarSimSciHit[nElements] : new ::ActarSimSciHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_ActarSimSciHit(void *p) {
      delete ((::ActarSimSciHit*)p);
   }
   static void deleteArray_ActarSimSciHit(void *p) {
      delete [] ((::ActarSimSciHit*)p);
   }
   static void destruct_ActarSimSciHit(void *p) {
      typedef ::ActarSimSciHit current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ActarSimSciHit(TBuffer &buf, void *obj) {
      ((::ActarSimSciHit*)obj)->::ActarSimSciHit::Streamer(buf);
   }
} // end of namespace ROOT for class ::ActarSimSciHit

//______________________________________________________________________________
void ActarSimSciRingHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class ActarSimSciRingHit.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> type;
      R__b >> copy;
      R__b >> time;
      R__b >> energy;
      R__b >> eventID;
      R__b >> runID;
      R__b >> trackID;
      R__b >> particleID;
      R__b >> particleCharge;
      R__b >> particleMass;
      R__b.CheckByteCount(R__s, R__c, ActarSimSciRingHit::IsA());
   } else {
      R__c = R__b.WriteVersion(ActarSimSciRingHit::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << type;
      R__b << copy;
      R__b << time;
      R__b << energy;
      R__b << eventID;
      R__b << runID;
      R__b << trackID;
      R__b << particleID;
      R__b << particleCharge;
      R__b << particleMass;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ActarSimSciRingHit(void *p) {
      return  p ? new(p) ::ActarSimSciRingHit : new ::ActarSimSciRingHit;
   }
   static void *newArray_ActarSimSciRingHit(Long_t nElements, void *p) {
      return p ? new(p) ::ActarSimSciRingHit[nElements] : new ::ActarSimSciRingHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_ActarSimSciRingHit(void *p) {
      delete ((::ActarSimSciRingHit*)p);
   }
   static void deleteArray_ActarSimSciRingHit(void *p) {
      delete [] ((::ActarSimSciRingHit*)p);
   }
   static void destruct_ActarSimSciRingHit(void *p) {
      typedef ::ActarSimSciRingHit current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ActarSimSciRingHit(TBuffer &buf, void *obj) {
      ((::ActarSimSciRingHit*)obj)->::ActarSimSciRingHit::Streamer(buf);
   }
} // end of namespace ROOT for class ::ActarSimSciRingHit

//______________________________________________________________________________
void ActarSimPlaHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class ActarSimPlaHit.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> detectorID;
      R__b >> xpos;
      R__b >> ypos;
      R__b >> zpos;
      R__b >> time;
      R__b >> energy;
      R__b >> eBeforePla;
      R__b >> eAfterPla;
      R__b >> eventID;
      R__b >> runID;
      R__b >> trackID;
      R__b >> particleID;
      R__b >> particleCharge;
      R__b >> particleMass;
      R__b >> stepsContributing;
      R__b.CheckByteCount(R__s, R__c, ActarSimPlaHit::IsA());
   } else {
      R__c = R__b.WriteVersion(ActarSimPlaHit::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << detectorID;
      R__b << xpos;
      R__b << ypos;
      R__b << zpos;
      R__b << time;
      R__b << energy;
      R__b << eBeforePla;
      R__b << eAfterPla;
      R__b << eventID;
      R__b << runID;
      R__b << trackID;
      R__b << particleID;
      R__b << particleCharge;
      R__b << particleMass;
      R__b << stepsContributing;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ActarSimPlaHit(void *p) {
      return  p ? new(p) ::ActarSimPlaHit : new ::ActarSimPlaHit;
   }
   static void *newArray_ActarSimPlaHit(Long_t nElements, void *p) {
      return p ? new(p) ::ActarSimPlaHit[nElements] : new ::ActarSimPlaHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_ActarSimPlaHit(void *p) {
      delete ((::ActarSimPlaHit*)p);
   }
   static void deleteArray_ActarSimPlaHit(void *p) {
      delete [] ((::ActarSimPlaHit*)p);
   }
   static void destruct_ActarSimPlaHit(void *p) {
      typedef ::ActarSimPlaHit current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ActarSimPlaHit(TBuffer &buf, void *obj) {
      ((::ActarSimPlaHit*)obj)->::ActarSimPlaHit::Streamer(buf);
   }
} // end of namespace ROOT for class ::ActarSimPlaHit

//______________________________________________________________________________
void ActarSimPrimaryInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class ActarSimPrimaryInfo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> nbPrimariesInEvent;
      R__b >> kineticEnergy;
      R__b >> theta;
      R__b >> phi;
      R__b >> PDGcode;
      R__b >> Px;
      R__b >> Py;
      R__b >> Pz;
      R__b >> trackID;
      R__b >> mass;
      R__b >> charge;
      R__b >> polX;
      R__b >> polY;
      R__b >> polZ;
      R__b >> Weight0;
      R__b >> properTime;
      R__b >> x0;
      R__b >> y0;
      R__b >> z0;
      R__b >> eventID;
      R__b >> runID;
      R__b.CheckByteCount(R__s, R__c, ActarSimPrimaryInfo::IsA());
   } else {
      R__c = R__b.WriteVersion(ActarSimPrimaryInfo::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << nbPrimariesInEvent;
      R__b << kineticEnergy;
      R__b << theta;
      R__b << phi;
      R__b << PDGcode;
      R__b << Px;
      R__b << Py;
      R__b << Pz;
      R__b << trackID;
      R__b << mass;
      R__b << charge;
      R__b << polX;
      R__b << polY;
      R__b << polZ;
      R__b << Weight0;
      R__b << properTime;
      R__b << x0;
      R__b << y0;
      R__b << z0;
      R__b << eventID;
      R__b << runID;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ActarSimPrimaryInfo(void *p) {
      return  p ? new(p) ::ActarSimPrimaryInfo : new ::ActarSimPrimaryInfo;
   }
   static void *newArray_ActarSimPrimaryInfo(Long_t nElements, void *p) {
      return p ? new(p) ::ActarSimPrimaryInfo[nElements] : new ::ActarSimPrimaryInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_ActarSimPrimaryInfo(void *p) {
      delete ((::ActarSimPrimaryInfo*)p);
   }
   static void deleteArray_ActarSimPrimaryInfo(void *p) {
      delete [] ((::ActarSimPrimaryInfo*)p);
   }
   static void destruct_ActarSimPrimaryInfo(void *p) {
      typedef ::ActarSimPrimaryInfo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ActarSimPrimaryInfo(TBuffer &buf, void *obj) {
      ((::ActarSimPrimaryInfo*)obj)->::ActarSimPrimaryInfo::Streamer(buf);
   }
} // end of namespace ROOT for class ::ActarSimPrimaryInfo

//______________________________________________________________________________
void ActarSimBeamInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class ActarSimBeamInfo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> energyEntrance;
      R__b >> energyVertex;
      R__b >> thetaEntrance;
      R__b >> phiEntrance;
      R__b >> thetaVertex;
      R__b >> phiVertex;
      R__b >> xEntrance;
      R__b >> yEntrance;
      R__b >> zEntrance;
      R__b >> xVertex;
      R__b >> yVertex;
      R__b >> zVertex;
      R__b >> nextZVertex;
      R__b >> timeVertex;
      R__b >> mass;
      R__b >> charge;
      R__b >> eventID;
      R__b >> runID;
      R__b.CheckByteCount(R__s, R__c, ActarSimBeamInfo::IsA());
   } else {
      R__c = R__b.WriteVersion(ActarSimBeamInfo::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << energyEntrance;
      R__b << energyVertex;
      R__b << thetaEntrance;
      R__b << phiEntrance;
      R__b << thetaVertex;
      R__b << phiVertex;
      R__b << xEntrance;
      R__b << yEntrance;
      R__b << zEntrance;
      R__b << xVertex;
      R__b << yVertex;
      R__b << zVertex;
      R__b << nextZVertex;
      R__b << timeVertex;
      R__b << mass;
      R__b << charge;
      R__b << eventID;
      R__b << runID;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ActarSimBeamInfo(void *p) {
      return  p ? new(p) ::ActarSimBeamInfo : new ::ActarSimBeamInfo;
   }
   static void *newArray_ActarSimBeamInfo(Long_t nElements, void *p) {
      return p ? new(p) ::ActarSimBeamInfo[nElements] : new ::ActarSimBeamInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_ActarSimBeamInfo(void *p) {
      delete ((::ActarSimBeamInfo*)p);
   }
   static void deleteArray_ActarSimBeamInfo(void *p) {
      delete [] ((::ActarSimBeamInfo*)p);
   }
   static void destruct_ActarSimBeamInfo(void *p) {
      typedef ::ActarSimBeamInfo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ActarSimBeamInfo(TBuffer &buf, void *obj) {
      ((::ActarSimBeamInfo*)obj)->::ActarSimBeamInfo::Streamer(buf);
   }
} // end of namespace ROOT for class ::ActarSimBeamInfo

namespace {
  void TriggerDictionaryInitialization_ActarSimDict_Impl() {
    static const char* headers[] = {
"/home/ats/Actarsim/include/ActarSimData.hh",
"/home/ats/Actarsim/include/ActarSimTrack.hh",
"/home/ats/Actarsim/include/ActarSimSimpleTrack.hh",
"/home/ats/Actarsim/include/ActarSimSilHit.hh",
"/home/ats/Actarsim/include/ActarSimSilRingHit.hh",
"/home/ats/Actarsim/include/ActarSimSciHit.hh",
"/home/ats/Actarsim/include/ActarSimSciRingHit.hh",
"/home/ats/Actarsim/include/ActarSimPlaHit.hh",
"/home/ats/Actarsim/include/ActarSimPrimaryInfo.hh",
"/home/ats/Actarsim/include/ActarSimBeamInfo.hh",
nullptr
    };
    static const char* includePaths[] = {
"/usr/local/root-6.26.04_build/include",
"/usr/local/root-6.26.04_build/include/",
"/home/ats/Actarsim_build/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "ActarSimDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate(R"ATTRDUMP(ROOT CINT)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$/home/ats/Actarsim/include/ActarSimData.hh")))  ActarSimData;
class __attribute__((annotate(R"ATTRDUMP(ROOT CINT)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$/home/ats/Actarsim/include/ActarSimTrack.hh")))  ActarSimTrack;
class __attribute__((annotate(R"ATTRDUMP(ROOT CINT)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$/home/ats/Actarsim/include/ActarSimSimpleTrack.hh")))  ActarSimSimpleTrack;
class __attribute__((annotate(R"ATTRDUMP(ROOT CINT)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$/home/ats/Actarsim/include/ActarSimSilHit.hh")))  ActarSimSilHit;
class __attribute__((annotate(R"ATTRDUMP(ROOT CINT)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$/home/ats/Actarsim/include/ActarSimSilRingHit.hh")))  ActarSimSilRingHit;
class __attribute__((annotate(R"ATTRDUMP(ROOT CINT)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$/home/ats/Actarsim/include/ActarSimSciHit.hh")))  ActarSimSciHit;
class __attribute__((annotate(R"ATTRDUMP(ROOT CINT)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$/home/ats/Actarsim/include/ActarSimSciRingHit.hh")))  ActarSimSciRingHit;
class __attribute__((annotate(R"ATTRDUMP(ROOT CINT)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$/home/ats/Actarsim/include/ActarSimPlaHit.hh")))  ActarSimPlaHit;
class __attribute__((annotate(R"ATTRDUMP(ROOT CINT)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$/home/ats/Actarsim/include/ActarSimPrimaryInfo.hh")))  ActarSimPrimaryInfo;
class __attribute__((annotate(R"ATTRDUMP(ROOT CINT)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$/home/ats/Actarsim/include/ActarSimBeamInfo.hh")))  ActarSimBeamInfo;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "ActarSimDict dictionary payload"

#ifndef HAVE_CONFIG_H
  #define HAVE_CONFIG_H 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/home/ats/Actarsim/include/ActarSimData.hh"
#include "/home/ats/Actarsim/include/ActarSimTrack.hh"
#include "/home/ats/Actarsim/include/ActarSimSimpleTrack.hh"
#include "/home/ats/Actarsim/include/ActarSimSilHit.hh"
#include "/home/ats/Actarsim/include/ActarSimSilRingHit.hh"
#include "/home/ats/Actarsim/include/ActarSimSciHit.hh"
#include "/home/ats/Actarsim/include/ActarSimSciRingHit.hh"
#include "/home/ats/Actarsim/include/ActarSimPlaHit.hh"
#include "/home/ats/Actarsim/include/ActarSimPrimaryInfo.hh"
#include "/home/ats/Actarsim/include/ActarSimBeamInfo.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"ActarSimBeamInfo", payloadCode, "@",
"ActarSimData", payloadCode, "@",
"ActarSimPlaHit", payloadCode, "@",
"ActarSimPrimaryInfo", payloadCode, "@",
"ActarSimSciHit", payloadCode, "@",
"ActarSimSciRingHit", payloadCode, "@",
"ActarSimSilHit", payloadCode, "@",
"ActarSimSilRingHit", payloadCode, "@",
"ActarSimSimpleTrack", payloadCode, "@",
"ActarSimTrack", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("ActarSimDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_ActarSimDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_ActarSimDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_ActarSimDict() {
  TriggerDictionaryInitialization_ActarSimDict_Impl();
}
