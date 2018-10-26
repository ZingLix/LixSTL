English | [简体中文](https://github.com/ZingLix/LixSTL/blob/master/Readme.zh.md)

# LixSTL

[![Build Status](https://travis-ci.com/ZingLix/LixSTL.svg?branch=master)](https://travis-ci.com/ZingLix/LixSTL)

LixSTL is an easy implementation of C++ STL in C++17, mainly container and algorithm part. 

I made it while I was learning the STL. In the process of learning, I refer to Hou Jie's "STL 源码剖析" which is a bit old. Therefore, I implement this repo with Modern C++ and have written several articles to introduce the components in STL (not finished yet).

> [Conclusion](https://zinglix.xyz/2018/09/20/STL-container/) and [Catalog](https://zinglix.xyz/archive/?tag=STL) (in Chinese only)

This repo is more suitable for the use of study. I hope this will help more people to understand the implementation of STL better and quicker. If you have any suggestion, issue is welcome. If you feel useful, you can press the star : D

## Build

The compiler needs to support C++17. MSVC 2017, g++ >= 6 and clang >= 6 all works。

If you want to use it in other projects, just add the LixSTL/include folder to the include path.

### Running tests

#### Visual Studio 2017

First, you need to install [Google Test](https://www.nuget.org/packages/googletest/) with Nuget. Then press F5 and the build will begin. Remember to set the language standard to C++17.

#### Linux

gtest and cmake are required. Then run the following commands.

```
cmake .
make
```

The output file will be in the build folder.

## Todo List

- algorithm part
- code improvement（see the TODO in the comment）
