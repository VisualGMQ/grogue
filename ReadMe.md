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

你也可以选择同时编译单元测试（默认不编译）:

```bash
cmake -S . -B build -G"MinGW Makefiles" -DBUILD_UNITTEST=ON
```

# 各个模块的说明

## Video和VideoMgr

`Video`是一个拥有Window和Renderer的结构。其将Window和对应的Renderer绑定在一起。

你可以通过VideoMgr创建Video（即创建新的窗口）。引擎初始化时默认创建一个Video。你可以额外使用

```cpp
VideoMgr::CreateVideo(title, width, height);
```

创建多于一个的窗口。

## Window

即对`SDL_Window`的简单封装

## Renderer

即对`SDL_Renderer`的简单封装

## Log和Logger

使用

```cpp
LOG_CRITICAL(...)
LOG_ERROR(...)   
LOG_WARN(...)    
LOG_INFO(...)    
LOG_DEBUG(...)   
LOG_TRACE(...)   
```

宏来输出日志。这会使用所有创建并启用的`Logger`进行日志输出。

通过

```cpp
Log::CreateConsoleLogger(name);
Log::CreateFileLogger(name, filename);
```

创建并启用新的Logger。要停止Logger的输出，可以将Logger的输出日志设为`NoLog`:

```cpp
logger->SetLevel(LogLevel::NoLog);
```

## Scence和Layer

Scence即场景，表示游戏中的场景（比如开始游戏的欢迎界面是一个场景，游戏本体的界面又是一个场景）。

通过继承Scence可以创造属于你自己的场景。

通过重载`OnInit`和`OnQuit`方法来掌控场景初始化和结束时的动作。你可以在`OnInit`中加载此场景所需的资源。


Layer则是在场景中更细致的划分，称为层。一个场景中通常会有很多层，较为常见的是拥有一个GUI层用于绘制和响应GUI，一个游戏层用于绘制和响应游戏本身。

Layer可以进行事件操控（重载`OnEventXXX`函数来实现）。返回true表示允许将事件传播到下一层。

通过

```cpp
scence->PushBackLayer<LayerClass>(params...);
scence->PushFrontLayer<LayerClass>(params...);
```

来向场景中添加层。场景初始化时是默认没有层的。

## Timer

Timer是静态类，用于记录相邻两帧的时间间隔。通过

```cpp
Timer::GetMsBetweenFrames();
```

得到以毫秒为单位的时间。

## UTF8String

即记录着UTF-8编码的字符串。主要是为了拆分很长一行string而制作的。

## Texture

对`SDL_Texture`的简单封装。

## Image

对`Texture`的简单封装，主要是将Texture的各种变换设置放在一起，并且指定绘制区域。

## GUI

自己实现的ImGUI

## Keyboard

静态类，提供对键盘的查询

## Mouse

静态类，提供对鼠标的查询。

需要注意的是`Mouse::Position()`函数返回的鼠标坐标会随着窗口大小的变化而自动伸缩。

使用`Mouse::RealPosition()`得到无伸缩的鼠标坐标。
