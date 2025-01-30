# abstract_data_types

Interface, implementations and testing for some ADTs in C language.

The following are the ADTs implemented:

* Stack
* Queue
* List
* Map
* SortedMap
* PriorityQueue

## Information

Each ADT has its own documentation in their own directory in a README file. You can also read it in the
`.h` file.

Every ADT is for generic types, which means you could use it with most C datatypes. However it is mandatory to specify the size of the datatype that will used.

The file `assert_msg.h` is used for the testing. However, it must always be in the parent directory of any `.c` implementation file because they use them.

## Testing

Each ADT directory has it own `_test.c` file that tests their operations, this tests don't depend on the implementation in the `.c` file. This means that with the same `.h` file, you can implement your own version that respects the signatures given.

## Compiling

A makefile is provided in every ADT directory so you can compile your files with the following line:

    make

You may change some variables to compile the files like you want.

## License
