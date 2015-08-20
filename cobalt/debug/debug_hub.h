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

#ifndef DEBUG_DEBUG_HUB_H_
#define DEBUG_DEBUG_HUB_H_

#include <map>
#include <string>

#include "base/message_loop.h"
#include "base/synchronization/lock.h"
#include "cobalt/base/log_message_handler.h"
#include "cobalt/script/callback_function.h"
#include "cobalt/script/wrappable.h"

namespace cobalt {
namespace debug {

// This class implements an interface to JS for debugging.
// Currently, there are just two bound methods: {Add,Remove}LogMessageCallback,
// which allows JS to register a callback to receive log messages.
// Other methods will be added as necessary.
class DebugHub : public script::Wrappable {
 public:
  // Type for log message callback on JS side
  typedef script::CallbackFunction<
      void(int severity, const std::string& file, int line,
           size_t message_start, const std::string& msg)> LogMessageCallback;

  // Type for stored callback info.
  // We store the message loop from which the callback was registered,
  // so we can run the callback on the same loop.
  struct LogMessageCallbackInfo {
    scoped_refptr<LogMessageCallback> callback;
    scoped_refptr<base::MessageLoopProxy> message_loop_proxy;
  };

  // Declare logging levels here for export to JS
  static const int kLogInfo = logging::LOG_INFO;
  static const int kLogWarning = logging::LOG_WARNING;
  static const int kLogError = logging::LOG_ERROR;
  static const int kLogErrorReport = logging::LOG_ERROR_REPORT;
  static const int kLogFatal = logging::LOG_FATAL;

  DebugHub();
  ~DebugHub();

  // Called from JS to register a log message callback.
  // Returns an id that can be used to remove the callback.
  int AddLogMessageCallback(const scoped_refptr<LogMessageCallback>& callback);

  // Called from JS to remove a callback using the id returned by
  // AddLogMessageCallback.
  void RemoveLogMessageCallback(int id);

  DEFINE_WRAPPABLE_TYPE(DebugHub);

 private:
  // Called by LogMessageHandler for each log message.
  void OnLogMessage(int severity, const char* file, int line,
                    size_t message_start, const std::string& str);

  typedef std::map<int, LogMessageCallbackInfo> LogMessageCallbacks;

  int next_log_message_callback_id_;
  LogMessageCallbacks log_message_callbacks_;
  base::Lock lock_;
  base::LogMessageHandler::CallbackId log_message_handler_callback_id_;
};

}  // namespace debug
}  // namespace cobalt

#endif  // DEBUG_DEBUG_HUB_H_
