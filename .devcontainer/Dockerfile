FROM ubuntu:latest
RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y wget sudo python3-pip && \
    apt-get install -y git cmake gcc-arm-none-eabi gcc g++ pkg-config && \
    apt-get install -y gdb-multiarch automake autoconf build-essential texinfo libtool libftdi-dev libusb-1.0-0-dev

WORKDIR pico

RUN git clone https://github.com/raspberrypi/pico-sdk.git && cd pico-sdk && git submodule update --init
ENV PICO_SDK_PATH /pico/pico-sdk
RUN echo "export PICO_SDK_PATH=/pico/pico-sdk" >> ~/.bashrc

RUN git clone https://github.com/raspberrypi/pico-examples.git && cd pico-examples && git submodule update --init
ENV PICO_EXAMPLES_PATH /pico/pico-examples
RUN echo "export PICO_EXAMPLES_PATH=/pico/pico-examples" >> ~/.bashrc

RUN git clone https://github.com/raspberrypi/pico-extras.git && cd pico-extras && git submodule update --init
ENV PICO_EXTRAS_PATH /pico/pico-extras
RUN echo "export PICO_EXTRAS_PATH=/pico/pico-extras" >> ~/.bashrc

RUN git clone https://github.com/raspberrypi/pico-playground.git && cd pico-playground && git submodule update --init
ENV PICO_PLAYGROUND_PATH /pico/pico-playground
RUN echo "export PICO_PLAYGROUND_PATH=/pico/pico-playground" >> ~/.bashrc

WORKDIR /pico/pico-examples/build
RUN cmake ../ -DCMAKE_BUILD_TYPE=Debug
WORKDIR /pico/pico-examples/build/blink
RUN make -j4

WORKDIR /pico
RUN git clone https://github.com/raspberrypi/picoprobe.git
WORKDIR /pico/picoprobe/build
RUN cmake ../
RUN make -j4

WORKDIR /pico
RUN git clone https://github.com/raspberrypi/picotool.git
WORKDIR /pico/picotool/build
RUN cmake ../
RUN make -j4

WORKDIR /pico
RUN git clone https://github.com/raspberrypi/openocd.git -b rp2040 --depth=1
WORKDIR /pico/openocd
RUN ./bootstrap
RUN ./configure --enable-ftdi --enable-sysfsgpio --enable-bcm2835gpio
RUN make -j4 && make install

# COPY ../.vscode/launch.json /pico/pico-examples/.vscode/launch.json
