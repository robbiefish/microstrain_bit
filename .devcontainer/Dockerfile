ARG ARCH="amd64"
ARG UBUNTU_VERSION="20.04"
FROM ${ARCH}/ubuntu:${UBUNTU_VERSION}

# Install some general tools not installed by rosdep
ENV DEBIAN_FRONTEND="noninteractive"
RUN set -ex \
    && apt-get update && apt-get install -y \
        gdb \
        git \
        vim \
        curl \
        make \
        cmake \
        zlib1g-dev \
        build-essential \
        bash-completion

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

# Add a user that will be used when shelling into this container and allow them to use devices
ARG USER_ID=1000
ARG GROUP_ID=1000
RUN set -ex \
    && apt-get update && apt-get install -y \
        sudo \
    && groupadd -g ${USER_ID} microstrain \
    && useradd \
        -N \
        -m \
        -u ${USER_ID} \
        -g ${GROUP_ID} \
        -G "dialout" \
        -s "/bin/bash" \
        microstrain \
    && echo 'microstrain ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers
