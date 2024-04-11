# cppgol

A [Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) in C++.

All graphics done with [Raylib](https://www.raylib.com/)

## Installation

Conan is used for dependency management. CMake is used to build.

## Usage

Before compiling you can choose the implementation by compiling with either `vector_grid` or `set_grid`.

To install dependencies:

```bash
conan install . --output-folder=build --build=missing
```

To build:

```bash
cmake --preset conan-release
cmake --build build
```


To run executable:

```bash
./build/cppgol
```
