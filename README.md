# Gump

## About

Life was like a box of chocolates, you never know what you’re gonna get.

by Gump.

## 编译

使用Qt Creator编译。

* 问题：

    在Mac Qt 5.7.1(homebrew安装)下，打包时，虽然可以改变依赖包的路径，但是存在问题。例如Framework`QtWidgets`依赖于`QtCore`，`Gump`也依赖于`QtCore`。查看依赖时，`otool -L Gump.app/Contents/MacOS/Gump`所依赖的`QtCore`是程序Framework内的`QtCore`。但是`otool -L Gump.app/Contents/Frameworks/QtWidgets.framework/Versions/5/QtWidgets`，发现`QtWidgets`仍然依赖于`Cellar`，即homebrew安装Qt库的位置。这样会导致在没有安装Qt的机器上，不能运行该程序。解决方式：静态链接Qt库。
    
* Qt编译静态库

    ``` sh
    cd qt/path
    ./configure -static
    make -j4
    sudo make install
    ```
    
## 使用

在Preferences中设置相关选项，设置后，Gump会每分钟刷新列表，双击流地址，即可播放观看。

## 其他

### 生成icon.icns

``` sh
iconutil -c icns icon.iconset
```
