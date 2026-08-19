param(
    [switch]$debug
)

$buildType = if ($debug) { "Debug" } else { "Release" }

docker run -it --rm `
    -v "${PWD}/../..:/workspace" `
    -w /workspace `
    debian12-i386 `
        bash -c "rm -rf build && mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=$buildType && cores=`$(( `$(nproc) / 2 )); if [ `$cores -lt 1 ]; then cores=1; fi && make -j`$cores && echo 'Build complete ($buildType mode). Press any key to exit.' && read -n 1"