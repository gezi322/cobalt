/*
 * Copyright 2016 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// This file has been auto-generated by bindings/code_generator_cobalt.py. DO NOT MODIFY!
// Auto-generated from template: bindings/mozjs/templates/interface.cc.template

// clang-format off

#include "MozjsCallbackFunctionInterface.h"

#include "base/debug/trace_event.h"
#include "cobalt/base/polymorphic_downcast.h"
#include "cobalt/script/global_object_proxy.h"
#include "cobalt/script/opaque_handle.h"
#include "cobalt/script/script_object.h"
#include "MozjsArbitraryInterface.h"
#include "cobalt/bindings/testing/arbitrary_interface.h"

#include "base/lazy_instance.h"
#include "cobalt/script/mozjs/conversion_helpers.h"
#include "cobalt/script/mozjs/mozjs_exception_state.h"
#include "cobalt/script/mozjs/mozjs_callback_function.h"
#include "cobalt/script/mozjs/mozjs_global_object_proxy.h"
#include "cobalt/script/mozjs/mozjs_object_handle.h"
#include "cobalt/script/mozjs/type_traits.h"
#include "cobalt/script/mozjs/wrapper_factory.h"
#include "cobalt/script/mozjs/wrapper_private.h"
#include "third_party/mozjs/js/src/jsapi.h"
#include "third_party/mozjs/js/src/jsfriendapi.h"

namespace {
using cobalt::bindings::testing::CallbackFunctionInterface;
using cobalt::bindings::testing::MozjsCallbackFunctionInterface;
using cobalt::bindings::testing::ArbitraryInterface;
using cobalt::bindings::testing::MozjsArbitraryInterface;
using cobalt::script::CallbackInterfaceTraits;
using cobalt::script::GlobalObjectProxy;
using cobalt::script::OpaqueHandle;
using cobalt::script::OpaqueHandleHolder;
using cobalt::script::ScriptObject;
using cobalt::script::Wrappable;

using cobalt::script::CallbackFunction;
using cobalt::script::CallbackInterfaceTraits;
using cobalt::script::mozjs::FromJSValue;
using cobalt::script::mozjs::kConversionFlagNullable;
using cobalt::script::mozjs::kConversionFlagRestricted;
using cobalt::script::mozjs::kConversionFlagTreatNullAsEmptyString;
using cobalt::script::mozjs::kConversionFlagTreatUndefinedAsEmptyString;
using cobalt::script::mozjs::kNoConversionFlags;
using cobalt::script::mozjs::InterfaceData;
using cobalt::script::mozjs::MozjsCallbackFunction;
using cobalt::script::mozjs::MozjsExceptionState;
using cobalt::script::mozjs::MozjsGlobalObjectProxy;
using cobalt::script::mozjs::MozjsObjectHandleHolder;
using cobalt::script::mozjs::ToJSValue;
using cobalt::script::mozjs::TypeTraits;
using cobalt::script::mozjs::WrapperPrivate;
using cobalt::script::mozjs::WrapperFactory;
using cobalt::script::Wrappable;
}  // namespace

namespace cobalt {
namespace bindings {
namespace testing {

namespace {

InterfaceData* CreateCachedInterfaceData() {
  InterfaceData* interface_data = new InterfaceData();
  memset(&interface_data->instance_class_definition, 0,
         sizeof(interface_data->instance_class_definition));
  memset(&interface_data->prototype_class_definition, 0,
         sizeof(interface_data->prototype_class_definition));
  memset(&interface_data->interface_object_class_definition, 0,
         sizeof(interface_data->interface_object_class_definition));

  JSClass* instance_class = &interface_data->instance_class_definition;
  const int kGlobalFlags = 0;
  instance_class->name = "CallbackFunctionInterface";
  instance_class->flags = kGlobalFlags | JSCLASS_HAS_PRIVATE;
  instance_class->addProperty = JS_PropertyStub;
  instance_class->delProperty = JS_DeletePropertyStub;
  instance_class->getProperty = JS_PropertyStub;
  instance_class->setProperty = JS_StrictPropertyStub;
  instance_class->enumerate = JS_EnumerateStub;
  instance_class->resolve = JS_ResolveStub;
  instance_class->convert = JS_ConvertStub;
  // Function to be called before on object of this class is garbage collected.
  instance_class->finalize = &WrapperPrivate::Finalizer;
  // Called to trace objects that can be referenced from this object.
  instance_class->trace = &WrapperPrivate::Trace;

  JSClass* prototype_class = &interface_data->prototype_class_definition;
  prototype_class->name = "CallbackFunctionInterfacePrototype";
  prototype_class->flags = 0;
  prototype_class->addProperty = JS_PropertyStub;
  prototype_class->delProperty = JS_DeletePropertyStub;
  prototype_class->getProperty = JS_PropertyStub;
  prototype_class->setProperty = JS_StrictPropertyStub;
  prototype_class->enumerate = JS_EnumerateStub;
  prototype_class->resolve = JS_ResolveStub;
  prototype_class->convert = JS_ConvertStub;

  JSClass* interface_object_class = &interface_data->interface_object_class_definition;
  interface_object_class->name = "CallbackFunctionInterfaceConstructor";
  interface_object_class->flags = 0;
  interface_object_class->addProperty = JS_PropertyStub;
  interface_object_class->delProperty = JS_DeletePropertyStub;
  interface_object_class->getProperty = JS_PropertyStub;
  interface_object_class->setProperty = JS_StrictPropertyStub;
  interface_object_class->enumerate = JS_EnumerateStub;
  interface_object_class->resolve = JS_ResolveStub;
  interface_object_class->convert = JS_ConvertStub;
  return interface_data;
}

JSBool get_callbackAttribute(
    JSContext* context, JS::HandleObject object, JS::HandleId id,
    JS::MutableHandleValue vp) {
  MozjsExceptionState exception_state(context);
  JS::RootedValue result_value(context);
  CallbackFunctionInterface* impl =
      WrapperPrivate::GetWrappable<CallbackFunctionInterface>(object);
  TypeTraits<CallbackFunctionInterface::VoidFunction >::ReturnType value =
      impl->callback_attribute();
  if (!exception_state.IsExceptionSet()) {
    ToJSValue(context, value, &exception_state, &result_value);
  }

  if (!exception_state.IsExceptionSet()) {
    vp.set(result_value);
  }
  return !exception_state.IsExceptionSet();
}

JSBool set_callbackAttribute(
    JSContext* context, JS::HandleObject object, JS::HandleId id,
    JSBool strict, JS::MutableHandleValue vp) {
  MozjsExceptionState exception_state(context);
  JS::RootedValue result_value(context);
  TypeTraits<CallbackFunctionInterface::VoidFunction >::ConversionType value;
  FromJSValue(context, vp, kNoConversionFlags, &exception_state,
              &value);
  if (exception_state.IsExceptionSet()) {
    return false;
  }
  CallbackFunctionInterface* impl =
      WrapperPrivate::GetWrappable<CallbackFunctionInterface>(object);
  impl->set_callback_attribute(value);
  result_value.set(JS::UndefinedHandleValue);

  return !exception_state.IsExceptionSet();
}

JSBool get_nullableCallbackAttribute(
    JSContext* context, JS::HandleObject object, JS::HandleId id,
    JS::MutableHandleValue vp) {
  MozjsExceptionState exception_state(context);
  JS::RootedValue result_value(context);
  CallbackFunctionInterface* impl =
      WrapperPrivate::GetWrappable<CallbackFunctionInterface>(object);
  TypeTraits<CallbackFunctionInterface::VoidFunction >::ReturnType value =
      impl->nullable_callback_attribute();
  if (!exception_state.IsExceptionSet()) {
    ToJSValue(context, value, &exception_state, &result_value);
  }

  if (!exception_state.IsExceptionSet()) {
    vp.set(result_value);
  }
  return !exception_state.IsExceptionSet();
}

JSBool set_nullableCallbackAttribute(
    JSContext* context, JS::HandleObject object, JS::HandleId id,
    JSBool strict, JS::MutableHandleValue vp) {
  MozjsExceptionState exception_state(context);
  JS::RootedValue result_value(context);
  TypeTraits<CallbackFunctionInterface::VoidFunction >::ConversionType value;
  FromJSValue(context, vp, (kConversionFlagNullable), &exception_state,
              &value);
  if (exception_state.IsExceptionSet()) {
    return false;
  }
  CallbackFunctionInterface* impl =
      WrapperPrivate::GetWrappable<CallbackFunctionInterface>(object);
  impl->set_nullable_callback_attribute(value);
  result_value.set(JS::UndefinedHandleValue);

  return !exception_state.IsExceptionSet();
}

JSBool fcn_takesFunctionThatReturnsString(
    JSContext* context, uint32_t argc, JS::Value *vp) {
  MozjsExceptionState exception_state(context);
  JS::RootedValue result_value(context);

  // Compute the 'this' value.
  JS::RootedValue this_value(context, JS_ComputeThis(context, vp));
  // 'this' should be an object.
  JS::RootedObject object(context);
  if (JS_TypeOfValue(context, this_value) != JSTYPE_OBJECT) {
    NOTREACHED();
    return false;
  }
  if (!JS_ValueToObject(context, this_value, object.address())) {
    NOTREACHED();
    return false;
  }

  JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
  const size_t kMinArguments = 1;
  if (args.length() < kMinArguments) {
    exception_state.SetSimpleException(
        script::ExceptionState::kTypeError, "Not enough arguments.");
    return false;
  }
  TypeTraits<CallbackFunctionInterface::FunctionThatReturnsString >::ConversionType cb;
  DCHECK_LT(0, args.length());
  FromJSValue(context, args.handleAt(0),
      kNoConversionFlags, &exception_state, &cb);
  if (exception_state.IsExceptionSet()) {
    return false;
  }
  CallbackFunctionInterface* impl =
      WrapperPrivate::GetWrappable<CallbackFunctionInterface>(object);
  impl->TakesFunctionThatReturnsString(cb);
  result_value.set(JS::UndefinedHandleValue);

  if (!exception_state.IsExceptionSet()) {
    args.rval().set(result_value);
  }
  return !exception_state.IsExceptionSet();
}

JSBool fcn_takesFunctionWithNullableParameters(
    JSContext* context, uint32_t argc, JS::Value *vp) {
  MozjsExceptionState exception_state(context);
  JS::RootedValue result_value(context);

  // Compute the 'this' value.
  JS::RootedValue this_value(context, JS_ComputeThis(context, vp));
  // 'this' should be an object.
  JS::RootedObject object(context);
  if (JS_TypeOfValue(context, this_value) != JSTYPE_OBJECT) {
    NOTREACHED();
    return false;
  }
  if (!JS_ValueToObject(context, this_value, object.address())) {
    NOTREACHED();
    return false;
  }

  JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
  const size_t kMinArguments = 1;
  if (args.length() < kMinArguments) {
    exception_state.SetSimpleException(
        script::ExceptionState::kTypeError, "Not enough arguments.");
    return false;
  }
  TypeTraits<CallbackFunctionInterface::FunctionWithNullableParameters >::ConversionType cb;
  DCHECK_LT(0, args.length());
  FromJSValue(context, args.handleAt(0),
      kNoConversionFlags, &exception_state, &cb);
  if (exception_state.IsExceptionSet()) {
    return false;
  }
  CallbackFunctionInterface* impl =
      WrapperPrivate::GetWrappable<CallbackFunctionInterface>(object);
  impl->TakesFunctionWithNullableParameters(cb);
  result_value.set(JS::UndefinedHandleValue);

  if (!exception_state.IsExceptionSet()) {
    args.rval().set(result_value);
  }
  return !exception_state.IsExceptionSet();
}

JSBool fcn_takesFunctionWithOneParameter(
    JSContext* context, uint32_t argc, JS::Value *vp) {
  MozjsExceptionState exception_state(context);
  JS::RootedValue result_value(context);

  // Compute the 'this' value.
  JS::RootedValue this_value(context, JS_ComputeThis(context, vp));
  // 'this' should be an object.
  JS::RootedObject object(context);
  if (JS_TypeOfValue(context, this_value) != JSTYPE_OBJECT) {
    NOTREACHED();
    return false;
  }
  if (!JS_ValueToObject(context, this_value, object.address())) {
    NOTREACHED();
    return false;
  }

  JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
  const size_t kMinArguments = 1;
  if (args.length() < kMinArguments) {
    exception_state.SetSimpleException(
        script::ExceptionState::kTypeError, "Not enough arguments.");
    return false;
  }
  TypeTraits<CallbackFunctionInterface::FunctionWithOneParameter >::ConversionType cb;
  DCHECK_LT(0, args.length());
  FromJSValue(context, args.handleAt(0),
      kNoConversionFlags, &exception_state, &cb);
  if (exception_state.IsExceptionSet()) {
    return false;
  }
  CallbackFunctionInterface* impl =
      WrapperPrivate::GetWrappable<CallbackFunctionInterface>(object);
  impl->TakesFunctionWithOneParameter(cb);
  result_value.set(JS::UndefinedHandleValue);

  if (!exception_state.IsExceptionSet()) {
    args.rval().set(result_value);
  }
  return !exception_state.IsExceptionSet();
}

JSBool fcn_takesFunctionWithSeveralParameters(
    JSContext* context, uint32_t argc, JS::Value *vp) {
  MozjsExceptionState exception_state(context);
  JS::RootedValue result_value(context);

  // Compute the 'this' value.
  JS::RootedValue this_value(context, JS_ComputeThis(context, vp));
  // 'this' should be an object.
  JS::RootedObject object(context);
  if (JS_TypeOfValue(context, this_value) != JSTYPE_OBJECT) {
    NOTREACHED();
    return false;
  }
  if (!JS_ValueToObject(context, this_value, object.address())) {
    NOTREACHED();
    return false;
  }

  JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
  const size_t kMinArguments = 1;
  if (args.length() < kMinArguments) {
    exception_state.SetSimpleException(
        script::ExceptionState::kTypeError, "Not enough arguments.");
    return false;
  }
  TypeTraits<CallbackFunctionInterface::FunctionWithSeveralParameters >::ConversionType cb;
  DCHECK_LT(0, args.length());
  FromJSValue(context, args.handleAt(0),
      kNoConversionFlags, &exception_state, &cb);
  if (exception_state.IsExceptionSet()) {
    return false;
  }
  CallbackFunctionInterface* impl =
      WrapperPrivate::GetWrappable<CallbackFunctionInterface>(object);
  impl->TakesFunctionWithSeveralParameters(cb);
  result_value.set(JS::UndefinedHandleValue);

  if (!exception_state.IsExceptionSet()) {
    args.rval().set(result_value);
  }
  return !exception_state.IsExceptionSet();
}

JSBool fcn_takesVoidFunction(
    JSContext* context, uint32_t argc, JS::Value *vp) {
  MozjsExceptionState exception_state(context);
  JS::RootedValue result_value(context);

  // Compute the 'this' value.
  JS::RootedValue this_value(context, JS_ComputeThis(context, vp));
  // 'this' should be an object.
  JS::RootedObject object(context);
  if (JS_TypeOfValue(context, this_value) != JSTYPE_OBJECT) {
    NOTREACHED();
    return false;
  }
  if (!JS_ValueToObject(context, this_value, object.address())) {
    NOTREACHED();
    return false;
  }

  JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
  const size_t kMinArguments = 1;
  if (args.length() < kMinArguments) {
    exception_state.SetSimpleException(
        script::ExceptionState::kTypeError, "Not enough arguments.");
    return false;
  }
  TypeTraits<CallbackFunctionInterface::VoidFunction >::ConversionType cb;
  DCHECK_LT(0, args.length());
  FromJSValue(context, args.handleAt(0),
      kNoConversionFlags, &exception_state, &cb);
  if (exception_state.IsExceptionSet()) {
    return false;
  }
  CallbackFunctionInterface* impl =
      WrapperPrivate::GetWrappable<CallbackFunctionInterface>(object);
  impl->TakesVoidFunction(cb);
  result_value.set(JS::UndefinedHandleValue);

  if (!exception_state.IsExceptionSet()) {
    args.rval().set(result_value);
  }
  return !exception_state.IsExceptionSet();
}


const JSPropertySpec prototype_properties[] = {
  {  // Read/Write property
      "callbackAttribute", 0,
      JSPROP_SHARED | JSPROP_ENUMERATE,
      JSOP_WRAPPER(&get_callbackAttribute),
      JSOP_WRAPPER(&set_callbackAttribute),
  },
  {  // Read/Write property
      "nullableCallbackAttribute", 0,
      JSPROP_SHARED | JSPROP_ENUMERATE,
      JSOP_WRAPPER(&get_nullableCallbackAttribute),
      JSOP_WRAPPER(&set_nullableCallbackAttribute),
  },
  JS_PS_END
};

const JSFunctionSpec prototype_functions[] = {
  {
      "takesFunctionThatReturnsString",
      JSOP_WRAPPER(&fcn_takesFunctionThatReturnsString),
      1,
      JSPROP_ENUMERATE,
      NULL,
  },
  {
      "takesFunctionWithNullableParameters",
      JSOP_WRAPPER(&fcn_takesFunctionWithNullableParameters),
      1,
      JSPROP_ENUMERATE,
      NULL,
  },
  {
      "takesFunctionWithOneParameter",
      JSOP_WRAPPER(&fcn_takesFunctionWithOneParameter),
      1,
      JSPROP_ENUMERATE,
      NULL,
  },
  {
      "takesFunctionWithSeveralParameters",
      JSOP_WRAPPER(&fcn_takesFunctionWithSeveralParameters),
      1,
      JSPROP_ENUMERATE,
      NULL,
  },
  {
      "takesVoidFunction",
      JSOP_WRAPPER(&fcn_takesVoidFunction),
      1,
      JSPROP_ENUMERATE,
      NULL,
  },
  JS_FS_END
};

const JSPropertySpec own_properties[] = {
  JS_PS_END
};

void InitializePrototypeAndInterfaceObject(
    InterfaceData* interface_data, JSContext* context) {
  DCHECK(!interface_data->prototype);
  DCHECK(!interface_data->interface_object);

  MozjsGlobalObjectProxy* global_object_proxy =
      static_cast<MozjsGlobalObjectProxy*>(JS_GetContextPrivate(context));
  JS::RootedObject global_object(context, global_object_proxy->global_object());
  DCHECK(global_object);
  JS::RootedObject parent_prototype(
      context, JS_GetObjectPrototype(context, global_object));
  DCHECK(parent_prototype);

  // Create the Prototype object.
  interface_data->prototype = JS_NewObjectWithGivenProto(
      context, &interface_data->prototype_class_definition, parent_prototype, NULL);
  bool success = JS_DefineProperties(
      context, interface_data->prototype, prototype_properties);
  DCHECK(success);
  success = JS_DefineFunctions(
      context, interface_data->prototype, prototype_functions);
  DCHECK(success);

  JS::RootedObject function_prototype(
      context, JS_GetFunctionPrototype(context, global_object));
  DCHECK(function_prototype);
  // Create the Interface object.
  interface_data->interface_object = JS_NewObjectWithGivenProto(
      context, &interface_data->interface_object_class_definition,
      function_prototype, NULL);

  // Add the InterfaceObject.name property.
  JS::RootedObject rooted_interface_object(
      context, interface_data->interface_object);
  JS::RootedValue name_value(context);
  const char name[] = "CallbackFunctionInterface";
  name_value.setString(JS_NewStringCopyZ(context, "CallbackFunctionInterface"));
  success =
      JS_DefineProperty(context, rooted_interface_object, "name", name_value,
                        JS_PropertyStub, JS_StrictPropertyStub,
                        JSPROP_READONLY);
  DCHECK(success);

  // Set the Prototype.constructor and Constructor.prototype properties.
  DCHECK(interface_data->interface_object);
  DCHECK(interface_data->prototype);
  JS::RootedObject rooted_prototype(context, interface_data->prototype);
  success = JS_LinkConstructorAndPrototype(
      context,
      rooted_interface_object,
      rooted_prototype);
  DCHECK(success);
}

InterfaceData* GetInterfaceData(JSContext* context) {
  MozjsGlobalObjectProxy* global_object_proxy =
      static_cast<MozjsGlobalObjectProxy*>(JS_GetContextPrivate(context));
  // Use the address of the properties definition for this interface as a
  // unique key for looking up the InterfaceData for this interface.
  intptr_t key = reinterpret_cast<intptr_t>(&own_properties);
  InterfaceData* interface_data = global_object_proxy->GetInterfaceData(key);
  if (!interface_data) {
    interface_data = CreateCachedInterfaceData();
    DCHECK(interface_data);
    global_object_proxy->CacheInterfaceData(key, interface_data);
    DCHECK_EQ(interface_data, global_object_proxy->GetInterfaceData(key));
  }
  return interface_data;
}

}  // namespace

// static
JSObject* MozjsCallbackFunctionInterface::CreateInstance(
    JSContext* context, const scoped_refptr<Wrappable>& wrappable) {
  InterfaceData* interface_data = GetInterfaceData(context);
  JS::RootedObject prototype(context, GetPrototype(context));
  DCHECK(prototype);
  JS::RootedObject new_object(context, JS_NewObjectWithGivenProto(
      context, &interface_data->instance_class_definition, prototype, NULL));
  DCHECK(new_object);
  WrapperPrivate::AddPrivateData(new_object, wrappable);
  return new_object;
}

// static
JSObject* MozjsCallbackFunctionInterface::GetPrototype(JSContext* context) {
  InterfaceData* interface_data = GetInterfaceData(context);
  if (!interface_data->prototype) {
    // Create new prototype that has all the props and methods
    InitializePrototypeAndInterfaceObject(interface_data, context);
  }
  DCHECK(interface_data->prototype);
  return interface_data->prototype;
}

// static
JSObject* MozjsCallbackFunctionInterface::GetInterfaceObject(JSContext* context) {
  InterfaceData* interface_data = GetInterfaceData(context);
  if (!interface_data->interface_object) {
    InitializePrototypeAndInterfaceObject(interface_data, context);
  }
  DCHECK(interface_data->interface_object);
  return interface_data->interface_object;
}


namespace {
}  // namespace


}  // namespace testing
}  // namespace bindings
}  // namespace cobalt
