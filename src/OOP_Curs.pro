QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17

SOURCES += \
    book.cpp \
    logger.cpp \
    parse.cpp \
    tests.cpp

HEADERS += \
    book.h \
    config.h \
    log.h \
    logger.h \
    parse.h

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
        mainwindow.cpp \
        restable.cpp

    HEADERS += \
        mainwindow.h \
        restable.h

    FORMS += \
        mainwindow.ui \
        restable.ui

    TARGET = OOP_CURS_GUI
}

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
