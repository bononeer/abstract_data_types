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

Every ADT is for generic types, which means you could use it with most C datatypes. It's important to be careful with the elements added and obtained from the ADTs and how you cast them.

The types of the elements added should be the same as the ones obtained if you don't want to lose information.

An example with ADT Stack:

```c
Stack s = stack_create();
int num = 1;
void* top = NULL;

stack_push(s, &num);
top = stack_pop(s);

int obtained = *(int*)top;
```

The file `assert_msg.h` is used for the testing. However, it must always be in the parent directory of any `.c` implementation file because they use them for error handling.

With the given `.h` file in an ADT directory, you can implement your own version that respects the signatures for its operations.

## Testing

Each ADT directory has it own `*_test.c` file that tests their operations, these tests don't depend on the implementation in the `.c` file.

## Compiling

A makefile is provided in every ADT directory so you can compile your files with the following line:

    make

You may change some variables to compile the files like you want.

## License
