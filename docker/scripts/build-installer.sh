# build
cmake -S . -B build -D BUILD_TYPE=Debug -G Ninja
cmake --build build

# create installer
cpack --config ./build/CPackConfig.cmake
# rename
mv *.deb `ls *.deb | sed 's/Linux/${1}/'`
