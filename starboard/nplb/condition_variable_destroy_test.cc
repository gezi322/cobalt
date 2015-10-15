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

// Destroy is mostly Sunny Day tested in Create.

#include "starboard/condition_variable.h"
#include "starboard/mutex.h"
#include "starboard/nplb/thread_helpers.h"
#include "starboard/thread.h"
#include "testing/gtest/include/gtest/gtest.h"

using namespace starboard::nplb;

namespace {

TEST(SbConditionVariableDestroyTest, SunnyDayAutoInit) {
  SbConditionVariable condition = SB_CONDITION_VARIABLE_INITIALIZER;
  EXPECT_TRUE(SbConditionVariableDestroy(&condition));
}

TEST(SbConditionVariableDestroyTest, RainyDayWithWaiters) {
  WaiterContext context;

  SbThread thread =
      SbThreadCreate(0, kSbThreadNoPriority, kSbThreadNoAffinity, true,
                     NULL, WaiterEntryPoint, &context);
  context.WaitForReturnSignal();

  // We won't be able to destroy it if it has waiters.
  EXPECT_FALSE(SbConditionVariableDestroy(&context.condition));

  // Signal the condition to make the thread wake up and exit.
  EXPECT_TRUE(SbConditionVariableSignal(&context.condition));

  // Now we wait for the thread to exit.
  EXPECT_TRUE(SbThreadJoin(thread, NULL));
}

TEST(SbConditionVariableDestroyTest, RainyDayNull) {
  EXPECT_FALSE(SbConditionVariableDestroy(NULL));
}

}  // namespace
