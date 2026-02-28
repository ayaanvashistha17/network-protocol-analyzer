#!/bin/bash
set -e
cd build
ctest --output-on-failure
# Or run test executables directly:
# ./tests/test_protocol
# ./tests/test_dissector
