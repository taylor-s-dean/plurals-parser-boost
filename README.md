# plural-parser
A CLI tool and header library intended to dynamically parse C++ style ternary, mathematical, and boolean expressions.

## Building

```sh
git clone git@github.com:limitz404/expression-parser.git
cd expression-parser
wget https://dl.bintray.com/boostorg/release/1.73.0/source/boost_1_73_0.tar.bz2
tar --bzip2 -xf boost_1_73_0.tar.bz2
rm boost_1_73_0.tar.bz2
clang++ expression-parser.cpp -o expression-parser -I./boost_1_73_0
```

## Usage

```sh
./expression-parser
```
