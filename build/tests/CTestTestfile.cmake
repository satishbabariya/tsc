# CMake generated Testfile for 
# Source directory: /workspace/tests
# Build directory: /workspace/build/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[BasicCompilation]=] "/workspace/build/tsc" "/workspace/tests/examples/hello.ts")
set_tests_properties([=[BasicCompilation]=] PROPERTIES  _BACKTRACE_TRIPLES "/workspace/tests/CMakeLists.txt;27;add_test;/workspace/tests/CMakeLists.txt;0;")
