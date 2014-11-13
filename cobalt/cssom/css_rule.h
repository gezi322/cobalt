/*
 * Copyright 2014 Google Inc. All Rights Reserved.
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

#ifndef CSSOM_CSS_RULE_H_
#define CSSOM_CSS_RULE_H_

#include "base/memory/ref_counted.h"

namespace cobalt {
namespace cssom {

// The CSSRule interface represents an abstract, base CSS style rule.
// Each distinct CSS style rule type is represented by a distinct interface
// that inherits from this interface.
//   http://dev.w3.org/csswg/cssom/#the-cssrule-interface
class CSSRule : public base::RefCounted<CSSRule> {
 protected:
  CSSRule() {}
  virtual ~CSSRule() {}

 private:
  friend class base::RefCounted<CSSRule>;
  DISALLOW_COPY_AND_ASSIGN(CSSRule);
};

}  // namespace cssom
}  // namespace cobalt

#endif  // CSSOM_CSS_RULE_H_
