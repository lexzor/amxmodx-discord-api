#!/usr/bin/env bash
set -e

ROOT="$(pwd)"
VENDOR="$ROOT/vendor"
BIN="$VENDOR/bin"

# Clean up any leftover source/build files from previous failed runs
rm -rf "$VENDOR/libssrc" "$VENDOR/dpp" "$VENDOR/curl" "$VENDOR/openssl" "$VENDOR/zlib"
mkdir -p "$BIN" "$VENDOR/libssrc" "$VENDOR/include"

echo "----- Building ZLIB -----"
cd "$ROOT/vendor/libssrc"
git clone --depth 1 https://github.com/madler/zlib.git
cd zlib
mkdir build && cd build
cmake .. \
   -DBUILD_SHARED_LIBS=OFF \
   -DCMAKE_C_COMPILER=gcc \
   -DCMAKE_C_FLAGS="-m32 -mstackrealign" \
   -DCMAKE_INSTALL_PREFIX="$VENDOR/zlib"
make -j$(nproc)
make install

echo "----- Building OpenSSL 4.0.1 -----"
cd "$ROOT/vendor/libssrc"
if [ ! -f openssl-4.0.1.tar.gz ]; then
    wget https://www.openssl.org/source/openssl-4.0.1.tar.gz
fi
tar xf openssl-4.0.1.tar.gz
cd openssl-4.0.1
./Configure linux-x86 no-shared no-docs --prefix="$VENDOR/openssl" CFLAGS="-m32 -mstackrealign" LDFLAGS="-m32"
make -j$(nproc)
make install_sw

echo "----- Building CURL 8.20.0 -----"
cd "$ROOT/vendor/libssrc"
if [ ! -f curl-8.20.0.tar.gz ]; then
    wget https://curl.se/download/curl-8.20.0.tar.gz
fi
tar xf curl-8.20.0.tar.gz
cd curl-8.20.0
mkdir build && cd build
cmake .. \
   -DBUILD_SHARED_LIBS=OFF \
   -DCMAKE_C_COMPILER=gcc \
   -DCMAKE_CXX_COMPILER=g++ \
   -DCMAKE_C_FLAGS="-m32 -mstackrealign" \
   -DCMAKE_CXX_FLAGS="-m32 -mstackrealign" \
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

echo "----- Building DPP (v10.1.6) -----"
cd "$ROOT/vendor/libssrc"
git clone --branch v10.1.6 --depth 1 https://github.com/brainboxdotcc/DPP.git
cd DPP
mkdir build && cd build
cmake .. \
  -DCMAKE_C_COMPILER=gcc \
  -DCMAKE_CXX_COMPILER=g++ \
  -DCMAKE_CXX_STANDARD=20 \
  -DCMAKE_C_FLAGS="-m32 -mstackrealign" \
  -DCMAKE_CXX_FLAGS="-m32 -mstackrealign -std=gnu++20" \
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

echo "----- Copying Installed Static Libraries and Headers -----"
cp "$VENDOR/dpp/lib/libdpp.a" "$BIN/"
cp "$VENDOR/curl/lib/libcurl.a" "$BIN/"
cp "$VENDOR/openssl/lib/libcrypto.a" "$BIN/"
cp "$VENDOR/openssl/lib/libssl.a" "$BIN/"
cp "$VENDOR/zlib/lib/libz.a" "$BIN/"

# Copy installed headers from install directories
cp -r "$VENDOR/dpp/include/"* "$VENDOR/include/"
cp -r "$VENDOR/openssl/include/"* "$VENDOR/include/"
cp -r "$VENDOR/curl/include/"* "$VENDOR/include/"
cp -r "$VENDOR/zlib/include/"* "$VENDOR/include/zlib"

echo "----- VERIFYING OUTPUT LIBS -----"
ls -lah "$BIN"

test -f "$BIN/libdpp.a"
test -f "$BIN/libcurl.a"
test -f "$BIN/libssl.a"
test -f "$BIN/libcrypto.a"
test -f "$BIN/libz.a"

echo "Cleaning up build artifacts..."
rm -rf "$VENDOR/curl" "$VENDOR/libssrc" "$VENDOR/openssl" "$VENDOR/zlib" "$VENDOR/dpp"

echo "----- Cleanup complete! -----"