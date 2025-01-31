# Abstract Data Types

Interface, implementations and testing for some ADTs in C language.

The following are the ADTs implemented:

* [Stack](stack)
* [Queue](queue)
* List
* Map
* SortedMap
* PriorityQueue

## Information

Each ADT has the documentation for their operations in the README file inside their own directory. You can also read it in the `.h` file.

Every ADT is for generic types, which means you could use it with most C datatypes. If you don't want to lose information, the types of the elements obtained should be casted as the same type as the ones added.

An example with ADT Stack:

```c
Stack s = stack_create();
int num = 1;
void* top = NULL;

stack_push(s, &num);
top = stack_pop(s);

int obtained = *(int*)top;
```

With the given `.h` file in an ADT directory, you can implement your own version that respects the signatures for its operations.

## Testing

Each ADT directory has it own `*_test.c` file that tests their operations, these tests don't depend on the implementation in the `.c` file.

The file `assert_msg.h` is used for the testing. For the correct working of the tests, it must always be in the parent directory of any ADT directory.

## Compiling

A makefile is provided in every ADT directory so you can compile your files with the following line:

    make

You may change some variables to compile the files like you want.

## License
