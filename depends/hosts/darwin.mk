OSX_MIN_VERSION=10.7
OSX_SDK_VERSION=10.9
OSX_SDK=$(SDK_PATH)/MacOSX$(OSX_SDK_VERSION).sdk
LD64_VERSION=241.9
darwin_CC=clang -target $(host) -mmacosx-version-min=$(OSX_MIN_VERSION) --sysroot $(OSX_SDK) -mlinker-version=$(LD64_VERSION)
darwin_CXX=clang++ -target $(host) -mmacosx-version-min=$(OSX_MIN_VERSION) --sysroot $(OSX_SDK) -mlinker-version=$(LD64_VERSION) -I/usr/include/c++/5/ -I/usr/include/x86_64-linux-gnu/c++/5/

darwin_CFLAGS=-pipe
darwin_CXXFLAGS=$(darwin_CFLAGS) -I/usr/include/c++/5/ -I/usr/include/x86_64-linux-gnu/c++/5/

darwin_release_CFLAGS=-O2
darwin_release_CXXFLAGS=$(darwin_release_CFLAGS) -I/usr/include/c++/5/ -I/usr/include/x86_64-linux-gnu/c++/5/

darwin_debug_CFLAGS=-O1
darwin_debug_CXXFLAGS=$(darwin_debug_CFLAGS) -I/usr/include/c++/5/ -I/usr/include/x86_64-linux-gnu/c++/5/

darwin_native_toolchain=native_cctools
