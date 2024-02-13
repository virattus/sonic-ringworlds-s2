#! /bin/bash

if [ -d ./build ]; then
	echo "found build dir"
else
	echo "Creating Build directories"
	mkdir -p build/{debug,release}
fi

cmake -Bbuild/debug -DCMAKE_BUILD_TYPE=Debug
cmake -Bbuild/release -DCMAKE_BUILD_TYPE=Release
