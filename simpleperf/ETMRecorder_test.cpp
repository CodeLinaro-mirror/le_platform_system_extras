/*
 * Copyright (C) 2025 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>

#include <android-base/strings.h>
#include <algorithm>

#include "ETMRecorder.h"

using namespace simpleperf;

// @CddTest = 6.1/C-0-2
TEST(ETMRecorder, etr_sinks) {
  ETMRecorder& recorder = ETMRecorder::GetInstance();
  if (!recorder.CheckEtmSupport().ok()) {
    GTEST_LOG_(INFO) << "Omit this test since etm isn't supported on this device";
    return;
  }

  std::set<EventType> event_types;
  recorder.BuildEventTypes(event_types);
  for (const auto& etr_name : recorder.GetETRSinks()) {
    // Recording via an ETR sink can fail if no ETE/ETM connects to it. So only do userspace checks.
    auto it =
        std::find_if(event_types.begin(), event_types.end(), [&](const EventType& event_type) {
          return android::base::EndsWith(event_type.name, "/@" + etr_name + "/");
        });
    ASSERT_TRUE(it != event_types.end());
    perf_event_attr attr;
    ASSERT_TRUE(recorder.SetEtmPerfEventAttr(*it, attr));
  }
}
