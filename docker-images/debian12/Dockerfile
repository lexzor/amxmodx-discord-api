FROM debian:12

ENV DEBIAN_FRONTEND=noninteractive
ENV LD_LIBRARY_PATH=/usr/lib/i386-linux-gnu:$LD_LIBRARY_PATH

# Enable i386 architecture BEFORE any apt commands
RUN dpkg --add-architecture i386

# Update + install dependencies
RUN apt-get update && apt-get install -y \
    ca-certificates \
    apt-transport-https \
    gnupg \
    curl \
    wget \
    git \
    make \
    pkg-config \
    build-essential \
    clang \
    lld \
    cmake \
    \
    # 32-bit toolchain
    gcc-multilib \
    g++-multilib \
    libstdc++6:i386 \
    \
    # 32-bit system development libraries
    libssl-dev:i386 \
    zlib1g-dev:i386 \
    libcurl4-openssl-dev:i386 \
    \
    # optional: required by openssl build tools
    perl \
    && rm -rf /var/lib/apt/lists/*

# Clean working path
WORKDIR /workspace

CMD ["/bin/bash"]
