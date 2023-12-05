#!/bin/sh

cd day1
g++-13 --std=c++20 -O3 -ltbb day1.cpp && time bash -c 'cat ./input.txt | ./a.out'
cd ..

cd day2
g++-13 --std=c++20 -O3 -ltbb day2.cpp && time bash -c 'cat ./input.txt | ./a.out'
cd ..

cd day3
g++-13 --std=c++20 -O3 -ltbb day3.cpp && time bash -c 'cat ./input.txt | ./a.out'
cd ..

cd day4
g++-13 --std=c++20 -O3 -ltbb day4.cpp && time bash -c 'cat ./input.txt | ./a.out'
cd ..

cd day5
g++-13 --std=c++20 -L/usr/lib64 -lstdc++ -ltbb -O3 day5.cpp && cat input.txt | ./a.out
cd ..
