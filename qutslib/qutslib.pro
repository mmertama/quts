 # This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt
include(../shared/config.pri)

TEMPLATE = lib
QT += qml quick
CONFIG += c++11

DEFINES += QUTSLIB_EXPORT

TARGET = quts

INCLUDEPATH +=          \
        ..              \
        ../shared       \
        ../objectfinder

DESTDIR = $$PWD/../../lib/$$OS/$$TYPEDIR

docTarget.target = quts_doc.h
docTarget.depends = \
    $$PWD/../shared/constants.h \
    $$PWD/quts_commands.cpp

win32: docTarget.commands = python.exe $$PWD/../tools/qutsdocgen.py $$PWD/quts_doc.h $${docTarget.depends}
else: docTarget.commands = python $$PWD/../tools/qutsdocgen.py $$PWD/quts_doc.h $${docTarget.depends}

PRE_TARGETDEPS += quts_doc.h
QMAKE_EXTRA_TARGETS += docTarget

QMAKE_EXTRA_TARGETS += deploy
POST_TARGETDEPS += deploy

HEADERS += $$PWD\..\quts.h

HEADERS +=  ../shared/constants.h       \
            ../shared/utils.h           \
            ../shared/subsystem.h       \
            ../shared/index.h           \
            ../shared/qutsengine.h      \
            ../shared/variantutils.h    \
            ../shared/command.h         \
            ../shared/comp_utils.h      \
            ../shared/subsystem_template.h


HEADERS +=  \
    quts_private.h \
    quts_doc.h \
    printer.h \
    scope.h \
    subsystemdata.h \
    subroutine.h \
    varianthash.h \
    routine.h \
    qutsparser.h


SOURCES +=  \
    $$PWD/quts.cpp \
    $$PWD/subsystem.cpp \
    quts_private.cpp \
    quts_commands.cpp \
    qutsengine.cpp \
    printer.cpp \
    scope.cpp \
    subsystemdata.cpp \
    subroutine.cpp \
    index.cpp \
    routine.cpp \
    qutsparser.cpp


DISTFILES += \
    ../tools/qutsdocgen.py \
    ../tools/qutsmdgen.py

LDIR= $$PWD/../../lib/$$OS/$$TYPEDIR
LIBS += -L$$LDIR -lobjectfinder

DEFINES += QUTS_DEFAULT_PATH=$${LDIR}

message(get ObjectFinder library from $$LDIR)

DEFINES += QUTSLIB



