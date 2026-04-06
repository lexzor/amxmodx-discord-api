cmake_minimum_required(VERSION 3.16)
project(discordapi_amxx_i386 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--strip-all")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g3")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static-libstdc++ -static-libgcc")

include_directories(
    vendor/include/amxmodx
    vendor/include/amxmodx/sdk
    vendor/include

    vendor/include/hlsdk
    vendor/include/hlsdk/cl_dll
    vendor/include/hlsdk/common
    vendor/include/hlsdk/dlls
    vendor/include/hlsdk/engine
    vendor/include/hlsdk/pm_shared

    vendor/include/metamod

    src/utils
    src/natives
    src
)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/**/*.cpp" "vendor/include/amxmodx/sdk/*.cpp")

add_library(discordapi_amxx_i386 SHARED ${SOURCES})

target_link_libraries(discordapi_amxx_i386
    ${CMAKE_SOURCE_DIR}/vendor/bin/libdpp.a -Wl,--no-whole-archive
    ${CMAKE_SOURCE_DIR}/vendor/bin/libcurl.a
    ${CMAKE_SOURCE_DIR}/vendor/bin/libssl.a
    ${CMAKE_SOURCE_DIR}/vendor/bin/libcrypto.a
    ${CMAKE_SOURCE_DIR}/vendor/bin/libz.a
)

set_target_properties(discordapi_amxx_i386 PROPERTIES
    OUTPUT_NAME "discordapi_amxx_i386"
    PREFIX ""
)