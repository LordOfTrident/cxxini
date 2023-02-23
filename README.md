<p align="center">
	<img width="300px" src="./res/logo.png">
</p>
<p align="center">
	<a href="./LICENSE">
		<img alt="License" src="https://img.shields.io/badge/license-MIT-26c374?style=for-the-badge">
	</a>
	<a href="https://github.com/LordOfTrident/celisp/issues">
		<img alt="Issues" src="https://img.shields.io/github/issues/LordOfTrident/cxxini?style=for-the-badge&color=4f79e4">
	</a>
	<a href="https://github.com/LordOfTrident/celisp/pulls">
		<img alt="GitHub pull requests" src="https://img.shields.io/github/issues-pr/LordOfTrident/cxxini?style=for-the-badge&color=4f79e4">
	</a>
	<br><br><br>
</p>

A simple single-header C++11 [INI](https://en.wikipedia.org/wiki/INI_file) reading and writing library.

## Table of contents
* [Documentation](#documentation)
  * [Constructing](#constructing)
    * [Initializer lists](#initializer-lists)
    * [Index operator](#index-operator)
    * [String literals](#string-literals)
  * [Writing](#writing)
  * [Reading](#reading)
* [Examples](#examples)
* [Quickstart](#quickstart)
* [Bugs](#bugs)

## Documentation
This library provides a single class `ini::record`, which inherits from `ini::sections`, which is
a `std::map<std::string, std::map<std::string, std::string>>`, so it has all of its functionality.

### Constructing
You can construct a record either by using initializer lists, index operator or string literals.

#### Initializer-lists
With the constructor
```cc
ini::record rec({
	{ini::default_section, { // ini::default_section is the unnamed section at the
		{"foo", "bar"},      // top of an ini file and is the same as []
	}},

	{"food-ratings", {
		{"salmon",       "10"},
		{"fish n chips", "8"},
		{"pizza",        "9"},
	}},
});
```
Or you can use assignment
```cc
auto rec = ini::record({
	// ...
});
```

#### Index operator
```cc
ini::record rec;
rec[ini::default_section]["foo"] = "bar";

rec["food-ratings"]["salmon"]       = "10";
rec["food-ratings"]["fish n chips"] = "8";
rec["food-ratings"]["pizza"]        = "9";
```

#### String literals
```cc
ini::record rec("foo = bar\n"
                "\n"
                "[food-ratings]\n"
                "salmon       = 10\n"
                "fish n chips = 8\n"
                "pizza        = 9\n");
```

### Writing
For writing, you can use the `stringify` method to return an `std::string`
```cc
ini::record rec;
rec[ini::default_section]["foo"] = "bar";

std::cout << rec.stringify() << std::endl;
```

or you can use `<<` to insert it into a stream (this just calls `stringify` under the hood)
```cc
ini::record rec;
rec[ini::default_section]["foo"] = "bar";

std::cout << rec << std::endl;
```

### Reading
Reading from a string
```cc
ini::record rec;
rec.parse("foo = bar");
```

from a file
```cc
ini::record rec;

std::ifstream file("test.ini");
rec.parse(file);
```
The return value of `parse` is 0 if there was no error, otherwise its the number of the line at
which the error occured (starting at 1)

you can also use the `>>` operator to read from a file
```cc
ini::record rec;

std::ifstream file("test.ini");
file >> rec;
```

## Examples
Examples can be found in the [examples](./examples) folder.
To build them, first bootstrap the builder with
```sh
$ cc build.c -o build
```
(you can also use a C++ compiler for it)

and then build
```sh
$ ./build
```

The binaries will be outputted into [bin](./bin).

## Quickstart
Either copy the header file [cxxini.hh](./cxxini.hh) into your project, or you can use git submodules:
```sh
$ git submodule add https://github.com/LordOfTrident/cxxini
```

## Bugs
If you find any bugs, please create an issue and report them.
