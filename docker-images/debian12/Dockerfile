FROM debian:12

ENV DEBIAN_FRONTEND=noninteractive

# Enable i386 architecture
RUN dpkg --add-architecture i386

WORKDIR /app

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
    gcc-multilib \
    g++-multilib \
    clang \
    lld \
    \
    # i386 libs
    libssl-dev:i386 \
    zlib1g-dev:i386 \
    libcurl4-openssl-dev:i386 \
    \
    # tools
    software-properties-common \
    cmake \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
CMD ["/bin/bash"]
