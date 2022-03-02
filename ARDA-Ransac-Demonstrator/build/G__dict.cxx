// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__dict
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
#include "cHit.h"
#include "cRawEvent.h"
#include "cLookupTable.h"
#include "cRawSignal.h"
#include "cPhysicalHit.h"
#include "cPhysicalEvent.h"
#include "cFittedLine.h"
#include "cFittedEvent.h"
#include "cVertex.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_cHit(void *p = 0);
   static void *newArray_cHit(Long_t size, void *p);
   static void delete_cHit(void *p);
   static void deleteArray_cHit(void *p);
   static void destruct_cHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cHit*)
   {
      ::cHit *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::cHit >(0);
      static ::ROOT::TGenericClassInfo 
         instance("cHit", ::cHit::Class_Version(), "cHit.h", 11,
                  typeid(::cHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::cHit::Dictionary, isa_proxy, 4,
                  sizeof(::cHit) );
      instance.SetNew(&new_cHit);
      instance.SetNewArray(&newArray_cHit);
      instance.SetDelete(&delete_cHit);
      instance.SetDeleteArray(&deleteArray_cHit);
      instance.SetDestructor(&destruct_cHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cHit*)
   {
      return GenerateInitInstanceLocal((::cHit*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::cHit*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_cRawSignal(void *p = 0);
   static void *newArray_cRawSignal(Long_t size, void *p);
   static void delete_cRawSignal(void *p);
   static void deleteArray_cRawSignal(void *p);
   static void destruct_cRawSignal(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cRawSignal*)
   {
      ::cRawSignal *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::cRawSignal >(0);
      static ::ROOT::TGenericClassInfo 
         instance("cRawSignal", ::cRawSignal::Class_Version(), "cRawSignal.h", 14,
                  typeid(::cRawSignal), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::cRawSignal::Dictionary, isa_proxy, 4,
                  sizeof(::cRawSignal) );
      instance.SetNew(&new_cRawSignal);
      instance.SetNewArray(&newArray_cRawSignal);
      instance.SetDelete(&delete_cRawSignal);
      instance.SetDeleteArray(&deleteArray_cRawSignal);
      instance.SetDestructor(&destruct_cRawSignal);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cRawSignal*)
   {
      return GenerateInitInstanceLocal((::cRawSignal*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::cRawSignal*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_cRawEvent(void *p = 0);
   static void *newArray_cRawEvent(Long_t size, void *p);
   static void delete_cRawEvent(void *p);
   static void deleteArray_cRawEvent(void *p);
   static void destruct_cRawEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cRawEvent*)
   {
      ::cRawEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::cRawEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("cRawEvent", ::cRawEvent::Class_Version(), "cRawEvent.h", 13,
                  typeid(::cRawEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::cRawEvent::Dictionary, isa_proxy, 4,
                  sizeof(::cRawEvent) );
      instance.SetNew(&new_cRawEvent);
      instance.SetNewArray(&newArray_cRawEvent);
      instance.SetDelete(&delete_cRawEvent);
      instance.SetDeleteArray(&deleteArray_cRawEvent);
      instance.SetDestructor(&destruct_cRawEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cRawEvent*)
   {
      return GenerateInitInstanceLocal((::cRawEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::cRawEvent*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_cLookupTable(void *p = 0);
   static void *newArray_cLookupTable(Long_t size, void *p);
   static void delete_cLookupTable(void *p);
   static void deleteArray_cLookupTable(void *p);
   static void destruct_cLookupTable(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cLookupTable*)
   {
      ::cLookupTable *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::cLookupTable >(0);
      static ::ROOT::TGenericClassInfo 
         instance("cLookupTable", ::cLookupTable::Class_Version(), "cLookupTable.h", 13,
                  typeid(::cLookupTable), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::cLookupTable::Dictionary, isa_proxy, 4,
                  sizeof(::cLookupTable) );
      instance.SetNew(&new_cLookupTable);
      instance.SetNewArray(&newArray_cLookupTable);
      instance.SetDelete(&delete_cLookupTable);
      instance.SetDeleteArray(&deleteArray_cLookupTable);
      instance.SetDestructor(&destruct_cLookupTable);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cLookupTable*)
   {
      return GenerateInitInstanceLocal((::cLookupTable*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::cLookupTable*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_cPhysicalHit(void *p = 0);
   static void *newArray_cPhysicalHit(Long_t size, void *p);
   static void delete_cPhysicalHit(void *p);
   static void deleteArray_cPhysicalHit(void *p);
   static void destruct_cPhysicalHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cPhysicalHit*)
   {
      ::cPhysicalHit *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::cPhysicalHit >(0);
      static ::ROOT::TGenericClassInfo 
         instance("cPhysicalHit", ::cPhysicalHit::Class_Version(), "cPhysicalHit.h", 6,
                  typeid(::cPhysicalHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::cPhysicalHit::Dictionary, isa_proxy, 4,
                  sizeof(::cPhysicalHit) );
      instance.SetNew(&new_cPhysicalHit);
      instance.SetNewArray(&newArray_cPhysicalHit);
      instance.SetDelete(&delete_cPhysicalHit);
      instance.SetDeleteArray(&deleteArray_cPhysicalHit);
      instance.SetDestructor(&destruct_cPhysicalHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cPhysicalHit*)
   {
      return GenerateInitInstanceLocal((::cPhysicalHit*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::cPhysicalHit*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_cPhysicalEvent(void *p = 0);
   static void *newArray_cPhysicalEvent(Long_t size, void *p);
   static void delete_cPhysicalEvent(void *p);
   static void deleteArray_cPhysicalEvent(void *p);
   static void destruct_cPhysicalEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cPhysicalEvent*)
   {
      ::cPhysicalEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::cPhysicalEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("cPhysicalEvent", ::cPhysicalEvent::Class_Version(), "cPhysicalEvent.h", 8,
                  typeid(::cPhysicalEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::cPhysicalEvent::Dictionary, isa_proxy, 4,
                  sizeof(::cPhysicalEvent) );
      instance.SetNew(&new_cPhysicalEvent);
      instance.SetNewArray(&newArray_cPhysicalEvent);
      instance.SetDelete(&delete_cPhysicalEvent);
      instance.SetDeleteArray(&deleteArray_cPhysicalEvent);
      instance.SetDestructor(&destruct_cPhysicalEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cPhysicalEvent*)
   {
      return GenerateInitInstanceLocal((::cPhysicalEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::cPhysicalEvent*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *cFittedLinelEcPhysicalHitgR_Dictionary();
   static void cFittedLinelEcPhysicalHitgR_TClassManip(TClass*);
   static void *new_cFittedLinelEcPhysicalHitgR(void *p = 0);
   static void *newArray_cFittedLinelEcPhysicalHitgR(Long_t size, void *p);
   static void delete_cFittedLinelEcPhysicalHitgR(void *p);
   static void deleteArray_cFittedLinelEcPhysicalHitgR(void *p);
   static void destruct_cFittedLinelEcPhysicalHitgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cFittedLine<cPhysicalHit>*)
   {
      ::cFittedLine<cPhysicalHit> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::cFittedLine<cPhysicalHit> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("cFittedLine<cPhysicalHit>", ::cFittedLine<cPhysicalHit>::Class_Version(), "cFittedLine.h", 14,
                  typeid(::cFittedLine<cPhysicalHit>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &cFittedLinelEcPhysicalHitgR_Dictionary, isa_proxy, 4,
                  sizeof(::cFittedLine<cPhysicalHit>) );
      instance.SetNew(&new_cFittedLinelEcPhysicalHitgR);
      instance.SetNewArray(&newArray_cFittedLinelEcPhysicalHitgR);
      instance.SetDelete(&delete_cFittedLinelEcPhysicalHitgR);
      instance.SetDeleteArray(&deleteArray_cFittedLinelEcPhysicalHitgR);
      instance.SetDestructor(&destruct_cFittedLinelEcPhysicalHitgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cFittedLine<cPhysicalHit>*)
   {
      return GenerateInitInstanceLocal((::cFittedLine<cPhysicalHit>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::cFittedLine<cPhysicalHit>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *cFittedLinelEcPhysicalHitgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::cFittedLine<cPhysicalHit>*)0x0)->GetClass();
      cFittedLinelEcPhysicalHitgR_TClassManip(theClass);
   return theClass;
   }

   static void cFittedLinelEcPhysicalHitgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *cFittedLinelEintgR_Dictionary();
   static void cFittedLinelEintgR_TClassManip(TClass*);
   static void *new_cFittedLinelEintgR(void *p = 0);
   static void *newArray_cFittedLinelEintgR(Long_t size, void *p);
   static void delete_cFittedLinelEintgR(void *p);
   static void deleteArray_cFittedLinelEintgR(void *p);
   static void destruct_cFittedLinelEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cFittedLine<int>*)
   {
      ::cFittedLine<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::cFittedLine<int> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("cFittedLine<int>", ::cFittedLine<int>::Class_Version(), "cFittedLine.h", 14,
                  typeid(::cFittedLine<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &cFittedLinelEintgR_Dictionary, isa_proxy, 4,
                  sizeof(::cFittedLine<int>) );
      instance.SetNew(&new_cFittedLinelEintgR);
      instance.SetNewArray(&newArray_cFittedLinelEintgR);
      instance.SetDelete(&delete_cFittedLinelEintgR);
      instance.SetDeleteArray(&deleteArray_cFittedLinelEintgR);
      instance.SetDestructor(&destruct_cFittedLinelEintgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cFittedLine<int>*)
   {
      return GenerateInitInstanceLocal((::cFittedLine<int>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::cFittedLine<int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *cFittedLinelEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::cFittedLine<int>*)0x0)->GetClass();
      cFittedLinelEintgR_TClassManip(theClass);
   return theClass;
   }

   static void cFittedLinelEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *cVertexlEcPhysicalHitgR_Dictionary();
   static void cVertexlEcPhysicalHitgR_TClassManip(TClass*);
   static void *new_cVertexlEcPhysicalHitgR(void *p = 0);
   static void *newArray_cVertexlEcPhysicalHitgR(Long_t size, void *p);
   static void delete_cVertexlEcPhysicalHitgR(void *p);
   static void deleteArray_cVertexlEcPhysicalHitgR(void *p);
   static void destruct_cVertexlEcPhysicalHitgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cVertex<cPhysicalHit>*)
   {
      ::cVertex<cPhysicalHit> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::cVertex<cPhysicalHit> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("cVertex<cPhysicalHit>", ::cVertex<cPhysicalHit>::Class_Version(), "cVertex.h", 10,
                  typeid(::cVertex<cPhysicalHit>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &cVertexlEcPhysicalHitgR_Dictionary, isa_proxy, 4,
                  sizeof(::cVertex<cPhysicalHit>) );
      instance.SetNew(&new_cVertexlEcPhysicalHitgR);
      instance.SetNewArray(&newArray_cVertexlEcPhysicalHitgR);
      instance.SetDelete(&delete_cVertexlEcPhysicalHitgR);
      instance.SetDeleteArray(&deleteArray_cVertexlEcPhysicalHitgR);
      instance.SetDestructor(&destruct_cVertexlEcPhysicalHitgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cVertex<cPhysicalHit>*)
   {
      return GenerateInitInstanceLocal((::cVertex<cPhysicalHit>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::cVertex<cPhysicalHit>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *cVertexlEcPhysicalHitgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::cVertex<cPhysicalHit>*)0x0)->GetClass();
      cVertexlEcPhysicalHitgR_TClassManip(theClass);
   return theClass;
   }

   static void cVertexlEcPhysicalHitgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *cVertexlEintgR_Dictionary();
   static void cVertexlEintgR_TClassManip(TClass*);
   static void *new_cVertexlEintgR(void *p = 0);
   static void *newArray_cVertexlEintgR(Long_t size, void *p);
   static void delete_cVertexlEintgR(void *p);
   static void deleteArray_cVertexlEintgR(void *p);
   static void destruct_cVertexlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cVertex<int>*)
   {
      ::cVertex<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::cVertex<int> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("cVertex<int>", ::cVertex<int>::Class_Version(), "cVertex.h", 10,
                  typeid(::cVertex<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &cVertexlEintgR_Dictionary, isa_proxy, 4,
                  sizeof(::cVertex<int>) );
      instance.SetNew(&new_cVertexlEintgR);
      instance.SetNewArray(&newArray_cVertexlEintgR);
      instance.SetDelete(&delete_cVertexlEintgR);
      instance.SetDeleteArray(&deleteArray_cVertexlEintgR);
      instance.SetDestructor(&destruct_cVertexlEintgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cVertex<int>*)
   {
      return GenerateInitInstanceLocal((::cVertex<int>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::cVertex<int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *cVertexlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::cVertex<int>*)0x0)->GetClass();
      cVertexlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void cVertexlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *cFittedEventlEcPhysicalHitgR_Dictionary();
   static void cFittedEventlEcPhysicalHitgR_TClassManip(TClass*);
   static void *new_cFittedEventlEcPhysicalHitgR(void *p = 0);
   static void *newArray_cFittedEventlEcPhysicalHitgR(Long_t size, void *p);
   static void delete_cFittedEventlEcPhysicalHitgR(void *p);
   static void deleteArray_cFittedEventlEcPhysicalHitgR(void *p);
   static void destruct_cFittedEventlEcPhysicalHitgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cFittedEvent<cPhysicalHit>*)
   {
      ::cFittedEvent<cPhysicalHit> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::cFittedEvent<cPhysicalHit> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("cFittedEvent<cPhysicalHit>", ::cFittedEvent<cPhysicalHit>::Class_Version(), "cFittedEvent.h", 11,
                  typeid(::cFittedEvent<cPhysicalHit>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &cFittedEventlEcPhysicalHitgR_Dictionary, isa_proxy, 4,
                  sizeof(::cFittedEvent<cPhysicalHit>) );
      instance.SetNew(&new_cFittedEventlEcPhysicalHitgR);
      instance.SetNewArray(&newArray_cFittedEventlEcPhysicalHitgR);
      instance.SetDelete(&delete_cFittedEventlEcPhysicalHitgR);
      instance.SetDeleteArray(&deleteArray_cFittedEventlEcPhysicalHitgR);
      instance.SetDestructor(&destruct_cFittedEventlEcPhysicalHitgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cFittedEvent<cPhysicalHit>*)
   {
      return GenerateInitInstanceLocal((::cFittedEvent<cPhysicalHit>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::cFittedEvent<cPhysicalHit>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *cFittedEventlEcPhysicalHitgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::cFittedEvent<cPhysicalHit>*)0x0)->GetClass();
      cFittedEventlEcPhysicalHitgR_TClassManip(theClass);
   return theClass;
   }

   static void cFittedEventlEcPhysicalHitgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *cFittedEventlEintgR_Dictionary();
   static void cFittedEventlEintgR_TClassManip(TClass*);
   static void *new_cFittedEventlEintgR(void *p = 0);
   static void *newArray_cFittedEventlEintgR(Long_t size, void *p);
   static void delete_cFittedEventlEintgR(void *p);
   static void deleteArray_cFittedEventlEintgR(void *p);
   static void destruct_cFittedEventlEintgR(void *p);
   static void streamer_cFittedEventlEintgR(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cFittedEvent<int>*)
   {
      ::cFittedEvent<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::cFittedEvent<int> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("cFittedEvent<int>", ::cFittedEvent<int>::Class_Version(), "cFittedEvent.h", 11,
                  typeid(::cFittedEvent<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &cFittedEventlEintgR_Dictionary, isa_proxy, 16,
                  sizeof(::cFittedEvent<int>) );
      instance.SetNew(&new_cFittedEventlEintgR);
      instance.SetNewArray(&newArray_cFittedEventlEintgR);
      instance.SetDelete(&delete_cFittedEventlEintgR);
      instance.SetDeleteArray(&deleteArray_cFittedEventlEintgR);
      instance.SetDestructor(&destruct_cFittedEventlEintgR);
      instance.SetStreamerFunc(&streamer_cFittedEventlEintgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cFittedEvent<int>*)
   {
      return GenerateInitInstanceLocal((::cFittedEvent<int>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::cFittedEvent<int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *cFittedEventlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::cFittedEvent<int>*)0x0)->GetClass();
      cFittedEventlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void cFittedEventlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr cHit::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *cHit::Class_Name()
{
   return "cHit";
}

//______________________________________________________________________________
const char *cHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cHit*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int cHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cHit*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *cHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cHit*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *cHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cHit*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr cRawSignal::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *cRawSignal::Class_Name()
{
   return "cRawSignal";
}

//______________________________________________________________________________
const char *cRawSignal::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cRawSignal*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int cRawSignal::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cRawSignal*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *cRawSignal::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cRawSignal*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *cRawSignal::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cRawSignal*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr cRawEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *cRawEvent::Class_Name()
{
   return "cRawEvent";
}

//______________________________________________________________________________
const char *cRawEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cRawEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int cRawEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cRawEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *cRawEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cRawEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *cRawEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cRawEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr cLookupTable::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *cLookupTable::Class_Name()
{
   return "cLookupTable";
}

//______________________________________________________________________________
const char *cLookupTable::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cLookupTable*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int cLookupTable::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cLookupTable*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *cLookupTable::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cLookupTable*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *cLookupTable::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cLookupTable*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr cPhysicalHit::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *cPhysicalHit::Class_Name()
{
   return "cPhysicalHit";
}

//______________________________________________________________________________
const char *cPhysicalHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cPhysicalHit*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int cPhysicalHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cPhysicalHit*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *cPhysicalHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cPhysicalHit*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *cPhysicalHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cPhysicalHit*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr cPhysicalEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *cPhysicalEvent::Class_Name()
{
   return "cPhysicalEvent";
}

//______________________________________________________________________________
const char *cPhysicalEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cPhysicalEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int cPhysicalEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cPhysicalEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *cPhysicalEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cPhysicalEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *cPhysicalEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cPhysicalEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr cFittedLine<cPhysicalHit>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *cFittedLine<cPhysicalHit>::Class_Name()
{
   return "cFittedLine<cPhysicalHit>";
}

//______________________________________________________________________________
template <> const char *cFittedLine<cPhysicalHit>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cFittedLine<cPhysicalHit>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int cFittedLine<cPhysicalHit>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cFittedLine<cPhysicalHit>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *cFittedLine<cPhysicalHit>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cFittedLine<cPhysicalHit>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *cFittedLine<cPhysicalHit>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cFittedLine<cPhysicalHit>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr cFittedLine<int>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *cFittedLine<int>::Class_Name()
{
   return "cFittedLine<int>";
}

//______________________________________________________________________________
template <> const char *cFittedLine<int>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cFittedLine<int>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int cFittedLine<int>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cFittedLine<int>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *cFittedLine<int>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cFittedLine<int>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *cFittedLine<int>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cFittedLine<int>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr cVertex<cPhysicalHit>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *cVertex<cPhysicalHit>::Class_Name()
{
   return "cVertex<cPhysicalHit>";
}

//______________________________________________________________________________
template <> const char *cVertex<cPhysicalHit>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cVertex<cPhysicalHit>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int cVertex<cPhysicalHit>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cVertex<cPhysicalHit>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *cVertex<cPhysicalHit>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cVertex<cPhysicalHit>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *cVertex<cPhysicalHit>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cVertex<cPhysicalHit>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr cVertex<int>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *cVertex<int>::Class_Name()
{
   return "cVertex<int>";
}

//______________________________________________________________________________
template <> const char *cVertex<int>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cVertex<int>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int cVertex<int>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cVertex<int>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *cVertex<int>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cVertex<int>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *cVertex<int>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cVertex<int>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr cFittedEvent<cPhysicalHit>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *cFittedEvent<cPhysicalHit>::Class_Name()
{
   return "cFittedEvent<cPhysicalHit>";
}

//______________________________________________________________________________
template <> const char *cFittedEvent<cPhysicalHit>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cFittedEvent<cPhysicalHit>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int cFittedEvent<cPhysicalHit>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cFittedEvent<cPhysicalHit>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *cFittedEvent<cPhysicalHit>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cFittedEvent<cPhysicalHit>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *cFittedEvent<cPhysicalHit>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cFittedEvent<cPhysicalHit>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr cFittedEvent<int>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *cFittedEvent<int>::Class_Name()
{
   return "cFittedEvent<int>";
}

//______________________________________________________________________________
template <> const char *cFittedEvent<int>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cFittedEvent<int>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int cFittedEvent<int>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cFittedEvent<int>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *cFittedEvent<int>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cFittedEvent<int>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *cFittedEvent<int>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cFittedEvent<int>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void cHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class cHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(cHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(cHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_cHit(void *p) {
      return  p ? new(p) ::cHit : new ::cHit;
   }
   static void *newArray_cHit(Long_t nElements, void *p) {
      return p ? new(p) ::cHit[nElements] : new ::cHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_cHit(void *p) {
      delete ((::cHit*)p);
   }
   static void deleteArray_cHit(void *p) {
      delete [] ((::cHit*)p);
   }
   static void destruct_cHit(void *p) {
      typedef ::cHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cHit

//______________________________________________________________________________
void cRawSignal::Streamer(TBuffer &R__b)
{
   // Stream an object of class cRawSignal.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(cRawSignal::Class(),this);
   } else {
      R__b.WriteClassBuffer(cRawSignal::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_cRawSignal(void *p) {
      return  p ? new(p) ::cRawSignal : new ::cRawSignal;
   }
   static void *newArray_cRawSignal(Long_t nElements, void *p) {
      return p ? new(p) ::cRawSignal[nElements] : new ::cRawSignal[nElements];
   }
   // Wrapper around operator delete
   static void delete_cRawSignal(void *p) {
      delete ((::cRawSignal*)p);
   }
   static void deleteArray_cRawSignal(void *p) {
      delete [] ((::cRawSignal*)p);
   }
   static void destruct_cRawSignal(void *p) {
      typedef ::cRawSignal current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cRawSignal

//______________________________________________________________________________
void cRawEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class cRawEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(cRawEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(cRawEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_cRawEvent(void *p) {
      return  p ? new(p) ::cRawEvent : new ::cRawEvent;
   }
   static void *newArray_cRawEvent(Long_t nElements, void *p) {
      return p ? new(p) ::cRawEvent[nElements] : new ::cRawEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_cRawEvent(void *p) {
      delete ((::cRawEvent*)p);
   }
   static void deleteArray_cRawEvent(void *p) {
      delete [] ((::cRawEvent*)p);
   }
   static void destruct_cRawEvent(void *p) {
      typedef ::cRawEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cRawEvent

//______________________________________________________________________________
void cLookupTable::Streamer(TBuffer &R__b)
{
   // Stream an object of class cLookupTable.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(cLookupTable::Class(),this);
   } else {
      R__b.WriteClassBuffer(cLookupTable::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_cLookupTable(void *p) {
      return  p ? new(p) ::cLookupTable : new ::cLookupTable;
   }
   static void *newArray_cLookupTable(Long_t nElements, void *p) {
      return p ? new(p) ::cLookupTable[nElements] : new ::cLookupTable[nElements];
   }
   // Wrapper around operator delete
   static void delete_cLookupTable(void *p) {
      delete ((::cLookupTable*)p);
   }
   static void deleteArray_cLookupTable(void *p) {
      delete [] ((::cLookupTable*)p);
   }
   static void destruct_cLookupTable(void *p) {
      typedef ::cLookupTable current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cLookupTable

//______________________________________________________________________________
void cPhysicalHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class cPhysicalHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(cPhysicalHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(cPhysicalHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_cPhysicalHit(void *p) {
      return  p ? new(p) ::cPhysicalHit : new ::cPhysicalHit;
   }
   static void *newArray_cPhysicalHit(Long_t nElements, void *p) {
      return p ? new(p) ::cPhysicalHit[nElements] : new ::cPhysicalHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_cPhysicalHit(void *p) {
      delete ((::cPhysicalHit*)p);
   }
   static void deleteArray_cPhysicalHit(void *p) {
      delete [] ((::cPhysicalHit*)p);
   }
   static void destruct_cPhysicalHit(void *p) {
      typedef ::cPhysicalHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cPhysicalHit

//______________________________________________________________________________
void cPhysicalEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class cPhysicalEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(cPhysicalEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(cPhysicalEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_cPhysicalEvent(void *p) {
      return  p ? new(p) ::cPhysicalEvent : new ::cPhysicalEvent;
   }
   static void *newArray_cPhysicalEvent(Long_t nElements, void *p) {
      return p ? new(p) ::cPhysicalEvent[nElements] : new ::cPhysicalEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_cPhysicalEvent(void *p) {
      delete ((::cPhysicalEvent*)p);
   }
   static void deleteArray_cPhysicalEvent(void *p) {
      delete [] ((::cPhysicalEvent*)p);
   }
   static void destruct_cPhysicalEvent(void *p) {
      typedef ::cPhysicalEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cPhysicalEvent

//______________________________________________________________________________
template <> void cFittedLine<cPhysicalHit>::Streamer(TBuffer &R__b)
{
   // Stream an object of class cFittedLine<cPhysicalHit>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(cFittedLine<cPhysicalHit>::Class(),this);
   } else {
      R__b.WriteClassBuffer(cFittedLine<cPhysicalHit>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_cFittedLinelEcPhysicalHitgR(void *p) {
      return  p ? new(p) ::cFittedLine<cPhysicalHit> : new ::cFittedLine<cPhysicalHit>;
   }
   static void *newArray_cFittedLinelEcPhysicalHitgR(Long_t nElements, void *p) {
      return p ? new(p) ::cFittedLine<cPhysicalHit>[nElements] : new ::cFittedLine<cPhysicalHit>[nElements];
   }
   // Wrapper around operator delete
   static void delete_cFittedLinelEcPhysicalHitgR(void *p) {
      delete ((::cFittedLine<cPhysicalHit>*)p);
   }
   static void deleteArray_cFittedLinelEcPhysicalHitgR(void *p) {
      delete [] ((::cFittedLine<cPhysicalHit>*)p);
   }
   static void destruct_cFittedLinelEcPhysicalHitgR(void *p) {
      typedef ::cFittedLine<cPhysicalHit> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cFittedLine<cPhysicalHit>

//______________________________________________________________________________
template <> void cFittedLine<int>::Streamer(TBuffer &R__b)
{
   // Stream an object of class cFittedLine<int>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(cFittedLine<int>::Class(),this);
   } else {
      R__b.WriteClassBuffer(cFittedLine<int>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_cFittedLinelEintgR(void *p) {
      return  p ? new(p) ::cFittedLine<int> : new ::cFittedLine<int>;
   }
   static void *newArray_cFittedLinelEintgR(Long_t nElements, void *p) {
      return p ? new(p) ::cFittedLine<int>[nElements] : new ::cFittedLine<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_cFittedLinelEintgR(void *p) {
      delete ((::cFittedLine<int>*)p);
   }
   static void deleteArray_cFittedLinelEintgR(void *p) {
      delete [] ((::cFittedLine<int>*)p);
   }
   static void destruct_cFittedLinelEintgR(void *p) {
      typedef ::cFittedLine<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cFittedLine<int>

//______________________________________________________________________________
template <> void cVertex<cPhysicalHit>::Streamer(TBuffer &R__b)
{
   // Stream an object of class cVertex<cPhysicalHit>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(cVertex<cPhysicalHit>::Class(),this);
   } else {
      R__b.WriteClassBuffer(cVertex<cPhysicalHit>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_cVertexlEcPhysicalHitgR(void *p) {
      return  p ? new(p) ::cVertex<cPhysicalHit> : new ::cVertex<cPhysicalHit>;
   }
   static void *newArray_cVertexlEcPhysicalHitgR(Long_t nElements, void *p) {
      return p ? new(p) ::cVertex<cPhysicalHit>[nElements] : new ::cVertex<cPhysicalHit>[nElements];
   }
   // Wrapper around operator delete
   static void delete_cVertexlEcPhysicalHitgR(void *p) {
      delete ((::cVertex<cPhysicalHit>*)p);
   }
   static void deleteArray_cVertexlEcPhysicalHitgR(void *p) {
      delete [] ((::cVertex<cPhysicalHit>*)p);
   }
   static void destruct_cVertexlEcPhysicalHitgR(void *p) {
      typedef ::cVertex<cPhysicalHit> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cVertex<cPhysicalHit>

//______________________________________________________________________________
template <> void cVertex<int>::Streamer(TBuffer &R__b)
{
   // Stream an object of class cVertex<int>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(cVertex<int>::Class(),this);
   } else {
      R__b.WriteClassBuffer(cVertex<int>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_cVertexlEintgR(void *p) {
      return  p ? new(p) ::cVertex<int> : new ::cVertex<int>;
   }
   static void *newArray_cVertexlEintgR(Long_t nElements, void *p) {
      return p ? new(p) ::cVertex<int>[nElements] : new ::cVertex<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_cVertexlEintgR(void *p) {
      delete ((::cVertex<int>*)p);
   }
   static void deleteArray_cVertexlEintgR(void *p) {
      delete [] ((::cVertex<int>*)p);
   }
   static void destruct_cVertexlEintgR(void *p) {
      typedef ::cVertex<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cVertex<int>

//______________________________________________________________________________
template <> void cFittedEvent<cPhysicalHit>::Streamer(TBuffer &R__b)
{
   // Stream an object of class cFittedEvent<cPhysicalHit>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(cFittedEvent<cPhysicalHit>::Class(),this);
   } else {
      R__b.WriteClassBuffer(cFittedEvent<cPhysicalHit>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_cFittedEventlEcPhysicalHitgR(void *p) {
      return  p ? new(p) ::cFittedEvent<cPhysicalHit> : new ::cFittedEvent<cPhysicalHit>;
   }
   static void *newArray_cFittedEventlEcPhysicalHitgR(Long_t nElements, void *p) {
      return p ? new(p) ::cFittedEvent<cPhysicalHit>[nElements] : new ::cFittedEvent<cPhysicalHit>[nElements];
   }
   // Wrapper around operator delete
   static void delete_cFittedEventlEcPhysicalHitgR(void *p) {
      delete ((::cFittedEvent<cPhysicalHit>*)p);
   }
   static void deleteArray_cFittedEventlEcPhysicalHitgR(void *p) {
      delete [] ((::cFittedEvent<cPhysicalHit>*)p);
   }
   static void destruct_cFittedEventlEcPhysicalHitgR(void *p) {
      typedef ::cFittedEvent<cPhysicalHit> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cFittedEvent<cPhysicalHit>

//______________________________________________________________________________
template <> void cFittedEvent<int>::Streamer(TBuffer &R__b)
{
   // Stream an object of class cFittedEvent<int>.

   TObject::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_cFittedEventlEintgR(void *p) {
      return  p ? new(p) ::cFittedEvent<int> : new ::cFittedEvent<int>;
   }
   static void *newArray_cFittedEventlEintgR(Long_t nElements, void *p) {
      return p ? new(p) ::cFittedEvent<int>[nElements] : new ::cFittedEvent<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_cFittedEventlEintgR(void *p) {
      delete ((::cFittedEvent<int>*)p);
   }
   static void deleteArray_cFittedEventlEintgR(void *p) {
      delete [] ((::cFittedEvent<int>*)p);
   }
   static void destruct_cFittedEventlEintgR(void *p) {
      typedef ::cFittedEvent<int> current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_cFittedEventlEintgR(TBuffer &buf, void *obj) {
      ((::cFittedEvent<int>*)obj)->::cFittedEvent<int>::Streamer(buf);
   }
} // end of namespace ROOT for class ::cFittedEvent<int>

namespace ROOT {
   static TClass *vectorlEdoublegR_Dictionary();
   static void vectorlEdoublegR_TClassManip(TClass*);
   static void *new_vectorlEdoublegR(void *p = 0);
   static void *newArray_vectorlEdoublegR(Long_t size, void *p);
   static void delete_vectorlEdoublegR(void *p);
   static void deleteArray_vectorlEdoublegR(void *p);
   static void destruct_vectorlEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<double>*)
   {
      vector<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<double>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<double>", -2, "vector", 386,
                  typeid(vector<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEdoublegR_Dictionary, isa_proxy, 4,
                  sizeof(vector<double>) );
      instance.SetNew(&new_vectorlEdoublegR);
      instance.SetNewArray(&newArray_vectorlEdoublegR);
      instance.SetDelete(&delete_vectorlEdoublegR);
      instance.SetDeleteArray(&deleteArray_vectorlEdoublegR);
      instance.SetDestructor(&destruct_vectorlEdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<double> >()));

      ::ROOT::AddClassAlternate("vector<double>","std::vector<double, std::allocator<double> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<double>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<double>*)0x0)->GetClass();
      vectorlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double> : new vector<double>;
   }
   static void *newArray_vectorlEdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double>[nElements] : new vector<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEdoublegR(void *p) {
      delete ((vector<double>*)p);
   }
   static void deleteArray_vectorlEdoublegR(void *p) {
      delete [] ((vector<double>*)p);
   }
   static void destruct_vectorlEdoublegR(void *p) {
      typedef vector<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<double>

namespace ROOT {
   static TClass *vectorlEcRawSignalgR_Dictionary();
   static void vectorlEcRawSignalgR_TClassManip(TClass*);
   static void *new_vectorlEcRawSignalgR(void *p = 0);
   static void *newArray_vectorlEcRawSignalgR(Long_t size, void *p);
   static void delete_vectorlEcRawSignalgR(void *p);
   static void deleteArray_vectorlEcRawSignalgR(void *p);
   static void destruct_vectorlEcRawSignalgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<cRawSignal>*)
   {
      vector<cRawSignal> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<cRawSignal>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<cRawSignal>", -2, "vector", 386,
                  typeid(vector<cRawSignal>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEcRawSignalgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<cRawSignal>) );
      instance.SetNew(&new_vectorlEcRawSignalgR);
      instance.SetNewArray(&newArray_vectorlEcRawSignalgR);
      instance.SetDelete(&delete_vectorlEcRawSignalgR);
      instance.SetDeleteArray(&deleteArray_vectorlEcRawSignalgR);
      instance.SetDestructor(&destruct_vectorlEcRawSignalgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<cRawSignal> >()));

      ::ROOT::AddClassAlternate("vector<cRawSignal>","std::vector<cRawSignal, std::allocator<cRawSignal> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<cRawSignal>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEcRawSignalgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<cRawSignal>*)0x0)->GetClass();
      vectorlEcRawSignalgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEcRawSignalgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEcRawSignalgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<cRawSignal> : new vector<cRawSignal>;
   }
   static void *newArray_vectorlEcRawSignalgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<cRawSignal>[nElements] : new vector<cRawSignal>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEcRawSignalgR(void *p) {
      delete ((vector<cRawSignal>*)p);
   }
   static void deleteArray_vectorlEcRawSignalgR(void *p) {
      delete [] ((vector<cRawSignal>*)p);
   }
   static void destruct_vectorlEcRawSignalgR(void *p) {
      typedef vector<cRawSignal> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<cRawSignal>

namespace ROOT {
   static TClass *vectorlEcHitgR_Dictionary();
   static void vectorlEcHitgR_TClassManip(TClass*);
   static void *new_vectorlEcHitgR(void *p = 0);
   static void *newArray_vectorlEcHitgR(Long_t size, void *p);
   static void delete_vectorlEcHitgR(void *p);
   static void deleteArray_vectorlEcHitgR(void *p);
   static void destruct_vectorlEcHitgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<cHit>*)
   {
      vector<cHit> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<cHit>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<cHit>", -2, "vector", 386,
                  typeid(vector<cHit>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEcHitgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<cHit>) );
      instance.SetNew(&new_vectorlEcHitgR);
      instance.SetNewArray(&newArray_vectorlEcHitgR);
      instance.SetDelete(&delete_vectorlEcHitgR);
      instance.SetDeleteArray(&deleteArray_vectorlEcHitgR);
      instance.SetDestructor(&destruct_vectorlEcHitgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<cHit> >()));

      ::ROOT::AddClassAlternate("vector<cHit>","std::vector<cHit, std::allocator<cHit> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<cHit>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEcHitgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<cHit>*)0x0)->GetClass();
      vectorlEcHitgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEcHitgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEcHitgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<cHit> : new vector<cHit>;
   }
   static void *newArray_vectorlEcHitgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<cHit>[nElements] : new vector<cHit>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEcHitgR(void *p) {
      delete ((vector<cHit>*)p);
   }
   static void deleteArray_vectorlEcHitgR(void *p) {
      delete [] ((vector<cHit>*)p);
   }
   static void destruct_vectorlEcHitgR(void *p) {
      typedef vector<cHit> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<cHit>

namespace ROOT {
   static TClass *maplEintcOcLookupTablecLcLchanDatagR_Dictionary();
   static void maplEintcOcLookupTablecLcLchanDatagR_TClassManip(TClass*);
   static void *new_maplEintcOcLookupTablecLcLchanDatagR(void *p = 0);
   static void *newArray_maplEintcOcLookupTablecLcLchanDatagR(Long_t size, void *p);
   static void delete_maplEintcOcLookupTablecLcLchanDatagR(void *p);
   static void deleteArray_maplEintcOcLookupTablecLcLchanDatagR(void *p);
   static void destruct_maplEintcOcLookupTablecLcLchanDatagR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,cLookupTable::chanData>*)
   {
      map<int,cLookupTable::chanData> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,cLookupTable::chanData>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,cLookupTable::chanData>", -2, "map", 100,
                  typeid(map<int,cLookupTable::chanData>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOcLookupTablecLcLchanDatagR_Dictionary, isa_proxy, 4,
                  sizeof(map<int,cLookupTable::chanData>) );
      instance.SetNew(&new_maplEintcOcLookupTablecLcLchanDatagR);
      instance.SetNewArray(&newArray_maplEintcOcLookupTablecLcLchanDatagR);
      instance.SetDelete(&delete_maplEintcOcLookupTablecLcLchanDatagR);
      instance.SetDeleteArray(&deleteArray_maplEintcOcLookupTablecLcLchanDatagR);
      instance.SetDestructor(&destruct_maplEintcOcLookupTablecLcLchanDatagR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,cLookupTable::chanData> >()));

      ::ROOT::AddClassAlternate("map<int,cLookupTable::chanData>","std::map<int, cLookupTable::chanData, std::less<int>, std::allocator<std::pair<int const, cLookupTable::chanData> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,cLookupTable::chanData>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOcLookupTablecLcLchanDatagR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,cLookupTable::chanData>*)0x0)->GetClass();
      maplEintcOcLookupTablecLcLchanDatagR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOcLookupTablecLcLchanDatagR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOcLookupTablecLcLchanDatagR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,cLookupTable::chanData> : new map<int,cLookupTable::chanData>;
   }
   static void *newArray_maplEintcOcLookupTablecLcLchanDatagR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,cLookupTable::chanData>[nElements] : new map<int,cLookupTable::chanData>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOcLookupTablecLcLchanDatagR(void *p) {
      delete ((map<int,cLookupTable::chanData>*)p);
   }
   static void deleteArray_maplEintcOcLookupTablecLcLchanDatagR(void *p) {
      delete [] ((map<int,cLookupTable::chanData>*)p);
   }
   static void destruct_maplEintcOcLookupTablecLcLchanDatagR(void *p) {
      typedef map<int,cLookupTable::chanData> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,cLookupTable::chanData>

namespace ROOT {
   static TClass *listlEstringgR_Dictionary();
   static void listlEstringgR_TClassManip(TClass*);
   static void *new_listlEstringgR(void *p = 0);
   static void *newArray_listlEstringgR(Long_t size, void *p);
   static void delete_listlEstringgR(void *p);
   static void deleteArray_listlEstringgR(void *p);
   static void destruct_listlEstringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const list<string>*)
   {
      list<string> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(list<string>));
      static ::ROOT::TGenericClassInfo 
         instance("list<string>", -2, "list", 552,
                  typeid(list<string>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &listlEstringgR_Dictionary, isa_proxy, 0,
                  sizeof(list<string>) );
      instance.SetNew(&new_listlEstringgR);
      instance.SetNewArray(&newArray_listlEstringgR);
      instance.SetDelete(&delete_listlEstringgR);
      instance.SetDeleteArray(&deleteArray_listlEstringgR);
      instance.SetDestructor(&destruct_listlEstringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< list<string> >()));

      ::ROOT::AddClassAlternate("list<string>","std::__cxx11::list<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const list<string>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *listlEstringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const list<string>*)0x0)->GetClass();
      listlEstringgR_TClassManip(theClass);
   return theClass;
   }

   static void listlEstringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_listlEstringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) list<string> : new list<string>;
   }
   static void *newArray_listlEstringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) list<string>[nElements] : new list<string>[nElements];
   }
   // Wrapper around operator delete
   static void delete_listlEstringgR(void *p) {
      delete ((list<string>*)p);
   }
   static void deleteArray_listlEstringgR(void *p) {
      delete [] ((list<string>*)p);
   }
   static void destruct_listlEstringgR(void *p) {
      typedef list<string> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class list<string>

namespace ROOT {
   static TClass *listlEintgR_Dictionary();
   static void listlEintgR_TClassManip(TClass*);
   static void *new_listlEintgR(void *p = 0);
   static void *newArray_listlEintgR(Long_t size, void *p);
   static void delete_listlEintgR(void *p);
   static void deleteArray_listlEintgR(void *p);
   static void destruct_listlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const list<int>*)
   {
      list<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(list<int>));
      static ::ROOT::TGenericClassInfo 
         instance("list<int>", -2, "list", 552,
                  typeid(list<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &listlEintgR_Dictionary, isa_proxy, 4,
                  sizeof(list<int>) );
      instance.SetNew(&new_listlEintgR);
      instance.SetNewArray(&newArray_listlEintgR);
      instance.SetDelete(&delete_listlEintgR);
      instance.SetDeleteArray(&deleteArray_listlEintgR);
      instance.SetDestructor(&destruct_listlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< list<int> >()));

      ::ROOT::AddClassAlternate("list<int>","std::__cxx11::list<int, std::allocator<int> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const list<int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *listlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const list<int>*)0x0)->GetClass();
      listlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void listlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_listlEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) list<int> : new list<int>;
   }
   static void *newArray_listlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) list<int>[nElements] : new list<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_listlEintgR(void *p) {
      delete ((list<int>*)p);
   }
   static void deleteArray_listlEintgR(void *p) {
      delete [] ((list<int>*)p);
   }
   static void destruct_listlEintgR(void *p) {
      typedef list<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class list<int>

namespace ROOT {
   static TClass *listlEcVertexlEintgRsPgR_Dictionary();
   static void listlEcVertexlEintgRsPgR_TClassManip(TClass*);
   static void *new_listlEcVertexlEintgRsPgR(void *p = 0);
   static void *newArray_listlEcVertexlEintgRsPgR(Long_t size, void *p);
   static void delete_listlEcVertexlEintgRsPgR(void *p);
   static void deleteArray_listlEcVertexlEintgRsPgR(void *p);
   static void destruct_listlEcVertexlEintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const list<cVertex<int> >*)
   {
      list<cVertex<int> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(list<cVertex<int> >));
      static ::ROOT::TGenericClassInfo 
         instance("list<cVertex<int> >", -2, "list", 552,
                  typeid(list<cVertex<int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &listlEcVertexlEintgRsPgR_Dictionary, isa_proxy, 4,
                  sizeof(list<cVertex<int> >) );
      instance.SetNew(&new_listlEcVertexlEintgRsPgR);
      instance.SetNewArray(&newArray_listlEcVertexlEintgRsPgR);
      instance.SetDelete(&delete_listlEcVertexlEintgRsPgR);
      instance.SetDeleteArray(&deleteArray_listlEcVertexlEintgRsPgR);
      instance.SetDestructor(&destruct_listlEcVertexlEintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< list<cVertex<int> > >()));

      ::ROOT::AddClassAlternate("list<cVertex<int> >","std::__cxx11::list<cVertex<int>, std::allocator<cVertex<int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const list<cVertex<int> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *listlEcVertexlEintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const list<cVertex<int> >*)0x0)->GetClass();
      listlEcVertexlEintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void listlEcVertexlEintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_listlEcVertexlEintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) list<cVertex<int> > : new list<cVertex<int> >;
   }
   static void *newArray_listlEcVertexlEintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) list<cVertex<int> >[nElements] : new list<cVertex<int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_listlEcVertexlEintgRsPgR(void *p) {
      delete ((list<cVertex<int> >*)p);
   }
   static void deleteArray_listlEcVertexlEintgRsPgR(void *p) {
      delete [] ((list<cVertex<int> >*)p);
   }
   static void destruct_listlEcVertexlEintgRsPgR(void *p) {
      typedef list<cVertex<int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class list<cVertex<int> >

namespace ROOT {
   static TClass *listlEcVertexlEcPhysicalHitgRsPgR_Dictionary();
   static void listlEcVertexlEcPhysicalHitgRsPgR_TClassManip(TClass*);
   static void *new_listlEcVertexlEcPhysicalHitgRsPgR(void *p = 0);
   static void *newArray_listlEcVertexlEcPhysicalHitgRsPgR(Long_t size, void *p);
   static void delete_listlEcVertexlEcPhysicalHitgRsPgR(void *p);
   static void deleteArray_listlEcVertexlEcPhysicalHitgRsPgR(void *p);
   static void destruct_listlEcVertexlEcPhysicalHitgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const list<cVertex<cPhysicalHit> >*)
   {
      list<cVertex<cPhysicalHit> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(list<cVertex<cPhysicalHit> >));
      static ::ROOT::TGenericClassInfo 
         instance("list<cVertex<cPhysicalHit> >", -2, "list", 552,
                  typeid(list<cVertex<cPhysicalHit> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &listlEcVertexlEcPhysicalHitgRsPgR_Dictionary, isa_proxy, 4,
                  sizeof(list<cVertex<cPhysicalHit> >) );
      instance.SetNew(&new_listlEcVertexlEcPhysicalHitgRsPgR);
      instance.SetNewArray(&newArray_listlEcVertexlEcPhysicalHitgRsPgR);
      instance.SetDelete(&delete_listlEcVertexlEcPhysicalHitgRsPgR);
      instance.SetDeleteArray(&deleteArray_listlEcVertexlEcPhysicalHitgRsPgR);
      instance.SetDestructor(&destruct_listlEcVertexlEcPhysicalHitgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< list<cVertex<cPhysicalHit> > >()));

      ::ROOT::AddClassAlternate("list<cVertex<cPhysicalHit> >","std::__cxx11::list<cVertex<cPhysicalHit>, std::allocator<cVertex<cPhysicalHit> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const list<cVertex<cPhysicalHit> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *listlEcVertexlEcPhysicalHitgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const list<cVertex<cPhysicalHit> >*)0x0)->GetClass();
      listlEcVertexlEcPhysicalHitgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void listlEcVertexlEcPhysicalHitgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_listlEcVertexlEcPhysicalHitgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) list<cVertex<cPhysicalHit> > : new list<cVertex<cPhysicalHit> >;
   }
   static void *newArray_listlEcVertexlEcPhysicalHitgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) list<cVertex<cPhysicalHit> >[nElements] : new list<cVertex<cPhysicalHit> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_listlEcVertexlEcPhysicalHitgRsPgR(void *p) {
      delete ((list<cVertex<cPhysicalHit> >*)p);
   }
   static void deleteArray_listlEcVertexlEcPhysicalHitgRsPgR(void *p) {
      delete [] ((list<cVertex<cPhysicalHit> >*)p);
   }
   static void destruct_listlEcVertexlEcPhysicalHitgRsPgR(void *p) {
      typedef list<cVertex<cPhysicalHit> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class list<cVertex<cPhysicalHit> >

namespace ROOT {
   static TClass *listlEcPhysicalHitgR_Dictionary();
   static void listlEcPhysicalHitgR_TClassManip(TClass*);
   static void *new_listlEcPhysicalHitgR(void *p = 0);
   static void *newArray_listlEcPhysicalHitgR(Long_t size, void *p);
   static void delete_listlEcPhysicalHitgR(void *p);
   static void deleteArray_listlEcPhysicalHitgR(void *p);
   static void destruct_listlEcPhysicalHitgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const list<cPhysicalHit>*)
   {
      list<cPhysicalHit> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(list<cPhysicalHit>));
      static ::ROOT::TGenericClassInfo 
         instance("list<cPhysicalHit>", -2, "list", 552,
                  typeid(list<cPhysicalHit>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &listlEcPhysicalHitgR_Dictionary, isa_proxy, 4,
                  sizeof(list<cPhysicalHit>) );
      instance.SetNew(&new_listlEcPhysicalHitgR);
      instance.SetNewArray(&newArray_listlEcPhysicalHitgR);
      instance.SetDelete(&delete_listlEcPhysicalHitgR);
      instance.SetDeleteArray(&deleteArray_listlEcPhysicalHitgR);
      instance.SetDestructor(&destruct_listlEcPhysicalHitgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< list<cPhysicalHit> >()));

      ::ROOT::AddClassAlternate("list<cPhysicalHit>","std::__cxx11::list<cPhysicalHit, std::allocator<cPhysicalHit> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const list<cPhysicalHit>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *listlEcPhysicalHitgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const list<cPhysicalHit>*)0x0)->GetClass();
      listlEcPhysicalHitgR_TClassManip(theClass);
   return theClass;
   }

   static void listlEcPhysicalHitgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_listlEcPhysicalHitgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) list<cPhysicalHit> : new list<cPhysicalHit>;
   }
   static void *newArray_listlEcPhysicalHitgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) list<cPhysicalHit>[nElements] : new list<cPhysicalHit>[nElements];
   }
   // Wrapper around operator delete
   static void delete_listlEcPhysicalHitgR(void *p) {
      delete ((list<cPhysicalHit>*)p);
   }
   static void deleteArray_listlEcPhysicalHitgR(void *p) {
      delete [] ((list<cPhysicalHit>*)p);
   }
   static void destruct_listlEcPhysicalHitgR(void *p) {
      typedef list<cPhysicalHit> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class list<cPhysicalHit>

namespace ROOT {
   static TClass *listlEcFittedLinelEintgRsPgR_Dictionary();
   static void listlEcFittedLinelEintgRsPgR_TClassManip(TClass*);
   static void *new_listlEcFittedLinelEintgRsPgR(void *p = 0);
   static void *newArray_listlEcFittedLinelEintgRsPgR(Long_t size, void *p);
   static void delete_listlEcFittedLinelEintgRsPgR(void *p);
   static void deleteArray_listlEcFittedLinelEintgRsPgR(void *p);
   static void destruct_listlEcFittedLinelEintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const list<cFittedLine<int> >*)
   {
      list<cFittedLine<int> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(list<cFittedLine<int> >));
      static ::ROOT::TGenericClassInfo 
         instance("list<cFittedLine<int> >", -2, "list", 552,
                  typeid(list<cFittedLine<int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &listlEcFittedLinelEintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(list<cFittedLine<int> >) );
      instance.SetNew(&new_listlEcFittedLinelEintgRsPgR);
      instance.SetNewArray(&newArray_listlEcFittedLinelEintgRsPgR);
      instance.SetDelete(&delete_listlEcFittedLinelEintgRsPgR);
      instance.SetDeleteArray(&deleteArray_listlEcFittedLinelEintgRsPgR);
      instance.SetDestructor(&destruct_listlEcFittedLinelEintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< list<cFittedLine<int> > >()));

      ::ROOT::AddClassAlternate("list<cFittedLine<int> >","std::__cxx11::list<cFittedLine<int>, std::allocator<cFittedLine<int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const list<cFittedLine<int> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *listlEcFittedLinelEintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const list<cFittedLine<int> >*)0x0)->GetClass();
      listlEcFittedLinelEintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void listlEcFittedLinelEintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_listlEcFittedLinelEintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) list<cFittedLine<int> > : new list<cFittedLine<int> >;
   }
   static void *newArray_listlEcFittedLinelEintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) list<cFittedLine<int> >[nElements] : new list<cFittedLine<int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_listlEcFittedLinelEintgRsPgR(void *p) {
      delete ((list<cFittedLine<int> >*)p);
   }
   static void deleteArray_listlEcFittedLinelEintgRsPgR(void *p) {
      delete [] ((list<cFittedLine<int> >*)p);
   }
   static void destruct_listlEcFittedLinelEintgRsPgR(void *p) {
      typedef list<cFittedLine<int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class list<cFittedLine<int> >

namespace ROOT {
   static TClass *listlEcFittedLinelEcPhysicalHitgRsPgR_Dictionary();
   static void listlEcFittedLinelEcPhysicalHitgRsPgR_TClassManip(TClass*);
   static void *new_listlEcFittedLinelEcPhysicalHitgRsPgR(void *p = 0);
   static void *newArray_listlEcFittedLinelEcPhysicalHitgRsPgR(Long_t size, void *p);
   static void delete_listlEcFittedLinelEcPhysicalHitgRsPgR(void *p);
   static void deleteArray_listlEcFittedLinelEcPhysicalHitgRsPgR(void *p);
   static void destruct_listlEcFittedLinelEcPhysicalHitgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const list<cFittedLine<cPhysicalHit> >*)
   {
      list<cFittedLine<cPhysicalHit> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(list<cFittedLine<cPhysicalHit> >));
      static ::ROOT::TGenericClassInfo 
         instance("list<cFittedLine<cPhysicalHit> >", -2, "list", 552,
                  typeid(list<cFittedLine<cPhysicalHit> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &listlEcFittedLinelEcPhysicalHitgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(list<cFittedLine<cPhysicalHit> >) );
      instance.SetNew(&new_listlEcFittedLinelEcPhysicalHitgRsPgR);
      instance.SetNewArray(&newArray_listlEcFittedLinelEcPhysicalHitgRsPgR);
      instance.SetDelete(&delete_listlEcFittedLinelEcPhysicalHitgRsPgR);
      instance.SetDeleteArray(&deleteArray_listlEcFittedLinelEcPhysicalHitgRsPgR);
      instance.SetDestructor(&destruct_listlEcFittedLinelEcPhysicalHitgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< list<cFittedLine<cPhysicalHit> > >()));

      ::ROOT::AddClassAlternate("list<cFittedLine<cPhysicalHit> >","std::__cxx11::list<cFittedLine<cPhysicalHit>, std::allocator<cFittedLine<cPhysicalHit> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const list<cFittedLine<cPhysicalHit> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *listlEcFittedLinelEcPhysicalHitgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const list<cFittedLine<cPhysicalHit> >*)0x0)->GetClass();
      listlEcFittedLinelEcPhysicalHitgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void listlEcFittedLinelEcPhysicalHitgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_listlEcFittedLinelEcPhysicalHitgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) list<cFittedLine<cPhysicalHit> > : new list<cFittedLine<cPhysicalHit> >;
   }
   static void *newArray_listlEcFittedLinelEcPhysicalHitgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) list<cFittedLine<cPhysicalHit> >[nElements] : new list<cFittedLine<cPhysicalHit> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_listlEcFittedLinelEcPhysicalHitgRsPgR(void *p) {
      delete ((list<cFittedLine<cPhysicalHit> >*)p);
   }
   static void deleteArray_listlEcFittedLinelEcPhysicalHitgRsPgR(void *p) {
      delete [] ((list<cFittedLine<cPhysicalHit> >*)p);
   }
   static void destruct_listlEcFittedLinelEcPhysicalHitgRsPgR(void *p) {
      typedef list<cFittedLine<cPhysicalHit> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class list<cFittedLine<cPhysicalHit> >

namespace {
  void TriggerDictionaryInitialization_libdict_Impl() {
    static const char* headers[] = {
"cHit.h",
"cRawEvent.h",
"cLookupTable.h",
"cRawSignal.h",
"cPhysicalHit.h",
"cPhysicalEvent.h",
"cFittedLine.h",
"cFittedEvent.h",
"cVertex.h",
0
    };
    static const char* includePaths[] = {
"/home/lorenzo/root/include",
"/home/lorenzo/Desktop/ARDAC/ARDA/inc",
"/home/lorenzo/Desktop/ARDAC/ARDA/libMFM",
"/home/lorenzo/Desktop/ARDAC/ARDA/pClasses/inc",
"/home/lorenzo/Desktop/ARDAC/ARDA/leuvenPSA/inc",
"/home/lorenzo/Desktop/ARDAC/ARDA",
"/home/lorenzo/root/include/",
"/home/lorenzo/Desktop/ARDAC/build/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libdict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
class __attribute__((annotate("$clingAutoload$cHit.h")))  cHit;
class __attribute__((annotate("$clingAutoload$cRawSignal.h")))  __attribute__((annotate("$clingAutoload$cRawEvent.h")))  cRawSignal;
class __attribute__((annotate("$clingAutoload$cRawEvent.h")))  cRawEvent;
class __attribute__((annotate("$clingAutoload$cPhysicalHit.h")))  cPhysicalHit;
template <class T> class __attribute__((annotate("$clingAutoload$cVertex.h")))  __attribute__((annotate("$clingAutoload$cFittedEvent.h")))  cVertex;

class __attribute__((annotate("$clingAutoload$cLookupTable.h")))  cLookupTable;
class __attribute__((annotate("$clingAutoload$cPhysicalEvent.h")))  cPhysicalEvent;
template <class T> class __attribute__((annotate("$clingAutoload$cFittedLine.h")))  cFittedLine;

template <class T> class __attribute__((annotate("$clingAutoload$cFittedEvent.h")))  cFittedEvent;

)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libdict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "cHit.h"
#include "cRawEvent.h"
#include "cLookupTable.h"
#include "cRawSignal.h"
#include "cPhysicalHit.h"
#include "cPhysicalEvent.h"
#include "cFittedLine.h"
#include "cFittedEvent.h"
#include "cVertex.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"cFittedEvent<cPhysicalHit>", payloadCode, "@",
"cFittedEvent<int>", payloadCode, "@",
"cFittedLine<cPhysicalHit>", payloadCode, "@",
"cFittedLine<int>", payloadCode, "@",
"cHit", payloadCode, "@",
"cLookupTable", payloadCode, "@",
"cPhysicalEvent", payloadCode, "@",
"cPhysicalHit", payloadCode, "@",
"cRawEvent", payloadCode, "@",
"cRawSignal", payloadCode, "@",
"cVertex<cPhysicalHit>", payloadCode, "@",
"cVertex<int>", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libdict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libdict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libdict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libdict() {
  TriggerDictionaryInitialization_libdict_Impl();
}
