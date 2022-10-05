ARG ARCH="amd64"
ARG UBUNTU_VERSION="20.04"
FROM ${ARCH}/ubuntu:${UBUNTU_VERSION}

# Install some general tools not installed by rosdep
ENV DEBIAN_FRONTEND="noninteractive"
RUN set -ex \
    && apt-get update && apt-get install -y \
        curl \
        make \
        cmake \
        build-essential

# Install MSCL
ARG ARCH
ARG MSCL_VERSION="64.3.0"
ARG MSCL_ARTIFACT="c++-mscl_${MSCL_VERSION}_${ARCH}.deb"
RUN set -ex \
    && apt-get update && apt-get install -y \
        jq \
    && curl -fsSLJo /tmp/${MSCL_ARTIFACT} -H 'Accept: application/octet-stream' $(curl -fsSl "https://api.github.com/repos/LORD-MicroStrain/MSCL/releases/tags/v${MSCL_VERSION}" | jq ".assets[] | select(.name==\"${MSCL_ARTIFACT}\") | .url" | sed 's/"// g') \
    && mkdir -p /usr/lib/python3/dist-packages \
    && dpkg-deb -x /tmp/${MSCL_ARTIFACT} /
ENV LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:/usr/share/c++-mscl"

# Build the code
COPY src /tmp/ws/src
COPY CMakeLists.txt /tmp/ws/CMakeLists.txt
RUN set -ex \
    && mkdir -p /tmp/ws/build \
    && cd /tmp/ws/build \
    && cmake /tmp/ws \
    && make -j \
    && make install

ENTRYPOINT ["/usr/local/bin/microstrain_bit"]