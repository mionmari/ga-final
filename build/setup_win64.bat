@echo off

mkdir ga3-win64
cd ga3-win64
cmake -G "Visual Studio 15 2017 Win64" ../../src/engine
cd ..
