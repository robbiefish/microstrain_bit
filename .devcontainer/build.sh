#!/bin/bash

# Get the arch from the command line
arch="${1:-amd64}"

# Find the script directory
script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
project_dir="${script_dir}/.."

image_name="${arch}/microstrain/bit"

docker build \
    -t "${image_name}" \
    -f "${script_dir}/Dockerfile" \
    --build-arg ARCH="${arch}" \
    --build-arg UBUNTU_VERSION="20.04" \
    "${project_dir}"