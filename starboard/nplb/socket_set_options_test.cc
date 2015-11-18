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

#include "starboard/nplb/socket_helpers.h"
#include "starboard/socket.h"
#include "starboard/time.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace sbnplb = starboard::nplb;

namespace {

TEST(SbSocketSetOptionsTest, TryThemAll) {
  SbSocket socket = sbnplb::CreateTcpIpv4Socket();

  EXPECT_TRUE(SbSocketSetBroadcast(socket, true));
  EXPECT_TRUE(SbSocketSetReuseAddress(socket, true));
  EXPECT_TRUE(SbSocketSetReceiveBufferSize(socket, 16 * 1024));
  EXPECT_TRUE(SbSocketSetSendBufferSize(socket, 16 * 1024));
  EXPECT_TRUE(SbSocketSetTcpKeepAlive(socket, true, 30 * kSbTimeSecond));
  EXPECT_TRUE(SbSocketSetTcpNoDelay(socket, true));

  // Returns false on unsupported platforms, so we can't check the return value
  // generically.
  SbSocketSetTcpWindowScaling(socket, true);

  EXPECT_TRUE(SbSocketDestroy(socket));
}

// TODO(***REMOVED***): Come up with some way to test the effects of these options.

}  // namespace
