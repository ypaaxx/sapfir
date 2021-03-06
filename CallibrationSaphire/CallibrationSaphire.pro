QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    sapfir.cpp

HEADERS += \
    mainwindow.h \
    sapfir.h

FORMS += \
    mainwindow.ui


unix|win32: LIBS += -L'C:/Program Files (x86)/L-Card/ltr/lib/mingw/' -lltr27api

INCLUDEPATH += 'C:/Program Files (x86)/L-Card/ltr/include'
DEPENDPATH += 'C:/Program Files (x86)/L-Card/ltr/include'

win32:!win32-g++: PRE_TARGETDEPS += 'C:/Program Files (x86)/L-Card/ltr/lib/mingw/ltr27api.lib'
else:unix|win32-g++: PRE_TARGETDEPS += 'C:/Program Files (x86)/L-Card/ltr/lib/mingw/libltr27api.a'
