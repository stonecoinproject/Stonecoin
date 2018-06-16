#!/bin/sh
PATH=$(echo $PATH|sed 's/\/usr\/local\/bin://') CONFIG_SITE=$PWD/depends/x86_64-w64-mingw32/share/config.site ./configure CXXFLAGS="--param ggc-min-expand=1 --param ggc-min-heapsize=32768" --prefix=/
make HOST=x86_64-w64-mingw32 -j12
make install DESTDIR=/mnt/c/Users/Zyber/Documents/StoneWindows

