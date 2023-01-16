#!/bin/zsh

PROJECT_ROOT=$(pwd)

git clone https://github.com/microsoft/pxt-microbit

cd pxt-microbit
npm install
pxt buildtarget

cd libs
ln -s ../../ magnetics

cd ..
pxt install

cd $PROJECT_ROOT
python3 scripts/add_libs.py