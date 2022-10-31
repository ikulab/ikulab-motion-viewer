FROM ubuntu:22.04

# install deps
RUN apt update -y && apt upgrade -y
RUN apt install tzdata
RUN apt install \
	git cmake ninja-build clang wget gnupg \
	libvulkan1 libvulkan-dev vulkan-validationlayers \
	libglfw3-dev libglm-dev \
	-y

# fetch shaderc source
RUN mkdir /download
WORKDIR /download
RUN wget https://github.com/google/shaderc/archive/refs/tags/v2022.3.tar.gz
RUN tar -xf v2022.3.tar.gz
WORKDIR /download/shaderc-2022.3

# build shaderc
RUN ./utils/git-sync-deps
RUN cmake -S . -B build -G Ninja
RUN cmake --build build
WORKDIR /

# build and generate installer
CMD /src/builder/docker/scripts/build-installer.sh ubuntu22.04