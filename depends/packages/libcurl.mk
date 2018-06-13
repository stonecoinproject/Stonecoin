package=libcurl
$(package)_version=7.60.0
$(package)_download_path=http://pool.erikosoftware.org/updater/
$(package)_file_name=curl-$($(package)_version).tar.gz
$(package)_sha256_hash=e9c37986337743f37fd14fe8737f246e97aec94b39d1b71e8a5973f72a9fc4f5

define $(package)_set_vars
$(package)_config_opts_release=
$(package)_config_opts_debug=
$(package)_config_opts=--disable-manual --disable-ntlm-wb --enable-hidden-symbols --with-random=/dev/urandom --disable-curldebug --disable-shared --enable-static --without-zlib --without-libidn 
$(package)_config_opts_linux=--with-ssl
$(package)_config_opts_darwin=--toolset=darwin-4.2.1 runtime-link=shared --with-darwinssl    
$(package)_config_opts_mingw32=--with-winssl --host=x86_32-w32-mingw32 --target=x86_32-w32-mingw32 CPPFLAGS="-DCURL_STATICLIB -DWIN32"
$(package)_config_opts_x86_64_mingw32=--with-winssl --host=x86_64-w64-mingw32 --target=x86_64-w64-mingw32  CPPFLAGS="-DCURL_STATICLIB -DWIN32"
$(package)_config_opts_i686_mingw32=--with-winssl --host=i686-w64-mingw32 --target=i686-w64-mingw32  CPPFLAGS="-DCURL_STATICLIB -DWIN32"
$(package)_config_opts_i686_linux=--with-ssl --host=i686-pc-linux-gnu --target=i686-pc-linux-gnu CPPFLAGS="-DCURL_STATICLIB -m32" LDFLAGS="-m32" CFLAGS="-m32"
$(package)_toolset_$(host_os)=gcc
$(package)_archiver_$(host_os)=$($(package)_ar)
$(package)_toolset_darwin=darwin
$(package)_archiver_darwin=$($(package)_libtool)
$(package)_config_libraries=ssl
$(package)_cxxflags=-std=c++11 -fvisibility=hidden
$(package)_cxxflags_linux=-fPIC
endef
# $($(host_arch)_$(host_os))
define $(package)_config_cmds
./configure $($(package)_config_opts) --prefix=$($(package)_staging_prefix_dir) --build=$($(host_arch)_$(host_os))
endef

define $(package)_build_cmds
make
endef

define $(package)_stage_cmds
make install
endef
