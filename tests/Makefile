# Compiler
CC =gcc
CFLAGS =-g -std=c99 -Wall -Wconversion -Wtype-limits -Wno-sign-conversion -Werror -pedantic

# The name of the file that will result from the compilation
OUTPUT_FILE =test

stack: ../stack/stack.*
	$(CC) $(CFLAGS) -o $(OUTPUT_FILE) stack_test.c ../stack/stack.c

queue: ../queue/queue.*
	$(CC) $(CFLAGS) -o $(OUTPUT_FILE) queue_test.c ../queue/queue.c

list: ../list/list.*
	$(CC) $(CFLAGS) -o $(OUTPUT_FILE) list_test.c ../list/list.c

clean:
	rm -rf $(OUTPUT_FILE)