CXX = clang++
AR = llvm-ar
CXXFLAGS = -std=c++20 -DUNICODE -D_UNICODE -Wall -Wextra -Werror

SRC_DIR = .
BUILD_DIR = build
LIB_DIR = lib

SOURCES = $(shell find $(SRC_DIR) -type f -name *.cpp)
OBJECT = $(patsubst %.cpp,%.o,$(SOURCES))
LIB = $(patsubst %.o,%.a,$(OBJECT))

run: $(OBJECT) $(LIB) move_objects

$(LIB): %.a: %.o
	$(AR) rcs $@ $<

$(OBJECT): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

move_objects:
	$(shell [ -d $(LIB_DIR) ] || mkdir -p $(LIB_DIR))
	$(foreach file,$(LIB),$(shell mv $(file) $(LIB_DIR)))
