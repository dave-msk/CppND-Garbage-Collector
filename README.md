# CppND-Garbage-Collector

This repository contains code for the Garbage Collector Project in the C++ OOP course provided by Udacity. The code is based on the [CppND-Garbage-Collector Starter Code](https://github.com/udacity/CppND-Garbage-Collector).

The code is reformatted to closely follow the Google C++ Style. See the following section for the list of changes.

## Modifications

1. Added header guards for header files.
2. Used the extension `.cc` for source files.
3. Variable names are renamed from camel case to snake (with trailing underscores for private class members).
4. Functions are renamed to Pascal case (camel case with first character capitalized).
5. A swap function is added to the `Pointer` class to employ the copy-and-swap idiom in assignment operators.

## Compilation & Execution

Run the `make` script:

```bash
$ chmod u+x make
$ ./make
```

Or compile the source code directly:

```bash
$ g++ -o main main.cc -lncurses -std=c++1y -Wall
$ ./main
```
