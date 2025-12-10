.PHONY: default build gcc-release gcc-debug clang-release clang-debug sanitize-debug sanitize-release clean tidy format debug-problems release-problems

MAKE := $(MAKE)
MAKEFLAGS += --no-print-directory
# Adjust parallel build jobs based on your available cores.
JOBS ?= $(shell (command -v nproc > /dev/null 2>&1 && echo "-j$$(nproc)") || echo "")
BUILD_DIR := build/

default: build

build:
	cmake --build $(BUILD_DIR) $(JOBS)

gcc-release:
	cmake --preset=my-gcc-release
	$(MAKE) build

gcc-debug:
	cmake --preset=my-gcc-debug
	$(MAKE) build

clang-release:
	cmake --preset=my-clang-release
	$(MAKE) build

clang-debug:
	cmake --preset=my-clang-debug
	$(MAKE) build

sanitize-release:
	cmake --preset=my-sanitize-release
	$(MAKE) build

sanitize-debug:
	cmake --preset=my-sanitize-debug
	$(MAKE) build

format:
	cmake --build $(BUILD_DIR) $(JOBS) --target format

tidy:
	cmake --build $(BUILD_DIR) $(JOBS) --target tidy $(JOBS)

debug-problems:
	cmake --build build $(JOBS) --target problems
	$(BUILD_DIR)debug/bin/run_problems $(BUILD_DIR)debug/bin/problems/
	@echo "RAN PROBLEMS"

release-problems:
	cmake --build build $(JOBS) --target problems
	$(BUILD_DIR)bin/run_problems $(BUILD_DIR)bin/problems/
	@echo "RAN PROBLEMS"

clean:
	rm -rf build/ install/
