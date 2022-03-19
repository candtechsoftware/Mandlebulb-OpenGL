.PHONY: all debug clean stats format
default: all

# Source
SOURCE_DIR = src 
INCLUDE_DIR = include  
RESOURCE_DIR = resources

# Build
BUILD_DIR = build
BUILD_TYPE = Release

all:
	@cmake -S . -B$(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) .
	@cmake --build $(BUILD_DIR) --config $(BUILD_TYPE)

run: 
	./build/mandlebrot
debug:
	@cmake -B$(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug .
	@cmake --build $(BUILD_DIR) --config Debug

win64:
	@cmake -B$(BUILD_DIR) -G "Visual Studio 15 Win64" .
	@cmake --build $(BUILD_DIR) --config $(BUILD_TYPE) -- /m

clean:
	@rm -rf $(BUILD_DIR)

stats:
	@cloc $(SOURCE_DIR) cmake CMakeLists.txt $(RESOURCE_DIR)/shaders

format:
	@find $(SOURCE_DIR) -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i
	@find $(INCLUDE_DIR) -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i
