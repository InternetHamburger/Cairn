EXE ?= Cairn

CC=gcc
CMAKE := cmake
BUILD_TYPE := Release
C_COMPILER := gcc
SOURCE_DIR := $(CURDIR)
BUILD_DIR := $(SOURCE_DIR)/cmake-build-release

NETWORK_NAME = net21.bin
NETWORK := $(SOURCE_DIR)/$(NETWORK_NAME)

NNUE_URL := https://raw.githubusercontent.com/InternetHamburger/Networks/main/$(NETWORK_NAME)

CURL := $(shell command -v curl 2>/dev/null)
WGET := $(shell command -v wget 2>/dev/null)

.PHONY: configure build

download:
	@echo "NNUE network '$(NETWORK_NAME)' not found."
	@if [ -n "$(CURL)" ]; then \
		echo "Downloading with curl..."; \
		curl -L --fail -o $(NETWORK_NAME) $(NNUE_URL); \
	elif [ -n "$(WGET)" ]; then \
		echo "Downloading with wget..."; \
		wget -O $(NETWORK_NAME) $(NNUE_URL); \
	else \
		echo "Error: neither curl nor wget is installed."; \
		exit 1; \
	fi
	@echo "Downloaded $(NETWORK_NAME)"

configure: download
	$(CMAKE) \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DCMAKE_C_COMPILER=$(C_COMPILER) \
		-DEXE_NAME=$(EXE) \
		-G "Ninja" \
		-S "$(SOURCE_DIR)" \
		-B "$(BUILD_DIR)"

build: configure
	cmake --build "$(BUILD_DIR)" --config $(BUILD_TYPE)

.DEFAULT_GOAL := build