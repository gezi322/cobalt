// Copyright 2015 The Cobalt Authors. All Rights Reserved.
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

#include "starboard/socket.h"

#include <netinet/tcp.h>
#include <sys/socket.h>

#include "starboard/shared/posix/socket_internal.h"

namespace sbposix = starboard::shared::posix;

bool SbSocketSetTcpKeepAlive(SbSocket socket, bool value, SbTime period) {
  bool result = sbposix::SetBooleanSocketOption(
      socket, SOL_SOCKET, SO_KEEPALIVE, "SO_KEEPALIVE", value);
  if (!result) {
    return false;
  }

  int period_seconds = static_cast<int>(period / kSbTimeSecond);
  result = sbposix::SetIntegerSocketOption(socket, SOL_TCP, TCP_KEEPIDLE,
                                           "TCP_KEEPIDLE", period_seconds);
  if (!result) {
    return false;
  }

  return sbposix::SetIntegerSocketOption(socket, SOL_TCP, TCP_KEEPINTVL,
                                         "TCP_KEEPINTVL", period_seconds);
}
