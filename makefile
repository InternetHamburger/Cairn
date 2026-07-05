EXE ?= Cairn

CC=gcc
CMAKE := cmake.exe
BUILD_TYPE := Release
C_COMPILER := C:/msys64/mingw64/bin/gcc.exe
SOURCE_DIR := $(CURDIR)
BUILD_DIR := $(SOURCE_DIR)/cmake-build-release

.PHONY: configure build

configure:
	$(CMAKE) \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DCMAKE_C_COMPILER=$(C_COMPILER) \
		-DEXE_NAME=$(EXE) \
		-G "MinGW Makefiles" \
		-S "$(SOURCE_DIR)" \
		-B "$(BUILD_DIR)"

build: configure
	cmake --build "$(BUILD_DIR)" --config $(BUILD_TYPE)

.DEFAULT_GOAL := build