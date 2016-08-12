#! /bin/sh

mkdir include/3rd\ party
mkdir "3rd party"
cd "3rd party"
git clone https://github.com/nlohmann/json.git
cp json/src/json.hpp ../include/3rd\ party

cd ..

mkdir build
cd build
cmake ..
make

cp init.json /build
