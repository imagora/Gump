#!/bin/bash
echo "CONFIG += static_ffmpeg static_openal" >> QtAV/.qmake.conf
export CPATH=/usr/local/Cellar/ffmpeg/3.2.2/include:$CPATH
export CPATH=/usr/local/Cellar/libass/0.13.7/include:$CPATH
export CPATH=/usr/local/Cellar/uchardet/0.0.6/include:$CPATH
export LIBRARY_PATH=/usr/local/Cellar/ffmpeg/3.2.2/lib:$LIBRARY_PATH
export LIBRARY_PATH=/usr/local/Cellar/libass/0.13.7/lib:$LIBRARY_PATH
export LIBRARY_PATH=/usr/local/Cellar/uchardet/0.0.6/lib:$LIBRARY_PATH
export DYLD_LIBRARY_PATH=/usr/local/Cellar/ffmpeg/3.2.2/lib:$DYLD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=/usr/local/Cellar/libass/0.13.7/lib:$DYLD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=/usr/local/Cellar/uchardet/0.0.6/lib:$DYLD_LIBRARY_PATH

rm -rf build
mkdir build
cd build
/usr/local/Cellar/qt/5.7.1_1/bin/qmake ../QtAV/QtAV.pro
make -j4
