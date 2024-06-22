# Use the official Ubuntu 20.04 as the base image
FROM ubuntu:20.04

# Set environment variables to non-interactive mode to avoid prompts during package installations
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    git \
    m4 \
    scons \
    zlib1g \
    zlib1g-dev \
    libprotobuf-dev \
    protobuf-compiler \
    libprotoc-dev \
    libgoogle-perftools-dev \
    python3-dev \
    python-is-python3 \
    libboost-all-dev \
    pkg-config \
    gcc-10 \
    g++-10 \
    python3-tk \
    wget \
    sudo \
    python3-six \
    libevent-dev \
    libhugetlbfs-dev

# Install Python 3.8 (default for Ubuntu 20.04 is Python 3.8)
RUN apt-get update && \
    apt-get install -y python3.8 python3.8-venv python3.8-dev

# Set up the default python to point to python3
RUN if [ ! -e /usr/bin/python3 ]; then ln -s /usr/bin/python3.8 /usr/bin/python3; fi && \
    if [ ! -e /usr/bin/python ]; then ln -s /usr/bin/python3 /usr/bin/python; fi && \
    if [ ! -e /usr/bin/pip ]; then ln -s /usr/bin/pip3 /usr/bin/pip; fi

# Clean up
RUN apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Set up a non-root user (optional)
RUN useradd -ms /bin/bash dockeruser && \
    echo 'dockeruser:dockeruser' | chpasswd && \
    adduser dockeruser sudo

USER dockeruser
WORKDIR /home/dockeruser

# Switch back to root user
USER root

# Entry point
CMD ["bash"]
