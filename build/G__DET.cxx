// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__DET
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
#include "Detector.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *Detector_Dictionary();
   static void Detector_TClassManip(TClass*);
   static void delete_Detector(void *p);
   static void deleteArray_Detector(void *p);
   static void destruct_Detector(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Detector*)
   {
      ::Detector *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Detector));
      static ::ROOT::TGenericClassInfo 
         instance("Detector", "Detector.h", 12,
                  typeid(::Detector), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &Detector_Dictionary, isa_proxy, 4,
                  sizeof(::Detector) );
      instance.SetDelete(&delete_Detector);
      instance.SetDeleteArray(&deleteArray_Detector);
      instance.SetDestructor(&destruct_Detector);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Detector*)
   {
      return GenerateInitInstanceLocal((::Detector*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Detector*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Detector_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Detector*)0x0)->GetClass();
      Detector_TClassManip(theClass);
   return theClass;
   }

   static void Detector_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrapper around operator delete
   static void delete_Detector(void *p) {
      delete ((::Detector*)p);
   }
   static void deleteArray_Detector(void *p) {
      delete [] ((::Detector*)p);
   }
   static void destruct_Detector(void *p) {
      typedef ::Detector current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Detector

namespace {
  void TriggerDictionaryInitialization_G__DET_Impl() {
    static const char* headers[] = {
"Detector.h",
0
    };
    static const char* includePaths[] = {
"/Users/zhengqiao/clhep-install/lib/CLHEP-2.4.1.3/../../include",
"/Users/zhengqiao/geant4/geant4.10.06.p01-install/include/Geant4",
"/Users/zhengqiao/test_geant4/test/include",
"/Users/zhengqiao/build/include/",
"/Users/zhengqiao/test_geant4/test/build/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "G__DET dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$Detector.h")))  Detector;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "G__DET dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Detector.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"Detector", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("G__DET",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_G__DET_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_G__DET_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_G__DET() {
  TriggerDictionaryInitialization_G__DET_Impl();
}
