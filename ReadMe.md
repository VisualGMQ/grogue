# Grogue

[![Build status](https://ci.appveyor.com/api/projects/status/dri0av5hvheh7o9r?svg=true)](https://ci.appveyor.com/project/VisualGMQ/grogue-32xuj)
![GitHub Workflow Status (with branch)](https://img.shields.io/github/actions/workflow/status/VisualGMQ/grogue/build.yml?branch=rewrite&label=clang%2B%2B&logo=ubuntu)

**此工程底层后面要大改，不建议现在阅读代码。**

Grogue是一个Roguelite游戏，借鉴了饥荒，CDDA。正在开发中。

可以在[这里](https://visualgmq.github.io/projects/grogue-demo/grogue.html)试玩最新的网页版Grogue，每周日更新。[这里](https://visualgmq.github.io/grogue/)则是Grogue的开发日志，每周增加的功能将会显示在这里。

更多内容，请看[Wiki部分](https://github.com/VisualGMQ/grogue/wiki)

代码目前有一些Bug和缺陷，看[这里](./bug.md)

## 如何编译

使用CMake进行编译，编译需要依赖库`SDL`,`SDL_ttf`,`SDL_image`,`SDL_mixer`。需要安装Python3和llvm来进行头文件解析。

如果你想要为项目贡献代码，编译前推荐安装`clang-format`和`CppCheck`，`clang-format`会帮你自动格式化，`CppCheck`会帮你做静态检查（仅在`Makefile`和`Ninja`工程中）


Windows下你需要设置SDL库的根目录：

```bash
cmake -S . -B cmake-build -DSDL2_ROOT=XXX -DSDL2_TTF_ROOT=XXX -DSDL2_MIXER_ROOT=XXX -DSDL2_TTF_ROOT=XXX
```

根目录是在Github上下载好的SDL-devel系列包解压后的根目录。

然后编译

```bash
cmake --build cmake-build
```

如果你不想手动下载SDL库，可以指定`FETCH_SDL=ON`来让CMake帮你下载（在Github上下载，所以很看网）。


Linux和MacOSX下，使用包管理器安装完SDL系列库后，可以直接进行编译：

```bash
cmake -S . -B build
cmake --build build
```


要编译成网页汇编（WASM），请先安装emscripten工具，然后使用CMake编译：

```bash
emcmake cmake -S . -B wasm-build
cmake --build build
```
