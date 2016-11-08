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

#ifndef COBALT_SPEECH_MICROPHONE_MANAGER_H_
#define COBALT_SPEECH_MICROPHONE_MANAGER_H_

#include "cobalt/speech/speech_configuration.h"

#if defined(SB_USE_SB_MICROPHONE)

#include "base/callback.h"
#include "base/optional.h"
#include "base/threading/thread.h"
#include "base/timer.h"
#include "cobalt/dom/event.h"
#include "media/base/shell_audio_bus.h"
#include "starboard/microphone.h"

namespace cobalt {
namespace speech {

// This class is used for microphone creation, control and destruction. It has
// a self-managed poller to fetch audio data from microphone.
class MicrophoneManager {
 public:
  typedef ::media::ShellAudioBus ShellAudioBus;
  typedef base::Callback<void(scoped_ptr<ShellAudioBus>)> DataReceivedCallback;
  typedef base::Callback<void(void)> CompletionCallback;
  typedef base::Callback<void(const scoped_refptr<dom::Event>&)> ErrorCallback;

  MicrophoneManager(int sample_rate, const DataReceivedCallback& data_received,
                    const CompletionCallback& completion,
                    const ErrorCallback& error);

  ~MicrophoneManager();

  // Open microphone to receive voice and start a timer to fetch audio data from
  // microphone.
  void Open();
  // Close microphone and stop the timer from receiving audio data.
  void Close();

 private:
  enum State { kStarted, kStopped, kError };

  void CreateInternal();
  void OpenInternal();
  void CloseInternal();
  void DestroyInternal();

  // Timer callback for fetching audio data.
  void Read();

  int sample_rate_;
  const DataReceivedCallback data_received_callback_;
  const CompletionCallback completion_callback_;
  const ErrorCallback error_callback_;

  SbMicrophone microphone_;
  // Minimum requested bytes per microphone read.
  int min_microphone_read_in_bytes_;
  // Microphone state.
  State state_;
  // Repeat timer to poll mic events.
  base::optional<base::RepeatingTimer<MicrophoneManager> >
      poll_mic_events_timer_;
  // Microphone thread.
  base::Thread thread_;
};

}  // namespace speech
}  // namespace cobalt

#endif  // defined(SB_USE_SB_MICROPHONE)

#endif  //  COBALT_SPEECH_MICROPHONE_MANAGER_H_
