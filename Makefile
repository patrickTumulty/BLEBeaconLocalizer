# ===== Config =====
ARCH ?= x86
BUILD_TYPE ?= Debug
BUILD_DIR := build
TARGET ?=

CYAN=\033[1;36m
RESET=\033[0m

# Toolchain (used for non-x86)
TOOLCHAIN_FILE := toolchains/arm64.cmake

# ===== CMake Configure =====
CMAKE_CONFIGURE := cmake -S . -B $(BUILD_DIR) -G Ninja \
	-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) 

ifeq ($(ARCH),arm64)
	CMAKE_CONFIGURE += -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN_FILE)
endif

# ===== Targets =====
.PHONY: all configure build clean rebuild nuke help link-compile-commands

all: build ## Build (default target)

configure: ## Run CMake configure step
	@echo "$(CYAN)==> Configuring (ARCH=$(ARCH), TYPE=$(BUILD_TYPE))$(RESET)"
	$(CMAKE_CONFIGURE)

build: configure ## Build project (optionally specify TARGET=...)
	@echo "$(CYAN)==> Building $(if $(TARGET),target '$(TARGET)',all targets)...$(RESET)"
	cmake --build $(BUILD_DIR) $(if $(TARGET),--target $(TARGET),)
	$(MAKE) link-compile-commands

link-compile-commands: ## Symlink compile_commands.json to project root
	@echo "$(CYAN)==> Linking compile_commands.json$(RESET)"
	@ln -sf $(BUILD_DIR)/compile_commands.json ./compile_commands.json

clean: ## Clean build artifacts (keeps CMake cache)
	@echo "$(CYAN)==> Cleaning build artifacts$(RESET)"
	cmake --build $(BUILD_DIR) --target clean || true

rebuild: clean build ## Clean + rebuild

nuke: ## Delete entire build directory + compile_commands.json
	@echo "$(CYAN)==> Nuking build directory ($(BUILD_DIR))$(RESET)"
	rm -rf $(BUILD_DIR)
	rm -f compile_commands.json

help: ## Show this help
	@echo ""
	@echo "Usage:"
	@echo "  make [target] [ARCH=x86|arm64] [BUILD_TYPE=Debug|Release] [TARGET=name]"
	@echo ""
	@echo "Targets:"
	@grep -E '^[a-zA-Z_-]+:.*?## ' $(MAKEFILE_LIST) | \
		awk 'BEGIN {FS = ":.*?## "}; {printf "  %-12s %s\n", $$1, $$2}'
	@echo ""
	@echo "Examples:"
	@echo "  make"
	@echo "  make TARGET=my_app"
	@echo "  make ARCH=arm64"
	@echo "  make BUILD_TYPE=Release"
	@echo "  make ARCH=arm64 TARGET=my_lib"
	@echo ""
