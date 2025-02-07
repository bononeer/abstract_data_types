# Abstract Data Types

Interface, implementations and testing for some ADTs in C language.

The following are the ADTs implemented:

* [Stack](stack)
* [Queue](queue)
* [List](list)
* [Map](map)
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

Each ADT has it own `*_test.c` file that tests their operations, these tests don't depend on the implementation in the `.c` file. You can find these in the [test](tests) directory.

The file `assert_msg.h` defines a macro used for testing.

To compile a test file for a generic `adt` just write in your command line:

```shell
    make adt
```

## Compiling

## License
