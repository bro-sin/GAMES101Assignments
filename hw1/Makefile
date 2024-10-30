default_target: all
.PHONY : default_target

# 生成构建工具链
build/conan_toolchain.cmake: conanfile.txt
	conan install . --output-folder=build --build=missing 

# 使用CMkae构建项目，并且生成clangd的编译数据库
build/Makefile: CMakeLists.txt build/conan_toolchain.cmake
	cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1

# 编译项目
all: build/Makefile
	cmake --build build 
.PHONY : all

# 清理项目
clean:
	rm -r build
.PHONY : clean