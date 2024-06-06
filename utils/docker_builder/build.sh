#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Usage: ${0} <os-name>" 
	echo "Available OS: ubuntu20.04 ubuntu22.04"
	exit 1
fi

ls ./builder/docker/dockerfiles | grep ${1} > /dev/null
if [ $? -ne 0 ]; then
	echo "${1} is not available"
	exit 1
fi

docker images | awk '{ print($1) }' | grep ${1} > /dev/null
if [ $? -ne 0 ]; then
	echo "Building docker image '${1}-imv-builder'..."
	docker build -t ${1}-imv-builder -f ./builder/docker/dockerfiles/${1}-builder.dockerfile .
fi

echo "Building for ${1}..."
mkdir -p ./builder/docker/result
docker run -v `pwd`:/src -v `pwd`/docker/result:/result --name build-imv-for-${1} --rm -it ${1}-imv-builder

echo "Done!"
echo "Generated installer in ./builder/docker/result"
