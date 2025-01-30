# Compiler
CC =gcc
CFLAGS =-Werror -Wall

# 
# Change the value of YOUR_FILE for the .c file that uses stack.h
# Change the value of OUTPUT_FILE for the name of the result compiled file
YOUR_FILE =stack_test.c
OUTPUT_FILE =test

all: $(OUTPUT_FILE)

$(OUTPUT_FILE): $(YOUR_FILE) stack.c
	$(CC) $(CFLAGS) -o $(OUTPUT_FILE) $(YOUR_FILE) stack.c

clean:
	rm -rf $(OUTPUT_FILE)