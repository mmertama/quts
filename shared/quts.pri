# This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt 
include(BeepPlayer/beepplayer.pri)

CONFIG += c++11

QT += testlib


HEADERS +=  $$PWD/quts.h \
            $$PWD/persistentdata.h \
            $$PWD/quts_private.h \
    $$PWD/audiosystem.h \
    $$PWD/subsystem.h \
    $$PWD/constants.h \
    $$PWD/mathsystem.h \
    $$PWD/mathsystem_template.h
            
    
SOURCES +=  $$PWD/quts.cpp \
            $$PWD/persistentdata.cpp \
    $$PWD/audiosystem.cpp \
    $$PWD/subsystem.cpp \
    $$PWD/mathsystem.cpp
            
        

DISTFILES += \
    $$PWD/test/scripts/twinkletwinkle.qts \
    $$PWD/test/scripts/fibonacci.qts \
    $$PWD/test/scripts/inputtest.qts \
    $$PWD/test/scripts/test1.qts \
    $$PWD/test/scripts/test2.qts \
    $$PWD/test/scripts/testSignal.qts \
    $$PWD/test/scripts/signals.qts \
    $$PWD/test/scripts/eval.qts \
    $$PWD/test/scripts/math.qts
