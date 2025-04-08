FROM ubuntu:latest

LABEL maintainer="NoCountry4OldCode@gmail.com"

ENV DEBIAN_FRONTEND=noninteractive \
    PATH="/venv/bin:${PATH}"

# Add the Ubuntu Toolchain PPA to get the latest GCC version
RUN apt-get update && apt-get install -y --no-install-recommends \
    software-properties-common && \
    add-apt-repository -y ppa:ubuntu-toolchain-r/test && \
    apt-get update


RUN apt-get update && apt-get install -y --no-install-recommends \
    # Mandatory
    cmake \
    # Toolchain (for building the application)  
    gcc \
    g++ \
    gdb \
    make \
    # Toolchain (for optional targets)
    # ! "code coverage" requires fitting versions of gcc/g++/gcovr 
    gcovr \  
    clang-tidy \
    clang-format \
    doxygen \
    graphviz \
    python3 \
    python3-pip \
    python3-venv \
    libgtest-dev \
    libgmock-dev \
    cppcheck \
    git && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

# Build and install GoogleTest (GTest & GMock)
RUN cd /usr/src/googletest && cmake . && ls && \
    cmake --build . --target install

# Set up a virtual environment and install cmakelang
RUN python3 -m venv /venv && \
    /venv/bin/pip install --no-cache-dir cmakelang

WORKDIR /workspace

SHELL ["/bin/bash", "-c"]
CMD ["/bin/bash"]
