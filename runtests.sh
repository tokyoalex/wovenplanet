#! /bin/bash
#

pushd build
rm -rf src
rm -rf test

cmake .. -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles"

make all

./test/wptest_git_tst


popd


