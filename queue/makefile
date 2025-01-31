# Compiler
CC =gcc
CFLAGS =-Werror -Wall

# Change the value of YOUR_FILE for the .c file that uses queue.h
# Change the value of OUTPUT_FILE for the name of the result compiled file
YOUR_FILE =queue_test.c
OUTPUT_FILE =test

all: $(OUTPUT_FILE)

$(OUTPUT_FILE): $(YOUR_FILE) queue.c
	$(CC) $(CFLAGS) -o $(OUTPUT_FILE) $(YOUR_FILE) queue.c

clean:
	rm -rf $(OUTPUT_FILE)