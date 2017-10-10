# This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt 
include(../../shared/config.pri)

TEMPLATE = app


CONFIG += c++11

QT += qml quick
QT += multimedia multimediawidgets
QT += network
QT += testlib
QT += widgets
QT += sql

QT += 3dcore 3drender 3dinput 3dlogic 3dextras 3danimation
QT += bluetooth
QT += positioning
QT += nfc
QT += svg
QT += websockets
QT += serialport

INCLUDEPATH +=                      \
                ../..               \
                ../../store


HEADERS     += qutsconsoleapp.h persistentdata.h

SOURCES += main.cpp persistentdata.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

CONFIG(debug, debug|release) {
    TYPEDIR = "debug"
}else {
    TYPEDIR = "release"
}

LDIR= $$PWD/../../../lib/$$OS/$$TYPEDIR
message(QutsConsole - get quts library from $$LDIR)

LIBS += -L$$LDIR -lquts -lobjectfinder



