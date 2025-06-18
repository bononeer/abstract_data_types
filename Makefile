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

# Compile
FILES = ./examples/stack_example.c
OUTPUT = xmpl

$(LIB): $(OBJS)
	ar rcs $@ $^
	rm -f $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

compile:
	$(CC) $(CFLAGS) $(FILES) -Iinclude -L. -l$(NAME) -o $(OUTPUT)

clean:
	rm -f $(LIB)