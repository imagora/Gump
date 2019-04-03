#---------------------------------------------------------------------
# PART 1: General Setting
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
OBJECTS_DIR = .objs
QT += core gui network networkauth avwidgets

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
macx {
    # Info.plist
    QMAKE_INFO_PLIST = Info.plist

    # Icon
    ICON = icon.icns
}

include(gitversion.pri)

SOURCES += \
    src/main.cpp \
    src/view/about_dialog.cpp \
    src/view/log_dock.cpp \
    src/view/main_menubar.cpp \
    src/controller/log_dock_appender.cpp \
    src/view/player_widget.cpp \
    src/view/application.cpp \
    src/controller/search_event.cpp \
    src/view/login_widget.cpp \
    src/view/gump_window.cpp \
    src/view/central_widget.cpp \
    src/view/line_edit.cpp \
    src/view/logging_widget.cpp \
    src/view/playlist_widget.cpp \
    src/view/play_widget.cpp \
    src/controller/auth_controller.cpp \
    src/view/tag.cpp \
    src/controller/config_controller.cpp \
    src/controller/stream_rule_controller.cpp \
    src/controller/online_controller.cpp \
    src/controller/tracer_controller.cpp \
    src/controller/player_controller.cpp
HEADERS += \
    src/view/about_dialog.h \
    src/commons/definations.h \
    src/view/main_menubar.h \
    src/view/log_dock.h \
    src/controller/log_dock_appender.h \
    src/view/player_widget.h \
    src/view/application.h \
    src/controller/search_event.h \
    src/view/login_widget.h \
    src/view/gump_window.h \
    src/view/central_widget.h \
    src/view/line_edit.h \
    src/view/logging_widget.h \
    src/view/playlist_widget.h \
    src/view/play_widget.h \
    src/controller/auth_controller.h \
    src/view/tag.h \
    src/controller/config_controller.h \
    src/commons/singleton.h \
    src/controller/stream_rule_controller.h \
    src/controller/online_controller.h \
    src/controller/tracer_controller.h \
    src/commons/stream_info.h \
    src/controller/player_controller.h
RESOURCES += \
    resource.qrc
