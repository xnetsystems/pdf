config = debug
system = linux

all: build/$(system)/$(config)/rules.ninja
	@cmake --build build/$(system)/$(config)

install: build/$(system)/release/rules.ninja
	@cmake --build build/$(system)/release --target install

format:
	@cmake -P "$(VCPKG_ROOT)/triplets/toolchains/format.cmake"

clean:
	@cmake -E remove_directory build/$(system)

build/windows/debug/rules.ninja: CMakeLists.txt
	@cmake -GNinja -DCMAKE_BUILD_TYPE=Debug \
	  -DCMAKE_INSTALL_PREFIX="$(MAKEDIR)\build\install" \
	  -DCMAKE_TOOLCHAIN_FILE="$(VCPKG_ROOT)\triplets\toolchains\windows.cmake" \
	  -B build/windows/debug

build/windows/release/rules.ninja: CMakeLists.txt
	@cmake -GNinja -DCMAKE_BUILD_TYPE=Release \
	  -DCMAKE_INSTALL_PREFIX="$(MAKEDIR)\build\install" \
	  -DCMAKE_TOOLCHAIN_FILE="$(VCPKG_ROOT)\triplets\toolchains\windows.cmake" \
	  -B build/windows/release

build/linux/debug/rules.ninja: CMakeLists.txt
	@cmake -GNinja -DCMAKE_BUILD_TYPE=Debug \
	  -DCMAKE_INSTALL_PREFIX="$(CURDIR)/build/install" \
	  -DCMAKE_TOOLCHAIN_FILE="$(VCPKG_ROOT)/triplets/toolchains/linux.cmake" \
	  -DCMAKE_CXX_CLANG_TIDY="clang-tidy" \
	  -B build/linux/debug

build/linux/release/rules.ninja: CMakeLists.txt
	@cmake -GNinja -DCMAKE_BUILD_TYPE=Release \
	  -DCMAKE_INSTALL_PREFIX="$(CURDIR)/build/install" \
	  -DCMAKE_TOOLCHAIN_FILE="$(VCPKG_ROOT)/triplets/toolchains/linux.cmake" \
	  -DCMAKE_CXX_CLANG_TIDY="clang-tidy" \
	  -B build/linux/release
