#!/bin/bash
set -e

BUILD_DIR=`dirname "$BASH_SOURCE"`/../build
mkdir -p $BUILD_DIR/coverage
cd $BUILD_DIR/coverage
cmake -DCMAKE_BUILD_TYPE=Debug -DFUN_BUILD_WITH_COVERAGE=ON ../..
cmake --build . -- -j8

lcov -d . -z
ctest --verbose

lcov -d . -c -o "coverage.info"
lcov -r "coverage.info" "*/usr/*" "*/Xcode.app/*" "*/untests/*" "*/3rdparty/*" -o "coverage.info"
lcov -l "coverage.info"

bash <(curl -s https://codecov.io/bash) || echo "Codecov did not collect coverage reports"
