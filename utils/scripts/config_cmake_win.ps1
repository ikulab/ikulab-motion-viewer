cmake `
	-S . `
	-B build `
	-G Ninja `
	-D BUILD_TYPE=Debug `
	-DCMAKE_EXPORT_COMPILE_COMMANDS=YES `
	-DCMAKE_C_COMPILER=clang `
	-DCMAKE_CXX_COMPILER=clang++
