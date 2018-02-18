CXX := g++
CFLAGS := -Wall -Iinclude -lpthread --std=c++11 -g -O0 -pthread
DFLAG := 

# Directories
SOURCE_DIR := src
INCLUDES_DIR := include
BUILD_DIR := build
EXEC_DIR := bin
TEST_DIR := test

vpath %.cpp $(SOURCE_DIR)
vpath %.cpp $(SOURCE_DIR)/server
vpath %.cpp $(SOURCE_DIR)/controller
vpath %.cpp $(SOURCE_DIR)/http

# Files
MAIN := $(SOURCE_DIR)/main.cpp

EXEC_NAME := http
PROGRAM := $(EXEC_DIR)/$(EXEC_NAME)
SOURCES := $(shell find $(SOURCE_DIR) -name "*.cpp")
OBJS := $(addprefix $(BUILD_DIR)/,$(patsubst %.cpp, %.o, $(notdir $(SOURCES))))
TESTALL_OUTPUT := http-out

MAKE_INFO := MAKE ::

all: git-commit server

server: prelude $(PROGRAM)

prelude:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(EXEC_DIR)

$(BUILD_DIR)/%.o: %.cpp
	@echo $(MAKE_INFO) "Building $<..."
	@$(CXX) $(CFLAGS) $(DFLAG) -c -o $@ $<

$(PROGRAM): $(OBJS)
	@echo $(MAKE_INFO) "Building executable $(EXEC_NAME)..."
	@$(CXX) $(CFLAGS) $(DFLAG) -o $(PROGRAM) $(OBJS)
	@echo $(MAKE_INFO) "Done. Executable located in $(PROGRAM)"

clean: test-clean prog-clean

test-clean:
	@rm -f $(TEST_DIR)/test*.out* $(TEST_DIR)/test*.org* $(TEST_DIR)/test*.req* $(TEST_DIR)/test*.cmp $(TEST_DIR)/*.valgrind $(TEST_DIR)/*.testall $(TESTALL_OUTPUT)

prog-clean:
	@rm -rf $(BUILD_DIR) $(PROGRAM)
	@mkdir -p $(BUILD_DIR)

git-commit:
	git add *.cpp *.hpp Makefile >> .local.git.out  || echo
	git commit -a -m 'Commit' >> .local.git.out || echo
	git push


.PHONY: clean test-clean prog-clean $(PROGRAM) prelude tests server
