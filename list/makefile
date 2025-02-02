# Compiler
CC =gcc
CFLAGS =-Werror -Wall

# Change the value of YOUR_FILE for the .c file that uses list.h
# Change the value of OUTPUT_FILE for the name of the result compiled file
YOUR_FILE =list_test.c
OUTPUT_FILE =test

all: $(OUTPUT_FILE)

$(OUTPUT_FILE): $(YOUR_FILE) list.c
	$(CC) $(CFLAGS) -o $(OUTPUT_FILE) $(YOUR_FILE) list.c

clean:
	rm -rf $(OUTPUT_FILE)