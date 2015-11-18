// Copyright 2015 Google Inc. All Rights Reserved.
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

#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include "starboard/log.h"
#include "starboard/shared/posix/handle_eintr.h"
#include "starboard/shared/posix/socket_internal.h"

namespace sbposix = starboard::shared::posix;

SbSocket SbSocketAccept(SbSocket socket) {
  if (!SbSocketIsValid(socket)) {
    errno = EBADF;
    return kSbSocketInvalid;
  }

  SB_DCHECK(socket->socket_fd >= 0);

  int socket_fd = HANDLE_EINTR(accept(socket->socket_fd, NULL, NULL));
  if (socket_fd < 0) {
    socket->error = sbposix::TranslateSocketErrno(errno);
    return kSbSocketInvalid;
  }

  // All Starboard sockets are non-blocking, so let's ensure it.
  if (!sbposix::SetNonBlocking(socket_fd)) {
    // Something went wrong, we'll clean up and return failure.
    socket->error = sbposix::TranslateSocketErrno(errno);
    HANDLE_EINTR(close(socket_fd));
    return kSbSocketInvalid;
  }

  socket->error = kSbSocketOk;

  // Adopt the newly accepted socket.
  SbSocket result = new SbSocketPrivate();
  result->protocol = socket->protocol;
  result->address_type = socket->address_type;
  result->socket_fd = socket_fd;
  result->error = kSbSocketOk;
  return result;
}
