#!/bin/zsh

cd day1
cd ..

cd day1
echo ""
echo "gcc"
gcc --std=c++23 -lstdc++ -ltbb -O3 day1.cpp && time zsh -c 'cat input.txt | ./a.out'
echo ""
echo "clang"
clang --std=c++23 -lstdc++ -ltbb -O3 day1.cpp && time zsh -c 'cat input.txt | ./a.out'
cd ..

cd day2
echo ""
echo "gcc"
gcc --std=c++23 -lstdc++ -ltbb -O3 day2.cpp && time zsh -c 'cat input.txt | ./a.out'
echo ""
echo "clang"
clang --std=c++23 -lstdc++ -ltbb -O3 day2.cpp && time zsh -c 'cat input.txt | ./a.out'
cd ..

cd day3
echo ""
echo "gcc"
gcc --std=c++23 -lstdc++ -ltbb -O3 day3.cpp && time zsh -c 'cat input.txt | ./a.out'
echo ""
echo "clang"
clang --std=c++23 -lstdc++ -ltbb -O3 day3.cpp && time zsh -c 'cat input.txt | ./a.out'
cd ..

cd day4
echo ""
echo "gcc"
gcc --std=c++23 -lstdc++ -ltbb -O3 day4.cpp && time zsh -c 'cat input.txt | ./a.out'
echo ""
echo "clang"
clang --std=c++23 -lstdc++ -ltbb -O3 day4.cpp && time zsh -c 'cat input.txt | ./a.out'
cd ..

cd day5
echo ""
echo "gcc"
gcc --std=c++23 -lstdc++ -ltbb -O3 day5.cpp && time zsh -c 'cat input.txt | ./a.out'
echo ""
echo "clang"
clang --std=c++23 -lstdc++ -ltbb -O3 day5.cpp && time zsh -c 'cat input.txt | ./a.out'
cd ..

cd day6
echo ""
echo "gcc"
gcc --std=c++23 -lstdc++ -ltbb -O3 day6.cpp && time zsh -c 'cat input.txt | ./a.out'
echo ""
echo "clang"
clang --std=c++23 -lstdc++ -ltbb -O3 day6.cpp && time zsh -c 'cat input.txt | ./a.out'
cd ..

cd day7
echo ""
echo "gcc"
gcc --std=c++23 -lstdc++ -ltbb -O3 day7.cpp && time zsh -c 'cat input.txt | ./a.out'
echo ""
echo "clang"
clang --std=c++23 -lstdc++ -ltbb -O3 day7.cpp && time zsh -c 'cat input.txt | ./a.out'
cd ..

cd day8
echo ""
echo "gcc"
gcc --std=c++23 -lstdc++ -ltbb -O3 day8.cpp && time zsh -c 'cat input.txt | ./a.out'
echo ""
echo "clang"
clang --std=c++23 -lstdc++ -ltbb -O3 day8.cpp && time zsh -c 'cat input.txt | ./a.out'
cd ..
