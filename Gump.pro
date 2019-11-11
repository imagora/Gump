#---------------------------------------------------------------------
# PART 1: General Setting
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
OBJECTS_DIR = .objs
QT += core gui network avwidgets

include(gitversion.pri)

#---------------------------------------------------------------------
# PART 2: target, library, flags
TARGET = bin/Gump
INCLUDEPATH += src
INCLUDEPATH += /usr/local/include
LIBS += -L/usr/lib -L/usr/local/lib -llog4cplus
DEFINES += QT_DEPRECATED_WARNINGS

#---------------------------------------------------------------------
# PART 3: Clean
QMAKE_CLEAN += $(TARGET) $(QMAKE_TARGET) bin/Gump

#---------------------------------------------------------------------
# PART 4: Input
SOURCES += \
    src/main.cpp \
    src/view/*.cpp \
    src/controller/*.cpp
HEADERS += \
    src/commons/definations.h \
    src/view/*.h \
    src/controller/*.h
RESOURCES += \
    resource.qrc

macx {
    # Info.plist
    QMAKE_INFO_PLIST = Info.plist

    # Icon
    ICON = icon.icns
}
