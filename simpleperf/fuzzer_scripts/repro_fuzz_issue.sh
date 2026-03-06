#!/bin/bash

# Exit on error, undefined vars, and pipe failures
set -euo pipefail

# --- Functions ---

show_help() {
  echo "Usage: $(basename "$0") [testcase_path]"
  echo ""
  echo "Automates building, syncing, and running an Android fuzzer."
  echo ""
  echo "Options:"
  echo "  -h, --help    Show this help message and exit"
  echo ""
  echo "Environment Requirements:"
  echo "  Must be run from within an AOSP repo after 'source build/envsetup.sh' and 'lunch'."
  exit 0
}

# --- Argument Parsing ---

# Check for help flag
if [[ "${1:-}" == "-h" || "${1:-}" == "--help" ]]; then
  show_help
fi

# --- Configuration ---
REPO_ROOT="${ANDROID_BUILD_TOP:-}"

if [[ -z "$REPO_ROOT" ]]; then
  echo "Error: ANDROID_BUILD_TOP not set. Did you run 'source build/envsetup.sh'?"
  exit 1
fi

FUZZER_TARGET="libsimpleperf_report_fuzzer"
REMOTE_DIR="/data/fuzzer"

# --- Pre-flight Checks ---
# Check if adb is available and a device is connected
if ! adb get-state &>/dev/null; then
  echo "Error: No device connected via ADB."
  exit 1
fi

# Ensure TARGET_PRODUCT is set (usually from 'lunch')
if [[ -z "${TARGET_PRODUCT:-}" ]]; then
  echo "Error: TARGET_PRODUCT is not set. Did you run 'lunch'?"
  exit 1
fi

# Capture device info
RAW_ARCH=$(adb shell getprop ro.product.cpu.abi | tr -d '\r')

if [[ "$RAW_ARCH" == "arm64-v8a" || "$RAW_ARCH" == "aarch64" ]]; then
  ARCH="arm64"
else
  ARCH="$RAW_ARCH"
fi

# Use the local TARGET_PRODUCT for paths to ensure consistency with the build system
PRODUCT="$TARGET_PRODUCT"

# Define local build paths for readability
LOCAL_FUZZ_DIR="$REPO_ROOT/out/target/product/$PRODUCT/symbols/data/fuzz/$ARCH"

# --- Build Phase ---
echo "--> Building $FUZZER_TARGET for $PRODUCT ($ARCH)..."
ANDROID_QUIET_BUILD=true m "$FUZZER_TARGET"

# --- Sync Phase ---
echo "--> Preparing device directories..."
adb shell "mkdir -p $REMOTE_DIR/testcase $REMOTE_DIR/lib"

echo "--> Syncing binaries..."
# Verify the local binary exists before pushing
if [[ ! -f "$LOCAL_FUZZ_DIR/$FUZZER_TARGET/$FUZZER_TARGET" ]]; then
  echo "Error: Could not find built binary at $LOCAL_FUZZ_DIR/$FUZZER_TARGET"
  exit 1
fi

adb push --sync "$LOCAL_FUZZ_DIR/lib/." "$REMOTE_DIR/lib/"
adb push "$LOCAL_FUZZ_DIR/$FUZZER_TARGET/$FUZZER_TARGET" "$REMOTE_DIR/"

# --- Execution Phase ---
if [[ -n "${1:-}" ]]; then
  TESTCASE_PATH="$1"
  TESTCASE="${TESTCASE_PATH##*/}"

  echo "--> Pushing testcase: $TESTCASE"
  adb push "$TESTCASE_PATH" "$REMOTE_DIR/testcase/"

  echo "--> Running fuzzer..."
  # Using 'set -x' inside the adb shell can help debug execution issues
  adb shell "LD_LIBRARY_PATH=$REMOTE_DIR/lib $REMOTE_DIR/$FUZZER_TARGET $REMOTE_DIR/testcase/$TESTCASE"
else
  echo "--> No testcase provided. Binaries synced to $REMOTE_DIR."
fi
