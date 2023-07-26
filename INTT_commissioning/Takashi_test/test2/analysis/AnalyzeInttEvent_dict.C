// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME AnalyzeInttEvent_dict
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

// The generated code does not explicitly qualifies STL entities
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "AnalyzeInttEvent.h"
#include "InttCluster.h"
#include "InttEvent.h"
#include "InttEventSync.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_InttHit(void *p = 0);
   static void *newArray_InttHit(Long_t size, void *p);
   static void delete_InttHit(void *p);
   static void deleteArray_InttHit(void *p);
   static void destruct_InttHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::InttHit*)
   {
      ::InttHit *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::InttHit >(0);
      static ::ROOT::TGenericClassInfo 
         instance("InttHit", ::InttHit::Class_Version(), "InttEvent.h", 9,
                  typeid(::InttHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::InttHit::Dictionary, isa_proxy, 4,
                  sizeof(::InttHit) );
      instance.SetNew(&new_InttHit);
      instance.SetNewArray(&newArray_InttHit);
      instance.SetDelete(&delete_InttHit);
      instance.SetDeleteArray(&deleteArray_InttHit);
      instance.SetDestructor(&destruct_InttHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::InttHit*)
   {
      return GenerateInitInstanceLocal((::InttHit*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::InttHit*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_InttEvent(void *p = 0);
   static void *newArray_InttEvent(Long_t size, void *p);
   static void delete_InttEvent(void *p);
   static void deleteArray_InttEvent(void *p);
   static void destruct_InttEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::InttEvent*)
   {
      ::InttEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::InttEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("InttEvent", ::InttEvent::Class_Version(), "InttEvent.h", 52,
                  typeid(::InttEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::InttEvent::Dictionary, isa_proxy, 4,
                  sizeof(::InttEvent) );
      instance.SetNew(&new_InttEvent);
      instance.SetNewArray(&newArray_InttEvent);
      instance.SetDelete(&delete_InttEvent);
      instance.SetDeleteArray(&deleteArray_InttEvent);
      instance.SetDestructor(&destruct_InttEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::InttEvent*)
   {
      return GenerateInitInstanceLocal((::InttEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::InttEvent*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *InttCluster_Dictionary();
   static void InttCluster_TClassManip(TClass*);
   static void delete_InttCluster(void *p);
   static void deleteArray_InttCluster(void *p);
   static void destruct_InttCluster(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::InttCluster*)
   {
      ::InttCluster *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::InttCluster));
      static ::ROOT::TGenericClassInfo 
         instance("InttCluster", "InttCluster.h", 14,
                  typeid(::InttCluster), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &InttCluster_Dictionary, isa_proxy, 0,
                  sizeof(::InttCluster) );
      instance.SetDelete(&delete_InttCluster);
      instance.SetDeleteArray(&deleteArray_InttCluster);
      instance.SetDestructor(&destruct_InttCluster);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::InttCluster*)
   {
      return GenerateInitInstanceLocal((::InttCluster*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::InttCluster*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *InttCluster_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::InttCluster*)0x0)->GetClass();
      InttCluster_TClassManip(theClass);
   return theClass;
   }

   static void InttCluster_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *InttClusterList_Dictionary();
   static void InttClusterList_TClassManip(TClass*);
   static void *new_InttClusterList(void *p = 0);
   static void *newArray_InttClusterList(Long_t size, void *p);
   static void delete_InttClusterList(void *p);
   static void deleteArray_InttClusterList(void *p);
   static void destruct_InttClusterList(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::InttClusterList*)
   {
      ::InttClusterList *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::InttClusterList));
      static ::ROOT::TGenericClassInfo 
         instance("InttClusterList", "InttCluster.h", 46,
                  typeid(::InttClusterList), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &InttClusterList_Dictionary, isa_proxy, 0,
                  sizeof(::InttClusterList) );
      instance.SetNew(&new_InttClusterList);
      instance.SetNewArray(&newArray_InttClusterList);
      instance.SetDelete(&delete_InttClusterList);
      instance.SetDeleteArray(&deleteArray_InttClusterList);
      instance.SetDestructor(&destruct_InttClusterList);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::InttClusterList*)
   {
      return GenerateInitInstanceLocal((::InttClusterList*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::InttClusterList*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *InttClusterList_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::InttClusterList*)0x0)->GetClass();
      InttClusterList_TClassManip(theClass);
   return theClass;
   }

   static void InttClusterList_TClassManip(TClass* ){
   }

} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr InttHit::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *InttHit::Class_Name()
{
   return "InttHit";
}

//______________________________________________________________________________
const char *InttHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::InttHit*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int InttHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::InttHit*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *InttHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::InttHit*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *InttHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::InttHit*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr InttEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *InttEvent::Class_Name()
{
   return "InttEvent";
}

//______________________________________________________________________________
const char *InttEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::InttEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int InttEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::InttEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *InttEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::InttEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *InttEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::InttEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void InttHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class InttHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(InttHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(InttHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_InttHit(void *p) {
      return  p ? new(p) ::InttHit : new ::InttHit;
   }
   static void *newArray_InttHit(Long_t nElements, void *p) {
      return p ? new(p) ::InttHit[nElements] : new ::InttHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_InttHit(void *p) {
      delete ((::InttHit*)p);
   }
   static void deleteArray_InttHit(void *p) {
      delete [] ((::InttHit*)p);
   }
   static void destruct_InttHit(void *p) {
      typedef ::InttHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::InttHit

//______________________________________________________________________________
void InttEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class InttEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(InttEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(InttEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_InttEvent(void *p) {
      return  p ? new(p) ::InttEvent : new ::InttEvent;
   }
   static void *newArray_InttEvent(Long_t nElements, void *p) {
      return p ? new(p) ::InttEvent[nElements] : new ::InttEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_InttEvent(void *p) {
      delete ((::InttEvent*)p);
   }
   static void deleteArray_InttEvent(void *p) {
      delete [] ((::InttEvent*)p);
   }
   static void destruct_InttEvent(void *p) {
      typedef ::InttEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::InttEvent

namespace ROOT {
   // Wrapper around operator delete
   static void delete_InttCluster(void *p) {
      delete ((::InttCluster*)p);
   }
   static void deleteArray_InttCluster(void *p) {
      delete [] ((::InttCluster*)p);
   }
   static void destruct_InttCluster(void *p) {
      typedef ::InttCluster current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::InttCluster

namespace ROOT {
   // Wrappers around operator new
   static void *new_InttClusterList(void *p) {
      return  p ? new(p) ::InttClusterList : new ::InttClusterList;
   }
   static void *newArray_InttClusterList(Long_t nElements, void *p) {
      return p ? new(p) ::InttClusterList[nElements] : new ::InttClusterList[nElements];
   }
   // Wrapper around operator delete
   static void delete_InttClusterList(void *p) {
      delete ((::InttClusterList*)p);
   }
   static void deleteArray_InttClusterList(void *p) {
      delete [] ((::InttClusterList*)p);
   }
   static void destruct_InttClusterList(void *p) {
      typedef ::InttClusterList current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::InttClusterList

namespace {
  void TriggerDictionaryInitialization_AnalyzeInttEvent_dict_Impl() {
    static const char* headers[] = {
"AnalyzeInttEvent.h",
"InttCluster.h",
"InttEvent.h",
"InttEventSync.h",
0
    };
    static const char* includePaths[] = {
"/home/repo/Debian/include",
"/home/repo/Debian/include",
"/home/repo/Debian/root-gcc8.3/include/",
"/home/phnxrc/INTT/hachiya/convertInttRaw/test2/analysis/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "AnalyzeInttEvent_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$InttEvent.h")))  __attribute__((annotate("$clingAutoload$InttCluster.h")))  InttHit;
class __attribute__((annotate("$clingAutoload$InttEvent.h")))  __attribute__((annotate("$clingAutoload$InttCluster.h")))  InttEvent;
class __attribute__((annotate("$clingAutoload$InttCluster.h")))  InttCluster;
class __attribute__((annotate("$clingAutoload$InttCluster.h")))  InttClusterList;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "AnalyzeInttEvent_dict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "AnalyzeInttEvent.h"
#include "InttCluster.h"
#include "InttEvent.h"
#include "InttEventSync.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"InitAnalysis", payloadCode, "@",
"InttCluster", payloadCode, "@",
"InttClusterList", payloadCode, "@",
"InttEvent", payloadCode, "@",
"InttHit", payloadCode, "@",
"RunAnalysis", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("AnalyzeInttEvent_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_AnalyzeInttEvent_dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_AnalyzeInttEvent_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_AnalyzeInttEvent_dict() {
  TriggerDictionaryInitialization_AnalyzeInttEvent_dict_Impl();
}
