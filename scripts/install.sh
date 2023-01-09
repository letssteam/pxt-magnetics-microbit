#!/bin/zsh

git clone https://github.com/microsoft/pxt-microbit
cd pxt-microbit
npm install
cd libs
ln -s ../../ magnetics
cd ..
pxt install