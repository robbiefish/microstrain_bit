ARG ARCH="amd64"
ARG UBUNTU_VERSION="20.04"
FROM ${ARCH}/ubuntu:${UBUNTU_VERSION} as builder

# Install some general tools not installed by rosdep
ENV DEBIAN_FRONTEND="noninteractive"
RUN set -ex \
    && apt-get update && apt-get install -y \
        make \
        cmake \
        build-essential

# Build the code
COPY src /tmp/ws/src
COPY CMakeLists.txt /tmp/ws/CMakeLists.txt
RUN set -ex \
    && mkdir -p /tmp/ws/build \
    && cd /tmp/ws/build \
    && cmake /tmp/ws \
    && make -j \
    && make install

# Make a smaller image with just the executable
FROM ${ARCH}/ubuntu:${UBUNTU_VERSION}
COPY --from=builder /usr/local/bin/microstrain_bit /usr/local/bin/microstrain_bit
ENTRYPOINT ["/usr/local/bin/microstrain_bit"]