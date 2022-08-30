#!/bin/bash

cd $PICO_SDK_PATH
git submodule set-url tinyusb https://github.com/atoktoto/tinyusb.git 
git submodule set-branch -b midihost-atoktoto tinyusb
git submodule sync
git submodule update --init --recursive --remote