#!/usr/bin/env bash
set -e

ROOT="$(pwd)"
VENDOR="$ROOT/vendor"
BIN="$VENDOR/bin"

mkdir -p "$BIN" "$VENDOR/libssrc"

echo "----- Building ZLIB -----"
 sleep 1

 cd "$ROOT/vendor/libssrc"
 if [ ! -d zlib ]; then
     git clone https://github.com/madler/zlib.git
 fi
 cd zlib

 cmake . \
   -DBUILD_SHARED_LIBS=OFF \
   -DCMAKE_C_FLAGS="-m32" \
   -DCMAKE_INSTALL_PREFIX="$VENDOR/zlib"

 make -j$(nproc)
 make install

 echo "----- Building OpenSSL 3.0.0 -----"
 sleep 1

 cd "$ROOT/vendor/libssrc"
 if [ ! -d openssl-3.0.0 ]; then
     if [ ! -f openssl-3.0.0.tar.gz ]; then
         wget https://www.openssl.org/source/openssl-3.0.0.tar.gz
     fi
     tar xf openssl-3.0.0.tar.gz
 fi
 cd openssl-3.0.0

 ./Configure linux-x86 no-shared --prefix="$VENDOR/openssl" CFLAGS="-m32" LDFLAGS="-m32"

 make -j$(nproc)
 make install

 echo "----- Building CURL 7.87.0 -----"
 sleep 1

 cd "$ROOT/vendor/libssrc"
 if [ ! -d curl-7.87.0 ]; then
     if [ ! -f curl-7.87.0.tar.gz ]; then
         wget https://curl.se/download/curl-7.87.0.tar.gz
     fi
     tar xf curl-7.87.0.tar.gz
 fi
 cd curl-7.87.0

 cmake . \
   -DBUILD_SHARED_LIBS=OFF \
   -DCMAKE_C_FLAGS="-m32" \
   -DCMAKE_CXX_FLAGS="-m32" \
   -DCMAKE_INSTALL_PREFIX="$VENDOR/curl" \
   -DCURL_USE_OPENSSL=ON \
   -DOPENSSL_ROOT_DIR="$VENDOR/openssl" \
   -DZLIB_ROOT="$VENDOR/zlib" \
   -DCURL_DISABLE_LDAP=ON \
   -DCURL_USE_LIBPSL=OFF \
   -DCURL_USE_LIBIDN2=OFF \
   -DCURL_USE_NGHTTP2=OFF

 make -j$(nproc)
 make install

echo "----- Building DPP -----"
sleep 1

cd "$ROOT/vendor/libssrc"
if [ ! -d DPP ]; then
    git clone https://github.com/brainboxdotcc/DPP.git
fi
cd DPP

cmake . \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_CXX_COMPILER=clang++ \
  -DCMAKE_CXX_STANDARD=17 \
  -DCMAKE_C_FLAGS="-m32" \
  -DCMAKE_CXX_FLAGS="-m32 -std=gnu++17" \
  -DCMAKE_INSTALL_PREFIX="$VENDOR/dpp" \
  -DBUILD_SHARED_LIBS=ON \
  -DDPP_STATIC=OFF \
  -DDPP_BUILD_TESTS=OFF \
  -DDPP_BUILD_EXAMPLES=OFF \
  -DDPP_NO_CORO=ON \
  -DDPP_FORMATTERS=OFF \
  -DZLIB_LIBRARY="$VENDOR/zlib/lib/libz.a" \
  -DZLIB_INCLUDE_DIR="$VENDOR/zlib/include" \
  -DOPENSSL_CRYPTO_LIBRARY="$VENDOR/openssl/lib/libcrypto.a" \
  -DOPENSSL_SSL_LIBRARY="$VENDOR/openssl/lib/libssl.a" \
  -DOPENSSL_INCLUDE_DIR="$VENDOR/openssl/include" \
  -DCURL_LIBRARY="$VENDOR/curl/lib/libcurl.a" \
  -DCURL_INCLUDE_DIR="$VENDOR/curl/include"

make -j$(nproc)
make install

echo "----- Finished building libraries! -----"

echo "Copying static libraries and headers to bin folder..."

cp "$VENDOR/libssrc/DPP/library/libdpp.a" "$BIN/"
cp "$VENDOR/curl/lib/libcurl.a" "$BIN/"
cp "$VENDOR/openssl/lib/libcrypto.a" "$BIN/"
cp "$VENDOR/openssl/lib/libssl.a" "$BIN/"
cp "$VENDOR/zlib/lib/libz.a" "$BIN/"
cp -r "$VENDOR/libssrc/DPP/include/dpp" "$VENDOR/include"

echo "Cleaning up source/build folders..."

rm -rf "$ROOT/vendor/curl"
rm -rf "$ROOT/vendor/libssrc"
rm -rf "$ROOT/vendor/openssl"
rm -rf "$ROOT/vendor/zlib"

echo "----- Cleanup complete! -----"