// This file was GENERATED by command:
//     pump.py mozjs_callback_function.h.pump
// DO NOT EDIT BY HAND!!!

// Copyright 2016 Google Inc. All Rights Reserved.
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

#ifndef COBALT_SCRIPT_MOZJS_45_MOZJS_CALLBACK_FUNCTION_H_
#define COBALT_SCRIPT_MOZJS_45_MOZJS_CALLBACK_FUNCTION_H_

#include "base/logging.h"
#include "cobalt/script/callback_function.h"
#include "cobalt/script/mozjs-45/conversion_helpers.h"
#include "cobalt/script/mozjs-45/convert_callback_return_value.h"
#include "cobalt/script/mozjs-45/util/exception_helpers.h"
#include "cobalt/script/mozjs-45/util/stack_trace_helpers.h"
#include "cobalt/script/mozjs-45/weak_heap_object.h"
#include "nb/memory_scope.h"
#include "third_party/mozjs-45/js/src/jsapi.h"
#include "third_party/mozjs-45/js/src/jscntxt.h"

namespace cobalt {
namespace script {
namespace mozjs {

// First, we forward declare the Callback class template. This informs the
// compiler that the template only has 1 type parameter which is the base
// CallbackFunction template class with parameters.
//
// See base/callback.h.pump for further discussion on this pattern.
template <typename Sig>
class MozjsCallbackFunction;

template <typename R>
class MozjsCallbackFunction<R(void)>
    : public CallbackFunction<R(void)> {
 public:
  typedef CallbackFunction<R()> BaseType;

  explicit MozjsCallbackFunction(JSContext* context, JS::HandleValue function)
      : context_(context), weak_function_(context, function) {
    DCHECK(context_);
    DCHECK(function.isObject());
    DCHECK(JS_ObjectIsFunction(context_, function.toObjectOrNull()));
  }

  CallbackResult<R> Run()
      const override {
    ENABLE_JS_STACK_TRACE_IN_SCOPE(context_);
    TRACK_MEMORY_SCOPE("Javascript");
    TRACE_EVENT0("cobalt::script::mozjs", "MozjsCallbackFunction::Run");
    CallbackResult<R> callback_result;
    JSAutoRequest auto_request(context_);
    JS::RootedObject function(context_, weak_function_.GetObject());
    if (!function) {
      DLOG(WARNING) << "Function was garbage collected.";
      callback_result.exception = true;
      return callback_result;
    }

    JSAutoCompartment auto_compartment(context_, function);
    JS::AutoSaveExceptionState auto_save_exception_state(context_);

    // https://www.w3.org/TR/WebIDL/#es-invoking-callback-functions
    // Callback 'this' is set to null, unless overridden by other specifications
    JS::RootedValue this_value(context_, JS::NullValue());
    JS::RootedValue return_value(context_);
    const int kNumArguments = 0;
    JS::AutoValueVector args(context_);
    bool call_result = JS::Call(context_, this_value, function, args,
        &return_value);
    if (!call_result) {
      DLOG(WARNING) << "Exception in callback: "
                    << util::GetExceptionString(context_);
      callback_result.exception = true;
    } else {
      callback_result = ConvertCallbackReturnValue<R>(context_, return_value);
    }
    return callback_result;
  }

  JSObject* handle() const { return weak_function_.GetObject(); }
  const JS::Value& value() const { return weak_function_.GetValue(); }
  bool WasCollected() const { return weak_function_.WasCollected(); }
  void Trace(JSTracer* js_tracer) { weak_function_.Trace(js_tracer); }

 private:
  JSContext* context_;
  WeakHeapObject weak_function_;
};

template <typename R, typename A1>
class MozjsCallbackFunction<R(A1)>
    : public CallbackFunction<R(A1)> {
 public:
  typedef CallbackFunction<R(A1)> BaseType;

  explicit MozjsCallbackFunction(JSContext* context, JS::HandleValue function)
      : context_(context), weak_function_(context, function) {
    DCHECK(context_);
    DCHECK(function.isObject());
    DCHECK(JS_ObjectIsFunction(context_, function.toObjectOrNull()));
  }

  CallbackResult<R> Run(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1)
      const override {
    ENABLE_JS_STACK_TRACE_IN_SCOPE(context_);
    TRACK_MEMORY_SCOPE("Javascript");
    TRACE_EVENT0("cobalt::script::mozjs", "MozjsCallbackFunction::Run");
    CallbackResult<R> callback_result;
    JSAutoRequest auto_request(context_);
    JS::RootedObject function(context_, weak_function_.GetObject());
    if (!function) {
      DLOG(WARNING) << "Function was garbage collected.";
      callback_result.exception = true;
      return callback_result;
    }

    JSAutoCompartment auto_compartment(context_, function);
    JS::AutoSaveExceptionState auto_save_exception_state(context_);

    // https://www.w3.org/TR/WebIDL/#es-invoking-callback-functions
    // Callback 'this' is set to null, unless overridden by other specifications
    JS::RootedValue this_value(context_, JS::NullValue());
    JS::RootedValue return_value(context_);
    const int kNumArguments = 1;
    JS::AutoValueArray<1> args(context_);
    ToJSValue(context_, a1, args[0]);

    bool call_result = JS::Call(context_, this_value, function, args,
        &return_value);
    if (!call_result) {
      DLOG(WARNING) << "Exception in callback: "
                    << util::GetExceptionString(context_);
      callback_result.exception = true;
    } else {
      callback_result = ConvertCallbackReturnValue<R>(context_, return_value);
    }
    return callback_result;
  }

  JSObject* handle() const { return weak_function_.GetObject(); }
  const JS::Value& value() const { return weak_function_.GetValue(); }
  bool WasCollected() const { return weak_function_.WasCollected(); }
  void Trace(JSTracer* js_tracer) { weak_function_.Trace(js_tracer); }

 private:
  JSContext* context_;
  WeakHeapObject weak_function_;
};

template <typename R, typename A1, typename A2>
class MozjsCallbackFunction<R(A1, A2)>
    : public CallbackFunction<R(A1, A2)> {
 public:
  typedef CallbackFunction<R(A1, A2)> BaseType;

  explicit MozjsCallbackFunction(JSContext* context, JS::HandleValue function)
      : context_(context), weak_function_(context, function) {
    DCHECK(context_);
    DCHECK(function.isObject());
    DCHECK(JS_ObjectIsFunction(context_, function.toObjectOrNull()));
  }

  CallbackResult<R> Run(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1,
      typename base::internal::CallbackParamTraits<A2>::ForwardType a2)
      const override {
    ENABLE_JS_STACK_TRACE_IN_SCOPE(context_);
    TRACK_MEMORY_SCOPE("Javascript");
    TRACE_EVENT0("cobalt::script::mozjs", "MozjsCallbackFunction::Run");
    CallbackResult<R> callback_result;
    JSAutoRequest auto_request(context_);
    JS::RootedObject function(context_, weak_function_.GetObject());
    if (!function) {
      DLOG(WARNING) << "Function was garbage collected.";
      callback_result.exception = true;
      return callback_result;
    }

    JSAutoCompartment auto_compartment(context_, function);
    JS::AutoSaveExceptionState auto_save_exception_state(context_);

    // https://www.w3.org/TR/WebIDL/#es-invoking-callback-functions
    // Callback 'this' is set to null, unless overridden by other specifications
    JS::RootedValue this_value(context_, JS::NullValue());
    JS::RootedValue return_value(context_);
    const int kNumArguments = 2;
    JS::AutoValueArray<2> args(context_);
    ToJSValue(context_, a1, args[0]);
    ToJSValue(context_, a2, args[1]);

    bool call_result = JS::Call(context_, this_value, function, args,
        &return_value);
    if (!call_result) {
      DLOG(WARNING) << "Exception in callback: "
                    << util::GetExceptionString(context_);
      callback_result.exception = true;
    } else {
      callback_result = ConvertCallbackReturnValue<R>(context_, return_value);
    }
    return callback_result;
  }

  JSObject* handle() const { return weak_function_.GetObject(); }
  const JS::Value& value() const { return weak_function_.GetValue(); }
  bool WasCollected() const { return weak_function_.WasCollected(); }
  void Trace(JSTracer* js_tracer) { weak_function_.Trace(js_tracer); }

 private:
  JSContext* context_;
  WeakHeapObject weak_function_;
};

template <typename R, typename A1, typename A2, typename A3>
class MozjsCallbackFunction<R(A1, A2, A3)>
    : public CallbackFunction<R(A1, A2, A3)> {
 public:
  typedef CallbackFunction<R(A1, A2, A3)> BaseType;

  explicit MozjsCallbackFunction(JSContext* context, JS::HandleValue function)
      : context_(context), weak_function_(context, function) {
    DCHECK(context_);
    DCHECK(function.isObject());
    DCHECK(JS_ObjectIsFunction(context_, function.toObjectOrNull()));
  }

  CallbackResult<R> Run(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1,
      typename base::internal::CallbackParamTraits<A2>::ForwardType a2,
      typename base::internal::CallbackParamTraits<A3>::ForwardType a3)
      const override {
    ENABLE_JS_STACK_TRACE_IN_SCOPE(context_);
    TRACK_MEMORY_SCOPE("Javascript");
    TRACE_EVENT0("cobalt::script::mozjs", "MozjsCallbackFunction::Run");
    CallbackResult<R> callback_result;
    JSAutoRequest auto_request(context_);
    JS::RootedObject function(context_, weak_function_.GetObject());
    if (!function) {
      DLOG(WARNING) << "Function was garbage collected.";
      callback_result.exception = true;
      return callback_result;
    }

    JSAutoCompartment auto_compartment(context_, function);
    JS::AutoSaveExceptionState auto_save_exception_state(context_);

    // https://www.w3.org/TR/WebIDL/#es-invoking-callback-functions
    // Callback 'this' is set to null, unless overridden by other specifications
    JS::RootedValue this_value(context_, JS::NullValue());
    JS::RootedValue return_value(context_);
    const int kNumArguments = 3;
    JS::AutoValueArray<3> args(context_);
    ToJSValue(context_, a1, args[0]);
    ToJSValue(context_, a2, args[1]);
    ToJSValue(context_, a3, args[2]);

    bool call_result = JS::Call(context_, this_value, function, args,
        &return_value);
    if (!call_result) {
      DLOG(WARNING) << "Exception in callback: "
                    << util::GetExceptionString(context_);
      callback_result.exception = true;
    } else {
      callback_result = ConvertCallbackReturnValue<R>(context_, return_value);
    }
    return callback_result;
  }

  JSObject* handle() const { return weak_function_.GetObject(); }
  const JS::Value& value() const { return weak_function_.GetValue(); }
  bool WasCollected() const { return weak_function_.WasCollected(); }
  void Trace(JSTracer* js_tracer) { weak_function_.Trace(js_tracer); }

 private:
  JSContext* context_;
  WeakHeapObject weak_function_;
};

template <typename R, typename A1, typename A2, typename A3, typename A4>
class MozjsCallbackFunction<R(A1, A2, A3, A4)>
    : public CallbackFunction<R(A1, A2, A3, A4)> {
 public:
  typedef CallbackFunction<R(A1, A2, A3, A4)> BaseType;

  explicit MozjsCallbackFunction(JSContext* context, JS::HandleValue function)
      : context_(context), weak_function_(context, function) {
    DCHECK(context_);
    DCHECK(function.isObject());
    DCHECK(JS_ObjectIsFunction(context_, function.toObjectOrNull()));
  }

  CallbackResult<R> Run(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1,
      typename base::internal::CallbackParamTraits<A2>::ForwardType a2,
      typename base::internal::CallbackParamTraits<A3>::ForwardType a3,
      typename base::internal::CallbackParamTraits<A4>::ForwardType a4)
      const override {
    ENABLE_JS_STACK_TRACE_IN_SCOPE(context_);
    TRACK_MEMORY_SCOPE("Javascript");
    TRACE_EVENT0("cobalt::script::mozjs", "MozjsCallbackFunction::Run");
    CallbackResult<R> callback_result;
    JSAutoRequest auto_request(context_);
    JS::RootedObject function(context_, weak_function_.GetObject());
    if (!function) {
      DLOG(WARNING) << "Function was garbage collected.";
      callback_result.exception = true;
      return callback_result;
    }

    JSAutoCompartment auto_compartment(context_, function);
    JS::AutoSaveExceptionState auto_save_exception_state(context_);

    // https://www.w3.org/TR/WebIDL/#es-invoking-callback-functions
    // Callback 'this' is set to null, unless overridden by other specifications
    JS::RootedValue this_value(context_, JS::NullValue());
    JS::RootedValue return_value(context_);
    const int kNumArguments = 4;
    JS::AutoValueArray<4> args(context_);
    ToJSValue(context_, a1, args[0]);
    ToJSValue(context_, a2, args[1]);
    ToJSValue(context_, a3, args[2]);
    ToJSValue(context_, a4, args[3]);

    bool call_result = JS::Call(context_, this_value, function, args,
        &return_value);
    if (!call_result) {
      DLOG(WARNING) << "Exception in callback: "
                    << util::GetExceptionString(context_);
      callback_result.exception = true;
    } else {
      callback_result = ConvertCallbackReturnValue<R>(context_, return_value);
    }
    return callback_result;
  }

  JSObject* handle() const { return weak_function_.GetObject(); }
  const JS::Value& value() const { return weak_function_.GetValue(); }
  bool WasCollected() const { return weak_function_.WasCollected(); }
  void Trace(JSTracer* js_tracer) { weak_function_.Trace(js_tracer); }

 private:
  JSContext* context_;
  WeakHeapObject weak_function_;
};

template <typename R, typename A1, typename A2, typename A3, typename A4,
    typename A5>
class MozjsCallbackFunction<R(A1, A2, A3, A4, A5)>
    : public CallbackFunction<R(A1, A2, A3, A4, A5)> {
 public:
  typedef CallbackFunction<R(A1, A2, A3, A4, A5)> BaseType;

  explicit MozjsCallbackFunction(JSContext* context, JS::HandleValue function)
      : context_(context), weak_function_(context, function) {
    DCHECK(context_);
    DCHECK(function.isObject());
    DCHECK(JS_ObjectIsFunction(context_, function.toObjectOrNull()));
  }

  CallbackResult<R> Run(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1,
      typename base::internal::CallbackParamTraits<A2>::ForwardType a2,
      typename base::internal::CallbackParamTraits<A3>::ForwardType a3,
      typename base::internal::CallbackParamTraits<A4>::ForwardType a4,
      typename base::internal::CallbackParamTraits<A5>::ForwardType a5)
      const override {
    ENABLE_JS_STACK_TRACE_IN_SCOPE(context_);
    TRACK_MEMORY_SCOPE("Javascript");
    TRACE_EVENT0("cobalt::script::mozjs", "MozjsCallbackFunction::Run");
    CallbackResult<R> callback_result;
    JSAutoRequest auto_request(context_);
    JS::RootedObject function(context_, weak_function_.GetObject());
    if (!function) {
      DLOG(WARNING) << "Function was garbage collected.";
      callback_result.exception = true;
      return callback_result;
    }

    JSAutoCompartment auto_compartment(context_, function);
    JS::AutoSaveExceptionState auto_save_exception_state(context_);

    // https://www.w3.org/TR/WebIDL/#es-invoking-callback-functions
    // Callback 'this' is set to null, unless overridden by other specifications
    JS::RootedValue this_value(context_, JS::NullValue());
    JS::RootedValue return_value(context_);
    const int kNumArguments = 5;
    JS::AutoValueArray<5> args(context_);
    ToJSValue(context_, a1, args[0]);
    ToJSValue(context_, a2, args[1]);
    ToJSValue(context_, a3, args[2]);
    ToJSValue(context_, a4, args[3]);
    ToJSValue(context_, a5, args[4]);

    bool call_result = JS::Call(context_, this_value, function, args,
        &return_value);
    if (!call_result) {
      DLOG(WARNING) << "Exception in callback: "
                    << util::GetExceptionString(context_);
      callback_result.exception = true;
    } else {
      callback_result = ConvertCallbackReturnValue<R>(context_, return_value);
    }
    return callback_result;
  }

  JSObject* handle() const { return weak_function_.GetObject(); }
  const JS::Value& value() const { return weak_function_.GetValue(); }
  bool WasCollected() const { return weak_function_.WasCollected(); }
  void Trace(JSTracer* js_tracer) { weak_function_.Trace(js_tracer); }

 private:
  JSContext* context_;
  WeakHeapObject weak_function_;
};

template <typename R, typename A1, typename A2, typename A3, typename A4,
    typename A5, typename A6>
class MozjsCallbackFunction<R(A1, A2, A3, A4, A5, A6)>
    : public CallbackFunction<R(A1, A2, A3, A4, A5, A6)> {
 public:
  typedef CallbackFunction<R(A1, A2, A3, A4, A5, A6)> BaseType;

  explicit MozjsCallbackFunction(JSContext* context, JS::HandleValue function)
      : context_(context), weak_function_(context, function) {
    DCHECK(context_);
    DCHECK(function.isObject());
    DCHECK(JS_ObjectIsFunction(context_, function.toObjectOrNull()));
  }

  CallbackResult<R> Run(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1,
      typename base::internal::CallbackParamTraits<A2>::ForwardType a2,
      typename base::internal::CallbackParamTraits<A3>::ForwardType a3,
      typename base::internal::CallbackParamTraits<A4>::ForwardType a4,
      typename base::internal::CallbackParamTraits<A5>::ForwardType a5,
      typename base::internal::CallbackParamTraits<A6>::ForwardType a6)
      const override {
    ENABLE_JS_STACK_TRACE_IN_SCOPE(context_);
    TRACK_MEMORY_SCOPE("Javascript");
    TRACE_EVENT0("cobalt::script::mozjs", "MozjsCallbackFunction::Run");
    CallbackResult<R> callback_result;
    JSAutoRequest auto_request(context_);
    JS::RootedObject function(context_, weak_function_.GetObject());
    if (!function) {
      DLOG(WARNING) << "Function was garbage collected.";
      callback_result.exception = true;
      return callback_result;
    }

    JSAutoCompartment auto_compartment(context_, function);
    JS::AutoSaveExceptionState auto_save_exception_state(context_);

    // https://www.w3.org/TR/WebIDL/#es-invoking-callback-functions
    // Callback 'this' is set to null, unless overridden by other specifications
    JS::RootedValue this_value(context_, JS::NullValue());
    JS::RootedValue return_value(context_);
    const int kNumArguments = 6;
    JS::AutoValueArray<6> args(context_);
    ToJSValue(context_, a1, args[0]);
    ToJSValue(context_, a2, args[1]);
    ToJSValue(context_, a3, args[2]);
    ToJSValue(context_, a4, args[3]);
    ToJSValue(context_, a5, args[4]);
    ToJSValue(context_, a6, args[5]);

    bool call_result = JS::Call(context_, this_value, function, args,
        &return_value);
    if (!call_result) {
      DLOG(WARNING) << "Exception in callback: "
                    << util::GetExceptionString(context_);
      callback_result.exception = true;
    } else {
      callback_result = ConvertCallbackReturnValue<R>(context_, return_value);
    }
    return callback_result;
  }

  JSObject* handle() const { return weak_function_.GetObject(); }
  const JS::Value& value() const { return weak_function_.GetValue(); }
  bool WasCollected() const { return weak_function_.WasCollected(); }
  void Trace(JSTracer* js_tracer) { weak_function_.Trace(js_tracer); }

 private:
  JSContext* context_;
  WeakHeapObject weak_function_;
};

template <typename R, typename A1, typename A2, typename A3, typename A4,
    typename A5, typename A6, typename A7>
class MozjsCallbackFunction<R(A1, A2, A3, A4, A5, A6, A7)>
    : public CallbackFunction<R(A1, A2, A3, A4, A5, A6, A7)> {
 public:
  typedef CallbackFunction<R(A1, A2, A3, A4, A5, A6, A7)> BaseType;

  explicit MozjsCallbackFunction(JSContext* context, JS::HandleValue function)
      : context_(context), weak_function_(context, function) {
    DCHECK(context_);
    DCHECK(function.isObject());
    DCHECK(JS_ObjectIsFunction(context_, function.toObjectOrNull()));
  }

  CallbackResult<R> Run(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1,
      typename base::internal::CallbackParamTraits<A2>::ForwardType a2,
      typename base::internal::CallbackParamTraits<A3>::ForwardType a3,
      typename base::internal::CallbackParamTraits<A4>::ForwardType a4,
      typename base::internal::CallbackParamTraits<A5>::ForwardType a5,
      typename base::internal::CallbackParamTraits<A6>::ForwardType a6,
      typename base::internal::CallbackParamTraits<A7>::ForwardType a7)
      const override {
    ENABLE_JS_STACK_TRACE_IN_SCOPE(context_);
    TRACK_MEMORY_SCOPE("Javascript");
    TRACE_EVENT0("cobalt::script::mozjs", "MozjsCallbackFunction::Run");
    CallbackResult<R> callback_result;
    JSAutoRequest auto_request(context_);
    JS::RootedObject function(context_, weak_function_.GetObject());
    if (!function) {
      DLOG(WARNING) << "Function was garbage collected.";
      callback_result.exception = true;
      return callback_result;
    }

    JSAutoCompartment auto_compartment(context_, function);
    JS::AutoSaveExceptionState auto_save_exception_state(context_);

    // https://www.w3.org/TR/WebIDL/#es-invoking-callback-functions
    // Callback 'this' is set to null, unless overridden by other specifications
    JS::RootedValue this_value(context_, JS::NullValue());
    JS::RootedValue return_value(context_);
    const int kNumArguments = 7;
    JS::AutoValueArray<7> args(context_);
    ToJSValue(context_, a1, args[0]);
    ToJSValue(context_, a2, args[1]);
    ToJSValue(context_, a3, args[2]);
    ToJSValue(context_, a4, args[3]);
    ToJSValue(context_, a5, args[4]);
    ToJSValue(context_, a6, args[5]);
    ToJSValue(context_, a7, args[6]);

    bool call_result = JS::Call(context_, this_value, function, args,
        &return_value);
    if (!call_result) {
      DLOG(WARNING) << "Exception in callback: "
                    << util::GetExceptionString(context_);
      callback_result.exception = true;
    } else {
      callback_result = ConvertCallbackReturnValue<R>(context_, return_value);
    }
    return callback_result;
  }

  JSObject* handle() const { return weak_function_.GetObject(); }
  const JS::Value& value() const { return weak_function_.GetValue(); }
  bool WasCollected() const { return weak_function_.WasCollected(); }
  void Trace(JSTracer* js_tracer) { weak_function_.Trace(js_tracer); }

 private:
  JSContext* context_;
  WeakHeapObject weak_function_;
};

template <typename Signature>
struct TypeTraits<CallbackFunction<Signature> > {
  typedef MozjsUserObjectHolder<MozjsCallbackFunction<Signature> >
      ConversionType;
  typedef const ScriptValue<CallbackFunction<Signature> >* ReturnType;
};

}  // namespace mozjs
}  // namespace script
}  // namespace cobalt

#endif  // COBALT_SCRIPT_MOZJS_45_MOZJS_CALLBACK_FUNCTION_H_
