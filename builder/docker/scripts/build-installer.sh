# build
cd /

# copy prebuild shaderc library
cp /download/shaderc-2022.3/build/libshaderc/libshaderc_combined.a \
	/src/ikura/libs/libshaderc_combined.a

cmake -S ./src -B /build -D BUILD_TYPE=Debug -G Ninja
cmake --build /build

# create installer
cpack --config /build/CPackConfig.cmake
# rename
mv *.deb `ls *.deb | sed "s/Linux/${1}/"`

# move to result
mv *.deb /result