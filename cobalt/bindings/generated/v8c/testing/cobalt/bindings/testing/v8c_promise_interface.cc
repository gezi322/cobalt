

// Copyright 2017 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// clang-format off

// This file has been auto-generated by bindings/code_generator_cobalt.py. DO NOT MODIFY!
// Auto-generated from template: bindings/v8c/templates/interface.cc.template

#include "cobalt/bindings/testing/v8c_promise_interface.h"

#include "base/debug/trace_event.h"
#include "cobalt/base/polymorphic_downcast.h"
#include "cobalt/script/global_environment.h"
#include "cobalt/script/script_value.h"
#include "cobalt/script/value_handle.h"

#include "v8c_gen_type_conversion.h"

#include "cobalt/script/callback_interface_traits.h"
#include "cobalt/script/v8c/callback_function_conversion.h"
#include "cobalt/script/v8c/conversion_helpers.h"
#include "cobalt/script/v8c/native_promise.h"
#include "cobalt/script/v8c/type_traits.h"
#include "cobalt/script/v8c/v8c_callback_function.h"
#include "cobalt/script/v8c/v8c_callback_interface_holder.h"
#include "cobalt/script/v8c/v8c_exception_state.h"
#include "cobalt/script/v8c/v8c_global_environment.h"
#include "cobalt/script/v8c/v8c_value_handle.h"
#include "cobalt/script/v8c/wrapper_private.h"
#include "v8/include/v8.h"


namespace {
using cobalt::bindings::testing::PromiseInterface;
using cobalt::bindings::testing::V8cPromiseInterface;
using cobalt::script::CallbackInterfaceTraits;
using cobalt::script::GlobalEnvironment;
using cobalt::script::ScriptValue;
using cobalt::script::ValueHandle;
using cobalt::script::ValueHandle;
using cobalt::script::ValueHandleHolder;
using cobalt::script::Wrappable;

using cobalt::script::v8c::FromJSValue;
using cobalt::script::v8c::InterfaceData;
using cobalt::script::v8c::kConversionFlagClamped;
using cobalt::script::v8c::kConversionFlagNullable;
using cobalt::script::v8c::kConversionFlagRestricted;
using cobalt::script::v8c::kConversionFlagTreatNullAsEmptyString;
using cobalt::script::v8c::kConversionFlagTreatUndefinedAsEmptyString;
using cobalt::script::v8c::kNoConversionFlags;
using cobalt::script::v8c::TypeTraits;
using cobalt::script::v8c::V8cExceptionState;
using cobalt::script::v8c::V8cGlobalEnvironment;
using cobalt::script::v8c::WrapperFactory;
using cobalt::script::v8c::WrapperPrivate;

v8::Local<v8::Object> DummyFunctor(V8cGlobalEnvironment*, const scoped_refptr<Wrappable>&) {
  NOTIMPLEMENTED();
  return {};
}

}  // namespace

namespace cobalt {
namespace bindings {
namespace testing {


namespace {

void PromiseInterfaceConstructor(const v8::FunctionCallbackInfo<v8::Value>& args) {
  NOTIMPLEMENTED();
  if (!args.IsConstructCall()) {
    // TODO: Probably throw something here...
    return;
  }

  DCHECK(args.This()->InternalFieldCount() == 1);
  args.This()->SetInternalField(0, v8::External::New(args.GetIsolate(), nullptr));
  args.GetReturnValue().Set(args.This());
}


void DummyFunction(const v8::FunctionCallbackInfo<v8::Value>& info) {
  LOG(INFO) << __func__;
}

void InitializeTemplate(
  V8cGlobalEnvironment* env,
  InterfaceData* interface_data) {
  v8::Isolate* isolate = env->isolate();
  v8::Local<v8::FunctionTemplate> function_template = v8::FunctionTemplate::New(
    isolate);
  function_template->SetClassName(
    v8::String::NewFromUtf8(isolate, "PromiseInterface",
        v8::NewStringType::kInternalized).ToLocalChecked());
  v8::Local<v8::ObjectTemplate> instance_template = function_template->InstanceTemplate();
  instance_template->SetInternalFieldCount(1);

  v8::Local<v8::ObjectTemplate> prototype_template = function_template->PrototypeTemplate();
  prototype_template->SetInternalFieldCount(1);


  instance_template->Set(
      v8::String::NewFromUtf8(
          isolate,
          "onSuccess",
          v8::NewStringType::kInternalized).ToLocalChecked(),
      v8::FunctionTemplate::New(isolate, DummyFunction)
  );
  instance_template->Set(
      v8::String::NewFromUtf8(
          isolate,
          "returnBooleanPromise",
          v8::NewStringType::kInternalized).ToLocalChecked(),
      v8::FunctionTemplate::New(isolate, DummyFunction)
  );
  instance_template->Set(
      v8::String::NewFromUtf8(
          isolate,
          "returnInterfacePromise",
          v8::NewStringType::kInternalized).ToLocalChecked(),
      v8::FunctionTemplate::New(isolate, DummyFunction)
  );
  instance_template->Set(
      v8::String::NewFromUtf8(
          isolate,
          "returnStringPromise",
          v8::NewStringType::kInternalized).ToLocalChecked(),
      v8::FunctionTemplate::New(isolate, DummyFunction)
  );
  instance_template->Set(
      v8::String::NewFromUtf8(
          isolate,
          "returnVoidPromise",
          v8::NewStringType::kInternalized).ToLocalChecked(),
      v8::FunctionTemplate::New(isolate, DummyFunction)
  );

  interface_data->templ.Set(env->isolate(), function_template);
}

inline InterfaceData* GetInterfaceData(V8cGlobalEnvironment* env) {
  const int kInterfaceUniqueId = 40;
  // By convention, the |V8cGlobalEnvironment| that we are associated with
  // will hold our |InterfaceData| at index |kInterfaceUniqueId|, as we asked
  // for it to be there in the first place, and could not have conflicted with
  // any other interface.
  return env->GetInterfaceData(kInterfaceUniqueId);
}

}  // namespace

v8::Local<v8::Object> V8cPromiseInterface::CreateWrapper(V8cGlobalEnvironment* env, const scoped_refptr<Wrappable>& wrappable) {
  v8::Isolate* isolate = env->isolate();
  v8::Isolate::Scope isolate_scope(isolate);
  v8::EscapableHandleScope handle_scope(isolate);
  v8::Local<v8::Context> context = env->context();
  v8::Context::Scope scope(context);

  InterfaceData* interface_data = GetInterfaceData(env);
  if (interface_data->templ.IsEmpty()) {
    InitializeTemplate(env, interface_data);
  }
  DCHECK(!interface_data->templ.IsEmpty());

  v8::Local<v8::FunctionTemplate> function_template = interface_data->templ.Get(isolate);
  DCHECK(function_template->InstanceTemplate()->InternalFieldCount() == 1);
  v8::Local<v8::Object> object = function_template->InstanceTemplate()->NewInstance(context).ToLocalChecked();
  DCHECK(object->InternalFieldCount() == 1);

  // |WrapperPrivate|'s lifetime will be managed by V8.
  new WrapperPrivate(isolate, wrappable, object);
  return handle_scope.Escape(object);
}

v8::Local<v8::FunctionTemplate> V8cPromiseInterface::CreateTemplate(V8cGlobalEnvironment* env) {
  InterfaceData* interface_data = GetInterfaceData(env);
  if (interface_data->templ.IsEmpty()) {
    InitializeTemplate(env, interface_data);
  }

  return interface_data->templ.Get(env->isolate());
}


}  // namespace testing
}  // namespace bindings
}  // namespace cobalt


