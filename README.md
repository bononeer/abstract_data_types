# Abstract Data Types

Interface, implementations and testing for some ADTs in C language.

The following are the ADTs implemented:

* ***[Stack](stack)***
* ***[Queue](queue)***
* ***[List](list)***
* ***[Map](map)***
* ***[BST](bst)***
* ***[PriorityQueue](priority_queue)***

## Information

Each ADT has the documentation for their operations in the README file inside their own directory. You can also read it in the `.h` file.

Every ADT is for generic types, which means you could use it with most C datatypes. If you don't want to lose information, the types of the elements obtained should be casted as the same type as the ones added.

An example with **ADT Stack**:

```c
Stack s = stack_create(free);
int* num = (int*)malloc(sizeof(int));
void* top = NULL;

*num = 10;

stack_push(s, num);
top = stack_pop(s);

int obtained = *(int*)top;
free(top);
```

With the given `.h` file in an ADT directory, you can implement your own version that respects the signatures for its operations.

## Testing

Each ADT has it own `*_test.c` file that tests their operations, these tests don't depend on the implementation in the `.c` file. You can find these in the [tests](tests) directory.

The file `assert_msg.h` defines a macro used for testing.

To compile a test file for any `adt` just write in your command line:

```shell
make adt
```

## Compiling

To compile a file that uses any `adt` the interface (`.h` file) and the implementation (`.c` file) is needed in the same directory as the file that uses the `adt`. For example:

```shell
/my_directory
    └──main.c // The file that includes the adt
    └──adt.h
    └──adt.c
```

Then, the following must be written in the terminal (depending on the prefered compiler, for this example I use gcc):

```shell
gcc -o main main.c adt.c
```

For the **ADT BST**, both the `bst.c` and `stack.c` must be added to the compilation.

## License
