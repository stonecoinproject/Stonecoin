OSX_MIN_VERSION=10.7
OSX_SDK_VERSION=10.9
OSX_SDK=$(SDK_PATH)/MacOSX$(OSX_SDK_VERSION).sdk
LD64_VERSION=241.9
darwin_CC=clang -target $(host) -mmacosx-version-min=$(OSX_MIN_VERSION) --sysroot $(OSX_SDK) -mlinker-version=$(LD64_VERSION)
darwin_CXX=clang++ -target $(host) -mmacosx-version-min=$(OSX_MIN_VERSION) --sysroot $(OSX_SDK) -mlinker-version=$(LD64_VERSION)

darwin_CFLAGS=-pipe
darwin_CXXFLAGS=$(darwin_CFLAGS) -isystem $(dirname $(xcrun -find clang))/../lib/c++/v1 -isystem $(dirname $(xcrun -find clang))/../include" -DC_INCLUDE_DIRS="$(dirname $(xcrun -find clang))/../lib/c++/v1:$(dirname $(xcrun -find clang))/../include:/usr/include"

darwin_release_CFLAGS=-O2
darwin_release_CXXFLAGS=$(darwin_release_CFLAGS) -isystem $(dirname $(xcrun -find clang))/../lib/c++/v1 -isystem $(dirname $(xcrun -find clang))/../include" -DC_INCLUDE_DIRS="$(dirname $(xcrun -find clang))/../lib/c++/v1:$(dirname $(xcrun -find clang))/../include:/usr/include"

darwin_debug_CFLAGS=-O1
darwin_debug_CXXFLAGS=$(darwin_debug_CFLAGS) -isystem $(dirname $(xcrun -find clang))/../lib/c++/v1 -isystem $(dirname $(xcrun -find clang))/../include" -DC_INCLUDE_DIRS="$(dirname $(xcrun -find clang))/../lib/c++/v1:$(dirname $(xcrun -find clang))/../include:/usr/include"

darwin_native_toolchain=native_cctools
