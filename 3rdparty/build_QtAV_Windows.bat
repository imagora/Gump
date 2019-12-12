
echo "Qt: %QT_HOME%"

SET ffmpeg_path=/usr/local/opt/ffmpeg
SET openal_path="C:\program files\openal\"
SET libass_path=/usr/local/opt/libass
SET uchardet_path=/usr/local/opt/uchardet

SET CPATH=%ffmpeg_path%/include;%libass_path%/include;%uchardet_path%/include;%CPATH%
SET LIBRARY_PATH=%ffmpeg_path%/lib;%libass_path%/lib;%uchardet_path%/lib;%LIBRARY_PATH%

RD /S /D build
MKDIR build
CD build
%QT_HOME%/bin/qmake.exe ../QtAV/QtAV.pro
make -j4