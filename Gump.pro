#---------------------------------------------------------------------
# PART 1: General Setting
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
OBJECTS_DIR = .objs
QT += core gui network

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
    src/view/main_window.cpp \
    src/view/about_dialog.cpp \
    src/view/main_menubar.cpp \
    src/view/preferences_dialog.cpp \
    src/view/main_central_widget.cpp \
    src/controller/stream_manager.cpp \
    src/view/main_toolbar.cpp \
    src/view/log_dock.cpp \
    src/controller/log_dock_appender.cpp
HEADERS  += \
    src/view/main_window.h \
    src/view/about_dialog.h \
    src/view/main_menubar.h \
    src/view/preferences_dialog.h \
    src/view/main_central_widget.h \
    src/controller/stream_manager.h \
    src/commons/definations.h \
    src/view/main_toolbar.h \
    src/view/log_dock.h \
    src/controller/log_dock_appender.h
RESOURCES += \
    resource.qrc
ICON = icon.icns
