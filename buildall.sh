#!/bin/sh

cd day1
g++ --std=c++20 -O3 -ltbb day1.cpp && time bash -c 'cat ./input.txt | ./a.out'
cd ..

cd day2
g++ --std=c++20 -O3 -ltbb day2.cpp && time bash -c 'cat ./input.txt | ./a.out'
cd ..

cd day3
g++ --std=c++20 -O3 -ltbb day3.cpp && time bash -c 'cat ./input.txt | ./a.out'
cd ..

cd day4
g++ --std=c++20 -O3 -ltbb day4.cpp && time bash -c 'cat ./input.txt | ./a.out'
cd ..

cd day5
g++ --std=c++20 -O3 -ltbb day5.cpp && time bash -c 'cat ./input.txt | ./a.out'
cd ..
