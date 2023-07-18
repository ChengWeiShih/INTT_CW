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

// Header files passed via #pragma extra_include

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
0
    };
    static const char* includePaths[] = {
"/home/repo/Debian/include",
"/home/repo/Debian/include",
"/home/repo/Debian/root-gcc8.3/include/",
"/home/phnxrc/INTT/hachiya/convertInttRaw/test1/analysis/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "AnalyzeInttEvent_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$InttCluster.h")))  InttCluster;
class __attribute__((annotate("$clingAutoload$InttCluster.h")))  InttClusterList;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "AnalyzeInttEvent_dict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "AnalyzeInttEvent.h"
#include "InttCluster.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"InitAnalysis", payloadCode, "@",
"InttCluster", payloadCode, "@",
"InttClusterList", payloadCode, "@",
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
