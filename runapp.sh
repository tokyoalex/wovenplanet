#! /bin/bash
#

pushd build
rm -rf src
rm -rf test

cmake .. -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles"

make all

./src/wptest_git_run

popd


