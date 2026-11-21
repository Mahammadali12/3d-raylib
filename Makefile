CC      := gcc
CFLAGS  := -g -Wall -Wextra -Isrc
LDFLAGS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SRC := $(wildcard src/*.c)
OBJ := $(SRC:src/%.c=build/%.o)
DEP := $(OBJ:.o=.d)

bin/main: $(OBJ) | bin
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

bin build:
	mkdir -p $@

run: bin/main
	./bin/main

clean:
	rm -rf build bin

-include $(DEP)
.PHONY: run clean
