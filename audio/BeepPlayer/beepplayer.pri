# This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt 
QT += multimedia

CONFIG += c++11

HEADERS +=  $$PWD/beepplayer_template.h \
            $$PWD/beepplayer.h \
            $$PWD/noteparser.h \
            $$PWD/audiobuffer.h
            
SOURCES +=  $$PWD/beepplayer.cpp \
            $$PWD/noteparser.cpp \
            $$PWD/audiobuffer.cpp
            
            
