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

#ifndef COBALT_BASE_TOKEN_H_
#define COBALT_BASE_TOKEN_H_

#include <string>

#include "base/basictypes.h"
#include "base/hash_tables.h"

namespace base {

// Token is a class used to represent a string constant.  It will never be
// destroyed once created.  Multiple instances that have the same values share
// the same copy of the string.
// Token is fully multi-thread safe.
// Note that as the Token object is small in size, it should be passed by value
// instead of by reference.
class Token {
 public:
  static const uint32 kHashSlotCount = 4 * 1024;
  static const uint32 kStringsPerSlot = 4;

  Token();
  explicit Token(const char* str);
  explicit Token(const std::string& str);

  const char* c_str() const { return str_->c_str(); }
  // TODO(***REMOVED***): Remove this accessor once all code are Token friendly.
  const std::string& str() const { return *str_; }

 private:
  void Initialize(const char* str);

  const std::string* str_;
};

inline bool operator==(const Token& lhs, const Token& rhs) {
  return lhs.c_str() == rhs.c_str();
}

// TODO(***REMOVED***): Remove implicit comparison functions to non Token type once
// all places using them are converted into explicit comparison.
inline bool operator==(const Token& lhs, const std::string& rhs) {
  return strcmp(lhs.c_str(), rhs.c_str()) == 0;
}

inline bool operator==(const std::string& lhs, const Token& rhs) {
  return strcmp(lhs.c_str(), rhs.c_str()) == 0;
}

inline bool operator!=(const Token& lhs, const Token& rhs) {
  return !(lhs == rhs);
}

inline bool operator!=(const Token& lhs, const std::string& rhs) {
  return !(lhs == rhs);
}

inline bool operator!=(const std::string& lhs, const Token& rhs) {
  return !(lhs == rhs);
}

inline bool operator<(const Token& lhs, const Token& rhs) {
  return strcmp(lhs.c_str(), rhs.c_str()) < 0;
}

inline std::ostream& operator<<(std::ostream& os, base::Token token) {
  os << token.str();
  return os;
}

}  // namespace base

namespace BASE_HASH_NAMESPACE {
#if defined(COMPILER_GCC) &&                                 \
    (!(defined(__LB_SHELL__) && !defined(__LB_LINUX__)) ||   \
     (defined(OS_STARBOARD) && defined(SB_HAS_HASH_VALUE) && \
      SB_HAS_HASH_VALUE))

template <>
struct hash<base::Token> {
  std::size_t operator()(const base::Token& token) const {
    return reinterpret_cast<size_t>(&(token.str()));
  }
};

#elif defined(COMPILER_MSVC) || defined(__LB_SHELL__) || \
    (defined(OS_STARBOARD) && defined(SB_HAS_HASH_VALUE) && SB_HAS_HASH_VALUE)

template <>
inline size_t hash_value<base::Token>(const base::Token& token) {
  return reinterpret_cast<size_t>(&(token.str()));
}

#endif  // COMPILER
}  // namespace BASE_HASH_NAMESPACE

#endif  // COBALT_BASE_TOKEN_H_
