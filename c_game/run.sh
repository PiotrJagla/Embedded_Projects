echo cmake output
cmake -B build/ -S .
cd build

echo make output
make

echo program output
./gamelib

