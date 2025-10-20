
# Collect AutoFDO profile for NDK apps

[TOC]

## Introduction

Starting with the Pixel 10 25Q4 release, we support collecting AutoFDO profiles for apps on user
builds. The app needs to be debuggable or released with the `<profileable android:shell="true" />`
flag set in its `AndroidManifests.xml`.

## An Example

### 1. Flash a user build on Pixel 10

On Pixel 10, AutoFDO profile collection for user build is supported on the BP4A and ZP1A branches,
as long as the build date is 250829 or newer. Here are two example builds.

```sh
$ adb shell getprop | grep build
[ro.build.fingerprint]: [google/frankel/frankel:16/BP4A.250916.007/14148704:user/dev-keys]

$ adb shell getprop | grep build
[ro.build.fingerprint]: [google/mustang/mustang:Baklava/ZP1A.250829.003.A1/14021912:user/dev-keys]
```

### 2. Find an app with C/C++ source code

This example uses the EndlessTunnel app from https://github.com/yabinc/simpleperf_demo/tree/master/test_apps/endless-tunnel.


### 3. Add additional debug info to help generating AutoFDO profile

Modify `CMakeLists.txt`.

```cmake

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdebug-info-for-profiling -mllvm -enable-fs-discriminator=true -mllvm -improved-fs-discriminator=true -funique-internal-linkage-names")

```

Build a debuggable app. A released app with the `<profileable android:shell="true" />` flag is also
fine, but it will need to use `-g` to add debug line info for converting instruction addresses to
source lines.

### 4. Install the app

```sh
$ app/build/outputs/apk/debug$ adb install -t app-debug.apk
$ app/build/outputs/apk/debug$ adb shell pm -l | grep tunnel
package:com.google.sample.tunnel
```

### 5. Record AutoFDO profile while running the app

```sh
(host) $ adb shell
mustang:/ $ cd /data/local/tmp
# Run simpleperf record, then start the app, play it for 10 seconds.
mustang:/data/local/tmp $ simpleperf record --app com.google.sample.tunnel -e cs-etm:u --duration 10 -z
simpleperf I environment.cpp:545] Waiting for process of app com.google.sample.tunnel
simpleperf I environment.cpp:537] Got process 23814 for package com.google.sample.tunnel
simpleperf I cmd_record.cpp:826] Recorded for 10.0316 seconds. Start post processing.
simpleperf I cmd_record.cpp:902] Aux data traced: 435,841,357
simpleperf I cmd_record.cpp:894] Record compressed: 15.24 MB (original 417.84 MB, ratio 27)

# Convert perf.data to branch list file.
# This can be done on device, but here we pull the file to the host for conversion.
(host) $ adb pull /data/local/tmp/perf.data
(host) $ simpleperf inject -i perf.data --output branch-list -o branch_list.data
# Dump branch list file, which shows branch lists for libgame.so.
(host) $ simpleperf inject --dump branch_list.data >branch_list.dump
(host) $ cat branch_list.dump | grep path
  ...
  binary[18].path: /data/app/~~ErW3Jcve4l3TJIpr-Xd1Gg==/com.google.sample.tunnel-__S5TlI-NVCqbBSHkMauTw==/base.apk!/lib/arm64-v8a/libgame.so
  ...
# Convert branch lists for libgame.so to AutoFDO profile.
(host) $ simpleperf inject -i branch_list.data -o libgame_autofdo.txt --binary libgame.so --log debug
simpleperf D command.cpp:288] command 'inject' starts running
simpleperf D cmd_inject.cpp:637] Not found debug binary for /data/app/~~ErW3Jcve4l3TJIpr-Xd1Gg==/com.google.sample.tunnel-__S5TlI-NVCqbBSHkMauTw==/base.apk!/lib/arm64-v8a/libgame.so with build id 0x288376bbae66d18a0e54e81cbef194b2342aa880
simpleperf D command.cpp:291] command 'inject' finished successfully
# The warning shows simpleperf didn't find debug binary for libgame.so. Let's provide it.
(host) $ $ simpleperf inject -i branch_list.data -o libgame_autofdo.txt --binary libgame --log debug --symdir ~/AndroidStudioProjects/simpleperf_demo/test_apps/endless-tunnel/app/build/intermediates/cxx/Debug/3bh34153/obj/arm64-v8a/
simpleperf D command.cpp:288] command 'inject' starts running
simpleperf D command.cpp:291] command 'inject' finished successfully
(host) $ ls -lh
-rw-r--r-- 1 yabinc primarygroup  52K Sep 22 15:39 libgame_autofdo.txt

# Run create_llvm_prof to convert AutoFDO input file to AutoFDO profile.
# create_llvm_prof can be downloaded or built from https://github.com/google/autofdo.
(host) $ create_llvm_prof --profiler text -profile=libgame_autofdo.txt --out=libgame.llvm_profdata --prof_sym_list=false --format=extbinary --binary ~/AndroidStudioProjects/simpleperf_demo/test_apps/endless-tunnel/app/build/intermediates/cxx/Debug/3bh34153/obj/arm64-v8a/libgame.so
(host) $ ls -lh
-rw-r--r-- 1 yabinc primarygroup  18K Sep 22 15:44 libgame.llvm_profdata

# Show hot functions in libgame.llvm_profdata.
# llvm-profdata can be downloaded or built from https://github.com/llvm/llvm-project.
(host) $ llvm-profdata show --sample --hot-func-list libgame.llvm_profdata >libgame_hot_functions.txt
```

To get good coverage, you usually need to record multiple times and record for a longer duration
each time. Multiple `branch_list.data` files can be merged into one when converting branch lists to
an AutoFDO profile, for example: `simpleperf inject -i branch_list1.data,branch_list2.data`.


### 6. Use AutoFDO profile when building the app


```sh
# Copy libgame.llvm_profdata to the source code directory.
$ cp libgame.llvm_profdata ~/AndroidStudioProjects/simpleperf_demo/test_apps/endless-tunnel/app/src/main/cpp
```

Modify `CMakeLists.txt`.

```cmake

# Set clang options to use AutoFDO profile
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdebug-info-for-profiling -mllvm -enable-fs-discriminator=true -mllvm -improved-fs-discriminator=true -funique-internal-linkage-names")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-sample-use=${CMAKE_CURRENT_SOURCE_DIR}/libgame.llvm_profdata")
# It's optional to add -fprofile-sample-accurate. Once added, it may de-optimize cold functions.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-sample-accurate")

```

To get performance gains from AutoFDO, you need to enable optimization, which means using `-O2`
or `-O3` in `CMakeLists.txt`, or creating a Release build.
