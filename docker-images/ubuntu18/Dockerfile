FROM i386/ubuntu:18.04

ENV DEBIAN_FRONTEND=noninteractive
WORKDIR /app

RUN apt-get update && apt-get install -y \
    software-properties-common \
    apt-transport-https \
    ca-certificates \
    gnupg \
    curl \
    wget \
    git \
    make \
    build-essential \
    gcc-multilib g++-multilib \
    libssl-dev:i386 \
    zlib1g-dev:i386 \
    libcurl4-openssl-dev:i386 \
    pkg-config \
    clang \
    lld \
    && rm -rf /var/lib/apt/lists/*

RUN curl -fsSL https://apt.kitware.com/keys/kitware-archive-latest.asc | apt-key add - && \
    add-apt-repository 'deb https://apt.kitware.com/ubuntu/ bionic main' && \
    apt-get update && apt-get install -y cmake && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
CMD ["/bin/bash"]

