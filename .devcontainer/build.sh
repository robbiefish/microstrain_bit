#!/bin/bash

# Find the script directory
script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
project_dir="${script_dir}/.."

image_name="microstrain/bit"

docker build \
    -t "${image_name}" \
    -f "${script_dir}/Dockerfile" \
    --build-arg ARCH="amd64" \
    --build-arg UBUNTU_VERSION="20.04" \
    "${project_dir}"