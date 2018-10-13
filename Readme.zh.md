[EngLish](https://github.com/ZingLix/LixSTL/blob/master/Readme.md) | 简体中文

# LixSTL

[![Build Status](https://travis-ci.com/ZingLix/LixSTL.svg?branch=master)](https://travis-ci.com/ZingLix/LixSTL)

LixSTL 是在 C++17 基础上实现的一份简易的 C++ STL，主要是 container 和 algorithm 两大部分。

这个项目是我在学习 STL 过程中诞生的，在学习的过程中参考了侯捷老师的《STL 源码剖析》，但是年代有些久远，所以我在实现这个项目的过程用了 Modern C++ 并有相关的笔记（但还没全部完成）。

> [总结](https://zinglix.xyz/2018/09/20/STL-container/) 与 [文章目录](https://zinglix.xyz/archive/?tag=STL)

这个库更多的是学习用途，我希望能让更多人在深入研究 STL 的时候能更好更快的理解，希望能帮助你。如果有建议，欢迎提 issue 并一同来完善。如果觉得有帮助，可以戳一下 star 哦：D

## Build

编译需要支持 C++17 的编译器，MSVC 2017、g++ >= 6、clang >= 6 测试通过。

如果你想在其他项目中使用，直接将 LixSTL/include 文件夹添加到 include 路径中即可。

### 运行测试

#### Visual Studio 2017

先需要通过 Nuget 安装 [Google Test](https://www.nuget.org/packages/googletest/)，之后按下 F5 就可以直接开始构建。记住要开启 C++17。

#### Linux

先安装 gtest 和 cmake，之后运行

```
cmake .
make
```

成功后进入 build 文件夹，`./LixSTL_test` 即可运行。

## Todo List

- 算法部分
- 代码改进（见代码中注释的 TODO）