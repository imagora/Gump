# QtAV

## About

[QtAV](http://qtav.org/) is a multimedia playback library based on Qt and FFmpeg. It can help you to write a player with less effort than ever before.

## 编译&安装

``` sh
export QT_HOME=qt/home/path
./build_QtAV_OSX.sh
sudo sh build/sdk_install.sh
```

## Qt 5.12.x 支持性问题

1. 修改头文件 Include 路径：

打开 `qt/home/path/mkspecs/features/av.prf` 和 `qt/home/path/mkspecs/features/avwidgets.prf`，将 `INCLUDEPATH` 参数中 Headers 后面的部分去掉。

2. 修改链接问题：

打开 `qt/home/path/mkspecs/modules/qt_lib_av.pri` 和 `qt/home/path/mkspecs/modules/qt_lib_avwidgets.pri` 文件，分别增加 `QT.av.module = QtAV` 和 `QT.avwidgets.module = QtAVWidgets` 参数，这样在 qmake 生成 Makefile 时，才能自动将 QtAV 和 QtAVWidgets 增加到链接中。

