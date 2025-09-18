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

#include "AddrFilter.h"

#include <string.h>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>
#include <string>
#include <vector>

#include "read_elf.h"

namespace simpleperf {

namespace {

static bool ConsumeStr(const char*& p, const char* s) {
  if (strncmp(p, s, strlen(s)) == 0) {
    p += strlen(s);
    return true;
  }
  return false;
}

static bool ConsumeAddr(const char*& p, uint64_t* addr) {
  errno = 0;
  char* end;
  *addr = strtoull(p, &end, 0);
  if (errno == 0 && p != end) {
    p = end;
    return true;
  }
  return false;
}

// To reduce function length, not all format errors are checked.
static bool ParseOneAddrFilter(const std::string& s, std::vector<AddrFilter>* filters) {
  std::vector<std::string> args = android::base::Split(s, " ");
  if (args.size() != 2) {
    return false;
  }

  uint64_t addr1;
  uint64_t addr2;
  uint64_t off1;
  uint64_t off2;
  std::string path;

  if (auto p = s.data(); ConsumeStr(p, "start") && ConsumeAddr(p, &addr1)) {
    if (*p == '\0') {
      // start <kernel_addr>
      filters->emplace_back(AddrFilter::KERNEL_START, addr1, 0, "");
      return true;
    }
    if (ConsumeStr(p, "@") && *p != '\0') {
      // start <vaddr>@<file_path>
      if (auto elf = ElfFile::Open(p);
          elf && elf->VaddrToOff(addr1, &off1) && android::base::Realpath(p, &path)) {
        filters->emplace_back(AddrFilter::FILE_START, off1, 0, path);
        return true;
      }
    }
  }
  if (auto p = s.data(); ConsumeStr(p, "stop") && ConsumeAddr(p, &addr1)) {
    if (*p == '\0') {
      // stop <kernel_addr>
      filters->emplace_back(AddrFilter::KERNEL_STOP, addr1, 0, "");
      return true;
    }
    if (ConsumeStr(p, "@") && *p != '\0') {
      // stop <vaddr>@<file_path>
      if (auto elf = ElfFile::Open(p);
          elf && elf->VaddrToOff(addr1, &off1) && android::base::Realpath(p, &path)) {
        filters->emplace_back(AddrFilter::FILE_STOP, off1, 0, path);
        return true;
      }
    }
  }
  if (auto p = s.data(); ConsumeStr(p, "filter") && ConsumeAddr(p, &addr1) && ConsumeStr(p, "-") &&
                         ConsumeAddr(p, &addr2)) {
    if (*p == '\0') {
      // filter <kernel_addr_start>-<kernel_addr_end>
      filters->emplace_back(AddrFilter::KERNEL_RANGE, addr1, addr2 - addr1, "");
      return true;
    }
    if (ConsumeStr(p, "@") && *p != '\0') {
      // filter <vaddr_start>-<vaddr_end>@<file_path>
      if (auto elf = ElfFile::Open(p); elf && elf->VaddrToOff(addr1, &off1) &&
                                       elf->VaddrToOff(addr2, &off2) &&
                                       android::base::Realpath(p, &path)) {
        filters->emplace_back(AddrFilter::FILE_RANGE, off1, off2 - off1, path);
        return true;
      }
    }
  }
  if (auto p = s.data(); ConsumeStr(p, "filter") && *p != '\0') {
    // filter <file_path>
    path = android::base::Trim(p);
    if (auto elf = ElfFile::Open(path); elf) {
      for (const ElfSegment& seg : elf->GetProgramHeader()) {
        if (seg.is_executable) {
          filters->emplace_back(AddrFilter::FILE_RANGE, seg.file_offset, seg.file_size, path);
        }
      }
      return true;
    }
  }
  return false;
}

}  // namespace

std::vector<AddrFilter> ParseAddrFilterOption(const std::string& s) {
  std::vector<AddrFilter> filters;
  for (const auto& str : android::base::Split(s, ",")) {
    if (!ParseOneAddrFilter(str, &filters)) {
      LOG(ERROR) << "failed to parse addr filter: " << str;
      return {};
    }
  }
  return filters;
}

}  // namespace simpleperf
