param(
    [switch]$debug,
    [int]$jobs = 16  # Capped at 2 to keep RAM usage under ~4GB during D++ compilation
)

$buildType = if ($debug) { "Debug" } else { "Release" }

docker run -it --rm `
    --cpu-shares 768 `
    -v "${PWD}/../..:/workspace" `
    -w /workspace `
    debian12-i386 `
        bash -c "rm -rf build && mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=$buildType && nice -n 10 make -j$jobs && echo 'Build complete ($buildType mode). Press any key to exit.' && read -n 1"