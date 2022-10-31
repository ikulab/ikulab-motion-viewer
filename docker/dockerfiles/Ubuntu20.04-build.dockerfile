FROM ubuntu:20.04

# install deps
RUN apt update -y && apt upgrade -y
RUN apt install tzdata
RUN apt install \
	git cmake ninja-build clang libvulkan1 libvulkan-dev \
	libglfw3-dev vulkan-validationlayers libglm-dev wget \
	-y
# vulkan 1.3 sdk
RUN wget -qO - https://packages.lunarg.com/lunarg-signing-key-pub.asc | apt-key add -
RUN wget -qO /etc/apt/sources.list.d/lunarg-vulkan-1.3.231-focal.list \
	https://packages.lunarg.com/vulkan/1.3.231/lunarg-vulkan-1.3.231-focal.list
RUN apt update
RUN apt install vulkan-sdk

# fetch shaderc source
RUN mkdir /download && cd /download
RUN wget https://github.com/google/shaderc/archive/refs/tags/v2022.3.tar.gz
RUN tar -xf v2022.3.tar.gz && cd shaderc-2022.3
# build shaderc
RUN ./utils/git-sync-deps
RUN cmake -S . -B build -G Ninja
RUN cmake --build build
# copy shaderc lib
RUN cp build/libshaderc/libshaderc_combined.a /work/ikura/libs/libshaderc_combined.a

# copy project files
COPY . work
WORKDIR /work

# build
CMD ./docker/scripts/build-installer.sh Ubuntu20.04