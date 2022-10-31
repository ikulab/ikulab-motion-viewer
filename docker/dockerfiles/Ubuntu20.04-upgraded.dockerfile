FROM ubuntu:20.04

# install deps
RUN apt update -y && apt upgrade -y
RUN apt install tzdata
RUN apt install git cmake ninja-build clang libvulkan1 libvulkan-dev libglfw3-dev vulkan-validationlayers libglm-dev -y

CMD /bin/bash