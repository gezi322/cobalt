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

#ifndef COBALT_DEBUG_DEBUG_SCRIPT_RUNNER_H_
#define COBALT_DEBUG_DEBUG_SCRIPT_RUNNER_H_

#include <string>

#include "base/callback.h"
#include "cobalt/script/global_object_proxy.h"
#include "cobalt/script/wrappable.h"

namespace cobalt {
namespace debug {

// Used by the various debug server components to run JavaScript and persist
// state. An object of this class creates a persistent JavaScript object bound
// to the global object, and executes methods on this object, passing in the
// JSON parameters as a parameter object, and returning the result as a
// serialized JSON object. Other classes may run scripts that attach additional
// data to the JavaScript object created by this class.
class DebugScriptRunner : public script::Wrappable {
 public:
  // Event callback. A callback of this type is specified in the constructor,
  // and used to send asynchronous debugging events that are not a direct
  // response to a command.
  // See: https://developer.chrome.com/devtools/docs/protocol/1.1/index
  typedef base::Callback<void(const std::string& method,
                              const base::optional<std::string>& params)>
      OnEventCallback;

  DebugScriptRunner(script::GlobalObjectProxy* global_object_proxy,
                    const OnEventCallback& on_event_callback);

  // Runs |command| on the JavaScript |runtimeInspector| object, passing in
  // |json_params| and putting the result in |json_result|.
  // Returns |true| if execution was successful, |false| otherwise.
  bool RunCommand(const std::string& command, const std::string& json_params,
                  std::string* json_result);

  // Loads JavaScript from file and executes the contents. Used to add
  // functionality to the JS object wrapped by this class.
  bool RunScriptFile(const std::string& filename);

  // Non-standard JavaScript extension API.
  // Called to send an event via the callback specified in the constructor.
  void SendEvent(const std::string& method,
                 const base::optional<std::string>& params);

  DEFINE_WRAPPABLE_TYPE(DebugScriptRunner);

 private:
  bool EvaluateScript(const std::string& js_code, std::string* result);
  bool EvaluateScriptFile(const std::string& filename, std::string* result);

  // No ownership.
  script::GlobalObjectProxy* global_object_proxy_;

  // Callback to send events.
  OnEventCallback on_event_callback_;
};

}  // namespace debug
}  // namespace cobalt

#endif  // COBALT_DEBUG_DEBUG_SCRIPT_RUNNER_H_
