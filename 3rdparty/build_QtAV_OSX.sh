#!/bin/bash
echo "Qt: $QT_HOME"
#echo "CONFIG += static_ffmpeg static_openal" >> QtAV/.qmake.conf

ffmpeg_lib_path=/usr/local/lib
ffmpeg_include_path=/usr/local/include
libass_path=/usr/local/opt/libass
uchardet_path=/usr/local/opt/uchardet

export CPATH=$ffmpeg_include_path:$libass_path/include:$uchardet_path/include:$CPATH
export LIBRARY_PATH=$ffmpeg_lib_path:$libass_path/lib:$uchardet_path/lib:$LIBRARY_PATH
export DYLD_LIBRARY_PATH=$ffmpeg_lib_path:$libass_path/lib:$uchardet_path/lib:$DYLD_LIBRARY_PATH

rm -rf build
mkdir build
cd build
$QT_HOME/bin/qmake ../QtAV/QtAV.pro
make -j4
