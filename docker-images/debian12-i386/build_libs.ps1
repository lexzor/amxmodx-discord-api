docker run -it --rm `
    -v "${PWD}/../..:/workspace" `
    -w /workspace `
    debian12-i386 `
    bash -c "
        chmod +x libs_builder.sh && \
        ./libs_builder.sh && \
        echo 'Press any key to exit the docker container' && \
        read -n 1
    "