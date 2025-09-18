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

#include <android-base/file.h>
#include <android-base/test_utils.h>
#include <filesystem>

#include "AddrFilter.h"
#include "get_test_data.h"

using namespace simpleperf;
namespace fs = std::filesystem;

// @CddTest = 6.1/C-0-2
TEST(ParseAddrFilterOption, smoke) {
  auto option_to_str = [](const std::string& option) {
    auto filters = ParseAddrFilterOption(option);
    std::string s;
    for (auto& filter : filters) {
      if (!s.empty()) {
        s += ',';
      }
      s += filter.ToString();
    }
    return s;
  };
  std::string path;
  ASSERT_TRUE(android::base::Realpath(GetTestData(ELF_FILE), &path));

  // Test file filters.
  ASSERT_EQ(option_to_str("filter " + path), "filter 0x0/0x73c@" + path);
  ASSERT_EQ(option_to_str("filter 0x400502-0x400527@" + path), "filter 0x502/0x25@" + path);
  ASSERT_EQ(option_to_str("start 0x400502@" + path + ",stop 0x400527@" + path),
            "start 0x502@" + path + ",stop 0x527@" + path);

  // Test '-' in file path. Create a temporary file with '-' in name.
  TemporaryDir tmpdir;
  fs::path tmpfile = fs::path(tmpdir.path) / "elf-with-hyphen";
  ASSERT_TRUE(fs::copy_file(path, tmpfile));
  ASSERT_EQ(option_to_str("filter " + tmpfile.string()), "filter 0x0/0x73c@" + tmpfile.string());

  // Test kernel filters.
  ASSERT_EQ(option_to_str("filter 0x12345678-0x1234567a"), "filter 0x12345678/0x2");
  ASSERT_EQ(option_to_str("start 0x12345678,stop 0x1234567a"), "start 0x12345678,stop 0x1234567a");
}
