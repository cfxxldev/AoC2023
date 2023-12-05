#!/bin/sh

pushd day1
g++ --std=c++20 -O3 -ltbb day1.cpp && time bash -c 'cat ./input.txt | ./a.out'
popd

pushd day2
g++ --std=c++20 -O3 -ltbb day2.cpp && time bash -c 'cat ./input.txt | ./a.out'
popd

pushd day3
g++ --std=c++20 -O3 -ltbb day3.cpp && time bash -c 'cat ./input.txt | ./a.out'
popd

pushd day4
g++ --std=c++20 -O3 -ltbb day4.cpp && time bash -c 'cat ./input.txt | ./a.out'
popd

pushd day5
g++ --std=c++20 -O3 -ltbb day5.cpp && time bash -c 'cat ./input.txt | ./a.out'
popd
