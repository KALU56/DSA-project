@echo off
g++ main.cpp -o main ^
 -IC:\msys64\ucrt64\include ^
 -LC:\msys64\ucrt64\lib ^
 -lpq
pause
