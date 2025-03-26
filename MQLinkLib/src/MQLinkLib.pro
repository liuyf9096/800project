QT -= gui

TEMPLATE = lib
DEFINES += MQLINKLIB_LIBRARY

CONFIG += c++17
VERSION = 1.0.0
CONFIG += skip_target_version_ext

unix:!macx{
    CONFIG += link_pkgconfig
    PKGCONFIG += libzmq
    CONFIG += link_pkgconfig
    PKGCONFIG += libmosquitto
}

SOURCES += \
    f_common.cpp \
    module/file/f_file_manager.cpp \
    module/logger/f_log_server.cpp \
    module/settings/f_settings.cpp \
    mqlink.cpp \
    mqtt/mqtt_client.cpp \
    mqtt/mqtt_server.cpp \
    zmq/zmq_client.cpp \
    zmq/zmq_server.cpp


HEADERS += \
    ../include/mqlink.h \
    f_common.h \
    module/file/f_file_manager.h \
    module/logger/f_log_server.h \
    module/logger/f_log_server_p.h \
    module/settings/f_settings.h \
    mqlink_private.h \
    mqtt/mqtt_client.h \
    mqtt/mqtt_messager_private.h \
    mqtt/mqtt_server.h \
    zmq/zmq_client.h \
    zmq/zmq_messager_private.h \
    zmq/zmq_server.h


CONFIG(release, debug|release): {
    DESTDIR = $$PWD/../output
    TARGET = libmqlink
} else:CONFIG(debug, debug|release): {
    DESTDIR = $$PWD/../output
    TARGET = libmqlinkd
}

INCLUDEPATH += \
    ../include \
    module \
    zmq \
    mqtt

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../zeromq/lib/ -llibzmq-mt-4_3_5
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../zeromq/debug/lib/ -llibzmq-mt-gd-4_3_5

INCLUDEPATH += $$PWD/../../../../../zeromq/include
DEPENDPATH += $$PWD/../../../../../zeromq/include
