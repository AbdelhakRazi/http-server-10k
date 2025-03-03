# we should include header files, because a rule detects when a file changes, in our case it's detecting only .cc since
# we're pasing only cc files, we should put headers used in prequesities fields, to be detected if changed.
# automatic variables
# $@: target of the rule
# $^: all prequisites: like parser.cc source.cc...
# $<: first prerequisite
# $*: target file name without extension
# % : is used for pattern matching, for example %.o matches all .o files, %.c matches all .c files
# -D: is used to generate macros: -DDEBUG equivalent to: #define DEBUG 1
# better approach now: % takes for each c file, and generate .o file.

CC = g++
CPPFLAGS = --std=c++17 -MMD -MP -I src -pthread

# Detect OS
UNAME_S := $(shell uname -s)

# Add OS-specific source files and flags
ifeq ($(UNAME_S),Linux)
    POLLING_SRC = src/polling/linux_polling.cc
    CPPFLAGS += -DLINUX_OS
else ifeq ($(UNAME_S),Darwin)  # Darwin is for macOS
    POLLING_SRC = src/polling/mac_polling.cc
    CPPFLAGS += -DMAC_OS
endif

BUILD_DIR = build
# Find all .cc files except polling implementations
SRC = $(shell find . -name "*.cc" ! -name "linux_polling.cc" ! -name "mac_polling.cc")
# Add the appropriate polling implementation
SRC += $(POLLING_SRC)
OBJ = $(SRC:%.cc=$(BUILD_DIR)/%.o)
DEP = $(OBJ:.o=.d)
TARGET = server.out

# Optionally define DEBUG or other macros
ifeq ($(DEBUG), 1)
    CPPFLAGS += -DDEBUG
endif

# Create necessary directories
$(BUILD_DIR)/%.o: %.cc | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR) $(BUILD_DIR)/src/server $(BUILD_DIR)/src/thread_pool $(BUILD_DIR)/src/task $(BUILD_DIR)/src/parser $(BUILD_DIR)/src/polling $(BUILD_DIR)/src/app

# Include dependency files
-include $(DEP)

# Default target
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CPPFLAGS) $^ -o $@

# Custom object rules
$(BUILD_DIR)/main.o: src/main.cc $(BUILD_DIR)/app/app.o $(BUILD_DIR)/server/tcp_server.o
	$(CC) $(CPPFLAGS) -c $< -o $@

# Modified tcp_server rule to use OS-specific polling
$(BUILD_DIR)/server/tcp_server.o: server/tcp_server.cc $(BUILD_DIR)/parser/http_parser.o $(BUILD_DIR)/thread_pool/thread_pool.o \
 $(BUILD_DIR)/task/add_client.o $(BUILD_DIR)/task/read_request.o $(BUILD_DIR)/task/send_response.o \
 $(BUILD_DIR)/polling/$(if $(filter Linux,$(UNAME_S)),linux_polling.o,mac_polling.o)
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/task/read_request.o: thread_pool/read_request.cc $(BUILD_DIR)/task/send_response.o
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/thread_pool/worker.o: thread_pool/worker.cc $(BUILD_DIR)/task/read_request.o $(BUILD_DIR)/task/send_response.o
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/thread_pool/thread_pool.o: thread_pool/thread_pool.cc $(BUILD_DIR)/task/worker.o
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/app/app.o: app/app.cc
	$(CC) $(CPPFLAGS) -c $< -o $@

# General rule for object files
$(BUILD_DIR)/%.o: %.cc
	$(CC) $(CPPFLAGS) -c $< -o $@

# Clean up build artifacts
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)