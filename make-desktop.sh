#! /bin/sh
conan profile new default --detect
conan profile update settings.compiler.libcxx=libstdc++11 default
cd build-naoqi-sdk
conan install .. -s build_type=Debug --build missing
cd .. 
qibuild configure -c naoqi-sdk body
qibuild make -c naoqi-sdk body