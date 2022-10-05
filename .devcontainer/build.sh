#!/bin/bash

# Find the script directory
script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
project_dir="${script_dir}/.."

docker build \
    -t microstrain_bit \
    -f "${script_dir}/Dockerfile" \
    --build-arg ARCH="amd64" \
    --build-arg UBUNTU_VERSION="20.04" \
    "${project_dir}"