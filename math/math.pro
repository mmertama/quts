 # This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt
include(../shared/config.pri)
TEMPLATE = lib
CONFIG += c++11
DEFINES += QUTS_EXPORT
TARGET = math_quts

DESTDIR = $$PWD/../../lib/$$OS/$$TYPEDIR

HEADERS +=  \
    mathsystem.h \
    mathsystem_template.h \
    quts_doc.h

SOURCES +=  \
    mathsystem.cpp

INCLUDEPATH += ../shared

docTarget.target = quts_doc.h
docTarget.depends = \
    $$PWD/mathsystem.cpp \
    $$PWD/mathsystem_template.h

win32: docTarget.commands = python.exe $$PWD/../tools/qutsdocgen.py $$PWD/quts_doc.h $${docTarget.depends}
else: docTarget.commands = python $$PWD/../tools/qutsdocgen.py $$PWD/quts_doc.h $${docTarget.depends}

PRE_TARGETDEPS += quts_doc.h
QMAKE_EXTRA_TARGETS += docTarget

LDIR= $$PWD/../../lib/$$OS/$$TYPEDIR
message(get quts library from $$LDIR)

LIBS += -L$$LDIR -lquts


