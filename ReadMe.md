# Grogue

此工程重写中。

## 如何编译

使用CMake进行编译，编译需要依赖库`SDL`,`SDL_ttf`,`SDL_image`,`SDL_mixer`

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