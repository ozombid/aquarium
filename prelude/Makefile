CC ?= gcc
DEBUG = gdb
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = 
BUILD_DIR = build

# build
all: 
	make --no-print-directory project

project: clean
	mkdir -p $(BUILD_DIR)
	make client

clean:
	rm -rf $(BUILD_DIR)/*.o client

# compilation
$(BUILD_DIR)/client.o: client.c 
	$(CC) $(CFLAGS) -c -o $@ $<

client: $(BUILD_DIR)/client.o
	$(CC) $^ -o $@ $(LDFLAGS)

.PHONY: client clean


