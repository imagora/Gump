# Gump

## About

Life was like a box of chocolates, you never know what you’re gonna get.

by Gump.

## 编译

1. 安装[Qt](https://www.qt.io/download)

2. 安装ffmpeg

   ``` sh
   brew install ffmpeg --with-sdl2
   ```

3. 安装[QtAV](3rdparty/README.md)

   ``` sh
   git submodule init
   git submodule update
   ```

4. 编译Gump

5. 打包

   ``` sh
   ./deploy.sh
   cd bin;
   qt_bin/macdeployqt Gump.app -dmg
   ```

## 使用

在Preferences中设置相关选项，设置后，Gump会每分钟刷新列表，双击流地址，即可播放观看。

## 其他

### 生成icon.icns

``` sh
iconutil -c icns icon.iconset
```
