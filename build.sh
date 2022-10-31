#!/bin/bash

mkdir -p ./docker/result
docker run -v `pwd`:/src -v `pwd`/docker/result:/result --name u20-imv-build --rm -it u20-imv-builder
