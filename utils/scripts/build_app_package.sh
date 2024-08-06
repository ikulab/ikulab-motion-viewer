#!/bin/bash

set -e

function green() {
    echo -e "\033[0;32m${*}\033[0m"
}

script_dir=$(readlink -f "$(dirname "$0")")
project_dir=$(readlink -f "${script_dir}/../..")

echo "script_dir: $script_dir"
echo "project_dir: $project_dir"

cd "$project_dir" || exit

green "Building binary..."

cmake --preset=default
cmake --build build

cd build

green "Generating package..."

cpack -G DragNDrop

green "Done"
