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

#include "starboard/raspi/shared/application_dispmanx.h"

#include <stdlib.h>
#include <unistd.h>

#include <algorithm>
#include <iomanip>

#include "starboard/input.h"
#include "starboard/key.h"
#include "starboard/log.h"
#include "starboard/memory.h"
#include "starboard/raspi/shared/window_internal.h"
#include "starboard/shared/posix/time_internal.h"
#include "starboard/time.h"

namespace starboard {
namespace raspi {
namespace shared {

SbWindow ApplicationDispmanx::CreateWindow(const SbWindowOptions* options) {
  if (SbWindowIsValid(window_)) {
    return kSbWindowInvalid;
  }

  InitializeDispmanx();

  SB_DCHECK(IsDispmanxInitialized());
  window_ = new SbWindowPrivate(display_, options);
  return window_;
}

bool ApplicationDispmanx::DestroyWindow(SbWindow window) {
  if (!SbWindowIsValid(window)) {
    return false;
  }

  SB_DCHECK(IsDispmanxInitialized());
  SB_DCHECK(window_ == window);
  delete window;
  window_ = kSbWindowInvalid;
  ShutdownDispmanx();
  return true;
}

void ApplicationDispmanx::Initialize() {}

void ApplicationDispmanx::Teardown() {
  ShutdownDispmanx();
}

bool ApplicationDispmanx::MayHaveSystemEvents() {
  SB_NOTIMPLEMENTED();
  return false;
}

::starboard::shared::starboard::Application::Event*
ApplicationDispmanx::PollNextSystemEvent() {
  SB_NOTIMPLEMENTED();
  return NULL;
}

::starboard::shared::starboard::Application::Event*
ApplicationDispmanx::WaitForSystemEventWithTimeout(SbTime time) {
  SB_NOTIMPLEMENTED();
  return NULL;
}

void ApplicationDispmanx::WakeSystemEventWait() {
  SB_NOTIMPLEMENTED();
}

void ApplicationDispmanx::InitializeDispmanx() {
  if (IsDispmanxInitialized()) {
    return;
  }

  bcm_host_init();
  display_ = vc_dispmanx_display_open(0);
  SB_DCHECK(IsDispmanxInitialized());
}

void ApplicationDispmanx::ShutdownDispmanx() {
  if (!IsDispmanxInitialized()) {
    return;
  }

  SB_DCHECK(!SbWindowIsValid(window_));
  int result = vc_dispmanx_display_close(display_);
  SB_DCHECK(result == 0);
  display_ = DISPMANX_NO_HANDLE;
  bcm_host_deinit();
}

}  // namespace shared
}  // namespace raspi
}  // namespace starboard
