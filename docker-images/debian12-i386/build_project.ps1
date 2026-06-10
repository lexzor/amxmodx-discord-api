param(
    [switch]$debug
)

$buildType = if ($debug) { "Debug" } else { "Release" }

docker run -it --rm `
    -v "${PWD}/../..:/workspace" `
    -w /workspace `
    debian12-i386 `
    bash -c "rm -rf build && mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=$buildType && make -j`$(nproc) && echo 'Build complete ($buildType mode). Press any key to exit.' && read -n 1"