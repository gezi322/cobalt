/*
 * Copyright 2015 Google Inc. All Rights Reserved.
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

#ifndef COBALT_DOM_CONSOLE_H_
#define COBALT_DOM_CONSOLE_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "cobalt/script/execution_state.h"
#include "cobalt/script/wrappable.h"

namespace cobalt {
namespace dom {

// The "console" object provides access to the browser's debugging console.
// The specifics of how it works vary from browser to browser, but there is
// a de facto set of features that are typically provided.
//    https://developer.mozilla.org/en-US/docs/Web/API/Console
//
// Although this feature is non-standard and is not on a standards track,
// WHATWG made an attempt to assess the level of support across browsers at
// https://github.com/DeveloperToolsWG/console-object.
class Console : public script::Wrappable {
 public:
  explicit Console(script::ExecutionState* execution_state);

  // Web API: Console
  //
  // Outputs a message.
  void Log(const std::string& text) const;
  // Outputs a JavaScript stack trace.
  void Trace() const;

  DEFINE_WRAPPABLE_TYPE(Console);

 private:
  ~Console() OVERRIDE {}

  script::ExecutionState* const execution_state_;

  DISALLOW_COPY_AND_ASSIGN(Console);
};

}  // namespace dom
}  // namespace cobalt

#endif  // COBALT_DOM_CONSOLE_H_
