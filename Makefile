.PHONY: build test clean help

BUILD_DIR := build

help: ## Show all targets
	@echo ""
	@echo "  nimblz — BLE Reliability Layer for BlueZ"
	@echo "  ═════════════════════════════════════════"
	@echo ""
	@echo "  make build       Build library + examples + tests"
	@echo "  make test        Run unit tests"
	@echo "  make clean       Remove build directory"
	@echo ""

build: ## Build library, examples, and tests
	@mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. && make

test: build ## Run unit tests
	cd $(BUILD_DIR) && ctest --output-on-failure

clean: ## Remove build artifacts
	rm -rf $(BUILD_DIR)
