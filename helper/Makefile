CXX = clang++
CXXFLAGS = -std=c++20 -DUNICODE -D_UNICODE -Wall -Wextra -Werror
MAIN_CXXFLAGS = $(CXXFLAGS) -static -static-libstdc++ -static-libgcc

SRC_DIR = src
RESOURCES_DIR = $(SRC_DIR)/resources
BUILD_DIR = build
BIN_DIR = bin

SOURCES = $(shell find $(SRC_DIR) -type f -name *.cpp)
RESOURCES = $(wildcard $(RESOURCES_DIR)/*.rc)

LIB_DIRS = ./../utils/lib
LIBS = -L$(LIB_DIRS)
LIBS += $(addprefix -l:,$(notdir $(wildcard $(LIB_DIRS)/*.a)))
INCLUDE = -I./../utils

APP_NAME = Helper
APP_TYPE = -mwindows

# Debug var
DEBUG_DIR = $(BUILD_DIR)/debug
DEBUG_BIN = $(BIN_DIR)/debug/$(APP_NAME)
DEBUG_OBJ = $(patsubst $(SRC_DIR)/%,$(DEBUG_DIR)/%,$(SOURCES:.cpp=.o))

DEBUG_DIR_RES = $(DEBUG_DIR)/resources
DEBUG_RESOURCES_OBJ = $(patsubst $(RESOURCES_DIR)/%,$(DEBUG_DIR_RES)/%,$(RESOURCES:.rc=.o))

# Release var
RELEASE_DIR = $(BUILD_DIR)/release
RELEASE_BIN = $(BIN_DIR)/release/$(APP_NAME)
RELEASE_OBJ = $(patsubst $(SRC_DIR)/%,$(RELEASE_DIR)/%,$(SOURCES:.cpp=.o))

RELEASE_DIR_RES = $(RELEASE_DIR)/resources
RELEASE_RESOURCES_OBJ = $(patsubst $(RESOURCES_DIR)/%,$(RELEASE_DIR_RES)/%,$(RESOURCES:.rc=.o))

$(info Make running)

debug: CXXFLAGS += -g
debug: $(DEBUG_BIN)

$(DEBUG_BIN): $(DEBUG_OBJ) $(DEBUG_RESOURCES_OBJ)
	mkdir -p bin/debug
	$(CXX) $(MAIN_CXXFLAGS) $(DEBUG_OBJ) $(DEBUG_RESOURCES_OBJ) -o $(DEBUG_BIN) $(APP_TYPE) $(LIBS)

$(DEBUG_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(DEBUG_DIR_RES)/%.o: $(SRC_DIR)/resources/%.rc
	mkdir -p $(@D)
	windres -i $< -o $@

release: CXXFLAGS += -O3
release: $(RELEASE_BIN)

$(RELEASE_BIN): $(RELEASE_OBJ) $(RELEASE_RESOURCES_OBJ)
	mkdir -p bin/release
	$(CXX) $(RELEASE_OBJ) $(MAIN_CXXFLAGS) $(RELEASE_RESOURCES_OBJ) -o $(RELEASE_BIN) $(APP_TYPE) $(LIBS)

$(RELEASE_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(RELEASE_DIR_RES)/%.o: $(SRC_DIR)/resources/%.rc
	mkdir -p $(@D)
	windres -i $< -o $@

clean:
	rm -rf $(BUILD_DIR) bin