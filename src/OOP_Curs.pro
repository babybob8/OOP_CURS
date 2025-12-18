QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17

SOURCES += \
    share/book.cpp \
    share/logger.cpp \
    share/parse.cpp \
    share/tests.cpp

HEADERS += \
    share/book.h \
    share/config.h \
    share/log.h \
    share/logger.h \
    share/parse.h \

CONFIG(debug, debug|release) {
    DEFINES += DEBUG_BUILD
}

CONFIG(console) {
    QT      -= widgets
    SOURCES += console_main.cpp
    TARGET = OOP_CURS_Console
} else {
    SOURCES += \
        main.cpp \
        ui/restable.cpp \
        ui/about.cpp \
        ui/logs.cpp \
        ui/mainwindow.cpp \
        ui/results.cpp \
        ui/students.cpp \
        ui/work.cpp

    HEADERS += \
        ui/restable.h \
        ui/results.h \
        ui/students.h \
        ui/work.h \
        ui/about.h \
        ui/logs.h \
        ui/mainwindow.h

    FORMS += \
        ui/restable.ui \
        ui/results.ui \
        ui/students.ui \
        ui/work.ui \
        ui/about.ui \
        ui/logs.ui \
        ui/mainwindow.ui

    TARGET = OOP_CURS_GUI
}

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

