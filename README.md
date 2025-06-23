# Abstract Data Types

[![version](https://img.shields.io/badge/version-0.3.1-red)](https://github.com/tu-usuario/tu-repo/releases)

Repository for my Abstract Data types Library for C Language.

## Content

- [Information](#information)
- [ADTs](#adts)
- [Compilation](#compilation)
<!-- - [Prerequisites](#prerequisites)
- [License](#license) -->

## Information

Every ADT has its own [header](include) file and its [implementation](src) files. The [adtcomm.h](include/adt/adtcomm.h) is a common header file for every ADT.

All of the ADT are intended to be used for the generic type (`void *`).

If you are not sure how to use an ADT, check [examples](examples). Test cases for the ADTs are provided in the [tests](tests) directory, you can compile them with the *test* target from the [Makefile](Makefile).

## ADTs

The following are the implemented ADTs:

- ***[Stack](include/adt/stack.h)***
- ***[Queue](include/adt/queue.h)***
- ***[List](include/adt/list.h)***

## Compilation

The [Makefile](Makefile) will help you create the static library file.

```bash
$ make
> gcc ...
> ar rcs libadts.a ...
```

This will create the static library file `libadts.a` in the repo's root directory.

To compile a file that includes one of its headers, you could do:

```bash
$ gcc source.c -Iinclude -L. -llibads -o a.out 
```

or you could also change some of the Makefile variables to match your preferances and do:

```bash
$ make compile
```

<!-- ## Prerequisites

All Makefiles and instructions are written with gcc as an example. However, you can use your favourite one.  

## License -->
