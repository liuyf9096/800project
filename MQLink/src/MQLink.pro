QT += core mqtt

CONFIG += c++17
VERSION = 1.0.0
CONFIG += skip_target_version_ext

DEFINES += INCLUDE_GUI

contains(DEFINES, INCLUDE_GUI) {
    QT += gui widgets

    SOURCES += mainwindow.cpp
    HEADERS += mainwindow.h
} else {
    CONFIG += cmdline
}

SOURCES += \
    f_common.cpp \
    f_init.cpp \
    main.cpp \
    messagecenter.cpp \
    module/file/f_file_manager.cpp \
    module/logger/f_log_server.cpp \
    module/settings/f_settings.cpp \
    mqtt/mqtt_client.cpp \
    mqtt/mqtt_messager.cpp \
    mqtt/mqtt_server.cpp \
    zmq/zmq_client.cpp \
    zmq/zmq_messager.cpp \
    zmq/zmq_server.cpp


HEADERS += \
    f_common.h \
    f_init.h \
    messagecenter.h \
    module/file/f_file_manager.h \
    module/logger/f_log_server.h \
    module/logger/f_log_server_p.h \
    module/settings/f_settings.h \
    mqtt/mqtt_client.h \
    mqtt/mqtt_messager.h \
    mqtt/mqtt_server.h \
    zmq/zmq_client.h \
    zmq/zmq_messager.h \
    zmq/zmq_server.h


FORMS += \
    mainwindow.ui

INCLUDEPATH += \
    module \
    zmq \
    mqtt

CONFIG(release, debug|release): DESTDIR = $$PWD/../output
else:CONFIG(debug, debug|release): DESTDIR = $$PWD/../output_d

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../zeromq/lib/ -llibzmq-mt-4_3_5
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../zeromq/debug/lib/ -llibzmq-mt-gd-4_3_5

INCLUDEPATH += $$PWD/../../../../../zeromq/include
DEPENDPATH += $$PWD/../../../../../zeromq/include


# win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../zeromq/lib/ -llibzmq-mt-4_3_5
# else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../zeromq/debug/lib/ -llibzmq-mt-gd-4_3_5

# INCLUDEPATH += $$PWD/../../../../zeromq/include
# DEPENDPATH += $$PWD/../../../../zeromq/include

