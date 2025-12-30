#!/usr/bin/env bash
set -e

ROOT="$(pwd)"
VENDOR="$ROOT/vendor"
LIBSRC="$VENDOR/libssrc"
LIB="$VENDOR/libs"
INC="$VENDOR/include"

mkdir -p "$LIBSRC" "$LIB" "$INC"


##############################################
# ZLIB
##############################################
cd "$LIBSRC"

if [ ! -d zlib ]; then
    git clone https://github.com/madler/zlib.git
fi

cd zlib
rm -rf build
mkdir build && cd build

cmake .. \
  -DBUILD_SHARED_LIBS=OFF \
  -DCMAKE_C_FLAGS="-m32"

make -j4


##############################################
# OPENSSL 1.1.1t
##############################################
cd "$LIBSRC"

if [ ! -d openssl-1.1.1t ]; then
    if [ ! -f openssl-1.1.1t.tar.gz ]; then
        wget https://www.openssl.org/source/openssl-1.1.1t.tar.gz
    fi
    tar xf openssl-1.1.1t.tar.gz
fi

cd openssl-1.1.1t
make clean || true
./Configure linux-generic32 no-shared CFLAGS="-m32" LDFLAGS="-m32"
make -j4


##############################################
# CURL
##############################################
cd "$LIBSRC"

if [ ! -d curl ]; then
    git clone https://github.com/curl/curl.git
fi

cd curl
rm -rf build
mkdir build && cd build

cmake .. \
  -DBUILD_SHARED_LIBS=OFF \
  -DCMAKE_C_FLAGS="-m32" \
  -DCMAKE_CXX_FLAGS="-m32" \
  -DCMAKE_USE_OPENSSL=ON \
  -DOPENSSL_ROOT_DIR="$LIBSRC/openssl-1.1.1t" \
  -DZLIB_ROOT="$LIBSRC/zlib" \
  -DCURL_USE_LIBPSL=OFF \
  -DCURL_USE_LIBIDN2=OFF \
  -DCURL_USE_NGHTTP2=OFF \
  -DCURL_DISABLE_LDAP=ON

make -j4


##############################################
# DPP (Forced C++17, no coroutines, no formatters)
##############################################
cd "$LIBSRC"

if [ ! -d DPP ]; then
    git clone https://github.com/brainboxdotcc/DPP.git
fi

cd DPP
sed -i 's|std::filesystem|stdc++fs|' ./CMakeLists.txt
rm -rf build
mkdir build && cd build

cmake .. \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_CXX_COMPILER=clang++ \
  -DBUILD_SHARED_LIBS=OFF \
  -DDPP_STATIC=ON \
  -DDPP_NO_CORO=ON \
  -DDPP_FORMATTERS=OFF \
  -DDPP_BUILD_TESTS=OFF \
  -DDPP_BUILD_EXAMPLES=OFF \
  -DDPP_BUILD_HTTP_TESTS=OFF \
  -DDPP_BUILD_SOAK_TESTS=OFF \
  -DDPP_BUILD_CORO_LOOP=OFF \
  -DCMAKE_C_FLAGS="-m32 -std=gnu17" \
  -DCMAKE_CXX_FLAGS="-m32 -std=gnu++20" \
  -DCMAKE_CXX_STANDARD=20 \
  -DCMAKE_CXX_STANDARD_REQUIRED=ON \
  -DZLIB_LIBRARY="$LIBSRC/zlib/build/libz.a" \
  -DZLIB_INCLUDE_DIR="$LIBSRC/zlib" \
  -DOPENSSL_CRYPTO_LIBRARY="$LIBSRC/openssl-1.1.1t/libcrypto.a" \
  -DOPENSSL_SSL_LIBRARY="$LIBSRC/openssl-1.1.1t/libssl.a" \
  -DOPENSSL_INCLUDE_DIR="$LIBSRC/openssl-1.1.1t/include"

make -j4

cp library/libdpp.a "$LIB"
cp -r ../include/dpp "$INC"

echo ""
echo "===================================="
echo "  ✅ DPP built in PURE C++17 mode"
echo "  ❌ No coroutines"
echo "  ❌ No C++20 formatters"
echo "===================================="
echo ""
echo "📦 static lib: vendor/libs/libdpp.a"
echo "📁 headers:    vendor/include/dpp/"

