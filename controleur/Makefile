CC ?= gcc
DEBUG = gdb
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = 
BUILD_DIR = build
VAL = valgrind
VALFLAGS = --leak-check=full --show-reachable=yes --track-origins=yes
EXEC = ./
VPATH = lib:lib
PORT ?= 8000

all: clean 
	mkdir -p $(BUILD_DIR)
	make --no-print-directory build

clean:
	rm -rf $(BUILD_DIR)/*.o server aquarium.*

# compilation
$(BUILD_DIR)/utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/move.o: move.c move.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/view.o: view.c view.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/fish.o: fish.c fish.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/syntax.o: syntax.c syntax.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/aquarium.o: aquarium.c aquarium.h
	$(CC) -Wall -Wextra -g -c -o $@ $<

$(BUILD_DIR)/client.o: client.c client.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/server.o: server.c server.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/main.o: main.c 
	$(CC) $(CFLAGS) -c -o $@ $<

# build
build: $(BUILD_DIR)/utils.o \
	   $(BUILD_DIR)/move.o \
	   $(BUILD_DIR)/view.o \
	   $(BUILD_DIR)/fish.o \
	   $(BUILD_DIR)/syntax.o \
	   $(BUILD_DIR)/aquarium.o \
	   $(BUILD_DIR)/client.o \
	   $(BUILD_DIR)/server.o \
       $(BUILD_DIR)/main.o 
	   $(CC) $^ -o server $(LDFLAGS)

server: 
	./main $(PORT)

valgrind: 
	$(VAL) $(VALFLAGS) ./main $(PORT)


.PHONY: build server clean


