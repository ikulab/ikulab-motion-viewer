FROM ubuntu:22.04

# install deps
#RUN apt update -y && apt upgrade -y
RUN apt install git cmake -y

# checkout ikura repository
RUN mkdir work && cd work
RUN git clone git@github.com:ikulab/ikura.git && cd ikura

# build / install ikura
RUN cmake -S . -B build -D BUILD_TYPE=Debug -G Ninja
RUN cmake --build build
RUN cmake --install build

CMD /bin/bash