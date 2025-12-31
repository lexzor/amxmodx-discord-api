docker run -it --rm `
    -v "${PWD}/../..:/workspace" `
    -w /workspace `
    debian12-i386 `
  bash -c "\
    rm -rf build && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make && \
    echo 'Press any key to exit the docker container' && \
    read -n 1"