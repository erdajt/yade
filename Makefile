CC := gcc

CFLAGS := -Wall -Wextra -std=c23 -Iinclude

SRC_DIR   := src
BUILD_DIR := build

TARGET := $(BUILD_DIR)/yade

SRCS := $(wildcard $(SRC_DIR)/*.c)

OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

run: $(TARGET)
	$(TARGET) $(ARGS)

.PHONY: all clean run
