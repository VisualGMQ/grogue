# 如何编译

Linux和MacOS下请确保安装了`SDL2`,`SDL2_image`,`SDL2_ttf`，然后执行cmake即可：

```bash
cmake -S . -B build
cmake --build build
```

Windows下将
[SDL2开发包](http://www.libsdl.org/release/SDL2-devel-2.0.20-mingw.tar.gz)
[SDL2_image开发包](http://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.0.5-mingw.tar.gz)
[SDL2_ttf开发包](https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.0.18/SDL2_ttf-devel-2.0.18-mingw.tar.gz)
下载下来，打开开发包之后将对应平台的内容（32位mingw就用i686-w64-mingw32,64位就用x86\_64-w64-mingw32）,将
文件夹内的所有内容拷贝到`3rdlib/SDL2XXX`下（SDL2拷贝到SDL2，SDL2\_ttf拷贝到SDL2\_ttf，SDL2\_image拷贝到SDL2_image下）

然后运行cmake:

```bash
cmake -S . -B build -G"MinGW Makefiles"
cmake --build build
```

即可
