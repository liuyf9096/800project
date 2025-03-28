QT = core

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        messager.cpp

DESTDIR = $$PWD/output

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    messager.h

unix:!macx{
    LIBS += -L$$PWD/output/ -llibmqlink
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/output/ -llibmqlink
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/output/ -llibmqlinkd

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
