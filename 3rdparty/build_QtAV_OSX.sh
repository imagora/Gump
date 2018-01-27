#!/bin/bash
echo "Qt: $QT_HOME"
#echo "CONFIG += static_ffmpeg static_openal" >> QtAV/.qmake.conf

ffmpeg_path=/usr/local/opt/ffmpeg
libass_path=/usr/local/opt/libass
uchardet_path=/usr/local/opt/uchardet

export CPATH=$ffmpeg_path/include:$libass_path/include:$uchardet_path/include:$CPATH
export LIBRARY_PATH=$ffmpeg_path/lib:$libass_path/lib:$uchardet_path/lib:$LIBRARY_PATH
export DYLD_LIBRARY_PATH=$ffmpeg_path/lib:$libass_path/lib:$uchardet_path/lib:$DYLD_LIBRARY_PATH

rm -rf build
mkdir build
cd build
$QT_HOME/bin/qmake ../QtAV/QtAV.pro
make -j4
