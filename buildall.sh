#!/bin/sh

cd day1
cd ..

cd day1
gcc --std=c++20 -lstdc++ -ltbb -O3 day1.cpp && time bash -c 'cat input.txt | ./a.out'
clang --std=c++20 -lstdc++ -ltbb -O3 day1.cpp && time bash -c 'cat input.txt | ./a.out'
cd ..

cd day2
gcc --std=c++20 -lstdc++ -ltbb -O3 day2.cpp && time bash -c 'cat input.txt | ./a.out'
clang --std=c++20 -lstdc++ -ltbb -O3 day2.cpp && time bash -c 'cat input.txt | ./a.out'
cd ..

cd day3
gcc --std=c++20 -lstdc++ -ltbb -O3 day3.cpp && time bash -c 'cat input.txt | ./a.out'
clang --std=c++20 -lstdc++ -ltbb -O3 day3.cpp && time bash -c 'cat input.txt | ./a.out'
cd ..

cd day4
gcc --std=c++20 -lstdc++ -ltbb -O3 day4.cpp && time bash -c 'cat input.txt | ./a.out'
clang --std=c++20 -lstdc++ -ltbb -O3 day4.cpp && time bash -c 'cat input.txt | ./a.out'
cd ..

cd day5
gcc --std=c++20 -lstdc++ -ltbb -O3 day5.cpp  && time bash -c 'cat input.txt | ./a.out'
clang --std=c++20 -lstdc++ -ltbb -O3 day5.cpp  && time bash -c 'cat input.txt | ./a.out'
cd ..
