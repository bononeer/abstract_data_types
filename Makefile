# Compiler and its flags
CC = gcc
CFLAGS := -g -std=c99 -Wall -Wconversion -Wtype-limits 
CFLAGS += -Wno-sign-conversion -Werror -pedantic
CFLAGS += -I./include -I./src

SRCS = $(wildcard src/*/*.c)
OBJS = $(SRCS:.c=.o)

# Library
NAME = adts
LIB = lib$(NAME).a

# Compile your file
FILES = ./examples/queue_example.c
OUTPUT = xmpl

# Testing
ADT = queue

$(LIB): $(OBJS)
	ar rcs $@ $^
	rm -f $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

compile:
	$(CC) $(CFLAGS) $(FILES) -Iinclude -L. -l$(NAME) -o $(OUTPUT)

valgrind: compile vlgrd

vlgrd: 
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 ./$(OUTPUT)

debug: compile gdb

gdb:
	gdb ./$(OUTPUT)

test: ./tests
	$(CC) $(CFLAGS) ./tests/$(ADT)_test.c ./tests/testaux.* -Iinclude -L. -l$(NAME) -o test

clean:
	rm -f $(LIB) test $(OUTPUT)