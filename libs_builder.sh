#!/usr/bin/env bash
set -e

ROOT="$(pwd)"
VENDOR="$ROOT/vendor"
BIN="$VENDOR/bin"

mkdir -p "$BIN" "$VENDOR/libssrc"

echo "----- Building ZLIB -----"

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

 echo "----- Building OpenSSL 4.0.1 -----"

 cd "$ROOT/vendor/libssrc"
 if [ ! -d openssl-4.0.1 ]; then
     if [ ! -f openssl-4.0.1.tar.gz ]; then
         wget https://www.openssl.org/source/openssl-4.0.1.tar.gz
     fi
     tar xf openssl-4.0.1.tar.gz
     rm openssl-4.0.1.tar.gz
 fi
 cd openssl-4.0.1

./Configure linux-x86 no-shared no-docs --prefix="$VENDOR/openssl" CFLAGS="-m32" LDFLAGS="-m32"

 make -j$(nproc)
 make install

 echo "----- Building CURL 8.20.0 -----"

 cd "$ROOT/vendor/libssrc"
 if [ ! -d curl-8.20.0 ]; then
     if [ ! -f curl-8.20.0.tar.gz ]; then
         wget https://curl.se/download/curl-8.20.0.tar.gz
     fi
     tar xf curl-8.20.0.tar.gz
 fi
 cd curl-8.20.0

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

echo "----- Building DPP (v10.1.5) -----"

cd "$ROOT/vendor/libssrc"
if [ ! -d DPP ]; then
    git clone --branch v10.1.5 --depth 1 https://github.com/brainboxdotcc/DPP.git
fi
cd DPP

cmake . \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_CXX_COMPILER=clang++ \
  -DCMAKE_CXX_STANDARD=17 \
  -DCMAKE_C_FLAGS="-m32 -I/workspace/vendor/libssrc/DPP/include" \
  -DCMAKE_CXX_FLAGS="-m32 -std=gnu++17 -I/workspace/vendor/libssrc/DPP/include" \
  -DCMAKE_INSTALL_PREFIX="$VENDOR/dpp" \
  -DBUILD_SHARED_LIBS=OFF \
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

cp "$VENDOR/dpp/lib/libdpp.a" "$BIN/"
cp "$VENDOR/curl/lib/libcurl.a" "$BIN/"
cp "$VENDOR/openssl/lib/libcrypto.a" "$BIN/"
cp "$VENDOR/openssl/lib/libssl.a" "$BIN/"
cp "$VENDOR/zlib/lib/libz.a" "$BIN/"
cp -r "$VENDOR/libssrc/DPP/include/dpp" "$VENDOR/include"

echo "----- VERIFYING OUTPUT LIBS -----"

ls -lah "$BIN"

test -f "$BIN/libdpp.a"
test -f "$BIN/libcurl.a"
test -f "$BIN/libssl.a"
test -f "$BIN/libcrypto.a"
test -f "$BIN/libz.a"

echo "Cleaning up source/build folders..."

rm -rf "$VENDOR/curl"
rm -rf "$VENDOR/libssrc"
rm -rf "$VENDOR/openssl"
rm -rf "$VENDOR/zlib"
rm -rf "$VENDOR/dpp"

echo "----- Cleanup complete! -----"